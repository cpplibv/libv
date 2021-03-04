// Project: libv.update, File: src/libv/update/common/client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/address.hpp>
#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/connection_cb.hpp>
// std
#include <string>
// pro
#include <libv/update/client/update_check_result.hpp>
#include <libv/update/net/protocol.hpp>
#include <libv/update/version_number.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class UpdaterNetworkClient {
private:
	libv::net::IOContext& io_context;
	libv::net::Address server_address;
//	libv::net::mtcp::ConnectionAsyncCB connection{io_context};

	msg::UpdateInfo update_info_;

public:
	UpdaterNetworkClient(libv::net::IOContext& ioContext, libv::net::Address serverAddress);
//	~UpdaterNetworkClient();

public:
	[[nodiscard]] update_check_result check_version(std::string program_name, std::string program_variant, version_number current_version);
	[[nodiscard]] inline const msg::UpdateInfo& update_info() const noexcept {
		return update_info_;
	}

//	void update_from();
//	void send(std::string m);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
