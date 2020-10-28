// Project: libv.net, File: src/libv/net/mtcp/acceptor.hpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/net/mtcp/acceptor.hpp>
// ext
#include <netts/internet.hpp>
// std
#include <memory>
#include <mutex>
// pro
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/log.hpp>
#include <libv/net/mtcp/connection.hpp>
#include <libv/net/mtcp/socket.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

class ImplAcceptorAsyncCB : public std::enable_shared_from_this<ImplAcceptorAsyncCB> {
private:
	bool accepting = false;
	netts::ip::tcp::acceptor acceptor;
	std::recursive_mutex mutex;
	IOContext& io_context;
	AcceptorAsyncCB::CBAccept cb_accept;
	AcceptorAsyncCB::CBError cb_error;

public:
	explicit ImplAcceptorAsyncCB(IOContext& io_context) :
		acceptor(io_context.context()),
		io_context(io_context) { }

public:
	inline void listen(uint16_t port, int backlog) {
		std::unique_lock lock{mutex};
		std::error_code ec;

		const auto endpoint = netts::ip::tcp::endpoint(netts::ip::tcp::v4(), port);

		acceptor.open(endpoint.protocol(), ec);
		log_net.error_if(ec, "open {}", libv::net::to_string(ec));

		acceptor.set_option(netts::ip::tcp::acceptor::reuse_address(true), ec);
		log_net.error_if(ec, "reuse_address {}", libv::net::to_string(ec));

		acceptor.bind(endpoint, ec);
		log_net.error_if(ec, "bind {}", libv::net::to_string(ec));

		acceptor.listen(backlog, ec);
		log_net.error_if(ec, "listen {}", libv::net::to_string(ec));
	}

	inline void accept() noexcept {
		std::unique_lock lock{mutex};
		accepting = true;
		accept(shared_from_this());
	}

	inline void cancel() noexcept {
		std::unique_lock lock{mutex};

		std::error_code ignore_ec;
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

	inline void accept(std::shared_ptr<ImplAcceptorAsyncCB>&& self_sp) noexcept {
		std::unique_lock lock{mutex};
		acceptor.async_accept(
				[self_sp = std::move(self_sp)]
				(const std::error_code ec, netts::ip::tcp::socket peer) mutable {

			std::unique_lock lock{self_sp->mutex};

			if (ec)
				self_sp->cb_error(ec);
			else
				self_sp->cb_accept(ConnectionAsnycCB(self_sp->io_context, Socket{std::move(peer)}));

			if (self_sp->accepting)
				self_sp->accept(std::move(self_sp));
		});
	}
};

// -------------------------------------------------------------------------------------------------

AcceptorAsyncCB::AcceptorAsyncCB(IOContext& io_context) :
	impl(std::make_shared<ImplAcceptorAsyncCB>(io_context)) {
}

void AcceptorAsyncCB::accept() noexcept {
	impl->accept();
}

void AcceptorAsyncCB::listen(uint16_t port, int backlog) noexcept {
	impl->listen(port, backlog);
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
