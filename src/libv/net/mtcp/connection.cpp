// Project: libv.net, File: src/libv/net/mtcp/connection.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/net/mtcp/connection.hpp>
// ext
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
//#include <netts/buffer.hpp>
//#include <netts/socket.hpp>
// libv
#include <libv/utility/byte_swap.hpp>
#include <libv/utility/enum.hpp>
// std
#include <deque>
#include <memory>
#include <mutex>
#include <system_error>
// pro
#include <libv/net/address.hpp>
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/log.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/mtcp/resolve_results.hpp>
#include <libv/net/mtcp/socket.hpp>


//// =================================================================================================
//// =================================================================================================
//// =================================================================================================
//
//
//// =================================================================================================
//// =================================================================================================
//// =================================================================================================
//
//#include <boost/beast/core/basic_stream.hpp>
//#include <boost/beast/core/rate_policy.hpp>
////#include <boost/asio/read.hpp>
//#include <boost/asio/ip/tcp.hpp>
//#include <netts/net.hpp>
//
//// -------------------------------------------------------------------------------------------------
//
///// A rate policy with simple, configurable limits on reads and writes.
/////
///// This rate policy allows for simple individual limits on the amount
///// of bytes per second allowed for reads and writes.
/////
///// @par Concepts
/////
///// @li <em>RatePolicy</em>
/////
///// @see beast::basic_stream
/////
//class libv_rate_policy {
//	friend class boost::beast::rate_policy_access;
//
//	static std::size_t constexpr all = std::numeric_limits<std::size_t>::max();
//
//private:
//	std::size_t rd_remain_ = all;
//	std::size_t wr_remain_ = all;
//	std::size_t rd_limit_ = all;
//	std::size_t wr_limit_ = all;
//
//private:
//	[[nodiscard]] std::size_t available_read_bytes() const noexcept {
//		return rd_remain_;
//	}
//
//	[[nodiscard]] std::size_t available_write_bytes() const noexcept {
//		return wr_remain_;
//	}
//
//	void transfer_read_bytes(std::size_t n) noexcept {
//		if (rd_remain_ != all)
//			rd_remain_ = (n < rd_remain_) ? rd_remain_ - n : 0;
//	}
//
//	void transfer_write_bytes(std::size_t n) noexcept {
//		if (wr_remain_ != all)
//			wr_remain_ = (n < wr_remain_) ? wr_remain_ - n : 0;
//	}
//
//	void on_timer() noexcept {
//		rd_remain_ = rd_limit_;
//		wr_remain_ = wr_limit_;
//	}
//
//public:
//	/// Set the limit of bytes per second to read, 0 means unlimited
//	void read_limit(std::size_t bytes_per_second) noexcept {
//		if (bytes_per_second == 0)
//			bytes_per_second = all;
//
//		rd_limit_ = bytes_per_second;
//		if (rd_remain_ > bytes_per_second)
//			rd_remain_ = bytes_per_second;
//	}
//
//	/// Set the limit of bytes per second to write, 0 means unlimited
//	void write_limit(std::size_t bytes_per_second) noexcept {
//		if (bytes_per_second == 0)
//			bytes_per_second = all;
//
//		wr_limit_ = bytes_per_second;
//		if (wr_remain_ > bytes_per_second)
//			wr_remain_ = bytes_per_second;
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//using mtcp_stream = boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, libv_rate_policy>;
////using mtcp_stream = boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, libv_rate_policy>;
//
//
//
//
////using mtcp_stream = boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, libv_rate_policy>;
////using mtcp_stream = boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, libv_rate_policy>;
////using mtcp_stream = boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, libv_rate_policy>;
//
////struct RatedSocket2 {
//////	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, boost::beast::simple_rate_policy> socket;
//////	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::io_context, boost::beast::simple_rate_policy> socket;
//////	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, rate_gauge> socket;
////
//////	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, boost::beast::simple_rate_policy> socket;
//////	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, libv_rate_policy> socket;
////	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, libv_rate_policy> socket;
////
////	void foo() {
////		socket.rate_policy().read_limit(100);
////		socket.rate_policy().write_limit(100);
////	}
////};
//
////
////// The policy object, which is default constructed, or
////// decay-copied upon construction, is attached to the stream
////// and may be accessed through the function `rate_policy`.
//////
////// Here we set individual rate limits for reading and writing
////
////stream.rate_policy().read_limit(10000); // bytes per second
////
////stream.rate_policy().write_limit(850000); // bytes per second
//
//
//// =================================================================================================
//// =================================================================================================
//// =================================================================================================

namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

class ImplConnectionAsnycCB : public std::enable_shared_from_this<ImplConnectionAsnycCB> {
	using SelfPtr = std::shared_ptr<ImplConnectionAsnycCB>;
	using ErrorSource = ConnectionAsnycCB::ErrorSource;

private:
	enum class State {
		ConstructedIO,
		ConstructedSocket,
		Connecting,
		Connected,
		Failure,
	};

private:
	ConnectionAsnycCB::CBConnect cb_connect;
	ConnectionAsnycCB::CBDisconnect cb_disconnect;
	ConnectionAsnycCB::CBError cb_error;
	ConnectionAsnycCB::CBRecive cb_receive;
	ConnectionAsnycCB::CBSend cb_send;

	boost::asio::ip::tcp::socket socket;
//	mtcp_stream stream;

private:
	using ID = int64_t;
	static inline std::atomic<ID> nextID = 0;
	const ID id = nextID++; /// Informative ID for logging and monitoring

private:
	std::recursive_mutex mutex;
	State state;
	int32_t queue_read = 0;
	bool queue_disconnect = false;

	PacketHeader read_header; // Temporary buffer for subsequent read header in network byte order
	Message read_body; // Temporary buffer for subsequent read body

	PacketHeader write_header; // Temporary buffer for subsequent write headers in network byte order
	std::deque<Message> write_packets; // Write queue // NOTE: Assumed that observing the first element is thread-safe

	IOContext& io_context;

public:
	explicit ImplConnectionAsnycCB(IOContext& io_context) noexcept;
	ImplConnectionAsnycCB(IOContext& io_context, Socket&& socket) noexcept;

private:
	void init_socket() noexcept;

public:
	/// Handlers must be set before any other function call
	inline void handle_connect(ConnectionAsnycCB::CBConnect callback) noexcept;
	inline void handle_disconnect(ConnectionAsnycCB::CBDisconnect callback) noexcept;
	inline void handle_error(ConnectionAsnycCB::CBError callback) noexcept;
	inline void handle_receive(ConnectionAsnycCB::CBRecive callback) noexcept;
	inline void handle_send(ConnectionAsnycCB::CBSend callback) noexcept;

public:
	inline void read_limit(size_t bytes_per_second) noexcept;
	inline void write_limit(size_t bytes_per_second) noexcept;

	[[nodiscard]] inline size_t total_write_bytes() const noexcept;
	[[nodiscard]] inline size_t total_read_bytes() const noexcept;

	[[nodiscard]] inline size_t total_write_messages() const noexcept;
	[[nodiscard]] inline size_t total_read_messages() const noexcept;

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
	/// Cancels every asynchronous send task.
	/// This operation will cancel any already started send operation and may yield operation_aborted error
	/// @thread safe
	void send_cancel() noexcept;

private:
	[[nodiscard]] inline bool _is_connecting() const noexcept;
	[[nodiscard]] inline bool _is_disconnecting() const noexcept;
	[[nodiscard]] inline bool _is_handlers_set() const noexcept;
	[[nodiscard]] inline bool _is_reading() const noexcept;
	[[nodiscard]] inline bool _is_writing() const noexcept;
	inline void _reset() noexcept;

private:
	void failure(SelfPtr&& self_sp, const ErrorSource source, const std::error_code ec) noexcept;
	void failure_logic_error(SelfPtr&& self_sp, const std::error_code ec) noexcept;

	void failure_connect(SelfPtr&& self_sp, const std::error_code ec) noexcept;
	void failure_receive(SelfPtr&& self_sp, const std::error_code ec) noexcept;
	void failure_send(SelfPtr&& self_sp, const std::error_code ec) noexcept;

	void success_connect(SelfPtr&& self_sp, Endpoint local_endpoint, Endpoint remote_endpoint) noexcept;
	void success_disconnect(SelfPtr&& self_sp) noexcept;
	void success_receive(SelfPtr&& self_sp) noexcept;
	void success_send(SelfPtr&& self_sp) noexcept;

private:
	static void do_logic_error(SelfPtr&& self_sp, const std::error_code ec) noexcept;

	static void do_start(SelfPtr&& self_sp) noexcept;
	static void do_resolve(SelfPtr&& self_sp, Address&& address) noexcept;
	static void do_connect(SelfPtr&& self_sp, boost::asio::ip::tcp::resolver::results_type&& endpoints) noexcept;

