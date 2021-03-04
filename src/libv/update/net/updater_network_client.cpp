// Project: libv.update, File: src/libv/update/common/client.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/net/updater_network_client.hpp>
// libv
#include <libv/mt/binary_latch.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/codec.hpp>
#include <libv/utility/hex_dump.hpp> // <<<
//#include <libv/net/error.hpp>
// std
#include <iostream>
// pro
#include <libv/update/log.hpp>
#include <libv/update/net/protocol.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class aux_UpdaterNetworkClient : public libv::net::mtcp::ConnectionHandler<aux_UpdaterNetworkClient> {
public:
	libv::mt::binary_latch latch;
	error_code error;
	std::string response;
	msg::UpdateInfo info;

	bool version_not_supported = false;
	bool version_outdated = false;
	bool version_up_to_date = false;

public:
	inline explicit aux_UpdaterNetworkClient(libv::net::Address address, std::string message) {
		connection.connect_async(address);
		connection.send_async(message);
	}

public:
	void receive(const msg::VersionNotSupported&) {
		version_not_supported = true;
	}
	void receive(msg::UpdateInfo info_) {
		version_outdated = true;
		info = std::move(info_);
	}
	void receive(const msg::VersionUpToDate&) {
		version_up_to_date = true;
	}

private:
	virtual void on_connect(error_code ec) override {
		if (ec) {
			error = ec;
			latch.raise();
		}
	}

	virtual void on_receive(error_code ec, message m) override {
		if (!ec) {
			response = m;
			connection.disconnect_async();
		}
	}

	virtual void on_send(error_code ec, message m) override {
	}

	virtual void on_disconnect(error_code ec) override {
		error = ec;
		latch.raise();
	}
};

namespace { // -------------------------------------------------------------------------------------

static auto codec = libv::serial::CodecClient<aux_UpdaterNetworkClient, libv::archive::Binary>{msg{}};

} // namespace -------------------------------------------------------------------------------------

UpdaterNetworkClient::UpdaterNetworkClient(net::IOContext& ioContext, net::Address serverAddress) :
	io_context(ioContext),
	server_address(std::move(serverAddress)) {}

update_check_result UpdaterNetworkClient::check_version(std::string program_name, std::string program_variant, version_number current_version) {
	const auto report = msg::ReportVersion(program_name, program_variant, current_version);

	const auto tmp = codec.encode(report);
	const auto message = std::string(
			reinterpret_cast<const char*>(tmp.data()),
			reinterpret_cast<const char*>(tmp.data() + tmp.size())
	);

	log_update.debug("Report: \n{}", libv::hex_dump_with_ascii(message)); // <<<

	const auto connection = libv::net::mtcp::Connection<aux_UpdaterNetworkClient>(io_context, server_address, message);
	//	connection.read_limit(100); // bytes per second
	//	connection.write_limit(100); // bytes per second

	connection->latch.wait();

	if (connection->error)
		return update_check_result::communication_error;

	log_update.debug("Response: \n{}", libv::hex_dump_with_ascii(connection->response)); // <<<

	codec.decode(*connection, std::span<const std::byte>(
			reinterpret_cast<const std::byte*>(connection->response.data()),
			connection->response.size()
	));

	if (connection->version_up_to_date)
		return update_check_result::version_up_to_date;

	if (connection->version_not_supported)
		return update_check_result::version_not_supported;

	if (connection->version_not_supported)
		return update_check_result::version_outdated;
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
