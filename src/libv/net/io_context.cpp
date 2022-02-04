// Project: libv.net, File: src/libv/net/io_context.cpp

// hpp
#include <libv/net/io_context.hpp>
// ext
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
// libv
#include <libv/mt/number.hpp>
// std
#include <thread>
#include <vector>
// pro
#include <libv/net/address.hpp>
#include <libv/net/detail/resolve_results.hpp>
#include <libv/net/log.hpp>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

class ImplIOContext {
private:
	boost::asio::io_context io_context;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard;

	boost::asio::ip::tcp::resolver resolver;

	std::vector<std::thread> threads;

private:
	using ID = int64_t;
	static inline std::atomic<ID> nextID = 0;
	const ID id = nextID++; /// Informative ID for logging and monitoring

private:
	void start_thread(std::size_t i, std::size_t thread_count) {
		const auto parentID = libv::thread_number();
		threads.emplace_back([this, parentID, i, thread_count] {
			log_net.trace("IOContext-{} worker thread {} ({}/{}) started by thread {}", id, libv::thread_number(), i + 1, thread_count, parentID);
			io_context.run();
		});
	}

public:
	explicit ImplIOContext(std::size_t thread_count = 1) :
		work_guard(io_context.get_executor()),
		resolver(io_context) {
		for (std::size_t i = 0; i < thread_count; ++i)
			start_thread(i, thread_count);
	}

	~ImplIOContext() {
		join();
		log_net.trace("IOContext-{}: ~IOContext", id);
	}

public:
	[[nodiscard]] boost::asio::io_context& context() noexcept {
		return io_context;
	}

public:
	template <typename F>
	void post(F&& func) {
		io_context.get_executor().post(std::forward<F>(func), std::allocator<void>{});
	}

	template <typename ResolveHandlerT>
	void resolve_async(const Address& addr, ResolveHandlerT&& handler) {
		log_net.debug("Resolving {}:{}...", addr.address, addr.service);

		resolver.async_resolve(addr.address, addr.service,
				[handler = std::forward<ResolveHandlerT>(handler)] (const auto& ec, auto results) mutable {
					if (!ec) {
						log_net.debug("Resolved to {} entry", results.size());
						for (const auto& entry : results) {
							log_net.debug("    ...{}:{}", entry.endpoint().address().to_string(), entry.endpoint().port());
						}
					}

					handler(ec, mtcp::ResolveResults{std::move(results)});
				});
	}

public:
	void join() {
		log_net.trace("IOContext-{} Joining...", id);
		work_guard.reset();
		for (auto& thread : threads) {
			if (thread.joinable())
				thread.join();
		}
		log_net.trace("IOContext-{} Joined", id);
	}
};

// -------------------------------------------------------------------------------------------------

IOContext::IOContext(std::size_t thread_count) :
	impl(std::make_unique<ImplIOContext>(thread_count)) {
}

IOContext::~IOContext() = default; // For the sake of forward declared unique_ptr

boost::asio::io_context& IOContext::context() noexcept {
	return impl->context();
}

void IOContext::post(std::function<void()> func) {
	impl->post(std::move(func));
}

void IOContext::resolve_async(const Address& addr, std::function<void(const std::error_code, mtcp::ResolveResults)> handler) {
	impl->resolve_async(addr, std::move(handler));
}

void IOContext::join() {
	impl->join();
}

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