	static void do_read(SelfPtr&& self_sp) noexcept;
	static void do_read_header(SelfPtr&& self_sp) noexcept;
	static void do_read_body(SelfPtr&& self_sp, size_t read_body_size) noexcept;

	static void do_write(SelfPtr&& self_sp) noexcept;
	static void do_write_header(SelfPtr&& self_sp) noexcept;
	static void do_write_body(SelfPtr&& self_sp) noexcept;

	static void do_disconnect(SelfPtr&& self_sp) noexcept;
};

// -------------------------------------------------------------------------------------------------

ImplConnectionAsnycCB::ImplConnectionAsnycCB(IOContext& io_context) noexcept :
	socket(io_context.context()),
//	stream(io_context.context()),
	state(State::ConstructedIO),
	io_context(io_context) { }

ImplConnectionAsnycCB::ImplConnectionAsnycCB(IOContext& io_context, Socket&& socket) noexcept :
	socket(std::move(socket.socket)),
//	stream(std::move(socket.socket)),
	state(State::ConstructedSocket),
	io_context(io_context) { }


void ImplConnectionAsnycCB::init_socket() noexcept {
	// TODO P1: Call init_socket to setup no_delay and keep_alive, might require research
	boost::system::error_code ec;

//	boost::system::error_code bec;
//	s<decltype(stream.socket())> x;
//	stream.socket().set_option(boost::asio::ip::tcp::no_delay{true}, ec);
//	stream.socket().set_option(boost::asio::ip::tcp::no_delay{true}, bec);
//	stream.socket().set_option(boost::asio::ip::tcp::socket::keep_alive{true}, bec);

	socket.set_option(boost::asio::ip::tcp::no_delay{true}, ec);
	log_net.error_if(ec, "MTCP-{} Could not set no_delay TCP option. {}", id, libv::net::to_string(ec));

	socket.set_option(boost::asio::ip::tcp::socket::keep_alive{true}, ec);
	log_net.error_if(ec, "MTCP-{} Could not set keep_alive TCP option. {}", id, libv::net::to_string(ec));
}

// -------------------------------------------------------------------------------------------------

inline void ImplConnectionAsnycCB::handle_connect(ConnectionAsnycCB::CBConnect callback) noexcept {
	std::unique_lock lock{mutex};
	cb_connect = std::move(callback);
}

inline void ImplConnectionAsnycCB::handle_disconnect(ConnectionAsnycCB::CBDisconnect callback) noexcept {
	std::unique_lock lock{mutex};
	cb_disconnect = std::move(callback);
}

inline void ImplConnectionAsnycCB::handle_error(ConnectionAsnycCB::CBError callback) noexcept {
	std::unique_lock lock{mutex};
	cb_error = std::move(callback);
}

inline void ImplConnectionAsnycCB::handle_receive(ConnectionAsnycCB::CBRecive callback) noexcept {
	std::unique_lock lock{mutex};
	cb_receive = std::move(callback);
}

inline void ImplConnectionAsnycCB::handle_send(ConnectionAsnycCB::CBSend callback) noexcept {
	std::unique_lock lock{mutex};
	cb_send = std::move(callback);
}

// -------------------------------------------------------------------------------------------------

inline void ImplConnectionAsnycCB::read_limit(size_t bytes_per_second) noexcept {
	(void) bytes_per_second;
	// TODO P1: Implement read_limit
}

inline void ImplConnectionAsnycCB::write_limit(size_t bytes_per_second) noexcept {
	(void) bytes_per_second;
	// TODO P1: Implement write_limit
}

inline size_t ImplConnectionAsnycCB::total_write_bytes() const noexcept {
	return 0; // TODO P1: Implement statistics
}

inline size_t ImplConnectionAsnycCB::total_read_bytes() const noexcept {
	return 0; // TODO P1: Implement statistics
}

inline size_t ImplConnectionAsnycCB::total_write_messages() const noexcept {
	return 0; // TODO P1: Implement statistics
}

inline size_t ImplConnectionAsnycCB::total_read_messages() const noexcept {
	return 0; // TODO P1: Implement statistics
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsnycCB::start() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Start requested", id);

	if (!_is_handlers_set()) {
		log_net.error("MTCP-{} Logic error: Called start on connection with not every handler set", id);
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}
	if (state != State::ConstructedSocket) {
		log_net.error("MTCP-{} Logic error: Called start on connection in an incorrect state {}. Expected state {}", id, libv::to_value(state), libv::to_value(State::ConstructedSocket));
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}
	state = State::Connecting;

	do_start(shared_from_this());
}

