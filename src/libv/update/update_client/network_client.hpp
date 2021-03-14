// Project: libv.update, File: src/libv/update/update_client/network_client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/address.hpp>
#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/connection_cb.hpp>
// std
#include <string>
// pro
#include <libv/update/update_client/update_check_result.hpp>
#include <libv/update/common/protocol_upd.hpp>
#include <libv/update/version_number.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class UpdateNetworkClient {
private:
	libv::net::IOContext& io_context;
	libv::net::Address server_address;
//	libv::net::mtcp::ConnectionAsyncCB connection{io_context};

	msg_upd::UpdateRoute update_info_;

public:
	UpdateNetworkClient(libv::net::IOContext& ioContext, libv::net::Address serverAddress);

public:
	[[nodiscard]] update_check_result check_version(std::string program_name, std::string program_variant, version_number current_version);
	[[nodiscard]] inline const msg_upd::UpdateRoute& update_info() const noexcept {
		return update_info_;
	}
};

// -------------------------------------------------------------------------------------------------

//struct update_info {
//  update_check_result outcome;
//  msg::UpdateRoute info;
//};
//
//[[nodiscard]] update_info download_update_info(libv::net::IOContext& io_context, libv::net::Address server,
//		std::string_view program, std::string_view variant, version_number version);

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
