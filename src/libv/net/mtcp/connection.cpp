// Project: libv.net, File: src/libv/net/mtcp/connection.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/net/mtcp/connection.hpp>
// ext
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/beast/core/basic_stream.hpp>
#include <boost/beast/core/rate_policy.hpp>
// libv
#include <libv/utility/byte_swap.hpp>
#include <libv/utility/enum.hpp>
// std
#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <system_error>
// pro
#include <libv/net/address.hpp>
#include <libv/net/detail/rate_policy.hpp>
#include <libv/net/detail/resolve_results.hpp>
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/log.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/mtcp/socket.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

using mtcp_stream = boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, rate_policy>;

// -------------------------------------------------------------------------------------------------

class ImplConnectionAsyncCB : public std::enable_shared_from_this<ImplConnectionAsyncCB> {
	using SelfPtr = std::shared_ptr<ImplConnectionAsyncCB>;
	using ErrorSource = ConnectionAsyncCB::ErrorSource;

private:
	enum class State {
		ConstructedIO,
		ConstructedSocket,
		Connecting,
		Connected,
		Failure,
		FailureConnected,
	};

private:
	ConnectionAsyncCB::CBConnect cb_connect;
	ConnectionAsyncCB::CBDisconnect cb_disconnect;
	ConnectionAsyncCB::CBError cb_error;
	ConnectionAsyncCB::CBRecive cb_receive;
	ConnectionAsyncCB::CBSend cb_send;

	mtcp_stream stream;

private:
	using ID = int64_t;
	static inline std::atomic<ID> nextID = 0;
	const ID id = nextID++; /// Informative ID for logging and monitoring

private:
	std::recursive_mutex mutex;
	State state;
	int32_t queue_read = 0;
	bool queue_disconnect = false;

	size_t num_total_message_read = 0;
	PacketHeader read_header{0}; // Temporary buffer for subsequent read header in network byte order
	Message read_body; // Temporary buffer for subsequent read body

	size_t num_total_message_write = 0;
	PacketHeader write_header{0}; // Temporary buffer for subsequent write headers in network byte order
	std::deque<Message> write_packets; // Write queue // NOTE: Assumed that observing the first element is thread-safe

	IOContext& io_context;

public:
	explicit ImplConnectionAsyncCB(IOContext& io_context) noexcept;
	ImplConnectionAsyncCB(IOContext& io_context, Socket&& socket) noexcept;

private:
	void init_socket() noexcept;

public:
	/// Handlers must be set before any other function call
	inline void handle_connect(ConnectionAsyncCB::CBConnect callback) noexcept;
	inline void handle_disconnect(ConnectionAsyncCB::CBDisconnect callback) noexcept;
	inline void handle_error(ConnectionAsyncCB::CBError callback) noexcept;
	inline void handle_receive(ConnectionAsyncCB::CBRecive callback) noexcept;
	inline void handle_send(ConnectionAsyncCB::CBSend callback) noexcept;

public:
	inline void read_limit(size_t bytes_per_second) noexcept;
	inline void write_limit(size_t bytes_per_second) noexcept;

	[[nodiscard]] inline size_t num_byte_read() const noexcept;
	[[nodiscard]] inline size_t num_byte_wrote() const noexcept;