void ImplConnectionAsnycCB::connect(Address address) noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Connect requested to {}", id, address);

	if (!_is_handlers_set()) {
		log_net.error("MTCP-{} Logic error: Called connect on connection with not every handler set", id);
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}
	if (state != State::ConstructedIO) {
		log_net.error("MTCP-{} Logic error: Called connect on connection in an incorrect state {}. Expected state {}", id, libv::to_value(state), libv::to_value(State::ConstructedIO));
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}
	state = State::Connecting;

	do_resolve(shared_from_this(), std::move(address));
}

void ImplConnectionAsnycCB::disconnect() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Disconnect requested", id);

	if (state != State::Connecting && state != State::Connected) {
		log_net.error("MTCP-{} Logic error: Called connect on connection in state {}. Expected states are {} or {}", id, libv::to_value(state), libv::to_value(State::Connecting), libv::to_value(State::Connected));
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}
	if (queue_disconnect) {
		log_net.error("MTCP-{} Logic error: Called disconnect on connection multiple times", id);
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}

	queue_disconnect = true;

	if (_is_connecting() || _is_writing() || _is_reading()) {
		return;
	}

	do_disconnect(shared_from_this());
}

void ImplConnectionAsnycCB::receive(int count) noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Receive requested: {}", id, count);

	if (state != State::Connecting && state != State::Connected) {
		log_net.error("MTCP-{} Logic error: Called receive on connection in state {}. Expected states are {} or {}", id, libv::to_value(state), libv::to_value(State::Connecting), libv::to_value(State::Connected));
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}

	const auto new_count = std::max(queue_read + count, 0);
	if (std::exchange(queue_read, new_count) != 0 || state != State::Connected)
		return;

	do_read(shared_from_this());
}

void ImplConnectionAsnycCB::receive_repeat() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Receive requested: repeat", id);

	if (state != State::Connecting && state != State::Connected) {
		log_net.error("MTCP-{} Logic error: Called receive on connection in state {}. Expected states are {} or {}", id, libv::to_value(state), libv::to_value(State::Connecting), libv::to_value(State::Connected));
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}

	if (std::exchange(queue_read, -1) != 0 || state != State::Connected)
		return;

	do_read(shared_from_this());
}

void ImplConnectionAsnycCB::receive_stop() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Receive requested: stop", id);

	queue_read = 0;
}

void ImplConnectionAsnycCB::receive_cancel() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Receive requested: cancel", id);

	// TODO P5: implement receive_cancel, somehow
	log_net.error("Not implemented yet");
}

void ImplConnectionAsnycCB::send(Message message) noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Send requested", id);

	if (state != State::Connecting && state != State::Connected) {
		log_net.error("MTCP-{} Logic error: Called send on connection in state {}. Expected states are {} or {}", id, libv::to_value(state), libv::to_value(State::Connecting), libv::to_value(State::Connected));
		do_logic_error(shared_from_this(), boost::asio::error::make_error_code(boost::asio::error::not_connected));
		return;
	}

	const auto was_writing = _is_writing();
	write_packets.emplace_back(std::move(message));

	if (was_writing || state != State::Connected)
		return;

	do_write(shared_from_this());
}

void ImplConnectionAsnycCB::send_cancel() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Send requested: cancel", id);

	// TODO P5: implement receive_cancel, somehow
	log_net.error("Not implemented yet");
}

// -------------------------------------------------------------------------------------------------

inline bool ImplConnectionAsnycCB::_is_connecting() const noexcept {
	return state == State::Connecting;
}

inline bool ImplConnectionAsnycCB::_is_disconnecting() const noexcept {
	return queue_disconnect;
}

inline bool ImplConnectionAsnycCB::_is_reading() const noexcept {
	return queue_read != 0;
}

inline bool ImplConnectionAsnycCB::_is_writing() const noexcept {
	return !write_packets.empty();
}

inline bool ImplConnectionAsnycCB::_is_handlers_set() const noexcept {
	return cb_connect && cb_disconnect && cb_error && cb_receive && cb_send;
}

