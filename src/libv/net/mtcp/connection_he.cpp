// Project: libv.net, File: src/libv/net/mtcp/connection_he.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/net/mtcp/connection_he.hpp>
// ext
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/beast/core/basic_stream.hpp>
#include <boost/beast/core/rate_policy.hpp>
// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <deque>
#include <memory>
#include <mutex>
#include <optional>
#include <system_error>
// pro
#include <libv/net/address.hpp>
#include <libv/net/detail/rate_policy.hpp>
#include <libv/net/detail/resolve_results.hpp>
#include <libv/net/detail/socket.hpp>
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/log.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/mtcp/message.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

using mtcp_stream = boost::beast::basic_stream<boost::asio::ip::tcp, boost::asio::executor, rate_policy>;

// -------------------------------------------------------------------------------------------------

struct message_entry {
	message_header header;
	message_body_bin memory_bin; /// Move-in target to keep alive memory
	message_body_str memory_str; /// Move-in target to keep alive memory
	message_body_view body_view; // Generic view to the memory wherever it might be

	inline message_entry() = default;
	inline message_entry(message_header header_, message_body_bin&& body) :
		header(header_),
		memory_bin(std::move(body)),
		body_view(memory_bin.data(), memory_bin.size()) {
	}
	inline message_entry(message_header header_, message_body_str&& body) :
		header(header_),
		memory_str(std::move(body)),
		body_view(reinterpret_cast<const std::byte*>(memory_str.data()), memory_str.size()) {
	}
	inline message_entry(message_header header_, message_body_bin_view body) :
		header(header_),
		body_view(body) {
	}
};

// -------------------------------------------------------------------------------------------------

class ImplBaseConnectionAsyncHE : public std::enable_shared_from_this<ImplBaseConnectionAsyncHE> {
	using SelfPtr = std::shared_ptr<ImplBaseConnectionAsyncHE>;

private:
	enum class State {
		Constructed,   // Does nothing
		Connecting,    // Resolve / Connect / Start on flight
		Connected,     // Stand by / Read / Write / Read and Write on flight
		Failure,       // Failing, canceled every operation and waiting for them to return
		Disconnecting, // Disconnect on flight
		Disconnected,  // Does nothing
	};

private:
	static inline std::atomic<uint64_t> nextID = 0;
	const uint64_t id = nextID++; /// Informative ID for logging and monitoring only

private:
	mutable std::recursive_mutex mutex; // Has to be recursive to handle reentry from callbacks
	IOContext& io_context;
	std::unique_ptr<BaseConnectionHandler> handler;

private:
	std::optional<mtcp_stream> stream;
	Endpoint local_endpoint;
	Endpoint remote_endpoint;

private:
	State state = State::Constructed;

	bool abandoned_handler = false;

	bool failure_happened = false;

	bool on_flight_connect = false;
	bool on_flight_write = false;
	bool on_flight_read = false;
	bool on_flight_disconnect = false;

	bool queue_disconnect = false;

	bool queue_read = true;
	message_header read_message_header; // Temporary buffer for subsequent reads
	message_body_bin read_message_body;  // Temporary buffer for subsequent reads

	message_header write_next_header; // Temporary buffer for subsequent writes
	/// Write queue
	/// NOTE: Assumed that observing the first element is thread-safe
	/// NOTE: Assumed to be memory stable, memory address of elements might be self pointing (std::string memory view support)
	std::deque<message_entry> write_messages;

private:
	size_t num_total_message_read = 0;
	size_t num_total_message_write = 0;

public:
	explicit inline ImplBaseConnectionAsyncHE(IOContext& io_context) noexcept;
	inline void inject_handler(std::unique_ptr<BaseConnectionHandler>&& handler) noexcept;
	inline void abandon_handler() noexcept;

public:
	inline void read_limit(size_t bytes_per_second) noexcept;
	inline void write_limit(size_t bytes_per_second) noexcept;

	[[nodiscard]] inline size_t num_byte_read() const noexcept;
	[[nodiscard]] inline size_t num_byte_wrote() const noexcept;

