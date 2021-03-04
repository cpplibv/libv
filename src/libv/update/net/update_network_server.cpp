// Project: libv.net, File: src/libv/update/net/updater_network_server.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/net/update_network_server.hpp>
// libv
#include <libv/net/mtcp/acceptor_he.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/codec.hpp>
#include <libv/utility/hex_dump.hpp> // <<<
//#include <libv/mt/binary_latch.hpp>
//#include <libv/net/error.hpp>
//#include <libv/log/log.hpp>
//#include <libv/net/address.hpp>
//#include <libv/net/error.hpp>
//#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/connection_he.hpp>
// std
#include <iostream>
#include <mutex>
#include <set>
// pro
#include <libv/update/log.hpp>
#include <libv/update/net/protocol.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct ServerUpdateInformation {
	version_number version_source;
	version_number version_target;
	uint64_t size;
	uint64_t signature;
};

struct ProgramVersionInformation {
	std::string variant;
	std::string program;
	version_number version;

//	std::vector<ServerUpdateInformation*> updates_sources;
//	std::vector<ServerUpdateInformation*> updates_targets;
//	ServerUpdateInformation* update_route;
};

struct ProgramVariant {
	version_number latest_version;
	std::unordered_map<version_number, ProgramVersionInformation> version_informations;
};

struct Program {
	std::unordered_map<std::string, ProgramVariant> variants;
};

// -------------------------------------------------------------------------------------------------

class SomeServerObject;

class aux_update_session : public libv::net::mtcp::ConnectionHandler<aux_update_session> {
public:
	std::shared_ptr<SomeServerObject> server;
//	libv::mt::binary_latch latch;
//	error_code error;
//	std::string response;
//	msg::UpdateInfo info;
//
//	bool version_not_supported = false;
//	bool version_outdated = false;
//	bool version_up_to_date = false;

public:
	inline explicit aux_update_session(std::shared_ptr<SomeServerObject> server);

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

class SomeServerObject {
	std::mutex mutex;
//	std::unordered_set<libv::net::mtcp::Connection<aux_update_session>> sessions; // <<< hash_set
	std::set<libv::net::mtcp::Connection<aux_update_session>> sessions;
	std::unordered_map<std::string, Program> programs;

public:
	auto make_response(const msg::ReportVersion& report) {
		const auto lock = std::unique_lock(mutex);

		auto pit = programs.find(report.program);
		if (pit == programs.end())
			return msg::VersionNotSupported{};

//		auto& variants = pit->second.variants;
//		auto vit = variants.find(report.variant);
//		if (vit == variants.end())
//			return msg::VersionNotSupported{};
//
//		auto& variant = vit->second;
//		if (variant.latest_version == report.version)
//			return msg::VersionUpToDate{};
//
//		msg::UpdateInfo response;
//		response.program = report.program;
//		response.variant = report.variant;
////		for (const auto& update_step : variant.update_route)
////			response.updates.emplace_back(
////					update_step.version_source,
////					update_step.version_target,
////					update_step.size,
////					update_step.signature
////			);
//
//		return response;
		return msg::VersionNotSupported{};
	}

	void join(libv::net::mtcp::Connection<aux_update_session> session) {
		const auto lock = std::unique_lock(mutex);

		sessions.insert(std::move(session));
	}

	void leave(libv::net::mtcp::Connection<aux_update_session> session) {
		const auto lock = std::unique_lock(mutex);

		sessions.erase(std::move(session));
	}

	void disconnect_all() {
		const auto lock = std::unique_lock(mutex);

		for (const auto& session : sessions)
			session.connection().cancel_and_disconnect_async();
	}
};

// -------------------------------------------------------------------------------------------------

inline aux_update_session::aux_update_session(std::shared_ptr<SomeServerObject> server):
	server(server) {
}

void aux_update_session::receive(const msg::ReportVersion& report) {
	const auto response = server->make_response(report);

//	const auto message = codec.encode(response);
	const auto tmp = codec.encode(response); // <<< Implement network/serial std::byte support
	const auto message = std::string(
			reinterpret_cast<const char*>(tmp.data()),
			reinterpret_cast<const char*>(tmp.data() + tmp.size())
	);

	log_update.debug("session response:\n{}", libv::hex_dump_with_ascii(message)); // <<<

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
	log_update.debug("session report:\n{}", libv::hex_dump_with_ascii(m)); // <<<

	if (!ec)
//		codec.decode(*this, m);
		codec.decode(*this, std::span<const std::byte>( // <<< Implement network/serial std::byte support
				reinterpret_cast<const std::byte*>(m.data()),
				m.size()
		));
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
	std::shared_ptr<SomeServerObject> server = std::make_shared<SomeServerObject>();

public:
	inline explicit aux_update_server(uint16_t port) { // <<< endpoint
		if (auto ec = acceptor.listen(port, 4))
			throw std::system_error(ec);

		acceptor.accept_async();
	}

	~aux_update_server() {
//		DEBUG_COUT("[Server] ~aux_update_server");
		log_update.info("~aux_update_server, Terminating every connection");
		server->disconnect_all();
	}

private:
	virtual libv::net::mtcp::Connection<> on_accept(io_context& io_context) override {
//		log_update.info("on_accept");
		return libv::net::mtcp::Connection<aux_update_session>(io_context, server);
	}
//	virtual void on_accept(error_code ec) override {
//		DEBUG_COUT("[Server] on_accept error " << libv::net::to_string(ec));
//	}
};

// -------------------------------------------------------------------------------------------------

UpdateNetworkServer::UpdateNetworkServer(net::IOContext& io_context) :
	io_context(io_context) {
}

UpdateNetworkServer::~UpdateNetworkServer() {
	// <<< close_all_connection and cancel
}

void UpdateNetworkServer::listen(libv::net::mtcp::Endpoint endpoint, int backlog) {
	acceptor = std::make_shared<libv::net::mtcp::Acceptor<aux_update_server>>(io_context, endpoint.port);
}

// -------------------------------------------------------------------------------------------------

//update_check_result UpdateNetworkServer::check_version(std::string program_name, std::string program_variant, version_number current_version) {
//	const auto report = msg::ReportVersion(program_name, program_variant, current_version);
//
////	const auto message = codec.encode(report);
//	const auto tmp = codec.encode(report); // <<< Implement network/serial std::byte support
//	const auto message = std::string(
//			reinterpret_cast<const char*>(tmp.data()),
//			reinterpret_cast<const char*>(tmp.data() + tmp.size())
//	);
//
//	log_update.debug("Report: \n{}", libv::hex_dump_with_ascii(message)); // <<<
//
//	const auto connection = libv::net::mtcp::Connection<aux_UpdateNetworkServer>(io_context, server_address, message);
//	// No need to limit for version checking
//	//	connection.read_limit(100); // bytes per second
//	//	connection.write_limit(100); // bytes per second
//
//	connection->latch.wait();
//
//	if (connection->error)
//		return update_check_result::communication_error;
//
//	log_update.debug("Response: \n{}", libv::hex_dump_with_ascii(connection->response)); // <<<
//
////	codec.decode(*connection, connection->response);
//	codec.decode(*connection, std::span<const std::byte>( // <<< Implement network/serial std::byte support
//			reinterpret_cast<const std::byte*>(connection->response.data()),
//			connection->response.size()
//	));
//
//	if (connection->version_up_to_date)
//		return update_check_result::version_up_to_date;
//
//	if (connection->version_not_supported)
//		return update_check_result::version_not_supported;
//
//	if (connection->version_not_supported)
//		return update_check_result::version_outdated;
//}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
