// Project: libv.net, File: src/libv/net/mtcp/connection_he.cpp

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
		body_view(memory_bin) {
	}
	inline message_entry(message_header header_, message_body_str&& body) :
		header(header_),
		memory_str(std::move(body)),
		body_view(memory_str) {
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

		Completing,    // Finishing any outstanding write, canceling reads, when every outstanding operation completes goes to disconnect
		Canceling,     // Canceled every operation and waiting for them to return, when every outstanding operation completes goes to disconnect

		Disconnected,  // Does nothing
	};

private:
	static inline std::atomic<uint64_t> nextID = 0;
public:
	const uint64_t id = nextID++; /// Informative ID for logging and monitoring only

private:
	using Mutex = std::mutex;

	mutable std::mutex strand; // Not a real strand, only simulates one, inefficient
	mutable std::mutex mutex;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard;
	IOContext& io_context;

//	mutable Mutex handler_mutex{"MTCP-" + std::to_string(id) + "-handler_mutex"};
////	mutable std::mutex handler_mutex; /// To acquire this lock, 'mutex' always has to be held first
	bool handler_abandoned = false;
	std::unique_ptr<BaseConnectionHandler> handler;

private:
	std::optional<mtcp_stream> stream;
	Endpoint local_endpoint;
	Endpoint remote_endpoint;

private:
	State state = State::Constructed;

	int32_t on_flight_operation_num = 0;

	std::error_code first_error{};
	bool on_connect_was_called_with_success = false;

	bool queue_read = true;
	message_header read_message_header; // Temporary buffer for subsequent reads
	message_body_bin read_message_body;  // Temporary buffer for subsequent reads

	message_header write_next_header; // Temporary buffer for subsequent writes
	/// Write queue
	/// NOTE: Assumed that observing the first element is thread-safe
	/// NOTE: Assumed to be memory stable, memory address of elements might be self pointing (std::string memory view support)
	std::deque<message_entry> write_messages;

private:
	std::size_t num_total_message_read = 0;
	std::size_t num_total_message_write = 0;

public:
	explicit inline ImplBaseConnectionAsyncHE(IOContext& io_context) noexcept;
	inline ~ImplBaseConnectionAsyncHE();
	inline void inject_handler(std::unique_ptr<BaseConnectionHandler>&& handler) noexcept;
	inline void abandon_handler() noexcept;

public:
	inline void read_limit(std::size_t bytes_per_second) noexcept;
	inline void write_limit(std::size_t bytes_per_second) noexcept;

	[[nodiscard]] inline std::size_t num_byte_read() const noexcept;
	[[nodiscard]] inline std::size_t num_byte_wrote() const noexcept;

	[[nodiscard]] inline std::size_t num_message_received() const noexcept;
	[[nodiscard]] inline std::size_t num_message_sent() const noexcept;

	[[nodiscard]] inline std::size_t send_queue_size() const noexcept;

public:
	inline void connect_async(detail::Socket&& socket) noexcept;
	inline void connect_sync(detail::Socket&& socket) noexcept;
	inline void connect_async(Address address) noexcept;
	inline void complete_and_disconnect_async() noexcept;
	inline void cancel_and_disconnect_async() noexcept;
	inline void pause_receive_async() noexcept;
	inline void resume_receive_async() noexcept;
	template <typename M>
	inline void send_async(M&& message) noexcept;

private:
	inline void _cancel_and_disconnect_async() noexcept;

	[[nodiscard]] inline bool _no_on_flight_operation() const noexcept;
	[[nodiscard]] inline bool _has_read_work() const noexcept;
	[[nodiscard]] inline bool _has_write_work() const noexcept;

private:
	inline void _outcome_connect(std::unique_lock<Mutex>& lock, SelfPtr&& self_sp, const std::error_code ec) noexcept;
	inline void outcome_connect(SelfPtr&& self_sp, const std::error_code ec) noexcept;
	inline void outcome_disconnect(SelfPtr&& self_sp) noexcept;
	inline void outcome_receive(SelfPtr&& self_sp, const std::error_code ec, bool eof_error_in_head = false) noexcept;
	inline void outcome_send(SelfPtr&& self_sp, const std::error_code ec) noexcept;