	[[nodiscard]] inline size_t num_message_received() const noexcept;
	[[nodiscard]] inline size_t num_message_sent() const noexcept;

	[[nodiscard]] inline size_t send_queue_size() const noexcept;

public:
	inline void connect_async(detail::Socket&& socket) noexcept;
	inline void connect_sync(detail::Socket&& socket) noexcept;
	inline void connect_async(Address address) noexcept;
	inline void disconnect_async() noexcept;
	inline void disconnect_if_connected_async() noexcept;
	inline void cancel_and_disconnect_async() noexcept;
	inline void pause_receive_async() noexcept;
	inline void resume_receive_async() noexcept;
	template <typename M>
	inline void send_async(M&& message) noexcept;

private:
	[[nodiscard]] inline bool _no_on_flight_operation() const noexcept;
	[[nodiscard]] inline bool _is_connecting() const noexcept;
	[[nodiscard]] inline bool _is_disconnecting() const noexcept;
	[[nodiscard]] inline bool _is_reading() const noexcept;
	[[nodiscard]] inline bool _is_writing() const noexcept;

private:
	inline void _terminate() noexcept;

private:
	inline void outcome_connect(SelfPtr&& self_sp, const std::error_code ec) noexcept;
	inline void outcome_disconnect(SelfPtr&& self_sp) noexcept;
	inline void outcome_receive(SelfPtr&& self_sp, const std::error_code ec) noexcept;
	inline void outcome_send(SelfPtr&& self_sp, const std::error_code ec) noexcept;

private:
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

inline ImplBaseConnectionAsyncHE::ImplBaseConnectionAsyncHE(IOContext& io_context) noexcept :
	io_context(io_context) { }

inline void ImplBaseConnectionAsyncHE::inject_handler(std::unique_ptr<BaseConnectionHandler>&& handler_) noexcept {
	handler = std::move(handler_);
}

inline void ImplBaseConnectionAsyncHE::abandon_handler() noexcept {
	std::unique_lock lock{mutex};
	abandoned_handler = true;

	if (state == State::Constructed || state == State::Disconnected)
		auto temp = std::move(handler); // Move handler to stack to commit suicide
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::read_limit(size_t bytes_per_second) noexcept {
	std::unique_lock lock{mutex};
	stream->rate_policy().read_limit(bytes_per_second);
}

inline void ImplBaseConnectionAsyncHE::write_limit(size_t bytes_per_second) noexcept {
	std::unique_lock lock{mutex};
	stream->rate_policy().write_limit(bytes_per_second);
}

inline size_t ImplBaseConnectionAsyncHE::num_byte_read() const noexcept {
	return stream->rate_policy().num_byte_read();
}

inline size_t ImplBaseConnectionAsyncHE::num_byte_wrote() const noexcept {
	return stream->rate_policy().num_byte_wrote();
}

inline size_t ImplBaseConnectionAsyncHE::num_message_received() const noexcept {
	return num_total_message_write;
}

inline size_t ImplBaseConnectionAsyncHE::num_message_sent() const noexcept {
	return num_total_message_read;
}

inline size_t ImplBaseConnectionAsyncHE::send_queue_size() const noexcept {
	std::unique_lock lock{mutex};
	return write_messages.size();
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::connect_async(detail::Socket&& socket) noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} connect_async(socket)", id);

	if (state != State::Constructed)
		return log_net.error("MTCP-{} Logic error: Called connect_async(socket) on connection in incorrect state {}. Expected state {}", id, libv::to_value(state), libv::to_value(State::Constructed));

	state = State::Connecting;
	stream.emplace(std::move(socket.socket));
	do_start(shared_from_this());
}

inline void ImplBaseConnectionAsyncHE::connect_sync(detail::Socket&& socket) noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} connect_sync(socket)", id);

	if (state != State::Constructed)
		return log_net.error("MTCP-{} Logic error: Called connect_async(socket) on connection in incorrect state {}. Expected state {}", id, libv::to_value(state), libv::to_value(State::Constructed));

	state = State::Connecting;
	stream.emplace(std::move(socket.socket));

	outcome_connect(shared_from_this(), std::error_code());
}

