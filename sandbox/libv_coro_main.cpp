// Project: libv, File: sandbox/libv_immer_main.cpp, Author: Császár Mátyás [Vader]

// std
#include <iostream>
#include <coroutine>
#include <string>
#include <chrono>
#include <thread>

#include <cppcoro/async_generator.hpp>
#include <cppcoro/generator.hpp>
#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/static_thread_pool.hpp>

#include <libv/mt/worker_thread_pool.hpp>


// -------------------------------------------------------------------------------------------------

struct my_coro_thread_pool {
	libv::mt::worker_thread_pool threads{4};

	my_coro_thread_pool(int32_t n) :
		threads(n) {
	}

	void impl_schedule(class schedule_operation*);

	class [[nodiscard]] schedule_operation {
	private:
		friend class my_coro_thread_pool;

		my_coro_thread_pool* m_threadPool;
		cppcoro::coroutine_handle<> m_awaitingCoroutine;
//		schedule_operation* m_next;

	public:
		explicit schedule_operation(my_coro_thread_pool* tp) noexcept : m_threadPool(tp) { }

		constexpr inline bool await_ready() noexcept {
			return false;
		}
		inline void await_suspend(cppcoro::coroutine_handle<> awaitingCoroutine) noexcept {
			m_awaitingCoroutine = awaitingCoroutine;
			m_threadPool->impl_schedule(this);
		}
		constexpr inline void await_resume() noexcept {}
	};

	void impl_schedule(schedule_operation* sop) {
		return threads.execute_async([sop]() {
			sop->m_awaitingCoroutine.resume();
		});
	}

	[[nodiscard]] schedule_operation schedule() {
		return schedule_operation{this};
	}
};

// -------------------------------------------------------------------------------------------------

//using thread_pool = cppcoro::static_thread_pool;
using thread_pool = my_coro_thread_pool;
//using thread_pool = libv::mt::worker_thread_pool;

cppcoro::async_generator<int> ticker(int count, thread_pool& tp) {
	std::cout << "Start Ticker. " << " Thread: " << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < count; ++i) {
		co_await tp.schedule();
		co_yield i;
	}
}

cppcoro::task<> consumer(thread_pool& tp) {
	std::cout << "Start Consumer. " << " Thread: " << std::this_thread::get_id() << std::endl;
	auto sequence = ticker(500, tp);
	auto it = co_await sequence.begin();
	const auto end = sequence.end();
	for	(; it != end;) {
		const auto& i = *it;
		std::cout << "Tick: " << i << " Thread: " << std::this_thread::get_id() << std::endl;
		co_await ++it;
	}
//	for	co_await(std::uint32_t	i :	sequence) {
//		std::cout << "Tick " << i << std::endl;
//	}
}

int main() {
	std::cout << "Hello coroutines" << " Thread: " << std::this_thread::get_id() << std::endl;
//	libv::mt::worker_thread_pool threads(4);
	thread_pool threads(4);

	cppcoro::sync_wait(consumer(threads));

	std::cout << "stopping and joining" << " Thread: " << std::this_thread::get_id() << std::endl;
//	threads.stop();
//	threads.join();
}

// -------------------------------------------------------------------------------------------------

//template <typename Threads>
//auto async_wait_receive(Threads& t, std::chrono::steady_clock::duration d) {
//	struct Awaiter {
//		struct Callback {
//			Awaiter* me;
//			std::coroutine_handle<> coro;
//
//			Callback(Awaiter* me, std::coroutine_handle<> coro) :
//				me(me),
//				coro(coro) {}
//
//			Callback(const Callback&) = delete;
//
//			Callback(Callback&& other) :
//				me(other.me),
//				coro(other.coro) {
//				other.coro = nullptr;
//			}
//
////			void operator()(std::error_code ec) {
//			void operator()() {
////				me->ec = ec;
//				auto tmp = coro;
//				coro = {};
//				std::cout << "execute_async" << std::endl;
//				tmp.resume();
//			}
//
//			~Callback() {
//				if (coro) {
////					me->ec = std::errc::operation_canceled;
//					std::cout << "canceling" << std::endl;
//					coro.resume();
//				}
//			}
//		};
//
//
//		Threads& t;
//		std::chrono::steady_clock::duration d;
////		std::error_code ec;
//
//		bool await_ready() {
////			return d.count() == 0;
//			return false;
//		}
//
//		void await_resume() {
////			if (ec)
////				throw std::system_error(ec);
//		}
//
////		auto await_result() {
////			return 42;
////		}
//
//		void await_suspend(std::coroutine_handle<> coro) {
//			t.execute_async(Callback{this, coro}, d);
////			t.execute_async([this, coro]() mutable {
//////				this->ec = ec;
////				std::cout << "execute_async" << std::endl;
////				coro.resume();
////			}, d);
//		}
//	};
//
//	return Awaiter{t, d};
//}
//
//// -------------------------------------------------------------------------------------------------
//
////cppcoro::task<std::string> usage() {
////cppcoro::task<> usage(libv::mt::worker_thread_pool& threads) {
//////	co_return "Hi";
////	co_await async_wait_receive(threads, std::chrono::seconds(1));
//////	co_return;
////}
//
//int main() {
//	std::cout << "Hello coroutines" << std::endl;
//	libv::mt::worker_thread_pool threads(4);
//
////	cppcoro::sync_wait(usage(threads));
////	auto x = async_wait_receive(threads, std::chrono::milliseconds(200));
////	cppcoro::sync_wait(x);
//	cppcoro::sync_wait(async_wait_receive(threads, std::chrono::milliseconds(200)));
//
//	std::cout << "stopping and joining" << std::endl;
//	threads.stop();
//	threads.join();
//}

