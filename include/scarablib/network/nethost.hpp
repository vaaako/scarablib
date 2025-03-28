#pragma once

#include "scarablib/typedef.hpp"
#include <functional>
#include <string_view>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

class NetHost {
	public:
		enum class Event : uint8 {
			NONE = 0,
			CONNECT,
			DISCONNECTED,
			RECEIVE,
			DISCONNECT_TIMEOUT
		};

		struct Packet {
			// Host that generated the packet
			std::string_view host;
			// Port that generated the packet
			uint16 port;
			// The event that generated the packet
			NetHost::Event event;
			// Channel that generated the packet
			uint8 channel;
			// Allocated data for packet
			uint8* data;
			// Size of the packet
			size_t data_size;

			// Delete packet
			~Packet() noexcept;
		};

		// This is used to make a Server
		NetHost(const uint16 port, const uint8 max_clients, const bool show_log = false);
		// This is used to make a Client
		NetHost(const bool show_log = false);

		~NetHost() noexcept;

		// Try to connect to a server.
		// If failed return false, return true otherwise.
		// `timeout_ms` (5ms default) is the time in milliseconds to wait for the connection attempt to succeed
		bool connect_to_server(const char* ipaddress, const uint16 port, const uint32 timeout_ms = 5000);

		// Try to disconnect from server.
		// `timeout_ms` (5ms default) is the time in milliseconds to wait for the disconnection attempt to succeed
		void disconnect_from_server(const uint32 timeout_ms = 5000) noexcept;

		// Gather all events coming from the client.
		// `timeout_ms` (1ms default) is the maximum amount of time to wait for an event.
		// If a packet is received, it is stored in Server::Packet::data
		// If a client is connected or disconnected, the packet object will only containg host and port fields
		std::vector<NetHost::Packet> gather_events(const uint32 timeout_ms = 1000) noexcept;

		// Use this for gather events in thread mode.
		// `timeout_ms` (1ms default) is the maximum amount of time to wait for an event.
		// The callback will receive the vector of events from Server::gather_events.
		// WARNING: Don't forget to call NetHost::stop when you're done
		void thread_gather_events(const uint32 timeout_ms, std::function<void(const std::vector<NetHost::Packet>&)> callback) noexcept;

		// Stop the thread
		void stop() noexcept;

		// How many clients are currently connected
		inline uint32 get_connected_clients() const noexcept {
			return this->connected_clients;
		}

	private:
		bool show_log = false;

		// Thread
		std::thread thread;
		bool running = false; // Is thread running

		// Server only
		uint32 connected_clients = 0;

		// ENet objects
		void* host; // ENetHost*
		void* peer; // ENetPeer* (used to disconnect from server)

		// Resolve a ENet IPv6 address to a string
		inline static std::string resolve_enet_ipv6(const struct in6_addr& host) {
			char ipv6[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &host, ipv6, INET6_ADDRSTRLEN);
			return std::string(ipv6);
		}

		// Use this for gather events in thread mode.
		// Start the thread with Server::start.
		// timeout_ms is the maximum amount of time to wait for an event.
		// The callback will receive the vector of events from Server::gather_events
		void thread_events(const uint32 timeout_ms, std::function<void(const std::vector<NetHost::Packet>&)> callback) noexcept; };