inline void ImplConnectionAsnycCB::_reset() noexcept {
	state = State::ConstructedIO;
	queue_disconnect = false;
	queue_read = 0;
	read_body.clear();
	read_body.shrink_to_fit();
	write_packets.clear();
	write_packets.shrink_to_fit();
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsnycCB::failure(SelfPtr&& self_sp, const ErrorSource source, const std::error_code ec) noexcept {
	(void) self_sp;
	std::unique_lock lock{mutex};

	const auto oldState = state;
	if (state != State::Failure) {
		state = State::Failure;
		cb_error(source, ec);

		boost::system::error_code ec_c;
		socket.cancel(ec_c);
		log_net.error_if(ec_c, "MTCP-{} Failed to cancel operations. {}", id, libv::net::to_string(ec_c));
		log_net.trace_if(!ec_c, "MTCP-{} Successfully cancelled operations", id);

		socket.shutdown(socket.shutdown_both, ec_c);
		log_net.error_if(ec_c, "MTCP-{} Error while shutting down socket. {}", id, libv::net::to_string(ec_c));
		log_net.trace_if(!ec_c, "MTCP-{} Successfully shutdown socket", id);

		socket.close(ec_c);
		log_net.error_if(ec_c, "MTCP-{} Error while closing socket. {}", id, libv::net::to_string(ec_c));
		log_net.trace_if(!ec_c, "MTCP-{} Successfully closed socket", id);
	}

	if (!_is_disconnecting() && !_is_reading() && !_is_writing()) {
		if (oldState == State::Connected) {
			cb_disconnect();
		}
		_reset();
	}
}

void ImplConnectionAsnycCB::failure_logic_error(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	failure(std::move(self_sp), ErrorSource::logic, ec);
}

void ImplConnectionAsnycCB::failure_connect(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	failure(std::move(self_sp), ErrorSource::connect, ec);
}

void ImplConnectionAsnycCB::failure_receive(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	// Abort further receives
	queue_read = 0;
	failure(std::move(self_sp), ErrorSource::receive, ec);
}

void ImplConnectionAsnycCB::failure_send(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	// Abort further sends
	write_packets.clear();
	failure(std::move(self_sp), ErrorSource::send, ec);
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsnycCB::success_connect(SelfPtr&& self_sp, Endpoint local_endpoint_, Endpoint remote_endpoint_) noexcept {
	std::unique_lock lock{mutex};

	state = State::Connected;
	cb_connect(local_endpoint_, remote_endpoint_);

	if (_is_reading() && _is_writing()) {
		auto self_sp2 = self_sp;
		do_read(std::move(self_sp2));
		do_write(std::move(self_sp));

	} else if (_is_reading()) {
		do_read(std::move(self_sp));

	} else if (_is_writing()) {
		do_write(std::move(self_sp));

	} else if (_is_disconnecting()) {
		do_disconnect(std::move(self_sp));
	}
}

void ImplConnectionAsnycCB::success_disconnect(SelfPtr&& self_sp) noexcept {
	(void) self_sp;
	std::unique_lock lock{mutex};

	cb_disconnect();
	_reset();
}

void ImplConnectionAsnycCB::success_receive(SelfPtr&& self_sp) noexcept {
	std::unique_lock lock{mutex};

	if (queue_read != -1)
		queue_read--;

	cb_receive(std::move(self_sp->read_body));

	if (state == State::Failure) {
		if (!_is_disconnecting() && !_is_reading() && !_is_writing()) {
			cb_disconnect();
			_reset();
		}
		return;
	}

	if (_is_reading()) {
		do_read(std::move(self_sp));

	} else if (_is_disconnecting() && !_is_writing()) {
		do_disconnect(std::move(self_sp));
	}
}

void ImplConnectionAsnycCB::success_send(SelfPtr&& self_sp) noexcept {
	std::unique_lock lock{mutex};

	cb_send(std::move(write_packets.front()));

	write_packets.pop_front();

	if (state == State::Failure) {
		if (!_is_disconnecting() && !_is_reading() && !_is_writing()) {
			cb_disconnect();
			_reset();
		}
		return;
	}

	if (_is_writing()) {
		do_write(std::move(self_sp));

	} else if (_is_disconnecting() && !_is_reading()) {
		do_disconnect(std::move(self_sp));
	}
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsnycCB::do_logic_error(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	const auto self = self_sp.get();

	self->io_context.post([self_sp = std::move(self_sp), ec]() mutable {
		const auto self = self_sp.get();
		self->failure_logic_error(std::move(self_sp), ec);
	});
}

void ImplConnectionAsnycCB::do_start(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	self->io_context.post([self_sp = std::move(self_sp)]() mutable {
		const auto self = self_sp.get();

		boost::system::error_code ignore_ec;
		const auto local_endpoint = self->socket.local_endpoint(ignore_ec);
		(void) ignore_ec; // Ignore error case where we are actually no longer connected
		const auto remote_endpoint = self->socket.remote_endpoint(ignore_ec);
		(void) ignore_ec; // Ignore error case where we are actually no longer connected

		self->success_connect(
				std::move(self_sp),
				Endpoint{local_endpoint.address().to_v4().to_bytes(), local_endpoint.port()},
				Endpoint{remote_endpoint.address().to_v4().to_bytes(), remote_endpoint.port()}
		);
	});
}

void ImplConnectionAsnycCB::do_resolve(SelfPtr&& self_sp, Address&& address) noexcept {
	const auto self = self_sp.get();

	self->io_context.resolve_async(address, [self_sp = std::move(self_sp)] (const auto& ec, auto&& endpoints) mutable {
		const auto self = self_sp.get();

		if (ec) {
			log_net.error("MTCP-{} Failed to resolve", self->id);
			self->failure_connect(std::move(self_sp), ec);
		} else {
			do_connect(std::move(self_sp), std::move(endpoints.results));
		}
	});
}

void ImplConnectionAsnycCB::do_connect(SelfPtr&& self_sp, boost::asio::ip::tcp::resolver::results_type&& endpoints) noexcept {
	const auto self = self_sp.get();

//	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, libv_rate_policy> socket2(self->io_context.context());
//	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, libv_rate_policy> socket2(ioc);
//	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, libv_rate_policy> socket2(self->io_context.context());
//	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, libv_rate_policy> socket2(self->io_context.context().get_executor());
//	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::any_io_executor, libv_rate_policy> socket2(ioc);

//	boost::asio::io_context ioc;
//	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, libv_rate_policy> socket2(ioc);
//	socket2.async_read_some();
//	socket2.async_write_some();
//	socket2.async_connect(endpoints, [self_sp = std::move(self_sp)](const auto& ec, const auto& remote_endpoint) mutable {

	boost::asio::async_connect(self->socket, endpoints, [self_sp = std::move(self_sp)](const auto& ec, const auto& remote_endpoint) mutable {
		const auto self = self_sp.get();

		if (ec) {
			log_net.error("MTCP-{} Failed to connect", self->id);
			self->failure_connect(std::move(self_sp), ec);

		} else {
			log_net.trace("MTCP-{} Successfully connected", self->id);

			boost::system::error_code ignore_ec;
			const auto local_endpoint = self->socket.local_endpoint(ignore_ec);
			(void) ignore_ec; // Ignore error case where we are actually no longer connected

			self->success_connect(
					std::move(self_sp),
					Endpoint{local_endpoint.address().to_v4().to_bytes(), local_endpoint.port()},
					Endpoint{remote_endpoint.address().to_v4().to_bytes(), remote_endpoint.port()}
			);
		}
	});
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsnycCB::do_disconnect(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	self->io_context.post([self_sp = std::move(self_sp)]() mutable {
		const auto self = self_sp.get();

		boost::system::error_code ec;

		self->socket.shutdown(self->socket.shutdown_both, ec);
		log_net.error_if(ec, "MTCP-{} Error while shutting down socket. {}", self->id, libv::net::to_string(ec));
		log_net.trace_if(!ec, "MTCP-{} Successfully shutdown socket", self->id);

		self->socket.close(ec);
		log_net.error_if(ec, "MTCP-{} Error while closing socket. {}", self->id, libv::net::to_string(ec));
		log_net.trace_if(!ec, "MTCP-{} Successfully closed socket", self->id);

		// Disconnecting will always succeed (in one way or an another)
		self->success_disconnect(std::move(self_sp));
	});
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsnycCB::do_read(SelfPtr&& self_sp) noexcept {
	do_read_header(std::move(self_sp));
}

void ImplConnectionAsnycCB::do_read_header(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

//	boost::asio::io_context ioc;
//	boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, libv_rate_policy> socket2(ioc);
//
//	boost::asio::async_read(
//			socket2,
//	socket2.async_read_some(
//			boost::asio::buffer(&self->read_header, sizeof(PacketHeader)),

	boost::asio::async_read(
			self->socket,
			boost::asio::buffer(&self->read_header, sizeof(PacketHeader)),
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to read {} header byte", self->id, size);
					self->failure_receive(std::move(self_sp), ec);

				} else {
					log_net.trace("MTCP-{} Successfully read {} header byte", self->id, size);
					const auto read_body_size = libv::network_to_host(self->read_header);

					if (read_body_size > MTCP_MESSAGE_MAX_SIZE) {
						log_net.error("MTCP-{} Payload size {} exceeds maximum size of {}", self->id, read_body_size, MTCP_MESSAGE_MAX_SIZE);
						self->failure_receive(std::move(self_sp), boost::asio::error::make_error_code(boost::asio::error::message_size));
					}

					do_read_body(std::move(self_sp), read_body_size);
				}
			});
}

void ImplConnectionAsnycCB::do_read_body(SelfPtr&& self_sp, size_t read_body_size) noexcept {
	const auto self = self_sp.get();

	self->read_body.clear();
	if (read_body_size > MTCP_MESSAGE_MAX_RESERVE) {
		log_net.warn("MTCP-{} Payload size {} exceeds maximum reserve size of {}", self->id, read_body_size, MTCP_MESSAGE_MAX_RESERVE);
		self->read_body.reserve(MTCP_MESSAGE_MAX_RESERVE);

	} else {
		self->read_body.reserve(read_body_size);
	}

	// TODO P5: Use a non dynamic buffer is read_body_size < MTCP_MESSAGE_MAX_RESERVE, with resize instead of reserve
	boost::asio::async_read(
			self->socket,
			boost::asio::dynamic_buffer(self->read_body, read_body_size),
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to read {} payload byte", self->id, size);
					self->failure_receive(std::move(self_sp), ec);
				} else {
					log_net.trace("MTCP-{} Successfully read {} payload byte", self->id, size);
					self->success_receive(std::move(self_sp));
				}
			});
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsnycCB::do_write(SelfPtr&& self_sp) noexcept {
	do_write_header(std::move(self_sp));
}

// TODO P1: Merge header and body send
// TODO P5: Maybe even merge multiple message sends
//void ImplConnectionAsnycCB::do_write_header(SelfPtr&& self_sp) noexcept {
//	const auto self = self_sp.get();
//
//	const auto write_body_size = self->write_packets.begin()->size();
//	assert(write_body_size <= MTCP_MESSAGE_MAX_SIZE);
////	self->write_header = libv::host_to_network(static_cast<PacketHeader> (write_body_size));
////	const auto header_buffer = boost::asio::buffer(&self->write_header, sizeof(PacketHeader));
//
////	boost::asio::async_write(
////			self->socket,
////			header_buffer,
////			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
////				const auto self = self_sp.get();
////
////				if (ec) {
////					log_net.error("Failed to write {} header byte", size);
////					self->failure_send(std::move(self_sp), ec);
////				} else {
////					log_net.trace("Successfully wrote {} header byte", size);
////					do_write_body(std::move(self_sp));
////				}
////			});
//
//	const auto header_buffer = boost::asio::buffer(&self->write_header, sizeof(PacketHeader));
//	const auto body_buffer = boost::asio::buffer(*self->write_packets.begin());
//	const auto buffers = std::array<boost::asio::const_buffer, 2>{header_buffer, body_buffer};
//
//	boost::asio::async_write(
//			self->socket,
//			buffers,
//			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
//				const auto self = self_sp.get();
//
//				if (ec) {
//					log_net.error("Failed to write {} header byte", size);
//					self->failure_send(std::move(self_sp), ec);
//				} else {
//					log_net.trace("Successfully wrote {} header byte", size);
//					do_write_body(std::move(self_sp));
//				}
//			});
//}

void ImplConnectionAsnycCB::do_write_header(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	const auto write_body_size = self->write_packets.begin()->size();
	assert(write_body_size <= MTCP_MESSAGE_MAX_SIZE);
	// TODO P3: replace assert with recoverable error

	self->write_header = libv::host_to_network(static_cast<PacketHeader> (write_body_size));
	const auto header_buffer = boost::asio::buffer(&self->write_header, sizeof(PacketHeader));

	boost::asio::async_write(
			self->socket,
			header_buffer,
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to write {} header byte", self->id, size);
					self->failure_send(std::move(self_sp), ec);

				} else {
					log_net.trace("MTCP-{} Successfully wrote {} header byte", self->id, size);
					do_write_body(std::move(self_sp));
				}
			});
}

