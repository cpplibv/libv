// File: acceptor.hpp - Created on 2018.05.06. 02:35 - Author: Vader

#pragma once

// ext
#include <netts/internet.hpp>
// std
#include <memory>
#include <mutex>
// pro
#include <libv/net/io_context.hpp>
#include <libv/net/log.hpp>
#include <libv/net/mtcp/connection.hpp>
#include <libv/net/mtcp/socket.hpp>


namespace libv {
namespace net {
namespace mtcp {
namespace detail {

// -------------------------------------------------------------------------------------------------

// TODO P4: libv.net Shit-show file
// TODO P4: libv.net Move into a source file

class ImplAcceptorAsyncCB : public std::enable_shared_from_this<ImplAcceptorAsyncCB> {
public:
	using CBAccept = std::function<void(ConnectionAsnycCB)>;
	using CBError = std::function<void(const std::error_code)>;

private:
	bool accepting = false;
	netts::ip::tcp::acceptor acceptor;
	std::recursive_mutex mutex;
	IOContext& io_context;
	CBAccept cb_accept;
	CBError cb_error;

public:
	ImplAcceptorAsyncCB(IOContext& io_context) :
		acceptor(io_context.context()),
		io_context(io_context) { }

public:
	inline void listen(uint16_t port, int backlog = 3) {
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
		(void) ec; // TODO P1: Proper handle ec / log
	}

	inline void accept() noexcept {
		std::unique_lock lock{mutex};
		accepting = true;
		accept(shared_from_this());
	}

	inline void cancel() noexcept {
		std::unique_lock lock{mutex};
		std::error_code ec;
		acceptor.close(ec);
		(void) ec; // Ignore close error

		accepting = false;
	}

	inline void handle_accept(CBAccept callback) noexcept {
		cb_accept = std::move(callback);
	}

	inline void handle_error(CBError callback) noexcept {
		cb_error = std::move(callback);
	}

	inline void accept(std::shared_ptr<ImplAcceptorAsyncCB>&& self_sp) noexcept {
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

} // namespace detail


struct AcceptorAsyncCB {
	using Impl = detail::ImplAcceptorAsyncCB;
	using Connection = ConnectionAsnycCB;

private:
	std::shared_ptr<Impl> impl;

public:
	inline AcceptorAsyncCB(IOContext& io_context) :
		impl(std::make_shared<Impl>(io_context)) {
	}

public:
	inline void accept() noexcept {
		impl->accept();
	}
	inline void listen(uint16_t port) noexcept {
		impl->listen(port);
	}
	inline void cancel() noexcept {
		impl->cancel();
	}
	inline void handle_accept(Impl::CBAccept callback) noexcept {
		impl->handle_accept(std::move(callback));
	}
	inline void handle_error(Impl::CBError callback) noexcept {
		impl->handle_error(std::move(callback));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
