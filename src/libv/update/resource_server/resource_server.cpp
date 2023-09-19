// Project: libv.update, File: src/libv/update/resource_server/resource_server.cpp

// hpp
#include <libv/update/resource_server/resource_server.hpp>
// libv
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor_he.hpp>
#include <libv/net/mtcp/connection_he.hpp>
//#include <libv/mt/worker_thread_pool.hpp>
// std
//#include <mutex>
// pro
#include <libv/update/log.hpp>
#include <libv/update/resource_server/resource_file.hxx>
#include <libv/update/resource_server/resource_peer.hxx>
#include <libv/update/resource_server/resource_server_state.hxx>





// libv
//#include <libv/serial/archive/binary.hpp>
//#include <libv/serial/codec.hpp>
//#include <queue>
//#include <filesystem>
// pro
//#include <libv/update/common/res_protocol.hpp>
//#include <utility>
//#include <libv/mt/async_value.hpp>





namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

//class NetworkResourcePeer : public libv::net::mtcp::ConnectionHandler<NetworkResourcePeer> {
//private:
//	ResourcePeer peer;
//
//public:
//	explicit inline NetworkResourcePeer(const std::shared_ptr<ServerState>& server) :
//		peer(server) {}
//};

class NetworkResourceServer : public libv::net::mtcp::AcceptorHandler<NetworkResourceServer> {
private:
	std::shared_ptr<ServerState> state;

public:
	explicit NetworkResourceServer(std::shared_ptr<ServerState> state) :
		state(std::move(state)) {}

	~NetworkResourceServer() {
		state->disconnect_all();
	}

private:
	virtual net::mtcp::Connection<void> on_accept(io_context& io_context) override {
		return net::mtcp::Connection<ResourcePeer>(io_context, state);
	}

	virtual void on_accept(error_code ec) override {
		log_update.error("Resource server accept failure: {}", libv::net::to_string(ec));
	}
};

// -------------------------------------------------------------------------------------------------

class ImplResourceServer {
public:
//	std::mutex mutex;
	const ResourceServer::Settings settings;

public: // Network
	libv::net::IOContext io_context;
//	BasicServer<ResourceServer> server;
//	std::optional<libv::net::mtcp::AcceptorAsyncCB> acceptor;
//	std::vector<Peer> peers;

public: // Filesystem
//	libv::mt::worker_thread_pool fs_threads;

public: // State
	std::shared_ptr<ServerState> state;
	libv::net::mtcp::Acceptor<NetworkResourceServer> networkResourceServer;


public:
	explicit ImplResourceServer(ResourceServer::Settings settings_) :
		settings(std::move(settings_)),
		io_context(settings.num_thread_net),
		state(std::make_shared<ServerState>(settings)),
		networkResourceServer(io_context, state) {
//		server(io_context),
//		fs_threads(settings.num_thread_fs), {

	}

public:
	void add_file(std::string id, std::filesystem::path path, resource_cache_policy fcp) {
//		const auto lock = std::unique_lock(mutex);

		log_update.trace("Load {} from: {} fcp:{}", id, path.generic_string(), libv::to_value(fcp));
		state->add(std::move(id), std::make_shared<ResourceFile>(path, fcp));
	}

	void start() {
//		const auto lock = std::unique_lock(mutex);
		networkResourceServer.acceptor().listen(settings.endpoint, settings.num_accept_backlog);
		networkResourceServer.acceptor().accept_async();
	}
};

// -------------------------------------------------------------------------------------------------

ResourceServer::ResourceServer(Settings settings_) :
	self(std::make_unique<ImplResourceServer>(std::move(settings_))) {
}

ResourceServer::~ResourceServer() = default; // For the sake of forward declared types

const ResourceServer::Settings& ResourceServer::settings() const noexcept {
	return self->settings;
}

void ResourceServer::add_file(std::string id, std::filesystem::path path, resource_cache_policy fcp) {
	self->add_file(id, path, fcp);
}

void ResourceServer::start() {
	self->start();
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