inline void ImplBaseConnectionAsyncHE::connect_async(Address address) noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} connect_async(address) to {}", id, address);

	if (state != State::Constructed)
		return log_net.error("MTCP-{} Logic error: Called connect on connect_async(address) in an incorrect state {}. Expected state {}", id, libv::to_value(state), libv::to_value(State::Constructed));

	state = State::Connecting;
	stream.emplace(io_context.context());
	do_resolve(shared_from_this(), std::move(address));
}

inline void ImplBaseConnectionAsyncHE::disconnect_async() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} disconnect_async", id);

	if (state != State::Connecting && state != State::Connected)
		return log_net.error("MTCP-{} Logic error: Called disconnect_async on connection in state {}. Expected states are {} or {}", id, libv::to_value(state), libv::to_value(State::Connecting), libv::to_value(State::Connected));

	if (_is_disconnecting())
		return log_net.error("MTCP-{} Logic error: Called disconnect on connection multiple times", id);

	queue_disconnect = true;

	// Stop reading
	queue_read = false;

	// If still connecting or still has something to write, do not disconnect immediately (queue_disconnect flag will take care of it)
	if (_is_connecting() || _is_writing())
		return;

	state = State::Disconnecting;
	do_disconnect(shared_from_this());
}

inline void ImplBaseConnectionAsyncHE::disconnect_if_connected_async() noexcept {
	std::unique_lock lock{mutex};

	if (state != State::Connecting && state != State::Connected)
		return;

	if (_is_disconnecting())
		return;

	log_net.trace("MTCP-{} disconnect_if_connected_async", id);

	queue_disconnect = true;

	// Stop reading
	queue_read = false;

	// If still connecting or still has something to write, do not disconnect immediately (queue_disconnect flag will take care of it)
	if (_is_connecting() || _is_writing())
		return;

	state = State::Disconnecting;
	do_disconnect(shared_from_this());
}

inline void ImplBaseConnectionAsyncHE::cancel_and_disconnect_async() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} cancel_and_disconnect_async", id);

	if (state != State::Connecting && state != State::Connected)
		return log_net.error("MTCP-{} Logic error: Called cancel_and_disconnect_async on connection in state {}. Expected states are {} or {}", id, libv::to_value(state), libv::to_value(State::Connecting), libv::to_value(State::Connected));

	if (queue_disconnect)
		return log_net.warn("MTCP-{} Logic error: Called disconnect on connection multiple times", id);

	queue_disconnect = true;

	// Stop reading
	queue_read = false;
	// Stop writing
	write_messages.clear();

	state = State::Disconnecting;
	do_disconnect(shared_from_this());
}

inline void ImplBaseConnectionAsyncHE::pause_receive_async() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} pause_receive_async", id);

	queue_read = false;
}

inline void ImplBaseConnectionAsyncHE::resume_receive_async() noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} resume_receive_async", id);

	if (state != State::Connecting && state != State::Connected)
		return log_net.error("MTCP-{} Logic error: Called receive on connection in state {}. Expected states are {} or {}", id, libv::to_value(state), libv::to_value(State::Connecting), libv::to_value(State::Connected));

	queue_read = true;

	if (state != State::Connected)
		return;

	do_read(shared_from_this());
}

template <typename M>
inline void ImplBaseConnectionAsyncHE::send_async(M&& message) noexcept {
	std::unique_lock lock{mutex};
	log_net.trace("MTCP-{} Send requested", id);

	if (state != State::Connecting && state != State::Connected)
		return log_net.error("MTCP-{} Logic error: Called send on connection in state {}. Expected states are {} or {}", id, libv::to_value(state), libv::to_value(State::Connecting), libv::to_value(State::Connected));

	const auto was_writing = _is_writing();
	write_messages.emplace_back(
			message_header{mtcp_message_type::message, 0, static_cast<uint32_t>(message.size())},
			std::move(message));

	if (was_writing || state != State::Connected)
		return;

	do_write(shared_from_this());
}

// -------------------------------------------------------------------------------------------------

inline bool ImplBaseConnectionAsyncHE::_no_on_flight_operation() const noexcept {
	return
		!on_flight_connect &&
		!on_flight_disconnect &&
		!on_flight_read &&
		!on_flight_write;
}

