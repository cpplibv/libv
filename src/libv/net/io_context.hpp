// Project: libv.net, File: src/libv/net/io_context.hpp

#pragma once

// fwd
#include <libv/net/detail/io_context_fwd.hpp>
#include <libv/net/detail/resolve_results_fwd.hpp>
// std
#include <functional>
#include <memory>
// pro
#include <libv/net/address.hpp>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

class IOContext {
private:
	std::unique_ptr<class ImplIOContext> impl;

public:
	explicit IOContext(std::size_t thread_count = 1);
	~IOContext();

public:
	[[nodiscard]] boost::asio::io_context& context() noexcept;

public:
	void post(std::function<void()> func);
	void resolve_async(const Address& addr, std::function<void(const std::error_code, mtcp::ResolveResults)> handler);

public:
	void join();
};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
