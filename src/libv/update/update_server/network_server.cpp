// Project: libv.update, File: src/libv/update/update_server/network_server.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/update_server/network_server.hpp>
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
#include <unordered_set>
// pro
//#include <libv/update/common/protocol.hpp>
#include <libv/update/log.hpp>
#include <libv/update/update_server/update_info_database.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class ServerState;

class aux_update_session : public libv::net::mtcp::ConnectionHandler<aux_update_session> {
public:
	std::shared_ptr<ServerState> server;

public:
	inline explicit aux_update_session(std::shared_ptr<ServerState> server);
	~aux_update_session();

public:
	void receive(const msg_upd::ReportVersion& report);

private:
	virtual void on_connect(error_code ec) override;
	virtual void on_receive(error_code ec, message m) override;
	virtual void on_send(error_code ec, message m) override;
	virtual void on_disconnect(error_code ec) override;
};

namespace { // -------------------------------------------------------------------------------------

static auto codec = libv::serial::CodecServer<aux_update_session, libv::archive::Binary>{msg_upd{}};

} // namespace -------------------------------------------------------------------------------------

class ServerState {
	std::mutex mutex;
	std::unordered_set<libv::net::mtcp::Connection<aux_update_session>> sessions;

public:
	std::shared_ptr<UpdateInfoDatabase> database;

public:
	explicit ServerState(std::shared_ptr<UpdateInfoDatabase> database) :
		database(std::move(database)) {}

	~ServerState() {
		log_update.fatal("~ServerState");
	}

public:
	auto make_response(const msg_upd::ReportVersion& report) {
		const auto lock = std::unique_lock(mutex);

		return database->get_update_route(report.program, report.variant, report.version);
	}

	void join(libv::net::mtcp::Connection<aux_update_session> session) {
		const auto lock = std::unique_lock(mutex);

		log_update.fatal("join");
		sessions.insert(session);
	}

	void leave(libv::net::mtcp::Connection<aux_update_session> session) {
		const auto lock = std::unique_lock(mutex);

		log_update.fatal("leave");
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
	log_update.fatal("~aux_update_session");
}

void aux_update_session::receive(const msg_upd::ReportVersion& report) {
	const auto response = server->make_response(report);
	const auto message = std::visit([](auto& r) { return codec.encode(r); }, response);

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
	inline explicit aux_update_server(std::shared_ptr<UpdateInfoDatabase> database) :
		server(std::make_shared<ServerState>(std::move(database))) {}

	~aux_update_server() {
		log_update.info("Destroying update server: Terminating every connection...");
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

	inline explicit ImplUpdateNetworkServer(libv::net::IOContext& io_context, std::shared_ptr<UpdateInfoDatabase> database) :
		acceptor(io_context, std::move(database)) {}
};

// -------------------------------------------------------------------------------------------------

UpdateNetworkServer::UpdateNetworkServer(libv::net::IOContext& io_context, std::shared_ptr<UpdateInfoDatabase> database) :
	self(std::make_unique<ImplUpdateNetworkServer>(io_context, std::move(database))) {
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