private:
	inline void _do_start(SelfPtr&& self_sp) noexcept;
	inline void _do_resolve(SelfPtr&& self_sp, Address&& address) noexcept;
	inline void _do_connect(SelfPtr&& self_sp, boost::asio::ip::tcp::resolver::results_type&& endpoints) noexcept;

	inline void _do_read(SelfPtr&& self_sp) noexcept;
	inline void _do_read_header(SelfPtr&& self_sp) noexcept;
	inline void _do_read_body(SelfPtr&& self_sp, std::size_t read_body_size) noexcept;

	inline void _do_write(SelfPtr&& self_sp) noexcept;

	inline void _do_disconnect(SelfPtr&& self_sp) noexcept;
};

// -------------------------------------------------------------------------------------------------

inline ImplBaseConnectionAsyncHE::ImplBaseConnectionAsyncHE(IOContext& io_context) noexcept :
	work_guard(io_context.context().get_executor()),
	io_context(io_context) { }

inline ImplBaseConnectionAsyncHE::~ImplBaseConnectionAsyncHE() {
	log_net.trace("MTCP-{} ~ImplBaseConnectionAsyncHE", id);
}

inline void ImplBaseConnectionAsyncHE::inject_handler(std::unique_ptr<BaseConnectionHandler>&& handler_) noexcept {
	// inject_handler only called from the "ctor", no need lock the mutex for it
	handler = std::move(handler_);
}

