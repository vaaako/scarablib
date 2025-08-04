#include "scarablib/network/network.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"

#include <cstring>

#define ENET_IMPLEMENTATION
#include <enet/enet.h>

// Inspired by https://github.com/Hopson97/open-builder/blob/e4ca7d0652a4b1e3e55278b79f28f01c8c087a68/src/common/common/network/enet.cpp

Network::Network(const bool show_log) : is_server(false), show_log(show_log) {
	if(enet_initialize() != 0) {
		throw ScarabError("Failed to initialize ENet");
	}

	this->host = enet_host_create(
		NULL, // Client host
		1, // Allow up to 1 client
		2, // Allow up to 2 channels to be used (0 and 1)
		0, // Assume any amount of incoming bandwidth
		0  // Assume any amount of outgoing bandwidth
	);

	if(this->host == NULL) {
		throw ScarabError("Failed to create ENet client");
	}
}

Network::Network(const uint16 port, const uint8 max_clients, const bool show_log)
	: is_server(true), show_log(show_log) {

	if(enet_initialize() != 0) {
		throw ScarabError("Failed to initialize ENet");
	}

	ENetAddress address = {
		.host = ENET_HOST_ANY, // localhost
		.port = port,
		.sin6_scope_id = 0
	};

	this->host = enet_host_create(
		&address, max_clients,
		2, // Allow up to 2 channels to be used (0 and 1)
		0, // Assume any amount of incoming bandwidth
		0  // Assume any amount of outgoing bandwidth
	);

	if(this->host == NULL) {
		throw ScarabError("Failed to create ENet server");
	}

	if(this->show_log) {
		LOG_FLAG("SERVER", "Server started on port %i", address.port);
	}
}

Network::~Network() noexcept {
	if(this->peer != nullptr) {
		enet_peer_reset((ENetPeer*)this->peer);
	}

	enet_host_destroy((ENetHost*)this->host);
	enet_deinitialize();

	if(this->running) {
		this->stop();
	}
}

Network::Packet::Packet(const void* data, const size_t data_size)
	: data_size(data_size) {

	this->data = new uint8[data_size];
	std::memcpy(this->data, data, data_size); // Copy to allocated data
}

Network::Packet::~Packet() noexcept {
	if(this->data != nullptr) {
		delete[] this->data;
	}
}


Network::Event::~Event() noexcept {
	if(this->packet != nullptr) {
		delete this->packet;
	}
}


Result Network::connect_to_server(const char* ipaddress, const uint16 port, const uint32 timeout_ms) {
	if(this->is_server) {
		return { "This is a server", false };
	}

	// Connect to server:port
	ENetAddress address;
	if(enet_address_set_host(&address, ipaddress) != 0) {
		throw ScarabError("Failed to create address");
	}
	address.port = port;

	if(this->show_log) {
		LOG_FLAG("SERVER", "Trying to connect to server %s:%i...", ipaddress, port);
	}

	// Init connection, allocating the two channels (0 and 1)
	this->peer = enet_host_connect((ENetHost*)this->host, &address, 2, 0);
	if(this->peer == NULL) {
		return { "Peer was not created successfully", false };
	}

	ENetEvent event;
	int result = enet_host_service((ENetHost*)this->host, &event, timeout_ms);
	if(result > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		if(this->show_log) {
			LOG_FLAG("SERVER", "Connected to server %s:%i", ipaddress, port);
		}

	} else {
		enet_peer_reset((ENetPeer*)this->peer);
		this->peer = nullptr;

		if(result == 0) {
			return { "Connection timed out", false };
		}

		if(result < 0) {
			return { "ENet error during host service", false };
		}

		return { "Unexpected error while connecting", false };
	}

	return true;
}

void Network::disconnect_from_server(const uint32 timeout_ms) noexcept {
	if(this->is_server) {
		return;
	}

	// Disconnect and don't send data when disconnected
	enet_peer_disconnect((ENetPeer*)this->peer, 0);

	// Wait for disconnection
	ENetEvent event;
	while(enet_host_service((ENetHost*)this->host, &event, timeout_ms) > 0) {
		switch (event.type) {
			case ENET_EVENT_TYPE_DISCONNECT:
				// Sends any queued packets on the host specified to its designated peers
				enet_host_flush((ENetHost*)this->host);

				if(this->show_log) {
					LOG_FLAG("SERVER", "Disconnected successfully!");
				}
				break;

			// Destroy any received packets
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;

			default:
				continue;
		}
	}

	enet_peer_reset((ENetPeer*)this->peer);
	this->peer = nullptr;
}

