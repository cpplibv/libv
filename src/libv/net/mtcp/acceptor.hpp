// Project: libv.net, File: src/libv/net/mtcp/acceptor.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/net/fwd.hpp>
#include <libv/net/mtcp/endpoint.hpp>
// std
#include <functional>
#include <memory>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

class AcceptorAsyncCB {
public:
	enum class ErrorSource {
		accept,
	};

public:
	using Connection = ConnectionAsyncCB;

public:
	using CBAccept = std::function<void(Connection)>;
	using CBError = std::function<void(ErrorSource, std::error_code)>;

private:
	std::shared_ptr<class ImplAcceptorAsyncCB> impl;

public:
	explicit AcceptorAsyncCB(IOContext& io_context);

public:
	/// Handlers must be set before any other function call
	void handle_accept(CBAccept callback) noexcept;
	void handle_error(CBError callback) noexcept;

public:
	/// Sets up the port to the default 0.0.0.0 address and the backlog connection count
	/// Its (only) required to call \c start if the connection object was constructed with a socket external object.
	/// @thread safe
	[[nodiscard]] std::error_code listen(uint16_t port, int backlog = 4) noexcept;
	/// Sets up the endpoint port and the  backlog connection count
	/// Its (only) required to call \c start if the connection object was constructed with a socket external object.
	/// @thread safe
	[[nodiscard]] std::error_code listen(Endpoint endpoint, int backlog = 4) noexcept;

	void accept() noexcept;
	void cancel() noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