// -------------------------------------------------------------------------------------------------

//using std::chrono::high_resolution_clock;
//using std::chrono::time_point;

//auto getTimeSince(const time_point<::high_resolution_clock>& start) {
//	auto end = high_resolution_clock::now();
//	std::chrono::duration<double> elapsed = end - start;
//	return elapsed.count();
//}
//
//cppcoro::task<> third(const time_point<high_resolution_clock>& start) {
//	std::cout << "Third waited " << getTimeSince(start) << " seconds." << std::endl;
//	std::this_thread::sleep_for(std::chrono::seconds(1));
//	co_return;
//}
//
//cppcoro::task<> second(const time_point<high_resolution_clock>& start) {
//	auto thi = third(start);
//	co_await thi;
//
//	std::cout << "Second waited " << getTimeSince(start) << " seconds." << std::endl;
//	std::this_thread::sleep_for(std::chrono::seconds(1));
//}
//
//cppcoro::task<> first(const time_point<high_resolution_clock>& start) {
//	auto sec = second(start);
//	co_await sec;
//
//	std::cout << "First waited " << getTimeSince(start) << " seconds." << std::endl;
//	std::this_thread::sleep_for(std::chrono::seconds(1));
//}
//
//int main() {
//	std::cout << "Hello coroutines" << std::endl;
//
//	cppcoro::sync_wait(async_wait_receive(threads, std::chrono::seconds(1)));
//
////	std::cout << std::endl;
////	auto start = ::high_resolution_clock::now();
////	cppcoro::sync_wait(first(start));
////	std::cout << "Main waited " << getTimeSince(start) << " seconds." << std::endl;
////	std::cout << std::endl;
//}

//// -------------------------------------------------------------------------------------------------
//
//
////cppcoro::single_consumer_event event;
////std::string value;
////
////cppcoro::task<> consumer() {
////	// Coroutine will suspend here until some thread calls event.set()
////	// eg. inside the producer() function below.
////	co_await event;
////
////	std::cout << value << std::endl;
////}
////
////void producer() {
////	value = "foo";
////
////	// This will resume the consumer() coroutine inside the call to set()
////	// if it is currently suspended.
////	event.set();
////}
//
//// -------------------------------------------------------------------------------------------------
//
////cppcoro::generator<uint64_t> fibonacci() {
////	std::uint64_t a = 0, b = 1;
////	while (true) {
////		co_yield b;
////		auto tmp = a;
////		a = b;
////		b += tmp;
////	}
////}
//


//cppcoro::task<std::string> usage() {
//	co_await async_wait_receive(threads, std::chrono::seconds(1));
//	co_return "Hi";
//}
//
//int main() {
//	std::cout << "Hello coroutines" << std::endl;
//
////	auto task = detail::make_sync_wait_task(std::forward<AWAITABLE>(awaitable));
////	detail::lightweight_manual_reset_event event;
////	task.start(event);
////	event.wait();
////	return task.result();
//
//	cppcoro::sync_wait(usage());
//}
//
//// -------------------------------------------------------------------------------------------------
//
//
////auto switch_to_new_thread(std::jthread& out) {
////	struct awaitable {
////		std::jthread* p_out;
////
////		bool await_ready() {
////			return false;
////		}
////
////		void await_suspend(std::coroutine_handle<> h) {
////			std::jthread& out = *p_out;
////			if (out.joinable())
////				throw std::runtime_error("Output jthread parameter not empty");
////			out = std::jthread([h] { h.resume(); });
////			// Potential undefined behavior: accessing potentially destroyed *this
////			// std::cout << "New thread ID: " << p_out->get_id() << '\n';
////			std::cout << "New thread ID: " << out.get_id() << '\n'; // this is OK
////		}
////
////		void await_resume() {}
////	};
////
////	return awaitable{&out};
////}
////
////struct task {
////	struct promise_type {
////		task get_return_object() {
////			return {};
////		}
////		std::suspend_never initial_suspend() {
////			return {};
////		}
////		std::suspend_never final_suspend() noexcept {
////			return {};
////		}
////		void return_void() {}
////		void unhandled_exception() {}
////	};
////};
////
////task resuming_on_new_thread(std::jthread& out) {
////	std::cout << "Coroutine started on thread: " << std::this_thread::get_id() << '\n';
////	co_await switch_to_new_thread(out);
////	// awaiter destroyed here
////	std::cout << "Coroutine resumed on thread: " << std::this_thread::get_id() << '\n';
////}
////
////int main() {
////	std::jthread out;
////	resuming_on_new_thread(out);
////}
