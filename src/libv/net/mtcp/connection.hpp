// Project: libv.net, File: src/libv/net/mtcp/connection.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/net/fwd.hpp>
// std
#include <functional>
#include <memory>
#include <system_error>
// pro
#include <libv/net/address.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/mtcp/message.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

// TODO P2: Document public api precondition (handlers must be set, list the possible logic error states)

// -------------------------------------------------------------------------------------------------

class ConnectionAsnycCB {
	friend class ImplAcceptorAsyncCB;

public:
	enum class ErrorSource {
		logic,

		connect,
		receive,
		send,
	};

public:
	using CBConnect = std::function<void(Endpoint, Endpoint)>;
	using CBDisconnect = std::function<void()>;
	using CBError = std::function<void(ErrorSource, std::error_code)>;
	using CBRecive = std::function<void(Message&&)>;
	using CBSend = std::function<void(Message&&)>;

private:
	std::shared_ptr<class ImplConnectionAsnycCB> impl;

public:
	explicit ConnectionAsnycCB(IOContext& io_context) noexcept;

private:
	/// Constructor to be used with an acceptor
	ConnectionAsnycCB(IOContext& io_context, Socket&& socket) noexcept;

public:
	/// Handlers must be set before any other function call
	void handle_connect(CBConnect callback) noexcept;
	void handle_disconnect(CBDisconnect callback) noexcept;
	void handle_error(CBError callback) noexcept;
	void handle_receive(CBRecive callback) noexcept;
	void handle_send(CBSend callback) noexcept;

public:
	void read_limit(size_t bytes_per_second) noexcept;
	void write_limit(size_t bytes_per_second) noexcept;

	[[nodiscard]] size_t total_read_bytes() const noexcept;
	[[nodiscard]] size_t total_write_bytes() const noexcept;

	[[nodiscard]] size_t total_read_messages() const noexcept;
	[[nodiscard]] size_t total_write_messages() const noexcept;

public:
	/// Queues an asynchronous start task.
	/// Required to be called if the connection object was created by an acceptor.
	///	Requires every handler to be set.
	/// @thread safe
	void start() noexcept;

	/// Queues an asynchronous connect task.
	/// @thread safe
	void connect(Address address) noexcept;

	/// Queues an asynchronous disconnect task.
	/// @thread safe
	void disconnect() noexcept;

	/// Queues \c count asynchronous receive task.
	/// @thread safe
	void receive(int count = 1) noexcept;

	/// Queues a repeating asynchronous receive task.
	/// @thread safe
	void receive_repeat() noexcept;

	/// Stops the repeating asynchronous receive task.
	/// This operation does not cancels any already started receive operation
	/// @thread safe
	void receive_stop() noexcept;

	/// Cancels every asynchronous receive task.
	/// This operation will cancel any already started receive operation and may yield operation_aborted error
	/// @thread safe
	void receive_cancel() noexcept;

	/// Queues an asynchronous send task.
	/// @thread safe
	void send(Message message) noexcept;

	/// Cancels every asynchronous send task.
	/// This operation will cancel any already started send operation and may yield operation_aborted error
	/// @thread safe
	void send_cancel() noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
