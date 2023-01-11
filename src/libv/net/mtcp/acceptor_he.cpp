// Project: libv.net, File: src/libv/net/mtcp/acceptor_he.cpp

// hpp
#include <libv/net/mtcp/acceptor_he.hpp>
// ext
#include <boost/asio/ip/tcp.hpp>
// std
#include <memory>
#include <mutex>
// pro
#include <libv/net/detail/socket.hpp>
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/log.hpp>
#include <libv/net/mtcp/connection_he.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

class ImplBaseAcceptorAsyncHE : public std::enable_shared_from_this<ImplBaseAcceptorAsyncHE> {
	using SelfPtr = std::shared_ptr<ImplBaseAcceptorAsyncHE>;

private:
	enum class State {
		Constructed,
		Listening,
		Accepting,
		Canceling,
		Disconnected,
	};

private:
	State state = State::Constructed;

private:
	mutable std::recursive_mutex mutex;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard;
	IOContext& io_context;

//	bool accepting = false;
//	bool listening = false;
	boost::asio::ip::tcp::acceptor acceptor;

	bool abandoned_handler = false;
	std::unique_ptr<BaseAcceptorHandler> handler;

public:
	explicit inline ImplBaseAcceptorAsyncHE(IOContext& io_context) :
			work_guard(io_context.context().get_executor()),
			io_context(io_context),
			acceptor(io_context.context()) {}

	~ImplBaseAcceptorAsyncHE() {
		log_net.trace("Acceptor: ~ImplBaseAcceptorAsyncHE");
	}

	inline void inject_handler(std::unique_ptr<BaseAcceptorHandler>&& handler_) noexcept {
		handler = std::move(handler_);
	}

	inline void abandon_handler() noexcept {
		std::unique_lock lock{mutex};
		log_net.debug("Acceptor abandon_handler (state:{})", to_underlying(state));

		abandoned_handler = true;

		switch (state) {
		case State::Constructed: {
			state = State::Disconnected;
			// Move handler to stack to commit suicide
			auto temp_handler = std::move(handler);
			lock.unlock();
			break;
		}

		case State::Listening: {
			_cancel();
			// Move handler to stack to commit suicide
			auto temp_handler = std::move(handler);
			lock.unlock();
			break;
		}

		case State::Accepting:
			_cancel();
			break;

		case State::Canceling:
			break; // Noop

		case State::Disconnected:
			// Move handler to stack to commit suicide
			auto temp_handler = std::move(handler);
			lock.unlock();
			break;
		}
	}

public:
	[[nodiscard]] inline std::error_code listen(Endpoint endpoint_, int backlog) {
		std::unique_lock lock{mutex};

		if (state != State::Constructed) {
			log_net.error("Attempted to listen on an acceptor that left the constructed state");
			return boost::asio::error::make_error_code(boost::asio::error::already_started);
		}

		boost::system::error_code ec;

		const auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4(endpoint_.address), endpoint_.port);

		acceptor.open(endpoint.protocol(), ec);
		if (ec) {
			log_net.error("Acceptor failed to open - {}", libv::net::to_string(ec));
			return ec;
		}

		acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec);
		if (ec) {
			log_net.error("Acceptor failed to set reuse_address option - {}", libv::net::to_string(ec));
			return ec;
		}

		acceptor.bind(endpoint, ec);
		if (ec) {
			log_net.error("Acceptor failed to bind to {} - {}", fmt::streamed(endpoint), libv::net::to_string(ec));
			return ec;
		}

		acceptor.listen(backlog, ec);
		if (ec) {
			log_net.error("Acceptor failed to start listening - {}", backlog, libv::net::to_string(ec));
			return ec;
		}

		state = State::Listening;

		log_net.info("Acceptor listening on {}...", fmt::streamed(endpoint));
		return ec;
	}

	inline void accept_async() noexcept {
		std::unique_lock lock{mutex};
		log_net.debug("Acceptor accept_async (state:{})", to_underlying(state));

		if (state != State::Listening)
			return log_net.error("Attempted to accept with a non listening acceptor (Missing call or missing error check for listen)");

		state = State::Accepting;

		_do_accept(shared_from_this());
	}

private:
	inline void _cancel() noexcept {
		if (state == State::Accepting)
			state = State::Canceling;
		else if (state == State::Disconnected)
			return; // Ignore cancel request if close already happened
		else
			state = State::Disconnected;

		boost::system::error_code ec;
		acceptor.cancel(ec);
		log_net.error_if(ec, "Failed to cancel acceptor: {}", libv::net::to_string(ec));

		acceptor.close(ec);
		log_net.error_if(ec, "Failed to close acceptor: {}", libv::net::to_string(ec));
	}

