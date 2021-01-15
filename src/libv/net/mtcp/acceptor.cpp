// Project: libv.net, File: src/libv/net/mtcp/acceptor.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/net/mtcp/acceptor.hpp>
// ext
#include <boost/asio/ip/tcp.hpp>
// std
#include <memory>
#include <mutex>
// pro
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/log.hpp>
#include <libv/net/mtcp/connection_cb.hpp>
#include <libv/net/mtcp/socket.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

class ImplAcceptorAsyncCB : public std::enable_shared_from_this<ImplAcceptorAsyncCB> {
	using ErrorSource = AcceptorAsyncCB::ErrorSource;

private:
	bool accepting = false;
	boost::asio::ip::tcp::acceptor acceptor;
	std::recursive_mutex mutex;
	IOContext& io_context;
	AcceptorAsyncCB::CBAccept cb_accept;
	AcceptorAsyncCB::CBError cb_error;

public:
	explicit ImplAcceptorAsyncCB(IOContext& io_context) :
		acceptor(io_context.context()),
		io_context(io_context) { }

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

		return ec;
	}

	inline void accept() noexcept {
		std::unique_lock lock{mutex};
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

	inline void handle_accept(AcceptorAsyncCB::CBAccept callback) noexcept {
		std::unique_lock lock{mutex};
		cb_accept = std::move(callback);
	}

	inline void handle_error(AcceptorAsyncCB::CBError callback) noexcept {
		std::unique_lock lock{mutex};
		cb_error = std::move(callback);
	}

	inline void _do_accept(std::shared_ptr<ImplAcceptorAsyncCB>&& self_sp) noexcept {
		acceptor.async_accept(
				[self_sp = std::move(self_sp)]
//				(const std::error_code ec, boost::asio::ip::tcp::socket peer) mutable {
				(boost::system::error_code ec, boost::asio::ip::tcp::socket peer) mutable {

			std::unique_lock lock{self_sp->mutex};

			if (ec)
				self_sp->cb_error(ErrorSource::accept, ec);
			else
				self_sp->cb_accept(ConnectionAsyncCB(self_sp->io_context, Socket{std::move(peer)}));

			if (self_sp->accepting)
				self_sp->_do_accept(std::move(self_sp));
		});
	}
};

// -------------------------------------------------------------------------------------------------

AcceptorAsyncCB::AcceptorAsyncCB(IOContext& io_context) :
	impl(std::make_shared<ImplAcceptorAsyncCB>(io_context)) {
}

std::error_code AcceptorAsyncCB::listen(Endpoint endpoint, int backlog) noexcept {
	return impl->listen(endpoint, backlog);
}

std::error_code AcceptorAsyncCB::listen(uint16_t port, int backlog) noexcept {
	return impl->listen(Endpoint(0, 0, 0, 0, port), backlog);
}

void AcceptorAsyncCB::accept() noexcept {
	impl->accept();
}

void AcceptorAsyncCB::cancel() noexcept {
	impl->cancel();
}

void AcceptorAsyncCB::handle_accept(CBAccept callback) noexcept {
	impl->handle_accept(std::move(callback));
}

void AcceptorAsyncCB::handle_error(CBError callback) noexcept {
	impl->handle_error(std::move(callback));
}

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