inline bool ImplBaseConnectionAsyncHE::_is_connecting() const noexcept {
	return state == State::Connecting;
}

inline bool ImplBaseConnectionAsyncHE::_is_disconnecting() const noexcept {
	return queue_disconnect;
}

inline bool ImplBaseConnectionAsyncHE::_is_reading() const noexcept {
	return queue_read;
}

inline bool ImplBaseConnectionAsyncHE::_is_writing() const noexcept {
	return !write_messages.empty();
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::_terminate() noexcept {
	boost::system::error_code ec_c;

	stream->socket().shutdown(stream->socket().shutdown_both, ec_c);
	log_net.warn_if(ec_c, "MTCP-{} Error while shutting down socket: {}", id, libv::net::to_string(ec_c));
	log_net.trace_if(!ec_c, "MTCP-{} Successfully shutdown socket", id);
	stream->close();

	state = State::Disconnected;
	handler->on_disconnect(ec_c);
	if (abandoned_handler)
		auto temp = std::move(handler); // Move handler to stack to commit suicide
}

inline void ImplBaseConnectionAsyncHE::outcome_connect(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	std::unique_lock lock{mutex};
	on_flight_connect = false;

	if (ec) {
		state = State::Constructed;
		handler->on_connect(ec);
		if (abandoned_handler)
			auto temp = std::move(handler); // Move handler to stack to commit suicide

	} else {
		{
			// Identify endpoints
			boost::system::error_code ignore_ec;
			const auto asio_local_endpoint = stream->socket().local_endpoint(ignore_ec);
			(void) ignore_ec; // Ignore error case where we are actually no longer connected
			const auto asio_remote_endpoint = stream->socket().remote_endpoint(ignore_ec);
			(void) ignore_ec; // Ignore error case where we are actually no longer connected

			local_endpoint = Endpoint{asio_local_endpoint.address().to_v4().to_bytes(), asio_local_endpoint.port()};
			remote_endpoint = Endpoint{asio_remote_endpoint.address().to_v4().to_bytes(), asio_remote_endpoint.port()};
		}

		{
			// Setup socket options
			boost::system::error_code option_ec;
			stream->socket().set_option(boost::asio::ip::tcp::no_delay{true}, option_ec);
			log_net.error_if(option_ec, "MTCP-{} Could not set no_delay TCP option. {}", id, libv::net::to_string(option_ec));

			stream->socket().set_option(boost::asio::ip::tcp::socket::keep_alive{true}, option_ec);
			log_net.error_if(option_ec, "MTCP-{} Could not set keep_alive TCP option. {}", id, libv::net::to_string(option_ec));
		}

		state = State::Connected;
		handler->on_connect(ec);

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
}

inline void ImplBaseConnectionAsyncHE::outcome_disconnect(SelfPtr&& self_sp) noexcept {
	std::unique_lock lock{mutex};
	on_flight_disconnect = false;

	(void) self_sp;

	stream->cancel();

	if (state != State::Connected) {
		if (_no_on_flight_operation())
			_terminate();
		// else
		//      Noop, the other on flight operation will take care of it
	}
}

inline void ImplBaseConnectionAsyncHE::outcome_receive(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	std::unique_lock lock{mutex};
	on_flight_read = false;

	if (ec) {
		if (state != State::Failure) {
			state = State::Failure;
			stream->cancel();
		}

		handler->on_receive(ec, read_message_body);

		if (_no_on_flight_operation())
			_terminate();
		// else
		//      Noop, the other on flight operation will take care of it

	} else {
		num_total_message_read++;
		handler->on_receive(ec, read_message_body);

		if (state != State::Connected) {
			if (_no_on_flight_operation())
				_terminate();
			// else
			//      Noop, the other on flight operation will take care of it

		} else if (_is_reading()) {
			do_read(std::move(self_sp));

		} else if (_is_disconnecting() && !_is_writing()) {
			state = State::Disconnecting;
			do_disconnect(std::move(self_sp));
		}
	}
}

inline void ImplBaseConnectionAsyncHE::outcome_send(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	std::unique_lock lock{mutex};
	on_flight_write = false;

	if (ec) {
		if (state != State::Failure) {
			state = State::Failure;
			stream->cancel();
		}

		handler->on_send(ec, std::move(write_messages.front().body_view));

		if (_no_on_flight_operation())
			_terminate();
		// else
		//      Noop, the other on flight operation will take care of it

	} else {
		num_total_message_write++;
		handler->on_send(ec, std::move(write_messages.front().body_view));

		write_messages.pop_front();

		if (state != State::Connected) {
			if (_no_on_flight_operation())
				_terminate();
			// else
			//      Noop, the other on flight operation will take care of it

		} else if (_is_writing()) {
			do_write(std::move(self_sp));

		} else if (_is_disconnecting() && !_is_reading()) {
			state = State::Disconnecting;
			do_disconnect(std::move(self_sp));
		}
	}
}

// -------------------------------------------------------------------------------------------------

void ImplBaseConnectionAsyncHE::do_start(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	self->on_flight_connect = true;
	self->io_context.post([self_sp = std::move(self_sp)]() mutable {
		const auto self = self_sp.get();

		self->outcome_connect(std::move(self_sp), std::error_code());
	});
}

void ImplBaseConnectionAsyncHE::do_resolve(SelfPtr&& self_sp, Address&& address) noexcept {
	const auto self = self_sp.get();

	self->on_flight_connect = true;
	self->io_context.resolve_async(address, [self_sp = std::move(self_sp)] (const auto& ec, auto&& endpoints) mutable {
		const auto self = self_sp.get();

		if (ec) {
			log_net.error("MTCP-{} Failed to resolve {}", self->id, libv::net::to_string(ec));
			self->outcome_connect(std::move(self_sp), ec);
		} else {
			log_net.trace("MTCP-{} Successfully resolved address", self->id);
			do_connect(std::move(self_sp), std::move(endpoints.results));
		}
	});
}

void ImplBaseConnectionAsyncHE::do_connect(SelfPtr&& self_sp, boost::asio::ip::tcp::resolver::results_type&& endpoints) noexcept {
	const auto self = self_sp.get();

	using EPEntry = boost::asio::ip::tcp::resolver::results_type::value_type;
	const auto connect_condition = [id = self->id](const auto& ec, const EPEntry& entry) {
		log_net.warn_if(ec, "MTCP-{} Failed to connect: {}", id, libv::net::to_string(ec));

		const auto& endpoint = entry.endpoint();
		const auto is_v4 = endpoint.address().is_v4();

		log_net.trace_if(is_v4, "MTCP-{} Connecting to {}:{}...", id, endpoint.address().to_string(), endpoint.port());
		return is_v4;
	};

	self->on_flight_connect = true;
	self->stream->async_connect(std::move(endpoints), connect_condition, [self_sp = std::move(self_sp)](const auto& ec, const auto& remote_endpoint) mutable {
		(void) remote_endpoint;

		const auto self = self_sp.get();

		log_net.error_if(ec, "MTCP-{} Failed to connect: {}", self->id, libv::net::to_string(ec));
		log_net.trace_if(!ec, "MTCP-{} Successfully connected", self->id);

		self->outcome_connect(std::move(self_sp), ec);
	});
}

// -------------------------------------------------------------------------------------------------

void ImplBaseConnectionAsyncHE::do_disconnect(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	self->on_flight_disconnect = true;
	self->io_context.post([self_sp = std::move(self_sp)]() mutable {
		const auto self = self_sp.get();

		// Disconnecting will always succeed (in one way or an another)
		self->outcome_disconnect(std::move(self_sp));
	});
}

// -------------------------------------------------------------------------------------------------

void ImplBaseConnectionAsyncHE::do_read(SelfPtr&& self_sp) noexcept {
	if (self_sp->on_flight_read)
		return;

	self_sp->on_flight_read = true;
	do_read_header(std::move(self_sp));
}

void ImplBaseConnectionAsyncHE::do_read_header(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	boost::asio::async_read(
			*self->stream,
			boost::asio::buffer(self->read_message_header.header_data(), self->read_message_header.header_size()),
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to read {} header byte: {}", self->id, size, libv::net::to_string(ec));
					self->outcome_receive(std::move(self_sp), ec);

				} else {
					log_net.trace("MTCP-{} Successfully read {} header byte", self->id, size);
					const auto read_body_size = self->read_message_header.size();

					if (read_body_size > MTCP_MESSAGE_MAX_SIZE) {
						log_net.error("MTCP-{} Payload size {} exceeds maximum size of {}", self->id, read_body_size, MTCP_MESSAGE_MAX_SIZE);
						self->outcome_receive(std::move(self_sp), boost::asio::error::make_error_code(boost::asio::error::message_size));

					} else {
						do_read_body(std::move(self_sp), read_body_size);
					}
				}
			});
}

