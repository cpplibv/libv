// Project: libv.net, File: src/libv/net/mtcp/acceptor.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/net/fwd.hpp>
// std
#include <atomic>
#include <cassert>
#include <concepts>
//#include <functional>
#include <memory>
#include <system_error>
// pro
#include <libv/net/detail/current_io_context.hpp>
#include <libv/net/detail/handler_guard.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/net/mtcp/endpoint.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

class BaseAcceptorHandler;

// -------------------------------------------------------------------------------------------------

class BaseAcceptorAsyncHE {
	friend class BaseAcceptorHandler;

private:
	std::shared_ptr<class ImplBaseAcceptorAsyncHE> internals;

public:
	explicit BaseAcceptorAsyncHE(IOContext& io_context);

private:
	void inject_handler(std::unique_ptr<BaseAcceptorHandler>&& handler) noexcept;
	void abandon_handler() noexcept;

public:
	/// Sets up the port to the default 0.0.0.0 address and the backlog connection count
	/// Its (only) required to call \c start if the connection object was constructed with a socket external object.
	/// @thread safe
	[[nodiscard]] std::error_code listen(uint16_t port, int backlog = 4) noexcept;
	/// Sets up the endpoint port and the backlog connection count
	/// Its (only) required to call \c start if the connection object was constructed with a socket external object.
	/// @thread safe
	[[nodiscard]] std::error_code listen(Endpoint endpoint, int backlog = 4) noexcept;

	void accept_async() noexcept;
	void cancel() noexcept;
};

// -------------------------------------------------------------------------------------------------

class BaseAcceptorHandler {
private:
	template <typename T1, typename T2> friend class detail::HandlerGuard;
	friend class ImplBaseAcceptorAsyncHE;

public:
	using error_code = std::error_code;
	using io_context = IOContext;

private:
	std::atomic<uint32_t> ref_count = 0;

public:
	BaseAcceptorAsyncHE acceptor;

public:
	explicit BaseAcceptorHandler() :
		acceptor(*libv::net::detail::current_io_context) {}
	virtual ~BaseAcceptorHandler() = default;

private:
	inline void inject_handler(std::unique_ptr<BaseAcceptorHandler>&& handler) noexcept {
		acceptor.inject_handler(std::move(handler));
	}

	inline void increment_ref_count() noexcept {
		++ref_count;
		// TODO P4: Figure out what should happen with revived handlers
		//		if (handler->ref_count() == 1)
		//			log_net.error("Reviving an abounded acceptor handler");
		//			OR
		//			handler->cancel_null_ref()
		//			OR
		//			increment_ref_count handle this case internally ! BEST
	}

	inline void decrement_ref_count() noexcept {
		if (--ref_count == 0) {
			acceptor.abandon_handler();
			acceptor.cancel();
		}
	}

private:
	virtual Connection<void> on_accept(io_context& io_context) = 0;
	virtual void on_accept(error_code) { }
};

// -------------------------------------------------------------------------------------------------

template <typename T = void>
class Acceptor : public detail::HandlerGuard<BaseAcceptorHandler, T> {
public:
	using detail::HandlerGuard<BaseAcceptorHandler, T>::HandlerGuard;

public:
	auto& acceptor() const {
		return this->handler().acceptor;
	}
};

template <typename CRTP>
class AcceptorHandler : public BaseAcceptorHandler {
protected:
	inline Acceptor<CRTP> connection_from_this() noexcept {
		return Acceptor<CRTP>(this, static_cast<CRTP*>(this));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
