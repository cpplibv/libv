// Project: libv.net, File: src/libv/net/mtcp/connection_he.hpp

#pragma once

// fwd
#include <libv/net/fwd.hpp>
// std
#include <atomic>
#include <cassert>
#include <memory>
#include <system_error>
// pro
#include <libv/net/address.hpp>
#include <libv/net/detail/current_io_context.hpp>
#include <libv/net/detail/handler_guard.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/mtcp/message.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

/// Usage:
///		UserHandler type shall derive from ConnectionHandler<CRTP>
///		A connection can be created with a Connection<UserHandler> object
///
/// Connection Invariants:
/// - The handler type is kept alive until there is a Connection<> object or an async operation associated with it
/// - When the last Connection<> reference destroyed an async cancellation and disconnect is requested
/// - Every on_* function is called on a connection strand by the io context's executor
/// - No on_* function is called before connect_*sync is called (acceptor internally calls connect_sync on the returned connection)
///
/// - on_connect will always be the first function to be called
/// - on_disconnect will always be the last function to be called
/// - both on_connect and on_disconnect called at most once
/// - on_disconnect will be called if there was a on_connect call without an error

// -------------------------------------------------------------------------------------------------

class BaseConnectionHandler;

// -------------------------------------------------------------------------------------------------

/// @thread safe
class BaseConnectionAsyncHE {
	friend class BaseConnectionHandler;

private:
	std::shared_ptr<class ImplBaseConnectionAsyncHE> internals;

public:
	explicit BaseConnectionAsyncHE(IOContext& io_context) noexcept;

private:
	void inject_handler(std::unique_ptr<BaseConnectionHandler>&& handler) noexcept;
	void abandon_handler() noexcept;

public:
	inline BaseConnectionAsyncHE(const BaseConnectionAsyncHE& orig) noexcept = default;
	inline BaseConnectionAsyncHE(BaseConnectionAsyncHE&& orig) noexcept = default;
	inline BaseConnectionAsyncHE& operator=(const BaseConnectionAsyncHE& orig) & noexcept = default;
	inline BaseConnectionAsyncHE& operator=(BaseConnectionAsyncHE&& orig) & noexcept = default;

public:
	inline ~BaseConnectionAsyncHE() noexcept = default;

	/// Can only return false if the connection was moved out from the current object
	[[nodiscard]] explicit inline operator bool() const noexcept {
		return internals != nullptr;
	}

public:
	void read_limit(size_t bytes_per_second) noexcept;
	void write_limit(size_t bytes_per_second) noexcept;

	[[nodiscard]] size_t num_byte_read() const noexcept;
	[[nodiscard]] size_t num_byte_wrote() const noexcept;

	[[nodiscard]] size_t num_message_received() const noexcept;
	[[nodiscard]] size_t num_message_sent() const noexcept;

	[[nodiscard]] size_t send_queue_size() const noexcept;

public:
	/// Queues an asynchronous connect task by adopting an already open socket.
	void connect_async(detail::Socket&& socket) noexcept;

	/// Connects synchronously by adopting an already open socket.
	void connect_sync(detail::Socket&& socket) noexcept;

	/// Queues an asynchronous connect task.
	void connect_async(Address address) noexcept;

	/// Queues an asynchronous disconnect task. Implicitly calls pause_receive_async
	void disconnect_async() noexcept;

	/// Calls disconnect_async if connection is connected (or connecting)
	void disconnect_if_connected_async() noexcept;

	/// Disconnects asynchronous as soon as possible canceling any pending async task
	void cancel_and_disconnect_async() noexcept;

	/// Pause the repeating asynchronous receive task.
	/// This operation does not cancels or pauses any already started receive operation
	void pause_receive_async() noexcept;

	/// Resumes the repeating asynchronous receive task.
	void resume_receive_async() noexcept;

	/// Queues an asynchronous send task. Moves the storage in
	void send_async(message_body_bin message) noexcept;
	/// Queues an asynchronous send task. Copies the data into an internal storage
	void send_async(message_body_bin_view message) noexcept;
	/// Queues an asynchronous send task. Moves the storage in
	void send_async(message_body_str message) noexcept;
	/// Queues an asynchronous send task. Copies the data into an internal storage
	void send_async(message_body_str_view message) noexcept;

	/// Queues an asynchronous send task with external reference to memory.
	/// Management of the memory is the responsibility of the caller
	void send_view_async(message_body_bin_view message) noexcept;
	/// Queues an asynchronous send task with external reference to memory.
	/// Management of the memory is the responsibility of the caller
	void send_view_async(message_body_str_view message) noexcept;
};

// -------------------------------------------------------------------------------------------------

class BaseConnectionHandler {
private:
	template <typename T1, typename T2> friend class detail::HandlerGuard;
	friend class ImplBaseAcceptorAsyncHE;
	friend class ImplBaseConnectionAsyncHE;

public:
	using error_code = std::error_code;
	using message_view = message_body_view;
	using io_context = IOContext;

private:
	std::atomic<uint32_t> ref_count = 0;

public:
	BaseConnectionAsyncHE connection;

public:
	explicit BaseConnectionHandler() :
		connection(*libv::net::detail::current_io_context) {}
	virtual ~BaseConnectionHandler() = default;

private:
	inline void inject_handler(std::unique_ptr<BaseConnectionHandler>&& handler) noexcept {
		connection.inject_handler(std::move(handler));
	}

	inline void increment_ref_count() noexcept {
		++ref_count;
		// TODO P4: Figure out what should happen with revived handlers
		//		if (handler->ref_count() == 1)
		//			log_net.error("Attempting to revive an abounded connection handler");
		//		| has to be hard error (unless if its after a new object and we are after the ctor)
	}

	inline void decrement_ref_count() noexcept {
		if (--ref_count == 0) {
			connection.abandon_handler();
			connection.disconnect_if_connected_async();
		}
	}

private:
	virtual void on_connect(error_code ec) = 0;
	virtual void on_receive(error_code ec, message_view m) = 0;
	virtual void on_send(error_code ec, message_view m) = 0;
	virtual void on_disconnect(error_code ec) = 0;

//	virtual void on_connect() = 0;
//	virtual void on_connect(error_code) {}
//	virtual void on_receive(message m) = 0;
//	virtual void on_receive(error_code) {}
//	virtual void on_send(message m) = 0;
//	virtual void on_send(error_code) {}
//	/// Receives the first error that occurred which resulted in the disconnect if there was any
//	/// !!! ^ Match this comment in implementation
//	virtual void on_disconnect(error_code ec) = 0;
};

// -------------------------------------------------------------------------------------------------

template <typename T = void>
class Connection : public detail::HandlerGuard<BaseConnectionHandler, T> {
public:
	using detail::HandlerGuard<BaseConnectionHandler, T>::HandlerGuard;

public:
	auto& connection() const {
		return this->handler().connection;
	}
};

template <typename CRTP>
class ConnectionHandler : public BaseConnectionHandler {
protected:
	inline Connection<CRTP> connection_from_this() noexcept {
		return Connection<CRTP>(this, static_cast<CRTP*>(this));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv

namespace std { // ---------------------------------------------------------------------------------

template <typename T>
struct hash<libv::net::mtcp::Connection<T>> {
	constexpr auto operator()(const libv::net::mtcp::Connection<T>& t) const noexcept {
		const auto& he = static_cast<const libv::net::mtcp::detail::HandlerGuard<libv::net::mtcp::BaseConnectionHandler, T>&>(t);
		return std::hash<std::remove_cvref_t<decltype(he)>>{}(he);
	}
};

} // namespace std ---------------------------------------------------------------------------------