void ImplBaseConnectionAsyncHE::do_read_body(SelfPtr&& self_sp, size_t read_body_size) noexcept {
	const auto self = self_sp.get();

	self->read_message_body.clear();
	if (read_body_size > MTCP_MESSAGE_MAX_RESERVE) {
		log_net.warn("MTCP-{} Payload size {} exceeds maximum reserve size of {}", self->id, read_body_size, MTCP_MESSAGE_MAX_RESERVE);
		self->read_message_body.reserve(MTCP_MESSAGE_MAX_RESERVE);

	} else {
		self->read_message_body.reserve(read_body_size);
	}

	// TODO P5: Use a non dynamic buffer if read_body_size < MTCP_MESSAGE_MAX_RESERVE, with resize instead of reserve
	self->on_flight_read = true;
	boost::asio::async_read(
			*self->stream,
			boost::asio::dynamic_buffer(self->read_message_body, read_body_size),
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				log_net.error_if(ec, "MTCP-{} Failed to read {} payload byte: {}", self->id, size, libv::net::to_string(ec));
				log_net.trace_if(!ec, "MTCP-{} Successfully read {} payload byte", self->id, size);

				self->outcome_receive(std::move(self_sp), ec);
			});
}

// -------------------------------------------------------------------------------------------------

void ImplBaseConnectionAsyncHE::do_write(SelfPtr&& self_sp) noexcept {
	if (self_sp->on_flight_write)
		return;

	self_sp->on_flight_write = true;
	do_write_header(std::move(self_sp));
}

