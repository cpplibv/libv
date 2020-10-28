// Project: libv.net, File: src/libv/net/mtcp/acceptor.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/net/fwd.hpp>
// std
#include <functional>
#include <memory>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

class AcceptorAsyncCB {
public:
	using Connection = ConnectionAsnycCB;

public:
	using CBAccept = std::function<void(Connection)>;
	using CBError = std::function<void(const std::error_code)>;

private:
	std::shared_ptr<class ImplAcceptorAsyncCB> impl;

public:
	explicit AcceptorAsyncCB(IOContext& io_context);

public:
	/// Handlers must be set before any other function call
	void handle_accept(CBAccept callback) noexcept;
	void handle_error(CBError callback) noexcept;

public:
	/// Sets up the port and
	/// Its (only) required to call \c start if the connection object was constructed with a socket external object.
	/// @thread safe
	void listen(uint16_t port, int backlog = 4) noexcept;
	void accept() noexcept;
	void cancel() noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
