// Project: libv.net, File: src/libv/net/mtcp/acceptor_he.hpp

#pragma once

// fwd
#include <libv/net/fwd.hpp>
// std
#include <atomic>
#include <cassert>
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

/// Usage:
///		UserHandler type shall derive from AcceptorHandler<CRTP>
///		An acceptor can be created with an Acceptor<UserHandler> object
///		To start the acceptor listen() then accept_async() should be called
///
/// Acceptor Invariants:
/// - The handler type is kept alive until there is a Acceptor<> object or an async operation associated with it
/// - When the last Connection<> reference destroyed an async cancellation and close is requested
/// - Every on_* function is called on a connection strand by the io context's executor
/// - No on_* function is called before accept_async
///
/// - The connection returned by on_accept is started with a connect_sync call by the acceptor

// -------------------------------------------------------------------------------------------------

class BaseAcceptorHandler;

// -------------------------------------------------------------------------------------------------

class BaseAcceptorAsyncHE {
	friend class BaseAcceptorHandler;

private:
	std::shared_ptr<class ImplBaseAcceptorAsyncHE> internals;

public:
	explicit BaseAcceptorAsyncHE(IOContext& io_context);
	~BaseAcceptorAsyncHE();

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

	virtual ~BaseAcceptorHandler();

private:
	inline void inject_handler(std::unique_ptr<BaseAcceptorHandler>&& handler) noexcept {
		acceptor.inject_handler(std::move(handler));
	}

	inline void increment_ref_count() noexcept {
		++ref_count;
		// TODO P1: Figure out what should happen with revived handlers
		//		if (handler->ref_count() == 1)
		//			log_net.error("Attempting to revive an abounded connection handler");
		//		| has to be hard error (unless if its after a new object and we are after the ctor)
		//		| Revive can happen when from inside the on_accept(ec)
		//  		last ref -> cancel -> on_accept(ec) -> acceptor_from_this
	}

	inline void decrement_ref_count() noexcept {
		if (--ref_count == 0) {
			// abandon_handler will discard this handler object by committing indirect suicide
			acceptor.abandon_handler();
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
	inline Acceptor<CRTP> acceptor_from_this() noexcept {
		return Acceptor<CRTP>(this, static_cast<CRTP*>(this));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv

namespace std { // ---------------------------------------------------------------------------------

template <typename T>
struct hash<libv::net::mtcp::Acceptor<T>> {
	static constexpr auto operator()(const libv::net::mtcp::Acceptor<T>& t) noexcept {
		const auto& he = static_cast<const libv::net::mtcp::detail::HandlerGuard<libv::net::mtcp::BaseAcceptorHandler, T>&>(t);
		return std::hash<std::remove_cvref_t<decltype(he)>>{}(he);
	}
};

} // namespace std ---------------------------------------------------------------------------------