// TODO P4: Merge header and body send
//void ImplBaseConnectionAsyncHE::do_write(SelfPtr&& self_sp) noexcept {
//	const auto header_buffer = boost::asio::buffer(&self->write_header, sizeof(PacketHeader));
//	const auto body_buffer = boost::asio::buffer(*self->write_packets.begin());
//	const auto buffers = std::array<boost::asio::const_buffer, 2>{header_buffer, body_buffer};
//
//	boost::asio::async_write(
//			self->socket,
//			buffers,

void ImplBaseConnectionAsyncHE::do_write_header(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();

	self->write_next_header = self->write_messages.front().header;

	const auto write_body_size = self->write_next_header.size();
	assert(write_body_size <= MTCP_MESSAGE_MAX_SIZE);
	// TODO P3: replace assert with recoverable error

	const auto header_buffer = boost::asio::buffer(self->write_next_header.header_data(), self->write_next_header.header_size());
	boost::asio::async_write(
			*self->stream,
			header_buffer,
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					log_net.error("MTCP-{} Failed to write {} header byte: {}", self->id, size, libv::net::to_string(ec));
					self->outcome_send(std::move(self_sp), ec);

				} else {
					log_net.trace("MTCP-{} Successfully wrote {} header byte", self->id, size);
					do_write_body(std::move(self_sp));
				}
			});
}

void ImplBaseConnectionAsyncHE::do_write_body(SelfPtr&& self_sp) noexcept {
	const auto self = self_sp.get();
	const auto& front_message = self->write_messages.front().body_view;
	const auto body_buffer = boost::asio::buffer(front_message.data(), front_message.size());

	self->on_flight_write = true;
	boost::asio::async_write(
			*self->stream,
			body_buffer,
			[self_sp = std::move(self_sp)](const std::error_code ec, size_t size) mutable {
				const auto self = self_sp.get();

				log_net.error_if(ec, "MTCP-{} Failed to write {} payload byte: {}", self->id, size, libv::net::to_string(ec));
				log_net.trace_if(!ec, "MTCP-{} Successfully wrote {} payload byte", self->id, size);

				self->outcome_send(std::move(self_sp), ec);
			});
}

