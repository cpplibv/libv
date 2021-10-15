// Project: libv.net, File: src/libv/net/_legacy_ideas/basic_network.hpp

#pragma once

// std
#include <unordered_set>



// =================================================================================================
// =================================================================================================

struct ResourceSession : libv::net::mtcp::SimpleSession {
	std::mutex mutex;
	ResourceServer& server;
//	ImplResourceServer& server;
//	ImplResourceConnectionSet& group; // or chat room, or something
	int sm;

	template <typename Message>
	void receive(const Message& message) {
		sm.process_event(message);
	}
};

class ResourceServer : libv::net::mtcp::SimpleServer {
	std::mutex mutex;

	libv::update::ResourceServer::Settings settings;

	libv::net::mtcp::ConnectionSet<ResourceSession> active_peers;
	libv::net::mtcp::ConnectionQueue<> queued_peers;

//	libv::load_trend peer_trend;
////	libv::trend<size_t> peer_satisfaction_trend;
////	libv::trend<size_t> peer_connection_trend;
	int file_maps;

private:
	virtual void connect(libv::net::mtcp::ConnectionAsyncCB connection) override {
		const auto lock = std::unique_lock(mutex);

		if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
			active_peers.emplace(connection, *this);
			return;
		}

		if (queued_peers.size() < settings.limit_peer_count_queue || settings.limit_peer_count_queue == 0) {
			queued_peers.emplace(connection);
			return;
		}

		// Drop connection
////		const auto drain_time = peer_satisfaction_trend.predict_time(settings.limit_peer_count_active + settings.limit_peer_count_queue);
////		const auto assumed_fill_count = peer_connection_trend.predict_count(drain_time);
////		const auto over_connection_rate = assumed_fill_count / (settings.limit_peer_count_active + settings.limit_peer_count_queue);
////		connection.send(ResponseBusy{});

//		connection.send(ResponseBusy{load_trend.busy_time()});
	}

	virtual void error(libv::net::mtcp::ConnectionAsyncCB connection, std::error_code ec) override {
		const auto lock = std::unique_lock(mutex);

		const auto ita = active_peers.find_opt(connection);
		if (ita)
			ita->net_io_error(ec);
	}

	virtual void disconnect(libv::net::mtcp::ConnectionAsyncCB connection) override {
		const auto lock = std::unique_lock(mutex);

		// BETTER IDEA:, Connection should auto dereg from ConnectionsSets
		if (!queued_peers.empty()) {
			if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
				active_peers.emplace(queued_peers.pop_front(), *this);
			}
		}

		// Old Idea: manual management
//		const auto ita = active_peers.find_opt(connection);
//		if (ita) {
//			active_peers.erase(ita, *this);
//			if (!queued_peers.empty()) {
//				if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
//					active_peers.emplace(queued_peers.pop_front(), *this);
//				}
//			}
//			return;
//		}
//
//		const auto itq = queued_peers.find_opt(connection);
//		if (itq) {
//			queued_peers.erase(itq);
//		}

		// assert false? Internal error, connection is not part of any connection set on the server
	}
};

// =================================================================================================
// =================================================================================================

template <typename NetworkProtocol>
class BasicClient {
public:
	using client_type = typename NetworkProtocol::ResourceClient;
	using server_type = typename NetworkProtocol::ResourceServer;
	using session_type = typename NetworkProtocol::ResourceSession;
	using archive_type = typename NetworkProtocol::archive;
	using messages_type = typename NetworkProtocol::messages;

	static inline auto codec = libv::serial::CodecClient<client_type, archive_type>{messages_type{}};

private:
	libv::net::mtcp::ConnectionAsyncCB connection;
	server_type& server;
	client_type session;
};

template <typename NetworkProtocol>
class BasicSession {
public:
	using client_type = typename NetworkProtocol::ResourceClient;
	using server_type = typename NetworkProtocol::ResourceServer;
	using session_type = typename NetworkProtocol::ResourceSession;
	using archive_type = typename NetworkProtocol::archive;
	using messages_type = typename NetworkProtocol::messages;

	static inline auto codec = libv::serial::CodecServer<client_type, archive_type>{messages_type{}};

private:
	libv::net::mtcp::ConnectionAsyncCB connection;
	server_type& server;
	client_type session;
};

template <typename NetworkProtocol>
class BasicServer {
public:
	using server_type = typename NetworkProtocol::ResourceServer;
	using session_type = typename NetworkProtocol::ResourceSession;
	using client_type = typename NetworkProtocol::ResourceClient;
	using archive_type = typename NetworkProtocol::archive;
	using messages_type = typename NetworkProtocol::messages;

private:
	libv::net::mtcp::AcceptorAsyncCB acceptor;
	server_type server;
	std::unordered_set<libv::net::mtcp::ConnectionAsyncCB> sessions;
//	using session_set = std::set<std::shared_ptr<session_type>>;

public:
	explicit BasicServer(libv::net::IOContext& ioContext) :
			acceptor(ioContext) {
		acceptor.handle_accept([](libv::net::mtcp::ConnectionAsyncCB connection) {
			connected(std::move(connection));
		});
		acceptor.handle_error([](libv::net::mtcp::AcceptorAsyncCB::ErrorSource, std::error_code ec) {
			libv::net::log_net.error("Failed to accept connection: {}", libv::net::to_string(ec));
		});
	}

	~BasicServer() {
//		acceptor.cancel();
	}

public:
	void listen(uint16_t port, int backlog = 4) noexcept {
		listen(libv::net::mtcp::Endpoint(0, 0, 0, 0, port), backlog);
	}

	void listen(libv::net::mtcp::Endpoint endpoint, int backlog = 4) noexcept {
		const auto ec = acceptor.listen(endpoint, backlog);
		if (ec)
			throw std::system_error(ec);
		acceptor.accept();
	}

	void adopt(libv::net::mtcp::ConnectionAsyncCB connection) {
		on_connect(std::move(connection));
	}

private:
	void on_connect(libv::net::mtcp::ConnectionAsyncCB connection) {
		connection.read_limit(100); // bytes per second
		connection.write_limit(100); // bytes per second

		server.connect(std::move(connection));

		connection.start();
	}

	void on_disconnect(libv::net::mtcp::ConnectionAsyncCB connection) {
		server.disconnect(std::move(connection));
	}
};

struct ResourceNetwork {
	using client = ResourceClient;
	using session = ResourceSession;
	using server = ResourceServer;

	using archive = libv::archive::Binary;
	using messages = ResourceMessages;
};

using _ResourceClient = BasicClient<ResourceNetwork>;
using _ResourceSession = BasicSession<ResourceNetwork>;
using _ResourceServer = BasicServer<ResourceNetwork>;

// =================================================================================================
// =================================================================================================
// =================================================================================================
