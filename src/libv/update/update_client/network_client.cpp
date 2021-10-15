// Project: libv.update, File: src/libv/update/update_client/network_client.cpp

// hpp
#include <libv/update/update_client/network_client.hpp>
// libv
#include <libv/mt/binary_latch.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/codec.hpp>
#include <libv/utility/hex_dump.hpp> // <<< hex_dump_with_ascii
//#include <libv/net/error.hpp>
// std
// pro
#include <libv/update/common/protocol_upd.hpp>
#include <libv/update/log.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class aux_UpdateNetworkClient : public libv::net::mtcp::ConnectionHandler<aux_UpdateNetworkClient> {
public:
	libv::mt::binary_latch latch;
	error_code error;
	std::vector<std::byte> response;
	msg_upd::UpdateRoute info;

	bool version_not_supported = false;
	bool version_outdated = false;
	bool version_up_to_date = false;

public:
	inline explicit aux_UpdateNetworkClient(libv::net::Address address, std::vector<std::byte> message) {
		connection.connect_async(std::move(address));
		connection.send_async(std::move(message));
	}

public:
	void receive(const msg_upd::VersionNotSupported&) {
		version_not_supported = true;
	}
	void receive(msg_upd::UpdateRoute info_) {
		version_outdated = true;
		info = std::move(info_);
	}
	void receive(const msg_upd::VersionUpToDate&) {
		version_up_to_date = true;
	}

private:
	virtual void on_connect(error_code ec) override {
		if (ec) {
			error = ec;
			latch.raise();
		}
	}

	virtual void on_receive(error_code ec, message_view m) override {
		if (!ec) {
			response = m.copy_bin();
			connection.disconnect_async();
		}
	}

	virtual void on_send(error_code ec, message_view m) override {
	}

	virtual void on_disconnect(error_code ec) override {
		error = ec;
		latch.raise();
	}
};

namespace { // -------------------------------------------------------------------------------------

static auto codec = libv::serial::CodecClient<aux_UpdateNetworkClient, libv::archive::Binary>{msg_upd{}};

} // namespace -------------------------------------------------------------------------------------

UpdateNetworkClient::UpdateNetworkClient(net::IOContext& ioContext, net::Address serverAddress) :
	io_context(ioContext),
	server_address(std::move(serverAddress)) {}

update_check_result UpdateNetworkClient::check_version(std::string program_name, std::string program_variant, version_number current_version) {
	const auto report = msg_upd::ReportVersion(program_name, program_variant, current_version);
	const auto message = codec.encode(report);

	log_update.debug("Report: \n{}", libv::hex_dump_with_ascii(message)); // <<< hex_dump_with_ascii

	const auto connection = libv::net::mtcp::Connection<aux_UpdateNetworkClient>(io_context, server_address, message);
	// No need to limit for version checking
	//	connection.read_limit(100); // bytes per second
	//	connection.write_limit(100); // bytes per second

	connection->latch.wait();

	if (connection->error)
		return update_check_result::communication_error;

	log_update.debug("Response: \n{}", libv::hex_dump_with_ascii(connection->response)); // <<< hex_dump_with_ascii

	codec.decode(*connection, connection->response);

	if (connection->version_up_to_date)
		return update_check_result::version_up_to_date;

	if (connection->version_not_supported)
		return update_check_result::version_not_supported;

	if (connection->version_outdated) {
		update_info_ = connection->info;
		return update_check_result::version_outdated;
	}

	assert(false && "Internal error");
	return update_check_result::communication_error;
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