// -------------------------------------------------------------------------------------------------

BaseConnectionAsyncHE::BaseConnectionAsyncHE(IOContext& io_context) noexcept :
	internals(std::make_shared<ImplBaseConnectionAsyncHE>(io_context)) {
}

void BaseConnectionAsyncHE::inject_handler(std::unique_ptr<BaseConnectionHandler>&& handler) noexcept {
	internals->inject_handler(std::move(handler));
}

void BaseConnectionAsyncHE::abandon_handler() noexcept {
	internals->abandon_handler();
}

void BaseConnectionAsyncHE::read_limit(size_t bytes_per_second) noexcept {
	internals->read_limit(bytes_per_second);
}

void BaseConnectionAsyncHE::write_limit(size_t bytes_per_second) noexcept {
	internals->write_limit(bytes_per_second);
}

size_t BaseConnectionAsyncHE::num_byte_wrote() const noexcept {
	return internals->num_byte_wrote();
}

size_t BaseConnectionAsyncHE::num_byte_read() const noexcept {
	return internals->num_byte_read();
}

size_t BaseConnectionAsyncHE::num_message_received() const noexcept {
	return internals->num_message_received();
}

size_t BaseConnectionAsyncHE::num_message_sent() const noexcept {
	return internals->num_message_sent();
}

size_t BaseConnectionAsyncHE::send_queue_size() const noexcept {
	return internals->send_queue_size();
}

void BaseConnectionAsyncHE::connect_async(detail::Socket&& socket) noexcept {
	internals->connect_async(std::move(socket));
}

void BaseConnectionAsyncHE::connect_sync(detail::Socket&& socket) noexcept {
	internals->connect_sync(std::move(socket));
}

void BaseConnectionAsyncHE::connect_async(Address address) noexcept {
	internals->connect_async(std::move(address));
}

void BaseConnectionAsyncHE::disconnect_async() noexcept {
	internals->disconnect_async();
}

void BaseConnectionAsyncHE::disconnect_if_connected_async() noexcept {
	internals->disconnect_if_connected_async();
}

void BaseConnectionAsyncHE::cancel_and_disconnect_async() noexcept {
	internals->cancel_and_disconnect_async();
}

void BaseConnectionAsyncHE::pause_receive_async() noexcept {
	internals->pause_receive_async();
}

void BaseConnectionAsyncHE::resume_receive_async() noexcept {
	internals->resume_receive_async();
}

void BaseConnectionAsyncHE::send_async(message_body_bin message) noexcept {
	internals->send_async(std::move(message));
}
void BaseConnectionAsyncHE::send_async(message_body_bin_view message_view) noexcept {
	std::vector<std::byte> message;

	message.resize(message_view.size());
	std::memcpy(message.data(), message_view.data(), message_view.size());

	internals->send_async(std::move(message));
}
void BaseConnectionAsyncHE::send_async(message_body_str message) noexcept {
	internals->send_async(std::move(message));
}
void BaseConnectionAsyncHE::send_async(message_body_str_view message_str_view) noexcept {
	std::vector<std::byte> message;

	message.resize(message_str_view.size());
	std::memcpy(message.data(), message_str_view.data(), message_str_view.size());

	internals->send_async(std::move(message));
}

void BaseConnectionAsyncHE::send_view_async(message_body_bin_view message) noexcept {
	internals->send_async(std::move(message));
}
void BaseConnectionAsyncHE::send_view_async(message_body_str_view message_str) noexcept {
	const auto message = std::span<const std::byte>(
			reinterpret_cast<const std::byte*>(message_str.data()),
			reinterpret_cast<const std::byte*>(message_str.data() + message_str.size())
	);
	internals->send_async(std::move(message));
}

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
