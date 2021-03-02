// Project: libv.update, File: src/libv/update/common/client.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/address.hpp>
#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/connection_cb.hpp>
// std
#include <string>


namespace app {

// -------------------------------------------------------------------------------------------------

struct UpdateClient {
	libv::net::IOContext io_context{1};
//	libv::net::mtcp::ConnectionAsyncCB connection{io_context};

public:
	explicit UpdateClient();
	~UpdateClient();

public:
	void update_from(libv::net::Address server_address);
	void send(std::string m);
};

// -------------------------------------------------------------------------------------------------

} // namespace
