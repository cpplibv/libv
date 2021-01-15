// Project: libv.net, File: src/libv/net/mtcp/acceptor.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/net/mtcp/acceptor_he.hpp>
// ext
#include <boost/asio/ip/tcp.hpp>
// std
#include <memory>
#include <mutex>
// pro
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/log.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/net/mtcp/socket.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

class ImplBaseAcceptorAsyncHE : public std::enable_shared_from_this<ImplBaseAcceptorAsyncHE> {
private:
	bool accepting = false;
	boost::asio::ip::tcp::acceptor acceptor;
	std::recursive_mutex mutex;
	IOContext& io_context;

	bool abandoned_handler = false;
	std::unique_ptr<BaseAcceptorHandler> handler;

public:
	explicit inline ImplBaseAcceptorAsyncHE(IOContext& io_context) :
		acceptor(io_context.context()),
		io_context(io_context) { }

	inline void inject_handler(std::unique_ptr<BaseAcceptorHandler>&& handler_) noexcept {
		handler = std::move(handler_);
	}

	inline void abandon_handler() noexcept {
		std::unique_lock lock{mutex};
		abandoned_handler = true;

		if (!accepting)
			auto temp = std::move(handler); // Move handler to stack to commit suicide
	}

public:
	[[nodiscard]] inline std::error_code listen(Endpoint endpoint_, int backlog) {
		std::unique_lock lock{mutex};
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
			log_net.error("Acceptor failed to bind to {} - {}", endpoint, libv::net::to_string(ec));
			return ec;
		}

		acceptor.listen(backlog, ec);
		if (ec) {
			log_net.error("Acceptor failed to start listening - {}", backlog, libv::net::to_string(ec));
			return ec;
		}

		log_net.trace("Acceptor listens to {}...", endpoint);
		return ec;
	}

	inline void accept_async() noexcept {
		std::unique_lock lock{mutex};

		log_net.trace("Acceptor accept_async");
		accepting = true;
		_do_accept(shared_from_this());
	}

	inline void cancel() noexcept {
		std::unique_lock lock{mutex};

		boost::system::error_code ignore_ec;
		acceptor.close(ignore_ec);
		(void) ignore_ec; // Ignore close error

		accepting = false;
	}

	inline void _do_accept(std::shared_ptr<ImplBaseAcceptorAsyncHE>&& self_sp) noexcept {
		acceptor.async_accept(
				[self_sp = std::move(self_sp)]
//				(const std::error_code ec, boost::asio::ip::tcp::socket peer) mutable {
				(boost::system::error_code ec, boost::asio::ip::tcp::socket peer) mutable {

			std::unique_lock lock{self_sp->mutex};

			if (ec) {
				self_sp->handler->on_accept(ec);

			} else {
//				self_sp->cb_accept(ConnectionAsyncCB(self_sp->io_context, Socket{std::move(peer)}));
				auto connection = self_sp->handler->on_accept(self_sp->io_context);
				connection.handler().connection.connect_sync(Socket{std::move(peer)});
			}

			if (self_sp->accepting)
				self_sp->_do_accept(std::move(self_sp));
			else if (self_sp->abandoned_handler)
				auto temp = std::move(self_sp->handler); // Move handler to stack to commit suicide
		});
	}
};

// -------------------------------------------------------------------------------------------------

BaseAcceptorAsyncHE::BaseAcceptorAsyncHE(IOContext& io_context) :
	internals(std::make_shared<ImplBaseAcceptorAsyncHE>(io_context)) {
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

} // namespace mtcp
} // namespace net
} // namespace libv
