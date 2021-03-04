// Project: libv.update, File: src/libv/update/common/session.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/net/mtcp/connection_cb.hpp>
// std
#include <string>


namespace app {

// -------------------------------------------------------------------------------------------------

struct UpdateSession {
//	libv::net::mtcp::ConnectionAsyncCB connection;
	class UpdateServer& server;

public:
	// Session memory address is leaked to callbacks, so address stability is required
	UpdateSession(const UpdateSession& orig) noexcept = delete;
	UpdateSession(UpdateSession&& orig) noexcept = delete;
	UpdateSession& operator=(const UpdateSession& orig) & noexcept = delete;
	UpdateSession& operator=(UpdateSession&& orig) & noexcept = delete;

public:
//	explicit UpdateSession(UpdateServer& server, libv::net::mtcp::ConnectionAsyncCB connection_);
	~UpdateSession();

public:
	void send(const std::string& message);
	void kick();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