	[[nodiscard]] inline size_t num_message_received() const noexcept;
	[[nodiscard]] inline size_t num_message_sent() const noexcept;

public:
	void start() noexcept;
	void connect(Address address) noexcept;
	void disconnect() noexcept;
	void disconnect_teardown() noexcept;
	void receive(int count = 1) noexcept;
	void receive_repeat() noexcept;
	void receive_stop() noexcept;
	void receive_cancel() noexcept;
	void send(Message message) noexcept;
	void send_cancel() noexcept;

private:
	[[nodiscard]] inline bool _is_connecting() const noexcept;
	[[nodiscard]] inline bool _is_disconnecting() const noexcept;
	[[nodiscard]] inline bool _is_handlers_set() const noexcept;
	[[nodiscard]] inline bool _is_reading() const noexcept;
	[[nodiscard]] inline bool _is_writing() const noexcept;
	inline void _reset() noexcept;

private:
	void _failure(SelfPtr&& self_sp, const ErrorSource source, const std::error_code ec) noexcept;

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

ImplConnectionAsyncCB::ImplConnectionAsyncCB(IOContext& io_context) noexcept :
	stream(io_context.context()),
	state(State::ConstructedIO),
	io_context(io_context) { }

ImplConnectionAsyncCB::ImplConnectionAsyncCB(IOContext& io_context, Socket&& socket) noexcept :
	stream(std::move(socket.socket)),
	state(State::ConstructedSocket),
	io_context(io_context) { }

void ImplConnectionAsyncCB::init_socket() noexcept {
	// TODO P1: Call init_socket to setup no_delay and keep_alive, might require research
	boost::system::error_code ec;

	stream.socket().set_option(boost::asio::ip::tcp::no_delay{true}, ec);
	log_net.error_if(ec, "MTCP-{} Could not set no_delay TCP option. {}", id, libv::net::to_string(ec));

	stream.socket().set_option(boost::asio::ip::tcp::socket::keep_alive{true}, ec);
	log_net.error_if(ec, "MTCP-{} Could not set keep_alive TCP option. {}", id, libv::net::to_string(ec));
}

// -------------------------------------------------------------------------------------------------

inline void ImplConnectionAsyncCB::handle_connect(ConnectionAsyncCB::CBConnect callback) noexcept {
	std::unique_lock lock{mutex};
	cb_connect = std::move(callback);
}

inline void ImplConnectionAsyncCB::handle_disconnect(ConnectionAsyncCB::CBDisconnect callback) noexcept {
	std::unique_lock lock{mutex};
	cb_disconnect = std::move(callback);
}

inline void ImplConnectionAsyncCB::handle_error(ConnectionAsyncCB::CBError callback) noexcept {
	std::unique_lock lock{mutex};
	cb_error = std::move(callback);
}

inline void ImplConnectionAsyncCB::handle_receive(ConnectionAsyncCB::CBRecive callback) noexcept {
	std::unique_lock lock{mutex};
	cb_receive = std::move(callback);
}

inline void ImplConnectionAsyncCB::handle_send(ConnectionAsyncCB::CBSend callback) noexcept {
	std::unique_lock lock{mutex};
	cb_send = std::move(callback);
}

// -------------------------------------------------------------------------------------------------

inline void ImplConnectionAsyncCB::read_limit(size_t bytes_per_second) noexcept {
	std::unique_lock lock{mutex};
	stream.rate_policy().read_limit(bytes_per_second);
}

inline void ImplConnectionAsyncCB::write_limit(size_t bytes_per_second) noexcept {
	std::unique_lock lock{mutex};
	stream.rate_policy().write_limit(bytes_per_second);
}

inline size_t ImplConnectionAsyncCB::num_byte_read() const noexcept {
	return stream.rate_policy().num_byte_read();
}

inline size_t ImplConnectionAsyncCB::num_byte_wrote() const noexcept {
	return stream.rate_policy().num_byte_wrote();
}

inline size_t ImplConnectionAsyncCB::num_message_received() const noexcept {
	return num_total_message_write;
}

inline size_t ImplConnectionAsyncCB::num_message_sent() const noexcept {
	return num_total_message_read;
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsyncCB::start() noexcept {
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

void ImplConnectionAsyncCB::connect(Address address) noexcept {
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

void ImplConnectionAsyncCB::disconnect() noexcept {
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

void ImplConnectionAsyncCB::disconnect_teardown() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Disconnect teardown requested", id);

	{
		// Receive requested: stop
		queue_read = 0;
	}

	if (state != State::Connecting && state != State::Connected)
		return;

	if (queue_disconnect)
		return;

	queue_disconnect = true;

	if (_is_connecting() || _is_writing() || _is_reading())
		return;

	do_disconnect(shared_from_this());
}

void ImplConnectionAsyncCB::receive(int count) noexcept {
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

void ImplConnectionAsyncCB::receive_repeat() noexcept {
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

void ImplConnectionAsyncCB::receive_stop() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Receive requested: stop", id);

	queue_read = 0;
}

void ImplConnectionAsyncCB::receive_cancel() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Receive requested: cancel", id);

	// TODO P5: implement receive_cancel, somehow
	log_net.error("Not implemented yet");
}

void ImplConnectionAsyncCB::send(Message message) noexcept {
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

void ImplConnectionAsyncCB::send_cancel() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Send requested: cancel", id);

	// TODO P5: implement receive_cancel, somehow
	log_net.error("Not implemented yet");
}

// -------------------------------------------------------------------------------------------------

inline bool ImplConnectionAsyncCB::_is_connecting() const noexcept {
	return state == State::Connecting;
}

inline bool ImplConnectionAsyncCB::_is_disconnecting() const noexcept {
	return queue_disconnect;
}

inline bool ImplConnectionAsyncCB::_is_reading() const noexcept {
	return queue_read != 0;
}

inline bool ImplConnectionAsyncCB::_is_writing() const noexcept {
	return !write_packets.empty();
}

inline bool ImplConnectionAsyncCB::_is_handlers_set() const noexcept {
	return cb_connect && cb_disconnect && cb_error && cb_receive && cb_send;
}

inline void ImplConnectionAsyncCB::_reset() noexcept {
	state = State::ConstructedIO;
	queue_disconnect = false;
	queue_read = 0;
	read_body.clear();
	read_body.shrink_to_fit();
	write_packets.clear();
	write_packets.shrink_to_fit();
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsyncCB::_failure(SelfPtr&& self_sp, const ErrorSource source, const std::error_code ec) noexcept {
	(void) self_sp;

	const auto oldState = state;

	cb_error(source, ec);

	if (state != State::Failure && state != State::FailureConnected) {
		state = oldState == State::Connected ? State::FailureConnected : State::Failure;

		stream.cancel();

		if (oldState == State::Connected) {
			boost::system::error_code ec_c;

//			socket.cancel(ec_c);
//			log_net.error_if(ec_c, "MTCP-{} Failed to cancel operations. {}", id, libv::net::to_string(ec_c));
//			log_net.trace_if(!ec_c, "MTCP-{} Successfully cancelled operations", id);

//			stream.shutdown(socket.shutdown_both, ec_c);
			stream.socket().shutdown(stream.socket().shutdown_both, ec_c);
			log_net.error_if(ec_c, "MTCP-{} Error while shutting down socket: {}", id, libv::net::to_string(ec_c));
			log_net.trace_if(!ec_c, "MTCP-{} Successfully shutdown socket", id);

//			socket.close(ec_c);
//			log_net.error_if(ec_c, "MTCP-{} Error while closing socket. {}", id, libv::net::to_string(ec_c));
//			log_net.trace_if(!ec_c, "MTCP-{} Successfully closed socket", id);
		}

		stream.close();

		log_net.trace("MTCP-{} Closed connection", id);
	}

	if (!_is_disconnecting() && !_is_reading() && !_is_writing()) {
		if (oldState == State::Connected || oldState == State::FailureConnected) {
			cb_disconnect();
		}
		_reset();
	}
}

void ImplConnectionAsyncCB::failure_logic_error(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	std::unique_lock lock{mutex};
	_failure(std::move(self_sp), ErrorSource::logic, ec);
}

void ImplConnectionAsyncCB::failure_connect(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	std::unique_lock lock{mutex};
	_failure(std::move(self_sp), ErrorSource::connect, ec);
}

void ImplConnectionAsyncCB::failure_receive(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	std::unique_lock lock{mutex};
	// Abort further receives
	queue_read = 0;
	_failure(std::move(self_sp), ErrorSource::receive, ec);
}

void ImplConnectionAsyncCB::failure_send(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	std::unique_lock lock{mutex};
	// Abort further sends
	write_packets.clear();
	_failure(std::move(self_sp), ErrorSource::send, ec);
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsyncCB::success_connect(SelfPtr&& self_sp, Endpoint local_endpoint_, Endpoint remote_endpoint_) noexcept {
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

void ImplConnectionAsyncCB::success_disconnect(SelfPtr&& self_sp) noexcept {
	(void) self_sp;
	std::unique_lock lock{mutex};

	boost::system::error_code ec;

//		std::unique_lock lock{self->mutex};
//		if (self->state == State::Failure || self->state == State::FailureConnected)
//			return;

//		self->socket.shutdown(self->socket.shutdown_both, ec);
	stream.socket().shutdown(stream.socket().shutdown_both, ec);
	log_net.error_if(ec, "MTCP-{} Error while shutting down socket. {}", id, libv::net::to_string(ec));
	log_net.trace_if(!ec, "MTCP-{} Successfully shutdown socket", id);

//		socket.close(ec);
	stream.close();
	log_net.error_if(ec, "MTCP-{} Error while closing socket. {}", id, libv::net::to_string(ec));
	log_net.trace_if(!ec, "MTCP-{} Successfully closed socket", id);

	cb_disconnect();
	_reset();
}

void ImplConnectionAsyncCB::success_receive(SelfPtr&& self_sp) noexcept {
	std::unique_lock lock{mutex};

	if (queue_read != -1)
		queue_read--;

	num_total_message_read++;
	cb_receive(std::move(self_sp->read_body));

	if (state == State::Failure || state == State::FailureConnected) {
		if (!_is_disconnecting() && !_is_reading() && !_is_writing()) {
			if (state == State::FailureConnected)
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

void ImplConnectionAsyncCB::success_send(SelfPtr&& self_sp) noexcept {
	std::unique_lock lock{mutex};

	num_total_message_write++;
	cb_send(std::move(write_packets.front()));

	write_packets.pop_front();

	if (state == State::Failure || state == State::FailureConnected) {
		if (!_is_disconnecting() && !_is_reading() && !_is_writing()) {
			if (state == State::FailureConnected)
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

void ImplConnectionAsyncCB::do_logic_error(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	const auto self = self_sp.get();

	self->io_context.post([self_sp = std::move(self_sp), ec]() mutable {
		const auto self = self_sp.get();
		self->failure_logic_error(std::move(self_sp), ec);
	});
}

void ImplConnectionAsyncCB::do_start(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	self->io_context.post([self_sp = std::move(self_sp)]() mutable {
		const auto self = self_sp.get();

		boost::system::error_code ignore_ec;
		const auto local_endpoint = self->stream.socket().local_endpoint(ignore_ec);
		(void) ignore_ec; // Ignore error case where we are actually no longer connected
		const auto remote_endpoint = self->stream.socket().remote_endpoint(ignore_ec);
		(void) ignore_ec; // Ignore error case where we are actually no longer connected

		self->success_connect(
				std::move(self_sp),
				Endpoint{local_endpoint.address().to_v4().to_bytes(), local_endpoint.port()},
				Endpoint{remote_endpoint.address().to_v4().to_bytes(), remote_endpoint.port()}
		);
	});
}

void ImplConnectionAsyncCB::do_resolve(SelfPtr&& self_sp, Address&& address) noexcept {
	const auto self = self_sp.get();

	self->io_context.resolve_async(address, [self_sp = std::move(self_sp)] (const auto& ec, auto&& endpoints) mutable {
		const auto self = self_sp.get();

		if (ec) {
			log_net.error("MTCP-{} Failed to resolve {}", self->id, libv::net::to_string(ec));
			self->failure_connect(std::move(self_sp), ec);
		} else {
			log_net.trace("MTCP-{} Successfully resolved address", self->id);
			do_connect(std::move(self_sp), std::move(endpoints.results));
		}
	});
}

void ImplConnectionAsyncCB::do_connect(SelfPtr&& self_sp, boost::asio::ip::tcp::resolver::results_type&& endpoints) noexcept {
	const auto self = self_sp.get();

	using EPEntry = boost::asio::ip::tcp::resolver::results_type::value_type;
	const auto connect_condition = [id = self->id](const auto& ec, const EPEntry& entry) {
		log_net.warn_if(ec, "MTCP-{} Failed to connect: {}", id, libv::net::to_string(ec));

		const auto& endpoint = entry.endpoint();
		const auto is_v4 = endpoint.address().is_v4();

		log_net.trace_if(is_v4, "MTCP-{} Connecting to {}:{}...", id, endpoint.address().to_string(), endpoint.port());
		return is_v4;
	};

	self->stream.async_connect(std::move(endpoints), connect_condition, [self_sp = std::move(self_sp)](const auto& ec, const auto& remote_endpoint) mutable {
		const auto self = self_sp.get();

		if (ec) {
			log_net.error("MTCP-{} Failed to connect: {}", self->id, libv::net::to_string(ec));
			self->failure_connect(std::move(self_sp), ec);

		} else {
			log_net.trace("MTCP-{} Successfully connected", self->id);

			boost::system::error_code ignore_ec;
			const auto local_endpoint = self->stream.socket().local_endpoint(ignore_ec);
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

void ImplConnectionAsyncCB::do_disconnect(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	self->io_context.post([self_sp = std::move(self_sp)]() mutable {
		const auto self = self_sp.get();

		// Disconnecting will always succeed (in one way or an another)
		self->success_disconnect(std::move(self_sp));
	});
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsyncCB::do_read(SelfPtr&& self_sp) noexcept {
	do_read_header(std::move(self_sp));
}

void ImplConnectionAsyncCB::do_read_header(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	boost::asio::async_read(
			self->stream,
			boost::asio::buffer(&self->read_header, sizeof(PacketHeader)),
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to read {} header byte: {}", self->id, size, libv::net::to_string(ec));
					self->failure_receive(std::move(self_sp), ec);

				} else {
					log_net.trace("MTCP-{} Successfully read {} header byte", self->id, size);
					const auto read_body_size = libv::network_to_host(self->read_header);

					if (read_body_size > MTCP_MESSAGE_MAX_SIZE) {
						log_net.error("MTCP-{} Payload size {} exceeds maximum size of {}", self->id, read_body_size, MTCP_MESSAGE_MAX_SIZE);
						self->failure_receive(std::move(self_sp), boost::asio::error::make_error_code(boost::asio::error::message_size));

					} else {
						do_read_body(std::move(self_sp), read_body_size);
					}
				}
			});
}

void ImplConnectionAsyncCB::do_read_body(SelfPtr&& self_sp, size_t read_body_size) noexcept {
	const auto self = self_sp.get();

	self->read_body.clear();
	if (read_body_size > MTCP_MESSAGE_MAX_RESERVE) {
		log_net.warn("MTCP-{} Payload size {} exceeds maximum reserve size of {}", self->id, read_body_size, MTCP_MESSAGE_MAX_RESERVE);
		self->read_body.reserve(MTCP_MESSAGE_MAX_RESERVE);

	} else {
		self->read_body.reserve(read_body_size);
	}

	// TODO P5: Use a non dynamic buffer if read_body_size < MTCP_MESSAGE_MAX_RESERVE, with resize instead of reserve
	boost::asio::async_read(
			self->stream,
			boost::asio::dynamic_buffer(self->read_body, read_body_size),
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to read {} payload byte: {}", self->id, size, libv::net::to_string(ec));
					self->failure_receive(std::move(self_sp), ec);
				} else {
					log_net.trace("MTCP-{} Successfully read {} payload byte", self->id, size);
					self->success_receive(std::move(self_sp));
				}
			});
}

// -------------------------------------------------------------------------------------------------

void ImplConnectionAsyncCB::do_write(SelfPtr&& self_sp) noexcept {
	do_write_header(std::move(self_sp));
}

// TODO P4: Merge header and body send
// TODO P5: Maybe even merge multiple message sends
//void ImplConnectionAsyncCB::do_write_header(SelfPtr&& self_sp) noexcept {
//	const auto self = self_sp.get();
//
//	const auto write_body_size = self->write_packets.begin()->size();
//	assert(write_body_size <= MTCP_MESSAGE_MAX_SIZE);
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

void ImplConnectionAsyncCB::do_write_header(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	const auto write_body_size = self->write_packets.begin()->size();
	assert(write_body_size <= MTCP_MESSAGE_MAX_SIZE);
	// TODO P3: replace assert with recoverable error

	self->write_header = libv::host_to_network(static_cast<PacketHeader> (write_body_size));
	const auto header_buffer = boost::asio::buffer(&self->write_header, sizeof(PacketHeader));

	boost::asio::async_write(
			self->stream,
			header_buffer,
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to write {} header byte: {}", self->id, size, libv::net::to_string(ec));
					self->failure_send(std::move(self_sp), ec);

				} else {
					log_net.trace("MTCP-{} Successfully wrote {} header byte", self->id, size);
					do_write_body(std::move(self_sp));
				}
			});
}

void ImplConnectionAsyncCB::do_write_body(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();
	const auto body_buffer = boost::asio::buffer(*self->write_packets.begin());

	boost::asio::async_write(
			self->stream,
			body_buffer,
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to write {} payload byte: {}", self->id, size, libv::net::to_string(ec));
					self->failure_send(std::move(self_sp), ec);

				} else {
					log_net.trace("MTCP-{} Successfully wrote {} payload byte", self->id, size);
					self->success_send(std::move(self_sp));
				}
			});
}

// -------------------------------------------------------------------------------------------------

ConnectionAsyncCB::ConnectionAsyncCB(IOContext& io_context) noexcept :
	impl(std::make_shared<ImplConnectionAsyncCB>(io_context)) { }

ConnectionAsyncCB::ConnectionAsyncCB(IOContext& io_context, Socket&& socket) noexcept :
	impl(std::make_shared<ImplConnectionAsyncCB>(io_context, std::move(socket))) { }

void ConnectionAsyncCB::handle_connect(CBConnect callback) noexcept {
	impl->handle_connect(std::move(callback));
}

void ConnectionAsyncCB::handle_disconnect(CBDisconnect callback) noexcept {
	impl->handle_disconnect(std::move(callback));
}

void ConnectionAsyncCB::handle_error(CBError callback) noexcept {
	impl->handle_error(std::move(callback));
}

void ConnectionAsyncCB::handle_receive(CBRecive callback) noexcept {
	impl->handle_receive(std::move(callback));
}

void ConnectionAsyncCB::handle_send(CBSend callback) noexcept {
	impl->handle_send(std::move(callback));
}

void ConnectionAsyncCB::read_limit(size_t bytes_per_second) noexcept {
	impl->read_limit(bytes_per_second);
}

void ConnectionAsyncCB::write_limit(size_t bytes_per_second) noexcept {
	impl->write_limit(bytes_per_second);
}

size_t ConnectionAsyncCB::num_byte_wrote() const noexcept {
	return impl->num_byte_wrote();
}

size_t ConnectionAsyncCB::num_byte_read() const noexcept {
	return impl->num_byte_read();
}

size_t ConnectionAsyncCB::num_message_received() const noexcept {
	return impl->num_message_received();
}

size_t ConnectionAsyncCB::num_message_sent() const noexcept {
	return impl->num_message_sent();
}

void ConnectionAsyncCB::start() noexcept {
	impl->start();
}

void ConnectionAsyncCB::connect(Address address) noexcept {
	impl->connect(std::move(address));
}

void ConnectionAsyncCB::disconnect() noexcept {
	impl->disconnect();
}

void ConnectionAsyncCB::disconnect_teardown() noexcept {
	impl->disconnect_teardown();
}

void ConnectionAsyncCB::receive(int count) noexcept {
	impl->receive(count);
}

void ConnectionAsyncCB::receive_repeat() noexcept {
	impl->receive_repeat();
}

void ConnectionAsyncCB::receive_stop() noexcept {
	impl->receive_stop();
}

void ConnectionAsyncCB::receive_cancel() noexcept {
	impl->receive_cancel();
}

void ConnectionAsyncCB::send(Message message) noexcept {
	impl->send(std::move(message));
}

void ConnectionAsyncCB::send_cancel() noexcept {
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
