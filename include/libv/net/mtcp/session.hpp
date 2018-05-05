// File: acceptor.hpp - Created on 2018.05.06. 02:35 - Author: Vader

#pragma once

// ext
#include <netts/internet.hpp>
// pro
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/connection.hpp>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

template <typename CRTP>
class SessionAsnyc {
private:
	netts::ip::tcp::acceptor acceptor;
	IOContext& io_context;

public:
	Acceptor(IOContext& io_context) :
		acceptor(io_context.context()),
		io_context(io_context) { }

public:
	inline void listen(uint16_t port) {
		std::error_code ec;
		acceptor.bind(netts::ip::tcp::endpoint(netts::ip::tcp::v4(), port), ec);

		(void) ec; // TODO P1: Handle ec
	}

	inline void handle_accept(CBAccept callback) noexcept {
		cb_accept = std::move(callback);
	}

	inline void accept() noexcept {
		acceptor.async_accept(
				[this, handler = std::move(handler)]
				(const std::error_code ec, netts::ip::tcp::socket peer) {
//			if (ec)
//				handler(outcome::failure(ec));
//			else
//				handler(outcome::success(Connection(create_ConnectionMTCP(io_context, std::move(peer)))));
		});
	}

	// Close
	[[maybe_unused]] inline OutcomeClose close() noexcept {
		OutcomeClose ec;
		acceptor.close(ec);
		return ec;
	}
};

struct Acceptor {
private:
	std::shared_ptr<Acceptor> internal;

public:
	inline Acceptor(const Acceptor&) noexcept = default;
	inline Acceptor(Acceptor&&) noexcept = default;

	inline explicit Acceptor(std::shared_ptr<Acceptor> internal) noexcept :
		internal(std::move(internal)) { }

	explicit Acceptor(IOContext& io_context, Port port, Protocol protocol) noexcept {
		(void) protocol;
		// TODO P4: libv.net Templatize both Acceptor and Connection. Elevate Protocol into struct.
		// TODO P4: libv.net Handle protocols
		// TODO P4: libv.net Move this constructor into a source file
		internal = std::make_shared<Acceptor>(io_context, port);
	}

	~Acceptor() noexcept = default;

public:
	// Accept
	template <typename HandlerAcceptT>
	inline void accept_async(HandlerAcceptT&& handler) noexcept {
		internal->accept_async(std::forward<HandlerAcceptT>(handler));
	}

	template <typename HandlerAcceptRepeatT>
	inline void accept_async_repeat(HandlerAcceptRepeatT&& handler) noexcept {
		internal->accept_async_repeat(std::forward<HandlerAcceptRepeatT>(handler));
	}

	[[nodiscard]] inline OutcomeAccept accept_sync() noexcept {
		return internal->accept_sync();
	}

	// Close
	[[maybe_unused]] inline OutcomeClose close() noexcept {
		return internal->close();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
