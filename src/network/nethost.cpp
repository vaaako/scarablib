#include "scarablib/network/nethost.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"

#include <cstring>
#include <enet/enet.h>

// Inspired by https://github.com/Hopson97/open-builder/blob/e4ca7d0652a4b1e3e55278b79f28f01c8c087a68/src/common/common/network/enet.cpp

NetHost::NetHost(const bool show_log) : show_log(show_log) {
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

NetHost::NetHost(const uint16 port, const uint8 max_clients, const bool show_log)
	: show_log(show_log) {

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

NetHost::~NetHost() noexcept {
	if(this->peer != nullptr) {
		enet_peer_reset((ENetPeer*)this->peer);
	}
	enet_host_destroy((ENetHost*)this->host);
	enet_deinitialize();

	if(this->running) {
		this->stop();
	}
}

NetHost::Packet::~Packet() noexcept {
	delete[] this->data;
}


bool NetHost::connect_to_server(const char* ipaddress, const uint16 port, const uint32 timeout_ms) {
	// Connect to server:port
	ENetAddress address;
	if(enet_address_set_host(&address, ipaddress) != 0) {
		throw ScarabError("Failed to create address");
	}
	address.port = port;

	if(this->show_log) {
		LOG_FLAG("SERVER", "Trying to connect to server %s:%i...", ipaddress, port);
		return false;
	}

	// Init connection, allocating the two channels (0 and 1)
	this->peer = enet_host_connect((ENetHost*)this->host, &address, 2, 0);
	if(this->peer == NULL) {
		LOG_ERROR("Failed to connect to server...");
		return false;
	}

	ENetEvent event;
	if(enet_host_service((ENetHost*)this->host, &event, timeout_ms) > 0
			&& event.type == ENET_EVENT_TYPE_CONNECT) {

		LOG_FLAG("SERVER", "Connected to server %s:%i", ipaddress, port);
	} else {
		enet_peer_reset((ENetPeer*)this->peer);
		this->peer = nullptr;
		LOG_ERROR("Failed to connect to %s:%i...", ipaddress, port);
		return false;
	}

	return true;
}

void NetHost::disconnect_from_server(const uint32 timeout_ms) noexcept {
	// Disconnect and don't send data when disconnected
	// TODO: change this later
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

// ENetPacket* createPacket(const sf::Packet& packet, u32 flags)
// {
//     return enet_packet_create(packet.getData(), packet.getDataSize(), flags);
// }
// void broadcastToPeers(ENetHost* host, const sf::Packet& packet, u8 channel, u32 flags)
// {
//     auto enetpacket = createPacket(packet, flags);
//     enet_host_broadcast(host, channel, enetpacket);
// }
// void Connection::send(const sf::Packet& packet, int channel, u32 flags)
// {
//     assert(peer);
//     auto enetPacket = createPacket(packet, flags);
//     enet_peer_send(peer, channel, enetPacket);
// }


void NetHost::stop() noexcept {
	if(!this->running) {
		LOG_ERROR("Server's thread has not started");
		return;
	}

	this->running = false;
	if(this->thread.joinable()) {
		this->thread.join();
	}
}

void NetHost::thread_gather_events(const uint32 timeout_ms, std::function<void(const std::vector<NetHost::Packet>&)> callback) noexcept {
	if(this->running) {
		LOG_ERROR("Server's thread has already been started");
		return;
	}

	this->running = true;
	this->thread = std::thread(&NetHost::thread_events, this, timeout_ms, callback);
	// Small delay to prevent high CPU usage
	// std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void NetHost::thread_events(const uint32 timeout_ms, std::function<void(const std::vector<NetHost::Packet>&)> callback) noexcept {
	if(!this->running) {
		LOG_ERROR("Server's thread has not started");
		return;
	}

	while(this->running) {
		callback(this->gather_events(timeout_ms));
	}
}

std::vector<NetHost::Packet> NetHost::gather_events(const uint32 timeout_ms) noexcept {
	std::vector<NetHost::Packet> packets;

	// Get all events in timeout
	ENetEvent event;
	// Wait 1 second for an event (by default)
	while(enet_host_service((ENetHost*)this->host, &event, timeout_ms) > 0) {
		NetHost::Packet packet = {
			.host      = "",
			.port      = 0,
			.event     = (NetHost::Event)event.type,
			.channel   = 0,
			.data      = nullptr,
			.data_size = 0,
		};

		switch(event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				this->connected_clients++;
				packet.host = this->resolve_enet_ipv6(event.peer->address.host);
				packet.port = event.peer->address.port;

				if(this->show_log) {
					LOG_FLAG("SERVER", "Client connected from %s:%u", packet.host.data(), packet.port);
				}
				break;

			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
			case ENET_EVENT_TYPE_DISCONNECT:
				this->connected_clients--;
				packet.host      = this->resolve_enet_ipv6(event.peer->address.host);
				packet.port      = event.peer->address.port;

				if(this->show_log) {
					LOG_FLAG("SERVER", "Client (%s:%u) disconnected", packet.host.data(), packet.port);
				}
				break;

			// NOTE: Packet must be destroyed after use
			// NOTE: Disconnect timeout. bad network or host crashed
			case ENET_EVENT_TYPE_RECEIVE:
				packet.host      = this->resolve_enet_ipv6(event.peer->address.host);
				packet.port      = event.peer->address.port;
				packet.channel   = event.channelID;
				packet.data_size = event.packet->dataLength;

				// Copy packet data to data, to avoid copying the packet in _packet
				packet.data = new uint8_t[packet.data_size];
				std::memcpy(packet.data, event.packet->data, packet.data_size);
				enet_packet_destroy(event.packet); // Destroy packet

				if(this->show_log) {
					LOG_FLAG("SERVER", "Packet of length %zu containing \"%s\" received from %s:%u on channel %u",
						packet.data_size,
						packet.data,
						packet.host.data(),
						packet.port,
						packet.channel
					);
				}
				break;

			case ENET_EVENT_TYPE_NONE:
			default:
				continue; // Skip irrelevant events
		}

		packets.push_back(std::move(packet));
	}

	return packets;
}