public:
	inline void cancel() noexcept {
		log_net.debug("Acceptor cancel (state:{})", to_underlying(state));

		std::unique_lock lock{mutex};
		_cancel();
	}

private:
	inline void outcome_accept(SelfPtr&& self_sp, std::error_code ec, boost::asio::ip::tcp::socket&& peer) noexcept {
		std::unique_lock lock{mutex};

		bool reset_work_guard = false;
		bool was_canceling = state == State::Canceling;

		switch (state) {
		case State::Constructed:
			break; // Never happens
		case State::Listening:
			break; // Never happens
		case State::Accepting:
			_do_accept(std::move(self_sp));
			break;
		case State::Canceling:
			state = State::Disconnected;
			reset_work_guard = true;
			break;
		case State::Disconnected:
			break; // Never happens
		}

		if (ec) {
			if (abandoned_handler) {
				// Move handler to stack to commit suicide
				auto temp_handler = std::move(handler);
				lock.unlock();
				if (!was_canceling || ec != boost::asio::error::make_error_code(boost::asio::error::operation_aborted))
					temp_handler->on_accept(ec);

			} else {
				lock.unlock();
				if (!was_canceling || ec != boost::asio::error::make_error_code(boost::asio::error::operation_aborted))
					handler->on_accept(ec);
				if (reset_work_guard)
					work_guard.reset();
			}

		} else {
			auto temp_peer = std::move(peer);

			if (abandoned_handler) {
				// Move handler to stack to commit suicide
				auto temp_handler = std::move(handler);
				lock.unlock();
				auto connection = temp_handler->on_accept(io_context);
				connection.handler().connection.connect_sync(detail::Socket{std::move(temp_peer)});

			} else {
				lock.unlock();
				auto connection = handler->on_accept(io_context);
				connection.handler().connection.connect_sync(detail::Socket{std::move(temp_peer)});
				if (reset_work_guard)
					work_guard.reset();
			}
		}
	}

	inline void _do_accept(std::shared_ptr<ImplBaseAcceptorAsyncHE>&& self_sp) noexcept {
		acceptor.async_accept(
				[self_sp = std::move(self_sp)]
				(std::error_code ec, boost::asio::ip::tcp::socket peer) mutable {
					auto self = self_sp.get();

					log_net.error_if(ec && ec != boost::asio::error::make_error_code(boost::asio::error::operation_aborted),
							"Acceptor failed to accept: {}", libv::net::to_string(ec));
					log_net.debug_if(ec == boost::asio::error::make_error_code(boost::asio::error::operation_aborted),
							"Acceptor accept operation was aborted");
					log_net.debug_if(!ec, "Acceptor successfully accepted a peer");

					self->outcome_accept(std::move(self_sp), ec, std::move(peer));
				});
	}
};

// -------------------------------------------------------------------------------------------------

BaseAcceptorAsyncHE::BaseAcceptorAsyncHE(IOContext& io_context) :
		internals(std::make_shared<ImplBaseAcceptorAsyncHE>(io_context)) {
}

BaseAcceptorAsyncHE::~BaseAcceptorAsyncHE() {
	log_net.debug("Acceptor: ~BaseAcceptorAsyncHE");
}

void BaseAcceptorAsyncHE::inject_handler(std::unique_ptr<BaseAcceptorHandler>&& handler) noexcept {
	internals->inject_handler(std::move(handler));
}

void BaseAcceptorAsyncHE::abandon_handler() noexcept {
	internals->abandon_handler();
}

std::error_code BaseAcceptorAsyncHE::listen(Endpoint endpoint, int backlog) noexcept {
	return internals->listen(endpoint, backlog);
}

std::error_code BaseAcceptorAsyncHE::listen(uint16_t port, int backlog) noexcept {
	return internals->listen(Endpoint(0, 0, 0, 0, port), backlog);
}

void BaseAcceptorAsyncHE::accept_async() noexcept {
	internals->accept_async();
}

void BaseAcceptorAsyncHE::cancel() noexcept {
	internals->cancel();
}

// -------------------------------------------------------------------------------------------------

BaseAcceptorHandler::~BaseAcceptorHandler() {
	log_net.trace("Acceptor: ~BaseAcceptorHandler");
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
