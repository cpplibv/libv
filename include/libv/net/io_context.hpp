// File: io_context.hpp Author: Vader Created on 2017. június 21., 2:37

#pragma once

// ext
#include <netts/internet.hpp>
#include <netts/io_context.hpp>
// libv
#include <libv/thread/number.hpp>
// std
#include <optional>
#include <thread>
#include <vector>
// pro
#include <libv/net/address.hpp>
#include <libv/net/log.hpp>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

class IOContext {
private:
	netts::io_context io_context;
	netts::ip::tcp::resolver resolver;
	netts::executor_work_guard<netts::io_context::executor_type> work_guard;

	std::vector<std::thread> threads;

private:
	using ID = int64_t;
	static inline std::atomic<ID> nextID = 0;
	const ID id = nextID++; /// Informative ID for logging and monitoring

private:
	void start_thread() {
		const auto parentID = libv::thread_number();
		threads.emplace_back([this, parentID] {
			log_net.trace("IOContext-{} worker {} started by thread {}", id, libv::thread_number(), parentID);
			io_context.run();
		});
	}

public:
	explicit IOContext(size_t thread_count = 1) :
		resolver(io_context),
		work_guard(io_context.get_executor()) {
		for (size_t i = 0; i < thread_count; ++i)
			start_thread();
	}

	netts::io_context& context() {
		return io_context;
	}

	template <typename F>
	void post(F&& func) {
		io_context.get_executor().post(std::forward<F>(func), std::allocator<void>{});
	}

	void join() {
		work_guard.reset();
		for (auto& thread : threads) {
			if (thread.joinable())
				thread.join();
		}
	}

	~IOContext() {
		work_guard.reset();
		io_context.stop();
		join();
	}

	template <typename ResolveHandlerT>
	void resolve_async(const Address& addr, ResolveHandlerT&& handler) {
		log_net.debug("Resolving {}:{}...", addr.address, addr.service);

		resolver.async_resolve(addr.address, addr.service,
				[handler = std::forward<ResolveHandlerT>(handler)] (const auto& ec, auto endpoints) mutable {
					handler(ec, std::move(endpoints));
				});
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