Result Network::send(const Network::Packet& packet, const bool flush, const Network::PacketFlag flag) const {
	if(!this->is_server) {
		return { "Host is not a client", false };
	}

	// Create an ENet packet
	ENetPacket* epacket = enet_packet_create(
		packet.data,
		packet.data_size,
		(ENetPacketFlag)flag // Ensures delivery (like TCP)
	);

	// Allocation failed
	if(epacket == NULL) {
		return { "Allocation failed while creating packet", false };
	}

	// Send the packet
	if(enet_peer_send((ENetPeer*)this->peer, 0, epacket) < 0) {
		enet_packet_destroy(epacket); // Clean up on failure
		return { "Failed to send packet", false };
	}

	// Force immediate flush
	if(flush) {
		enet_host_flush((ENetHost*)this->host);
	}

	return true;
}

Result Network::broadcast(const Network::Packet& packet, const Network::PacketFlag flag) const {
	if(!this->is_server) {
		return { "Host is a client, cannot broadcast", false };
	}

	if(this->connected_clients == 0) {
		return { "No clients connected", false };
	}

	ENetPacket* epacket = enet_packet_create(
		packet.data,
		packet.data_size,
		(ENetPacketFlag)flag // Ensures delivery (like TCP)
	);

	// Allocation failed
	if(epacket == NULL) {
		return { "Allocation failed while creating packet", false };
	}

	enet_host_broadcast((ENetHost*)this->host, 0, epacket);

	return true;
}


Result Network::stop() noexcept {
	if(!this->running) {
		return { "Server's thread has not started", false };
	}

	this->running = false;
	if(this->thread.joinable()) {
		// Wait for thread to finish
		this->thread.join();
	}

	return true;
}

void Network::threaded_poll_events(const uint32 timeout_ms, const NetworkCallback& callback) noexcept {
	// Just ignore, is no sense returning a Result
	if(this->running) {
		return;
	}

	this->running = true;
	this->thread  = std::thread(&Network::thread_events, this, timeout_ms, callback);

	// Small delay to prevent high CPU usage
	// std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Network::thread_events(const uint32 timeout_ms, const NetworkCallback& callback) noexcept {
	while(this->running) {
		callback(this->poll_events(timeout_ms));

		// Avoid high CPU usage
		if(timeout_ms == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

std::vector<Network::Event> Network::poll_events(const uint32 timeout_ms) noexcept {
	std::vector<Network::Event> events;

	// Get all events in timeout
	ENetEvent event;
	// Wait 1 second for an event (by default)
	while(enet_host_service((ENetHost*)this->host, &event, timeout_ms) > 0) {
		Network::Event netevent = {
			.type     = (Network::EventType)event.type,
			.host      = "",
			.port      = 0,
			.channel   = 0,
			.packet    = nullptr
		};

		switch(event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				this->connected_clients++;
				netevent.host = this->resolve_enet_address(&event.peer->address);
				netevent.port = event.peer->address.port;

				if(this->show_log) {
					LOG_FLAG("SERVER", "Client connected from %s:%u", netevent.host.data(), netevent.port);
				}
				break;

			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
			case ENET_EVENT_TYPE_DISCONNECT:
				this->connected_clients--;
				netevent.host      = this->resolve_enet_address(&event.peer->address);
				netevent.port      = event.peer->address.port;

				if(this->show_log) {
					LOG_FLAG("SERVER", "Client (%s:%u) disconnected", netevent.host.data(), netevent.port);
				}
				break;

			// NOTE: Packet must be destroyed after use
			// NOTE: Disconnect timeout. bad network or host crashed
			case ENET_EVENT_TYPE_RECEIVE:
				netevent.host      = this->resolve_enet_address(&event.peer->address);
				netevent.port      = event.peer->address.port;
				netevent.channel   = event.channelID;

				// netevent.packet = new Network::Packet {
				// 	.data = new uint8_t[event.packet->dataLength],
				// 	.data_size = event.packet->dataLength
				// };

				netevent.packet->data = new uint8_t[event.packet->dataLength];
				netevent.packet->data_size = event.packet->dataLength;

				// Copy packet data to packet to avoid copying the packet
				std::memcpy(netevent.packet->data, event.packet->data, netevent.packet->data_size);

				// Destroy enet packet
				enet_packet_destroy(event.packet);

				if(this->show_log) {
					LOG_FLAG("SERVER", "Packet of length %zu received from %s:%u on channel %u",
						netevent.packet->data_size,
						netevent.host.data(),
						netevent.port,
						netevent.channel
					);
				}
				break;

			case ENET_EVENT_TYPE_NONE:
			default:
				continue; // Skip irrelevant events
		}

		events.push_back(std::move(netevent));
	}

	return events;
}


std::string Network::resolve_enet_address(void* address) {
	char ipv6[INET6_ADDRSTRLEN];
	enet_address_get_host_ip((ENetAddress*)address, ipv6, sizeof(ipv6));
	return std::string(ipv6);
}
