// Project: libv.net, File: src/libv/update/net/updater_network_server.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/server/network_server.hpp>
// libv
#include <libv/net/error.hpp>
#include <libv/net/mtcp/acceptor_he.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/codec.hpp>
#include <libv/utility/hex_dump.hpp> // <<< hex_dump_with_ascii
//#include <libv/mt/binary_latch.hpp>
//#include <libv/net/error.hpp>
//#include <libv/log/log.hpp>
//#include <libv/net/address.hpp>
//#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/connection_he.hpp>
// std
//#include <mutex>
#include <set> // <<<
// pro
//#include <libv/update/common/protocol.hpp>
#include <libv/update/log.hpp>
#include <libv/update/server/info.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class ServerState;

class aux_update_session : public libv::net::mtcp::ConnectionHandler<aux_update_session> {
public:
	std::shared_ptr<ServerState> server;
//	libv::mt::binary_latch latch;
//	error_code error;
//	std::string response;
//	msg::UpdateRoute info;
//
//	bool version_not_supported = false;
//	bool version_outdated = false;
//	bool version_up_to_date = false;

public:
	inline explicit aux_update_session(std::shared_ptr<ServerState> server);
	~aux_update_session();

public:
	void receive(const msg::ReportVersion& report);

private:
	virtual void on_connect(error_code ec) override;
	virtual void on_receive(error_code ec, message m) override;
	virtual void on_send(error_code ec, message m) override;
	virtual void on_disconnect(error_code ec) override;
};

namespace { // -------------------------------------------------------------------------------------

static auto codec = libv::serial::CodecServer<aux_update_session, libv::archive::Binary>{msg{}};

} // namespace -------------------------------------------------------------------------------------

class ServerState {
	std::mutex mutex;
//	std::unordered_set<libv::net::mtcp::Connection<aux_update_session>> sessions; // <<< hash_set
	std::set<libv::net::mtcp::Connection<aux_update_session>> sessions;

public:
	std::shared_ptr<UpdateInfoDictionary> dictionary;

public:
	explicit ServerState(std::shared_ptr<UpdateInfoDictionary> dictionary) :
		dictionary(std::move(dictionary)) {}

	~ServerState() {
		log_update.info("~ServerState");
	}

public:
	auto make_response(const msg::ReportVersion& report) {
		const auto lock = std::unique_lock(mutex);

		return dictionary->get_update_route(report.program, report.variant, report.version);
	}

	void join(libv::net::mtcp::Connection<aux_update_session> session) {
		const auto lock = std::unique_lock(mutex);

		log_update.info("join");
		sessions.insert(session);
	}

	void leave(libv::net::mtcp::Connection<aux_update_session> session) {
		const auto lock = std::unique_lock(mutex);

		log_update.info("leave");
		sessions.erase(session);
	}

	void disconnect_all() {
		const auto lock = std::unique_lock(mutex);

		for (const auto& session : sessions)
			session.connection().cancel_and_disconnect_async();
	}
};

// -------------------------------------------------------------------------------------------------

inline aux_update_session::aux_update_session(std::shared_ptr<ServerState> server) :
	server(std::move(server)) {
}

aux_update_session::~aux_update_session() {
	log_update.info("~aux_update_session");
}

void aux_update_session::receive(const msg::ReportVersion& report) {
	const auto response = server->make_response(report);
//	const auto message = std::visit([](auto& r) { return codec.encode(r); }, response);

	const auto tmp = std::visit([](auto& r) { return codec.encode(r); }, response);
	const auto message = std::string(
			reinterpret_cast<const char*>(tmp.data()),
			reinterpret_cast<const char*>(tmp.data() + tmp.size())
	);
//	const auto message = codec.encode(response);
//	const auto tmp = codec.encode(response); // <<< Implement network/serial std::byte support
//	const auto message = std::string(
//			reinterpret_cast<const char*>(tmp.data()),
//			reinterpret_cast<const char*>(tmp.data() + tmp.size())
//	);

	log_update.debug("session response:\n{}", libv::hex_dump_with_ascii(message)); // <<< hex_dump_with_ascii

	connection.send_async(message);
	connection.disconnect_async(); // Disconnect right after we send the response
}

void aux_update_session::on_connect(error_code ec) {
//		if (ec) {
//			error = ec;
//			latch.raise();
//		}
	if (!ec)
		server->join(connection_from_this());
}

void aux_update_session::on_receive(error_code ec, message m) {
	log_update.debug("session report:\n{}", libv::hex_dump_with_ascii(m)); // <<< hex_dump_with_ascii

	if (!ec)
		codec.decode(*this, m);
}

void aux_update_session::on_send(error_code ec, message m) {
}

void aux_update_session::on_disconnect(error_code ec) {
//		error = ec;
//		latch.raise();
	server->leave(connection_from_this());
}

// -------------------------------------------------------------------------------------------------

class aux_update_server : public libv::net::mtcp::AcceptorHandler<aux_update_server> {
private:
	std::shared_ptr<ServerState> server;

public:
	inline explicit aux_update_server(std::shared_ptr<UpdateInfoDictionary> dictionary) :
		server(std::make_shared<ServerState>(std::move(dictionary))) {}

	~aux_update_server() {
		log_update.info("~aux_update_server, Terminating every connection...");
		server->disconnect_all();
	}

private:
	virtual libv::net::mtcp::Connection<> on_accept(io_context& io_context) override {
//		log_update.info("on_accept");
		return libv::net::mtcp::Connection<aux_update_session>(io_context, server);
	}

	virtual void on_accept(error_code ec) override {
		log_update.error("Update server accept failure: {}", libv::net::to_string(ec));
	}
};

// -------------------------------------------------------------------------------------------------

struct ImplUpdateNetworkServer {
	libv::net::mtcp::Acceptor<aux_update_server> acceptor;

	inline explicit ImplUpdateNetworkServer(libv::net::IOContext& io_context, std::shared_ptr<UpdateInfoDictionary> dictionary) :
		acceptor(io_context, std::move(dictionary)) {}
};

// -------------------------------------------------------------------------------------------------

UpdateNetworkServer::UpdateNetworkServer(libv::net::IOContext& io_context, std::shared_ptr<UpdateInfoDictionary> dictionary) :
	self(std::make_unique<ImplUpdateNetworkServer>(io_context, std::move(dictionary))) {
}

UpdateNetworkServer::~UpdateNetworkServer() {
	log_update.info("~UpdateNetworkServer");
	// For the sake of forward declared unique_ptr
}

void UpdateNetworkServer::listen(libv::net::mtcp::Endpoint endpoint, int backlog) {
	if (auto ec = self->acceptor->acceptor.listen(endpoint, backlog))
		throw std::system_error(ec);
}

void UpdateNetworkServer::start() {
	self->acceptor->acceptor.accept_async();
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