void ImplConnectionAsnycCB::do_write_body(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();
	const auto body_buffer = boost::asio::buffer(*self->write_packets.begin());

	boost::asio::async_write(
			self->socket,
			body_buffer,
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to write {} payload byte", self->id, size);
					self->failure_send(std::move(self_sp), ec);

				} else {
					log_net.trace("MTCP-{} Successfully wrote {} payload byte", self->id, size);
					self->success_send(std::move(self_sp));
				}
			});
}

// -------------------------------------------------------------------------------------------------

ConnectionAsnycCB::ConnectionAsnycCB(IOContext& io_context) noexcept :
	impl(std::make_shared<ImplConnectionAsnycCB>(io_context)) { }

ConnectionAsnycCB::ConnectionAsnycCB(IOContext& io_context, Socket&& socket) noexcept :
	impl(std::make_shared<ImplConnectionAsnycCB>(io_context, std::move(socket))) { }

void ConnectionAsnycCB::handle_connect(CBConnect callback) noexcept {
	impl->handle_connect(std::move(callback));
}

void ConnectionAsnycCB::handle_disconnect(CBDisconnect callback) noexcept {
	impl->handle_disconnect(std::move(callback));
}

void ConnectionAsnycCB::handle_error(CBError callback) noexcept {
	impl->handle_error(std::move(callback));
}

