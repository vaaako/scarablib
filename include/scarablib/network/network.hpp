#pragma once

#include "scarablib/proper/result.hpp"
#include "scarablib/typedef.hpp"
#include <functional>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

class Network {
	public:
		// Flags to tell how sending packets should be handled.
		// Can be combined, for example: RELIABLE | UNRELIABLE_FRAGMENT (reliable, but fraagmented, for large critical data)
		enum PacketFlag : uint8 {
			// Guarantees delivery (like TCP), packets will resent until is received.
			// Use case: Critical data (e.g. game state, chat messages).
			// Higher latency
			RELIABLE            = (1 << 0),
			// Packets may arrive out of order.
			// Use case: Real-time dataa where old packets can be ignored (e.g. voice chat, real time position).
			// Faster but may drop packets
			UNSEQUENCED         = (1 << 1),
			// Splits large packets into fragments
			// Use case: Large, non-critical data (e.g. file transfers).
			// Avoid using for small packets (overhead)
			UNRELIABLE_FRAGMENT = (1 << 3)
		};

		// Types of events
		enum class EventType : uint8 {
			NONE = 0,
			CONNECT,
			DISCONNECTED,
			RECEIVE,
			DISCONNECT_TIMEOUT
		};

		// Packet sent over the network
		struct Packet {
			// Allocated data for packet
			uint8* data;
			// Size of the packet
			size_t data_size;

			// Automatically allocates data for packet.
			// `data` is a pointer to the data. (this will be copied and allocated to the packet)
			// `data_size` is the size of this data (use sizeof)
			Packet(const void* data, const size_t data_size);

			// Delete packet
			~Packet() noexcept;
		};

		// Class used to hold information about an event
		struct Event {
			// The event that generated the packet
			Network::EventType type;
			// Host that generated the packet
			std::string host; // I need to use std::string here
			// Port that generated the packet
			uint16 port;
			// Channel that generated the packet
			uint8 channel;

			// Packet send with the event (may be nullptr)
			Packet* packet;

			~Event() noexcept;
		};


		using NetworkCallback = std::function<void(std::vector<Network::Event>)>;

		// This is used to make a Server
		Network(const uint16 port, const uint8 max_clients, const bool show_log = false);
		// This is used to make a Client
		Network(const bool show_log = false);

		~Network() noexcept;

		// Try to connect to a server.
		// If failed return false, return true otherwise.
		// `timeout_ms` (5ms default) is the time in milliseconds to wait for the connection attempt to succeed
		Result connect_to_server(const char* ipaddress, const uint16 port, const uint32 timeout_ms = 5000);

		// Try to disconnect from server.
		// `timeout_ms` (5ms default) is the time in milliseconds to wait for the disconnection attempt to succeed
		void disconnect_from_server(const uint32 timeout_ms = 5000) noexcept;

		// Send a packet to the server.
		// Will only work if object is a client.
		// `flush` will force the packet to be sent immediately
		Result send(const Network::Packet& packet, const bool flush = false, const Network::PacketFlag flag = Network::PacketFlag::RELIABLE) const;

		// Server broadcast packet to all connected clients
		Result broadcast(const Network::Packet& packet, const Network::PacketFlag flag = Network::PacketFlag::RELIABLE) const;

		// Gather all events coming from the client.
		// `timeout_ms` (1ms default) is the maximum amount of time to wait for an event.
		// If a packet is received, it is stored in Server::Event::Packet::data
		// If a client is connected or disconnected, the event object will only contain the host and port
		std::vector<Network::Event> poll_events(const uint32 timeout_ms = 1000) noexcept;

		// Use this for gather events in thread mode.
		// `timeout_ms` (1ms default) is the maximum amount of time to wait for an event.
		// The callback will receive the vector of events from Server::poll_events.
		// WARNING: Don't forget to call Network::stop when you're done
		void threaded_poll_events(const uint32 timeout_ms, const NetworkCallback& callback) noexcept;

		// Stop the thread
		Result stop() noexcept;

		// How many clients are currently connected
		inline uint32 get_connected_clients() const noexcept {
			return this->connected_clients;
		}

	private:
		bool is_server  = false; // True if object is a server
		bool show_log = false;

		// Thread
		std::thread thread;
		// Controls when the thread should stop
		std::atomic<bool> running = false; // Avoid multithread synchronization and invalid access

		// Server only
		uint32 connected_clients = 0;

		// ENet objects
		void* host; // ENetHost*
		void* peer = nullptr; // ENetPeer* (used to disconnect from server)

		// Resolve a ENet IPv6 address to a string
		std::string resolve_enet_address(void* host); // (ENetAddress) Using void* to avoid including enet in header

		// Use this for gather events in thread mode.
		// Start the thread with Server::start.
		// timeout_ms is the maximum amount of time to wait for an event.
		// The callback will receive the vector of events from Server::poll_events
		void thread_events(const uint32 timeout_ms, const NetworkCallback& callback) noexcept; };
