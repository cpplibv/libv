// Project: libv.update, File: src/libv/update/resource/resource_server.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/resource/resource_server.hpp>
// libv
//#include <libv/process/lock_file.hpp>
//#include <libv/utility/read_file.hpp>
//#include <libv/utility/write_file.hpp>
#include <libv/serial/codec.hpp>
#include <libv/mt/async_value.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
#include <mutex>
// pro
//#include <libv/update/resource/codec.hpp>
//#include <libv/update/resource/resource_common.lpp>



#include <libv/update/common/res_protocol.hpp>
#include <libv/serial/archive/binary.hpp>

#include <libv/serial/serial.hpp>
//#include <libv/net/mtcp/acceptor.hpp>
//#include <libv/net/mtcp/connection_cb.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/address.hpp>
#include <libv/state/state.hpp>

//#include <fstream>

//#include <span>
//#include <typeindex>
//#include <vector>
//#include <thread>
//#include <fstream>


#include <libv/net/io_context.hpp>

#include <libv/mt/worker_thread_pool.hpp>
#include <libv/utility/read_file_stop.hpp>
#include <libv/net/log.hpp>
#include <libv/net/error.hpp>


namespace libv {
namespace update {

// =================================================================================================

//struct ResourceSession : /*libv::net::*/BaseSession<ResourceServer> {
//struct ResourceSession : libv::net::mtcp::SimpleSession {
//	std::mutex mutex;
//	ResourceServer& server;
////	ImplResourceServer& server;
////	ImplResourceConnectionSet& group; // or chat room, or something
//	int sm;
//
//	template <typename Message>
//	void receive(const Message& message) {
//		sm.process_event(message);
//	}
//};
//
//class ResourceServer : libv::net::mtcp::SimpleServer {
//	std::mutex mutex;
//
//	libv::update::ResourceServer::Settings settings;
//
//	libv::net::mtcp::ConnectionSet<ResourceSession> active_peers;
//	libv::net::mtcp::ConnectionQueue<> queued_peers;
//
////	libv::load_trend peer_trend;
//////	libv::trend<size_t> peer_satisfaction_trend;
//////	libv::trend<size_t> peer_connection_trend;
//	int file_maps;
//
//private:
//	virtual void connect(libv::net::mtcp::ConnectionAsyncCB connection) override {
//		const auto lock = std::unique_lock(mutex);
//
//		if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
//			active_peers.emplace(connection, *this);
//			return;
//		}
//
//		if (queued_peers.size() < settings.limit_peer_count_queue || settings.limit_peer_count_queue == 0) {
//			queued_peers.emplace(connection);
//			return;
//		}
//
//		// Drop connection
//////		const auto drain_time = peer_satisfaction_trend.predict_time(settings.limit_peer_count_active + settings.limit_peer_count_queue);
//////		const auto assumed_fill_count = peer_connection_trend.predict_count(drain_time);
//////		const auto over_connection_rate = assumed_fill_count / (settings.limit_peer_count_active + settings.limit_peer_count_queue);
//////		connection.send(ResponseBusy{});
//
////		connection.send(ResponseBusy{load_trend.busy_time()});
//	}
//
//	virtual void error(libv::net::mtcp::ConnectionAsyncCB connection, std::error_code ec) override {
//		const auto lock = std::unique_lock(mutex);
//
//		const auto ita = active_peers.find_opt(connection);
//		if (ita)
//			ita->net_io_error(ec);
//	}
//
//	virtual void disconnect(libv::net::mtcp::ConnectionAsyncCB connection) override {
//		const auto lock = std::unique_lock(mutex);
//
//		// BETTER IDEA:, Connection should auto dereg from ConnectionsSets
//		if (!queued_peers.empty()) {
//			if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
//				active_peers.emplace(queued_peers.pop_front(), *this);
//			}
//		}
//
//		// Old Idea: manual management
////		const auto ita = active_peers.find_opt(connection);
////		if (ita) {
////			active_peers.erase(ita, *this);
////			if (!queued_peers.empty()) {
////				if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
////					active_peers.emplace(queued_peers.pop_front(), *this);
////				}
////			}
////			return;
////		}
////
////		const auto itq = queued_peers.find_opt(connection);
////		if (itq) {
////			queued_peers.erase(itq);
////		}
//
//		// assert false? Internal error, connection is not part of any connection set on the server
//	}
//};

// =================================================================================================

//struct ResourceReady {};
//
//using ev_ResourceReady = libv::state::ev<const ResourceReady&>;

// =================================================================================================
// =================================================================================================
// =================================================================================================
//
//template <typename NetworkProtocol>
//class BasicClient {
//public:
//	using client_type = typename NetworkProtocol::ResourceClient;
//	using server_type = typename NetworkProtocol::ResourceServer;
//	using session_type = typename NetworkProtocol::ResourceSession;
//	using archive_type = typename NetworkProtocol::archive;
//	using messages_type = typename NetworkProtocol::messages;
//
//	static inline auto codec = libv::serial::CodecClient<client_type, archive_type>{messages_type{}};
//
//private:
//	libv::net::mtcp::ConnectionAsyncCB connection;
//	server_type& server;
//	client_type session;
//};
//
//template <typename NetworkProtocol>
//class BasicSession {
//public:
//	using client_type = typename NetworkProtocol::ResourceClient;
//	using server_type = typename NetworkProtocol::ResourceServer;
//	using session_type = typename NetworkProtocol::ResourceSession;
//	using archive_type = typename NetworkProtocol::archive;
//	using messages_type = typename NetworkProtocol::messages;
//
//	static inline auto codec = libv::serial::CodecServer<client_type, archive_type>{messages_type{}};
//
//private:
//	libv::net::mtcp::ConnectionAsyncCB connection;
//	server_type& server;
//	client_type session;
//};
//
//template <typename NetworkProtocol>
//class BasicServer {
//public:
//	using server_type = typename NetworkProtocol::ResourceServer;
//	using session_type = typename NetworkProtocol::ResourceSession;
//	using client_type = typename NetworkProtocol::ResourceClient;
//	using archive_type = typename NetworkProtocol::archive;
//	using messages_type = typename NetworkProtocol::messages;
//
//private:
//	libv::net::mtcp::AcceptorAsyncCB acceptor;
//	server_type server;
//	std::unordered_set<libv::net::mtcp::ConnectionAsyncCB> sessions;
////	using session_set = std::set<std::shared_ptr<session_type>>;
//
//public:
//	explicit BasicServer(libv::net::IOContext& ioContext) :
//		acceptor(ioContext) {
//		acceptor.handle_accept([](libv::net::mtcp::ConnectionAsyncCB connection) {
//			connected(std::move(connection));
//		});
//		acceptor.handle_error([](libv::net::mtcp::AcceptorAsyncCB::ErrorSource, std::error_code ec) {
//			libv::net::log_net.error("Failed to accept connection: {}", libv::net::to_string(ec));
//		});
//	}
//
//	~BasicServer() {
////		acceptor.cancel();
//	}
//
//public:
//	void listen(uint16_t port, int backlog = 4) noexcept {
//		listen(libv::net::mtcp::Endpoint(0, 0, 0, 0, port), backlog);
//	}
//
//	void listen(libv::net::mtcp::Endpoint endpoint, int backlog = 4) noexcept {
//		const auto ec = acceptor.listen(endpoint, backlog);
//		if (ec)
//			throw std::system_error(ec);
//		acceptor.accept();
//	}
//
//	void adopt(libv::net::mtcp::ConnectionAsyncCB connection) {
//		on_connect(std::move(connection));
//	}
//
//private:
//	void on_connect(libv::net::mtcp::ConnectionAsyncCB connection) {
//		connection.read_limit(100); // bytes per second
//		connection.write_limit(100); // bytes per second
//
//		server.connect(std::move(connection));
//
//		connection.start();
//	}
//
//	void on_disconnect(libv::net::mtcp::ConnectionAsyncCB connection) {
//		server.disconnect(std::move(connection));
//	}
//};
//
//struct ResourceNetwork {
//	using client = ResourceClient;
//	using session = ResourceSession;
//	using server = ResourceServer;
//
//	using archive = libv::archive::Binary;
//	using messages = ResourceMessages;
//};
//
//using _ResourceClient = BasicClient<ResourceNetwork>;
//using _ResourceSession = BasicSession<ResourceNetwork>;
//using _ResourceServer = BasicServer<ResourceNetwork>;

// =================================================================================================
// =================================================================================================
// =================================================================================================

//namespace { // -------------------------------------------------------------------------------------
//
//class Peer;
//
//struct Resource {
//};
//
//struct ResourceFile : Resource {
//	std::filesystem::path filepath;
//	FileCachePolicy cache_policy;
//
////	std::mutex cache_m;
//	libv::mt::async_value<libv::ReadFileResult> memory;
//
//	void fetch_for(std::weak_ptr<Peer> peer) {
//		auto task = [peer_wp = std::move(peer), this](std::stop_token st) {
//			libv::read_file_stop(st);
//
//			auto peer_sp = peer.lock();
//			if (not peer_sp)
//				return;
//
//			peer_sp->resourceReady(*this);
//		};
//
//		memory.load_async(io_threads, std::move(task), filepath);
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//class Peer {
//	static inline libv::serial::CodecServer<Peer, libv::archive::Binary> codec{ResourceMessages{}};
//
//	ImplResourceServer& server;
//
//	std::mutex mutex;
//	libv::net::mtcp::ConnectionAsyncCB connection;
//
//	uint64_t requested_offset = 0;
//	uint64_t requested_amount = 0;
//	uint64_t next_chunk_offset = 0;
//
//	std::shared_ptr<ResourceFile> selected_resource;
//
////	std::chrono::steady_clock::time_point last_action;
////	std::shared_ptr<ResourceFile> selected_resource;
//
//	ResourcePeerSM sm;
//
//public:
//	Peer(libv::net::mtcp::ConnectionAsyncCB connection) : connection(connection) {
//		sm.initiate();
//	//	sm.process_event(ev_NetIOError());
//	}
//
//public:
//	void resourceReady() {
//
//	}
//
//	template <typename T>
//	void send(const T& response) {
//		const auto lock = std::unique_lock(mutex);
//		connection.send(codec.encode(response));
//	}
//
//	// <<< This is for the connection API callback/entry point
//	void receive(const std::span<const std::byte> network_message) {
//		const auto lock = std::unique_lock(mutex);
//		codec.decode(*this, network_message);
//	}
//
////	result react(const ev_RequestResourceSelect& event) {
////		auto resource = server.get_file(request.resource);
////		const auto lock = std::unique_lock(mutex);
////
////		if (resource == nullptr)
////			send(ResourceMessages::ResponseResourceNotFound{});
////		else
////			return transit<struct Selected>(resource);
////	}
////
////	result react(const ev_RequestResourceDescription&) {
////		auto resource = server.get_file(request.resource);
////
////		const auto lock = std::unique_lock(mutex);
////		resource = std::move(resource);
////
////		if (resource == nullptr) {
////			send(ResourceMessages::ResponseResourceNotFound{});
////		} else {
////			resource->fetch_with_description(weak_ptr_from_this);
////			return transit<struct Selected>(resource);
////		}
////	}
////
////	result react(const ev_RequestCancel&) {
////		const auto lock = std::unique_lock(mutex);
////
////		selected_resource = nullptr;
////
////		return transit<StateStandBy>();
////	}
////
////	result react(const ev_RequestResourceData&) {
////		const auto lock = std::unique_lock(mutex);
////
////		requested_offset = request.offset;
////		requested_amount = request.amount;
////		next_chunk_offset = requested_offset;
////
////		selected_resource->fetch_send(weak_ptr_from_this);
////
////		return transit<struct Selected>();
////	}
////
////	result react(const ev_ResourceReady& event) {
////		auto lock = std::unique_lock(mutex);
////
////		if (selected_resource != resource)
////			return discard_event();
////		else
////			return transit<struct Transferring>();
////
////		//		send(ResourceMessages::ResponseResourceDescription{selected_resource->cache.size()});
////	}
////
////	result react(const ev_ChunkSent& event) {
////		//		send(ResourceMessages::ResponseResourceChunk{next_chunk_offset, selected_resource->cache});
////		//		next_chunk_offset += server.settings.resource_network_chunk_size;
////		//		next_chunk_offset;
////		//		requested_offset;
////		return discard_event();
////	}
//};
//
//} // namespace -------------------------------------------------------------------------------------

class ImplResourceServer {
public:
	std::mutex mutex;
	const ResourceServer::Settings settings;

public: // Network
	libv::net::IOContext io_context;
//	BasicServer<ResourceServer> server;
//	std::optional<libv::net::mtcp::AcceptorAsyncCB> acceptor;
//	std::vector<Peer> peers;

public: // Filesystem
	libv::mt::worker_thread_pool fs_threads;
//	std::unordered_map<std::string, std::shared_ptr<ResourceFile>> files;
//	std::unordered_map<std::weak_ptr<ResourceFile>, std::weak_ptr<Peer>> x;

public:
//	explicit ImplResourceServer(ResourceServer::Settings settings) :
//		settings(std::move(settings)),
//		io_context(this->settings.num_threads_net),
//		fs_threads(this->settings.num_threads_fs) {
//	}
	explicit ImplResourceServer(ResourceServer::Settings settings_) :
		settings(std::move(settings_)),
		io_context(settings.num_thread_net),
//		server(io_context),
		fs_threads(settings.num_thread_fs) {

//		server.listen(settings.acceptor_endpoint, settings.acceptor_connection_backlog);
	}

public:
//	std::shared_ptr<ResourceFile> getFile(const std::string& id) {
//		const auto lock = std::unique_lock(mutex);
//		const auto it = files.find(id);
//
//		if (it == files.end())
//			return nullptr;
//		else
//			return it->second;
//	}
};

// -------------------------------------------------------------------------------------------------

ResourceServer::ResourceServer(Settings settings_) :
	self(std::make_unique<ImplResourceServer>(std::move(settings_))) {
}

ResourceServer::~ResourceServer() = default; // For the sake of forward declared unique_ptr

const ResourceServer::Settings& ResourceServer::settings() const noexcept {
	return self->settings;
}

void ResourceServer::add_file(std::string id, std::filesystem::path path, FileCachePolicy fcp) {
	const auto lock = std::unique_lock(self->mutex);
//	self->files.emplace(id, std::make_shared<ResourceFile>(path, fcp));
}

void ResourceServer::listen(libv::net::IOContext& io_context, const libv::net::mtcp::Endpoint& endpoint) {
	const auto lock = std::unique_lock(self->mutex);
//	self->acceptor.emplace(io_context);
////	self->acceptor->handle_error();
////	self->acceptor->handle_accept();
//	self->acceptor->listen(endpoint, self->settings.acceptor_connection_queue);
//	self->acceptor->accept();
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