void ConnectionAsnycCB::handle_receive(CBRecive callback) noexcept {
	impl->handle_receive(std::move(callback));
}

void ConnectionAsnycCB::handle_send(CBSend callback) noexcept {
	impl->handle_send(std::move(callback));
}

void ConnectionAsnycCB::read_limit(size_t bytes_per_second) noexcept {
	impl->read_limit(bytes_per_second);
}

void ConnectionAsnycCB::write_limit(size_t bytes_per_second) noexcept {
	impl->write_limit(bytes_per_second);
}

size_t ConnectionAsnycCB::total_write_bytes() const noexcept {
	return impl->total_write_bytes();
}

size_t ConnectionAsnycCB::total_read_bytes() const noexcept {
	return impl->total_read_bytes();
}

size_t ConnectionAsnycCB::total_write_messages() const noexcept {
	return impl->total_write_messages();
}

size_t ConnectionAsnycCB::total_read_messages() const noexcept {
	return impl->total_read_messages();
}

void ConnectionAsnycCB::start() noexcept {
	impl->start();
}

void ConnectionAsnycCB::connect(Address address) noexcept {
	impl->connect(std::move(address));
}

void ConnectionAsnycCB::disconnect() noexcept {
	impl->disconnect();
}

void ConnectionAsnycCB::receive(int count) noexcept {
	impl->receive(count);
}

