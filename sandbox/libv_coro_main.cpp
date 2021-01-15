// Project: libv, File: sandbox/libv_coro_main.cpp, Author: Császár Mátyás [Vader]

// ext
#include <cppcoro/async_generator.hpp>
#include <cppcoro/generator.hpp>
#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/static_thread_pool.hpp>
// libv
#include <libv/mt/worker_thread_pool.hpp>
// std
#include <chrono>
#include <coroutine>
#include <iostream>
#include <string>
#include <thread>


// -------------------------------------------------------------------------------------------------

struct my_coro_thread_pool {
	libv::mt::worker_thread_pool threads;

public:
	explicit my_coro_thread_pool(int32_t n) : threads(n) {}

public:
	struct schedule_operation : std::suspend_always {
		my_coro_thread_pool* thread_pool;

		inline void await_suspend(cppcoro::coroutine_handle<> coro) noexcept {
			thread_pool->threads.execute_async([coro] { coro.resume(); });
		}
	};

	struct schedule_operation_after : std::suspend_always {
		my_coro_thread_pool* thread_pool;
		std::chrono::steady_clock::duration after;

		inline void await_suspend(cppcoro::coroutine_handle<> coro) noexcept {
			thread_pool->threads.execute_async([coro] { coro.resume(); }, after);
		}
	};

	struct schedule_operation_at : std::suspend_always {
		my_coro_thread_pool* thread_pool;
		std::chrono::steady_clock::time_point at;

		inline void await_suspend(cppcoro::coroutine_handle<> coro) noexcept {
			thread_pool->threads.execute_async([coro] { coro.resume(); }, at);
		}
	};

public:
	[[nodiscard]] schedule_operation schedule() {
		return schedule_operation{{}, this};
	}

	[[nodiscard]] schedule_operation_after schedule(std::chrono::steady_clock::duration after) {
		return schedule_operation_after{{}, this, after};
	}

	[[nodiscard]] schedule_operation_at schedule(std::chrono::steady_clock::time_point at) {
		return schedule_operation_at{{}, this, at};
	}
};

// -------------------------------------------------------------------------------------------------

//template <typename CoroSequence, typename F>
//inline void for_co_await(CoroSequence&& coro_sequence, F&& func) {
//	auto it = co_await coro_sequence.begin();
//	const auto end = coro_sequence.end();
//	while (it != end) {
//		func(std::forward<decltype(*it)>(*it));
//		co_await ++it;
//	}
//}

//for (auto&& s = SEQUENCE, auto it = co_await s.begin(), const auto end = s.end(); it != end; co_await ++it)

//auto s = ticker(500, tp);
//	for (auto it = co_await s.begin(); it != s.end(); co_await ++it)

//auto s = ticker(500, tp);
//for (auto it = co_await s.begin(); it != s.end(); co_await ++it) {
//	const auto& i = *it;
//	...
//}

// -------------------------------------------------------------------------------------------------

//using thread_pool = cppcoro::static_thread_pool;
using thread_pool = my_coro_thread_pool;

cppcoro::async_generator<int> ticker(int count, thread_pool& tp) {
	std::cout << "Start Ticker. " << " Thread: " << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < count; ++i) {
		std::cout << "Work A" << " Thread: " << std::this_thread::get_id() << std::endl;
		co_await tp.schedule();
		std::cout << "Work B" << " Thread: " << std::this_thread::get_id() << std::endl;
		co_await tp.schedule(std::chrono::microseconds(50));
		std::cout << "Work C" << " Thread: " << std::this_thread::get_id() << std::endl;
		co_yield i;
		std::cout << "Work D" << " Thread: " << std::this_thread::get_id() << std::endl;
	}
}

cppcoro::task<> consumer(thread_pool& tp) {
	std::cout << "Start Consumer. " << " Thread: " << std::this_thread::get_id() << std::endl;

//	for_co_await(ticker(500, tp), [](auto i){
//		std::cout << "Tick: " << i << " Thread: " << std::this_thread::get_id() << std::endl;
//	});

//	for_co_await(i, ticker(500, tp)) {
//		std::cout << "Tick: " << i << " Thread: " << std::this_thread::get_id() << std::endl;
//	}

	auto s = ticker(500, tp);
	for (auto it = co_await s.begin(); it != s.end(); co_await ++it) {
		const auto& i = *it;
		std::cout << "Tick: " << i << " Thread: " << std::this_thread::get_id() << std::endl;
	}

//	for	co_await(std::uint32_t	i :	sequence) {
//		std::cout << "Tick " << i << std::endl;
//	}
}

int main() {
	std::cout << "Hello coroutines" << " Thread: " << std::this_thread::get_id() << std::endl;
	thread_pool threads(4);

	cppcoro::sync_wait(consumer(threads));

	std::cout << "stopping and joining" << " Thread: " << std::this_thread::get_id() << std::endl;
}

// =================================================================================================

//template <typename C>
//cppcoro::task<> example_test_network_protocol_task(C connection) {
//	int codec;
//	int con = codec + connection + intent_to_coro;
//	int* resource;
//
//	{
//		const auto message = co_await con.receive());
//
//		if (const auto request = message.handle<RequestResourceSelect>(); request) {
//			resource = server.find_resource(request->resource);
//			if (!resource)
//				con.send(ResponseResourceNotFound{});
//		}
//		if (const auto request = message.handle<RequestResourceDescription>(); request) {
//			resource = server.find_resource(request->resource);
//			if (!resource)
//				con.send(ResponseResourceNotFound{});
//			else
//				con.send(ResponseResourceDescription{resource->size()});
//		}
//		if (message.is_unhandled()) {
//			log.error("...");
//			co_return disconnect_communtion_error;
//		}
//	}
//
//
//	{
//		const auto message = co_await con.receive());
//
//		if (const auto request = message.handle<RequestResourceSelect>(); request) {
//			resource = server.find_resource(request->resource);
//			if (!resource)
//				con.send(ResponseResourceNotFound{});
//		}
//		if (const auto request = message.handle<RequestResourceDescription>(); request) {
//			resource = server.find_resource(request->resource);
//			if (!resource)
//				con.send(ResponseResourceNotFound{});
//			else
//				con.send(ResponseResourceDescription{resource->size()});
//		}
//		if (const auto request = message.handle<RequestResourceData>(); request) {
//			if (!resource){
//				log.error("...");
//				co_return disconnect_state_error;
//			}
//
//			for (int i = request.offset; i < request.offset + request.amount; i += network_chunk_size)
//				// "SPAWN" ASYNC STATE to handle cancel
//				co_await con.send_with_queue(2, ResponseResourceChunk{i, resource->chunk(i)});
//		}
//		if (message.is_unhandled()) {
//			log.error("...");
//			co_return disconnect_communtion_error;
//		}
//	}
//}
//
//	co_await con.receive(
//			[](RequestResourceSelect) {
//			},
//			[](RequestResourceDescription) {
//			},
//			[](auto&&) {
//			}
//	);
//

// -------------------------------------------------------------------------------------------------
