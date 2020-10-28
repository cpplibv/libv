// Project: libv.net, File: src/libv/net/io_context.hpp, Author: Császár Mátyás [Vader]

#pragma once

#include <libv/net/fwd.hpp>
// ext
#include <netts/netfwd.hpp>
// std
#include <functional>
#include <memory>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

class IOContext {
private:
	std::unique_ptr<class ImplIOContext> impl;

public:
	explicit IOContext(size_t thread_count = 1);
	~IOContext();

public:
	[[nodiscard]] netts::io_context& context() noexcept;

public:
	void post(std::function<void()> func);
	void resolve_async(const Address& addr, std::function<void(const std::error_code, mtcp::ResolveResults)> handler);

public:
	void join();
};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