void ConnectionAsnycCB::receive_repeat() noexcept {
	impl->receive_repeat();
}

void ConnectionAsnycCB::receive_stop() noexcept {
	impl->receive_stop();
}

void ConnectionAsnycCB::receive_cancel() noexcept {
	impl->receive_cancel();
}

void ConnectionAsnycCB::send(Message message) noexcept {
	impl->send(std::move(message));
}

void ConnectionAsnycCB::send_cancel() noexcept {
	impl->send_cancel();
}

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv

// log_net.trace("Error while closing socket. Reason: {}", ec.message());
// log_net.trace("Failed to read packet body from unknown. Reason: {}", ec.message());
// log_net.trace("Failed to read packet body from {}:{}. Reason: {}", re.address(), re.port(), ec.message());
// log_net.trace("Failed to read packet header from unknown. Reason: {}", ec.message());
// log_net.trace("Failed to read packet header from {}:{}. Reason: {}", re.address(), re.port(), re_ec.message());
// log_net.trace("Failed to write packet body from unknown. Reason: {}", ec.message());
// log_net.trace("Failed to write packet body from {}:{}. Reason: {}", re.address(), re.port(), ec.message());
// log_net.trace("Failed to write packet header from unknown. Reason: {}", ec.message());
// log_net.trace("Failed to write packet header from {}:{}. Reason: {}", re.address(), re.port(), ec.message());
// log_net.trace("Queue message: [{}]", hex_dump(packet));
// log_net.trace("Received header: Packet size: [{}]", packetRead.size());
// log_net.trace("Received message: [{}]", hex_dump(packetRead.body()));