inline void ImplBaseConnectionAsyncHE::abandon_handler() noexcept {
	auto lock = std::unique_lock(mutex);
	log_net.trace("MTCP-{} abandon_handler", id);

	handler_abandoned = true;

	if (state != State::Canceling && state != State::Completing && state != State::Disconnected) {
		log_net.warn("MTCP-{} abandoned a connection handler without specifying complete or cancel. Implicitly calling cancel_and_disconnect_async", id);
		_cancel_and_disconnect_async();

	} else if (state == State::Disconnected) {
		// If in disconnected state, that means that there can't be more on_flight_operation
		// Has to post the self-destruction as it could be called from a handler callback and this destroy the handler's state in the callback
		io_context.post([self_sp = shared_from_this()] {
			auto self = self_sp.get();

			auto slock = std::unique_lock(self->strand);
			auto lock = std::unique_lock(self->mutex);

			// Move handler to stack to commit suicide
			auto temp_handler = std::move(self->handler);

			lock.unlock();
			slock.unlock();

			// Handler dtor
		});
	}
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::read_limit(std::size_t bytes_per_second) noexcept {
	auto lock = std::unique_lock(mutex);
	stream->rate_policy().read_limit(bytes_per_second);
}

inline void ImplBaseConnectionAsyncHE::write_limit(std::size_t bytes_per_second) noexcept {
	auto lock = std::unique_lock(mutex);
	stream->rate_policy().write_limit(bytes_per_second);
}

inline std::size_t ImplBaseConnectionAsyncHE::num_byte_read() const noexcept {
	auto lock = std::unique_lock(mutex);
	return stream->rate_policy().num_byte_read();
}

inline std::size_t ImplBaseConnectionAsyncHE::num_byte_wrote() const noexcept {
	auto lock = std::unique_lock(mutex);
	return stream->rate_policy().num_byte_wrote();
}

inline std::size_t ImplBaseConnectionAsyncHE::num_message_received() const noexcept {
	auto lock = std::unique_lock(mutex);
	return num_total_message_write;
}

inline std::size_t ImplBaseConnectionAsyncHE::num_message_sent() const noexcept {
	auto lock = std::unique_lock(mutex);
	return num_total_message_read;
}

inline std::size_t ImplBaseConnectionAsyncHE::send_queue_size() const noexcept {
	auto lock = std::unique_lock(mutex);
	return write_messages.size();
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::connect_async(detail::Socket&& socket) noexcept {
	auto lock = std::unique_lock(mutex);
	log_net.debug("MTCP-{} connect_async(socket)", id);

	if (state != State::Constructed)
		return log_net.error("MTCP-{} Logic error: Called connect_async(socket) on connection in incorrect state {}. Expected state {}", id, libv::to_value(state), libv::to_value(State::Constructed));

	state = State::Connecting;
	stream.emplace(std::move(socket.socket));
	_do_start(shared_from_this());
}

inline void ImplBaseConnectionAsyncHE::connect_sync(detail::Socket&& socket) noexcept {
	auto lock = std::unique_lock(mutex);
	log_net.debug("MTCP-{} connect_sync(socket)", id);

	if (state != State::Constructed)
		return log_net.error("MTCP-{} Logic error: Called connect_async(socket) on connection in incorrect state {}. Expected state {}", id, libv::to_value(state), libv::to_value(State::Constructed));

	state = State::Connecting;
	stream.emplace(std::move(socket.socket));

	_outcome_connect(lock, shared_from_this(), std::error_code());
}

inline void ImplBaseConnectionAsyncHE::connect_async(Address address) noexcept {
	auto lock = std::unique_lock(mutex);
	log_net.debug("MTCP-{} connect_async(address) to {}", id, address);

	if (state != State::Constructed)
		return log_net.error("MTCP-{} Logic error: Called connect on connect_async(address) in an incorrect state {}. Expected state {}", id, libv::to_value(state), libv::to_value(State::Constructed));

	state = State::Connecting;
	stream.emplace(io_context.context());
	_do_resolve(shared_from_this(), std::move(address));
}

inline void ImplBaseConnectionAsyncHE::_cancel_and_disconnect_async() noexcept {
	log_net.debug("MTCP-{} cancel_and_disconnect_async", id);

	if (!first_error)
		first_error = boost::asio::error::make_error_code(boost::asio::error::connection_aborted);

	switch (state) {
	case State::Constructed:
		state = State::Disconnected;
		_do_disconnect(shared_from_this());
		break;

	case State::Connecting: [[fallthrough]];
	case State::Connected: [[fallthrough]];
	case State::Completing:
		state = State::Canceling;
		stream->cancel();
		if (_no_on_flight_operation())
			_do_disconnect(shared_from_this());
		break;

	case State::Canceling:
		break; // Already canceling: noop

	case State::Disconnected:
		break; // Already disconnected, there is nothing to cancel: noop
	}
}

inline void ImplBaseConnectionAsyncHE::cancel_and_disconnect_async() noexcept {
	auto lock = std::unique_lock(mutex);
	_cancel_and_disconnect_async();
}

inline void ImplBaseConnectionAsyncHE::complete_and_disconnect_async() noexcept {
	auto lock = std::unique_lock(mutex);
	log_net.debug("MTCP-{} complete_and_disconnect_async", id);

	switch (state) {
	case State::Constructed:
		state = State::Disconnected;
		work_guard.reset();
		break;

	case State::Connecting: [[fallthrough]];
	case State::Connected:
		state = State::Completing;
		if (_no_on_flight_operation())
			_do_disconnect(shared_from_this());
		break;

	case State::Completing:
		break; // Already completing: noop

	case State::Canceling:
		log_net.error("MTCP-{} Requested complete on a connection that is being canceled", id);
		break;

	case State::Disconnected:
		log_net.warn("MTCP-{} Requested complete on a connection that has been disconnected", id);
		break;
	}
}

inline void ImplBaseConnectionAsyncHE::pause_receive_async() noexcept {
	auto lock = std::unique_lock(mutex);
	log_net.debug("MTCP-{} pause_receive_async", id);

	queue_read = false;
}

inline void ImplBaseConnectionAsyncHE::resume_receive_async() noexcept {
	auto lock = std::unique_lock(mutex);
	log_net.debug("MTCP-{} resume_receive_async", id);

	const auto was_queue_read = queue_read;

	switch (state) {
	case State::Constructed:
		log_net.error("MTCP-{} Requested receive on a connection that hasn't been connected", id);
		break;

	case State::Connecting:
		queue_read = true;
		break;

	case State::Connected:
		queue_read = true;
		if (not was_queue_read)
			_do_read(shared_from_this());
		break;

	case State::Completing:
		log_net.error("MTCP-{} Requested receive on a connection that is being completed", id);
		break;

	case State::Canceling:
		log_net.error("MTCP-{} Requested receive on a connection that is being canceled", id);
		break;

	case State::Disconnected:
		log_net.warn("MTCP-{} Requested receive on a connection that has been disconnected", id);
		break;
	}
}

template <typename M>
inline void ImplBaseConnectionAsyncHE::send_async(M&& message) noexcept {
	auto lock = std::unique_lock(mutex);
	log_net.debug("MTCP-{} Send requested for {} byte", id, message.size());

	switch (state) {
	case State::Constructed:
		log_net.error("MTCP-{} Requested send on a connection that was not connected", id);
		break;

	case State::Connecting:
		write_messages.emplace_back(
				message_header{mtcp_message_type::message, 0, static_cast<uint32_t>(message.size())},
				std::move(message));
		// Still connecting, writing will start from outcome_connect if successful
		break;

	case State::Connected: {
		const auto first_message_in_queue = write_messages.empty();
		write_messages.emplace_back(
				message_header{mtcp_message_type::message, 0, static_cast<uint32_t>(message.size())},
				std::move(message));
		if (first_message_in_queue)
			_do_write(shared_from_this());
		break;
	}

	case State::Completing:
		log_net.error("MTCP-{} Requested send on a connection that is being completed", id);
		break;

	case State::Canceling:
		log_net.error("MTCP-{} Requested send on a connection that is being canceled", id);
		break;

	case State::Disconnected:
		log_net.warn("MTCP-{} Requested send on a connection that has been disconnected", id);
		break;
	}
}

// -------------------------------------------------------------------------------------------------

inline bool ImplBaseConnectionAsyncHE::_no_on_flight_operation() const noexcept {
	return on_flight_operation_num == 0;
}

inline bool ImplBaseConnectionAsyncHE::_has_read_work() const noexcept {
	return queue_read;
}

inline bool ImplBaseConnectionAsyncHE::_has_write_work() const noexcept {
	return !write_messages.empty();
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::_outcome_connect(std::unique_lock<Mutex>& lock, SelfPtr&& self_sp, const std::error_code ec) noexcept {
	if (ec) {
		state = State::Canceling;
		// stream->cancel(); // NOTE: No need to cancel as there cannot be outstanding operation

	} else {
		boost::system::error_code l_ec;

		// Identify endpoints
		const auto asio_local_endpoint = stream->socket().local_endpoint(l_ec);
		(void) l_ec; // Ignore error case where we are actually no longer connected
		const auto asio_remote_endpoint = stream->socket().remote_endpoint(l_ec);
		(void) l_ec; // Ignore error case where we are actually no longer connected

		local_endpoint = Endpoint{asio_local_endpoint.address().to_v4().to_bytes(), asio_local_endpoint.port()};
		remote_endpoint = Endpoint{asio_remote_endpoint.address().to_v4().to_bytes(), asio_remote_endpoint.port()};

		// Setup socket options
		stream->socket().set_option(boost::asio::ip::tcp::no_delay{true}, l_ec);
		log_net.error_if(l_ec, "MTCP-{} Could not set no_delay TCP option. {}", id, libv::net::to_string(l_ec));

		stream->socket().set_option(boost::asio::ip::tcp::socket::keep_alive{true}, l_ec);
		log_net.error_if(l_ec, "MTCP-{} Could not set keep_alive TCP option. {}", id, libv::net::to_string(l_ec));
	}

	switch (state) {
	case State::Constructed: break; // Never happens
	case State::Connecting:
		state = State::Connected;
		break;

	case State::Connected: break; // Never happens
	case State::Completing:
		// Cancel reads, let writes happen
		queue_read = false;
		break;

	case State::Canceling:
		// Cancel reads and writes
		queue_read = false;
		write_messages.clear();
		break;

	case State::Disconnected: break; // Never happens
	}

	if (_has_read_work() && _has_write_work()) {
		auto self_sp2 = self_sp;
		_do_read(std::move(self_sp2));
		_do_write(std::move(self_sp));

	} else if (_has_read_work()) {
		_do_read(std::move(self_sp));

	} else if (_has_write_work()) {
		_do_write(std::move(self_sp));

	} else if (state == State::Completing || state == State::Canceling) {
		assert(_no_on_flight_operation());
		_do_disconnect(std::move(self_sp));
	}

	on_connect_was_called_with_success = !ec;

	if (ec && !first_error)
		first_error = ec;

	lock.unlock();
	if (ec)
		handler->on_connect_error(ec);
	else
		handler->on_connect();
}

inline void ImplBaseConnectionAsyncHE::outcome_connect(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	auto slock = std::unique_lock(strand);
	auto lock = std::unique_lock(mutex);

	--on_flight_operation_num;
	_outcome_connect(lock, std::move(self_sp), ec);
}

inline void ImplBaseConnectionAsyncHE::outcome_disconnect(SelfPtr&& self_sp) noexcept {
	auto slock = std::unique_lock(strand);
	auto lock = std::unique_lock(mutex);

	--on_flight_operation_num;
	assert(_no_on_flight_operation());

	if (stream) { // Stream can be nullopt if it was never connected
		boost::system::error_code ec_c;
		stream->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec_c);
		log_net.warn_if(ec_c, "MTCP-{} Error while shutting down socket: {}", id, libv::net::to_string(ec_c));
		log_net.debug_if(!ec_c, "MTCP-{} Successfully shutdown socket", id);
		stream->close();
	}

	state = State::Disconnected;
	work_guard.reset();

	if (handler_abandoned) {
		const auto stack_on_connect_was_called_with_success = on_connect_was_called_with_success;
		const auto stack_first_error = first_error;
		// Move handler to stack to commit suicide
		auto temp_handler = std::move(handler);
		lock.unlock();
		if (stack_on_connect_was_called_with_success)
			temp_handler->on_disconnect(stack_first_error);
		slock.unlock();
	} else {
		lock.unlock();
		if (on_connect_was_called_with_success)
			handler->on_disconnect(first_error);
	}

	(void) self_sp; // Let self die (if not references from anywhere else), but disconnect was the last operation
}

inline void ImplBaseConnectionAsyncHE::outcome_receive(SelfPtr&& self_sp, const std::error_code ec, bool eof_error_in_head) noexcept {
	auto slock = std::unique_lock(strand);
	auto lock = std::unique_lock(mutex);

	--on_flight_operation_num;

	auto temp_message_body = std::move(read_message_body);

	if (ec) {
		state = State::Canceling;
		stream->cancel();
	} else {
		++num_total_message_read;
	}

	switch (state) {
	case State::Constructed: break; // Never happens
	case State::Connecting: break; // Never happens
	case State::Connected:
		if (_has_read_work())
			_do_read(std::move(self_sp));
		break;

	case State::Completing: [[fallthrough]];
	case State::Canceling: [[fallthrough]];
	case State::Disconnected:
		queue_read = false;
		if (_no_on_flight_operation())
			_do_disconnect(std::move(self_sp));
		break;
	}

	if (ec && !eof_error_in_head && !first_error)
		first_error = ec;

	lock.unlock();
	if (!eof_error_in_head) {
		if (ec)
			handler->on_receive_error(ec, message_body_view{temp_message_body});
		else
			handler->on_receive(message_body_view{temp_message_body});
	}
}

inline void ImplBaseConnectionAsyncHE::outcome_send(SelfPtr&& self_sp, const std::error_code ec) noexcept {
	auto slock = std::unique_lock(strand);
	auto lock = std::unique_lock(mutex);

	--on_flight_operation_num;

	auto temp_message_body = std::move(write_messages.front().body_view);
	write_messages.pop_front();

	if (ec) {
		state = State::Canceling;
		stream->cancel();
	} else {
		++num_total_message_write;
	}

	switch (state) {
	case State::Constructed: break; // Never happens
	case State::Connecting: break; // Never happens
	case State::Connected: [[fallthrough]];
	case State::Completing:
		if (_has_write_work())
			_do_write(std::move(self_sp));
		else if (_no_on_flight_operation())
			_do_disconnect(std::move(self_sp));
		break;

	case State::Canceling: [[fallthrough]];
	case State::Disconnected:
		write_messages.clear();
		if (_no_on_flight_operation())
			_do_disconnect(std::move(self_sp));
		break;
	}

	if (ec && !first_error)
		first_error = ec;

	lock.unlock();
	if (ec)
		handler->on_send_error(ec, std::move(temp_message_body));
	else
		handler->on_send(std::move(temp_message_body));
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::_do_start(SelfPtr&& self_sp) noexcept {
	++on_flight_operation_num;
	io_context.post([self_sp = std::move(self_sp)] mutable {
		const auto self = self_sp.get();

		self->outcome_connect(std::move(self_sp), std::error_code());
	});
}

inline void ImplBaseConnectionAsyncHE::_do_resolve(SelfPtr&& self_sp, Address&& address) noexcept {
	++on_flight_operation_num;
	io_context.resolve_async(address, [self_sp = std::move(self_sp)] (const auto& ec, auto&& endpoints) mutable {
		const auto self = self_sp.get();

		if (ec) {
			log_net.error("MTCP-{} Failed to resolve {}", self->id, libv::net::to_string(ec));
			self->outcome_connect(std::move(self_sp), ec);
		} else {
			log_net.debug("MTCP-{} Successfully resolved address", self->id);
			self->_do_connect(std::move(self_sp), std::move(endpoints.results));
		}
	});
}

inline void ImplBaseConnectionAsyncHE::_do_connect(SelfPtr&& self_sp, boost::asio::ip::tcp::resolver::results_type&& endpoints) noexcept {
	using EPEntry = boost::asio::ip::tcp::resolver::results_type::value_type;

	const auto connect_condition = [id = id](const auto& ec, const EPEntry& entry) {
		log_net.warn_if(ec, "MTCP-{} Failed to connect: {}", id, libv::net::to_string(ec));

		const auto& endpoint = entry.endpoint();
		const auto is_v4 = endpoint.address().is_v4();

		log_net.debug_if(is_v4, "MTCP-{} Connecting to {}:{}...", id, endpoint.address().to_string(), endpoint.port());
		return is_v4;
	};

	stream->async_connect(std::move(endpoints), connect_condition, [self_sp = std::move(self_sp)](const auto& ec, const auto& remote_endpoint) mutable {
		(void) remote_endpoint;

		const auto self = self_sp.get();

		log_net.error_if(ec, "MTCP-{} Failed to connect: {}", self->id, libv::net::to_string(ec));
		log_net.debug_if(!ec, "MTCP-{} Successfully connected", self->id);

		self->outcome_connect(std::move(self_sp), ec);
	});
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::_do_disconnect(SelfPtr&& self_sp) noexcept {
	++on_flight_operation_num;
	io_context.post([self_sp = std::move(self_sp)] mutable {
		const auto self = self_sp.get();

		// Disconnecting will always succeed (in one way or another)
		self->outcome_disconnect(std::move(self_sp));
	});
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::_do_read(SelfPtr&& self_sp) noexcept {
	++on_flight_operation_num;
	_do_read_header(std::move(self_sp));
}

inline void ImplBaseConnectionAsyncHE::_do_read_header(SelfPtr&& self_sp) noexcept {
	boost::asio::async_read(
			*stream,
			boost::asio::buffer(read_message_header.header_data(), read_message_header.header_size()),
			[self_sp = std::move(self_sp)](const std::error_code ec, std::size_t size) mutable {
				const auto self = self_sp.get();

				if (ec) {
					const auto is_eof = ec == boost::asio::error::make_error_code(boost::asio::error::eof);
					log_net.error_if(!is_eof, "MTCP-{} Failed to read header after {} byte: {}", self->id, size, libv::net::to_string(ec));
					self->outcome_receive(std::move(self_sp), ec, is_eof);

				} else {
					log_net.debug("MTCP-{} Successfully read {} header byte", self->id, size);
					const auto read_body_size = self->read_message_header.size();

					if (read_body_size > MTCP_MESSAGE_MAX_SIZE) {
						log_net.error("MTCP-{} Payload size {} exceeds maximum size of {}", self->id, read_body_size, MTCP_MESSAGE_MAX_SIZE);
						self->outcome_receive(std::move(self_sp), boost::asio::error::make_error_code(boost::asio::error::message_size));

					} else {
						self->_do_read_body(std::move(self_sp), read_body_size);
					}
				}
			});
}

inline void ImplBaseConnectionAsyncHE::_do_read_body(SelfPtr&& self_sp, std::size_t read_body_size) noexcept {
	read_message_body.clear();
	if (read_body_size > MTCP_MESSAGE_MAX_RESERVE) {
		log_net.warn("MTCP-{} Payload size {} exceeds maximum reserve size of {}", id, read_body_size, MTCP_MESSAGE_MAX_RESERVE);
		read_message_body.reserve(MTCP_MESSAGE_MAX_RESERVE);

	} else {
		read_message_body.reserve(read_body_size);
	}

	// TODO P5: Use a non dynamic buffer if read_body_size < MTCP_MESSAGE_MAX_RESERVE, with resize instead of reserve
	boost::asio::async_read(
			*stream,
			boost::asio::dynamic_buffer(read_message_body, read_body_size),
			[self_sp = std::move(self_sp)](const std::error_code ec, std::size_t size) mutable {
				const auto self = self_sp.get();

				log_net.error_if(ec, "MTCP-{} Failed to read payload after {} byte: {}", self->id, size, libv::net::to_string(ec));
				log_net.debug_if(!ec, "MTCP-{} Successfully read {} payload byte", self->id, size);

				self->outcome_receive(std::move(self_sp), ec);
			});
}

// -------------------------------------------------------------------------------------------------

inline void ImplBaseConnectionAsyncHE::_do_write(SelfPtr&& self_sp) noexcept {
	++on_flight_operation_num;

	const auto& front_message = write_messages.front();
	const auto& front_message_body = front_message.body_view;
	write_next_header = front_message.header;

	assert(write_next_header.size() <= MTCP_MESSAGE_MAX_SIZE);
	// TODO P1: In not assert builds this will just try to execute, make a return / abort branch for it
	// 			| replace assert with recoverable error (Simple exception) | No can do: different thread
	//			| Would rather verify this inside the .send_*() functions, then exceptions would be possible

	const auto header_buffer = boost::asio::buffer(write_next_header.header_data(), write_next_header.header_size());
	const auto body_buffer = boost::asio::buffer(front_message_body.data(), front_message_body.size());
	const auto buffers = std::array<boost::asio::const_buffer, 2>{header_buffer, body_buffer};

	boost::asio::async_write(
			*stream,
			buffers,
			[self_sp = std::move(self_sp)](const std::error_code ec, std::size_t size) mutable {
				const auto self = self_sp.get();

				log_net.error_if(ec, "MTCP-{} Failed to write {} + {} byte: {}", self->id, sizeof(message_header), size - sizeof(message_header), libv::net::to_string(ec));
				log_net.debug_if(!ec, "MTCP-{} Successfully wrote {} + {} byte", self->id, sizeof(message_header), size - sizeof(message_header));

				self->outcome_send(std::move(self_sp), ec);
			});
}

// -------------------------------------------------------------------------------------------------

BaseConnectionAsyncHE::BaseConnectionAsyncHE(IOContext& io_context) noexcept :
	internals(std::make_shared<ImplBaseConnectionAsyncHE>(io_context)) {
}

BaseConnectionAsyncHE::~BaseConnectionAsyncHE() {
	log_net.trace("MTCP-{} ~BaseConnectionAsyncHE", internals->id);
}

void BaseConnectionAsyncHE::inject_handler(std::unique_ptr<BaseConnectionHandler>&& handler) noexcept {
	internals->inject_handler(std::move(handler));
}

void BaseConnectionAsyncHE::abandon_handler() noexcept {
	internals->abandon_handler();
}

void BaseConnectionAsyncHE::read_limit(std::size_t bytes_per_second) noexcept {
	internals->read_limit(bytes_per_second);
}

void BaseConnectionAsyncHE::write_limit(std::size_t bytes_per_second) noexcept {
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

void BaseConnectionAsyncHE::cancel_and_disconnect_async() noexcept {
	internals->cancel_and_disconnect_async();
}

void BaseConnectionAsyncHE::complete_and_disconnect_async() noexcept {
	internals->complete_and_disconnect_async();
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
	internals->send_async(std::as_bytes(std::span(message_str)));
}

// -------------------------------------------------------------------------------------------------

BaseConnectionHandler::~BaseConnectionHandler() {
	log_net.trace("MTCP-{} ~BaseConnectionHandler", connection.internals->id);
}

void BaseConnectionHandler::on_connect_error(error_code ec) {
	(void) ec;
}

void BaseConnectionHandler::on_receive_error(error_code ec, message_view m) {
	(void) ec;
	(void) m;
}

void BaseConnectionHandler::on_send_error(error_code ec, message_view m) {
	(void) ec;
	(void) m;
}

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
