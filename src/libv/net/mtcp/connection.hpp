// Project: libv.net, File: src/libv/net/mtcp/connection.hpp, Author: Császár Mátyás [Vader]

#pragma once

#include <libv/utility/storage_size.hpp>
// std
#include <functional>
#include <memory>
#include <string>
#include <system_error>
// pro
#include <libv/net/address.hpp>
#include <libv/net/fwd.hpp>
#include <libv/net/mtcp/endpoint.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

//using Message = std::vector<std::byte>; // TODO P2: Switch to std::vector<std::byte>
using Message = std::string;
using PacketHeader = uint32_t;

constexpr auto MTCP_MESSAGE_MAX_RESERVE = libv::MB(1);
constexpr auto MTCP_MESSAGE_MAX_SIZE = std::numeric_limits<PacketHeader>::max();

// -------------------------------------------------------------------------------------------------

class ConnectionAsnycCB {
	using Impl = detail::ImplConnectionAsnycCB;
	using CBConnect = std::function<void(const Endpoint)>;
	using CBDisconnect = std::function<void()>;
	using CBError = std::function<void(const std::error_code)>;
	using CBRecive = std::function<void(Message&&)>;
	using CBSend = std::function<void(Message&&)>;

	std::shared_ptr<Impl> impl;

public:
	ConnectionAsnycCB(IOContext& io_context) noexcept;
	ConnectionAsnycCB(IOContext& io_context, Socket&& socket) noexcept;

public:
	/// Handlers must be set before any other function call
	void handle_connect(CBConnect callback) noexcept;
	void handle_disconnect(CBDisconnect callback) noexcept;
	void handle_error(CBError callback) noexcept;
	void handle_receive(CBRecive callback) noexcept;
	void handle_send(CBSend callback) noexcept;

public:
	/// Queues an asynchronous start task.
	/// Its (only) required to call \c start if the connection object was constructed with a socket external object.
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
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
