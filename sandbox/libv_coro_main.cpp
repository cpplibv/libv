// Project: libv, File: sandbox/libv_coro_main.cpp

// ext
#include <cppcoro/async_generator.hpp>
#include <cppcoro/generator.hpp>
#include <cppcoro/schedule_on.hpp>
#include <cppcoro/shared_task.hpp>
#include <cppcoro/shared_task.hpp>
#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/static_thread_pool.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/when_all.hpp>
#include <cppcoro/when_all_ready.hpp>
//#include <cppcoro/resume_on.hpp>
// libv
#include <libv/mt/worker_thread_pool.hpp>
// std
#include <chrono>
#include <coroutine>
#include <iostream>
#include <string>
#include <thread>


// =================================================================================================
// =================================================================================================

struct Flow;

#define LIBV_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER 0

class flow_promise_base {
private:
	cppcoro::coroutine_handle<> m_continuation;

#if !LIBV_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
	// Initially false. Set to true when either a continuation is registered
	// or when the coroutine has run to completion. Whichever operation
	// successfully transitions from false->true got there first.
	std::atomic<bool> m_state{false};
#endif

private:
	friend struct final_awaitable;

	struct final_awaitable {
		bool await_ready() const noexcept { return false; }

#if LIBV_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
		template<typename PROMISE>
		cppcoro::coroutine_handle<> await_suspend(
			cppcoro::coroutine_handle<PROMISE> coro) noexcept
		{
			return coro.promise().m_continuation;
		}
#else

		// HACK: Need to add CPPCORO_NOINLINE to await_suspend() method
		// to avoid MSVC 2017.8 from spilling some local variables in
		// await_suspend() onto the coroutine frame in some cases.
		// Without this, some tests in async_auto_reset_event_tests.cpp
		// were crashing under x86 optimised builds.
		template <typename PROMISE>
		CPPCORO_NOINLINE
		void await_suspend(cppcoro::coroutine_handle<PROMISE> coroutine) noexcept {
			flow_promise_base& promise = coroutine.promise();

			// Use 'release' memory semantics in case we finish before the
			// awaiter can suspend so that the awaiting thread sees our
			// writes to the resulting value.
			// Use 'acquire' memory semantics in case the caller registered
			// the continuation before we finished. Ensure we see their write
			// to m_continuation.
			if (promise.m_state.exchange(true, std::memory_order_acq_rel))
				promise.m_continuation.resume();
		}
#endif
		void await_resume() noexcept {}
	};

public:

	flow_promise_base() noexcept
#if !LIBV_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
			: m_state(false)
#endif
	{}

	auto initial_suspend() noexcept {
		return cppcoro::suspend_always{};
	}

	auto final_suspend() noexcept {
		return final_awaitable{};
	}

#if LIBV_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
	void set_continuation(cppcoro::coroutine_handle<> continuation) noexcept {
		m_continuation = continuation;
	}
#else

	bool try_set_continuation(cppcoro::coroutine_handle<> continuation) {
		m_continuation = continuation;
		return !m_state.exchange(true, std::memory_order_acq_rel);
	}
#endif
};

class flow_promise : public flow_promise_base {
private:
	std::exception_ptr m_exception;

public:
	flow_promise() noexcept = default;

	Flow get_return_object() noexcept;

	void return_void() noexcept {}

	void unhandled_exception() noexcept {
		m_exception = std::current_exception();
	}

	void result() {
		if (m_exception)
			std::rethrow_exception(m_exception);
	}
};

struct Flow {
	using promise_type = flow_promise;
	using value_type = void;

private:
	cppcoro::coroutine_handle<promise_type> m_coroutine;

//private:
//	struct awaitable_base {
//		cppcoro::coroutine_handle<promise_type> m_coroutine;
//
//		awaitable_base(cppcoro::coroutine_handle<promise_type> coroutine) noexcept
//				: m_coroutine(coroutine) {}
//
//		bool await_ready() const noexcept {
//			return !m_coroutine || m_coroutine.done();
//		}
//
//#if LIBV_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
//		cppcoro::coroutine_handle<> await_suspend(cppcoro::coroutine_handle<> awaitingCoroutine) noexcept {
//			m_coroutine.promise().set_continuation(awaitingCoroutine);
//			return m_coroutine;
//		}
//#else
//
//		bool await_suspend(cppcoro::coroutine_handle<> awaitingCoroutine) noexcept {
//			// NOTE: We are using the bool-returning version of await_suspend() here
//			// to work around a potential stack-overflow issue if a coroutine
//			// awaits many synchronously-completing tasks in a loop.
//			//
//			// We first start the task by calling resume() and then conditionally
//			// attach the continuation if it has not already completed. This allows us
//			// to immediately resume the awaiting coroutine without increasing
//			// the stack depth, avoiding the stack-overflow problem. However, it has
//			// the down-side of requiring a std::atomic to arbitrate the race between
//			// the coroutine potentially completing on another thread concurrently
//			// with registering the continuation on this thread.
//			//
//			// We can eliminate the use of the std::atomic once we have access to
//			// coroutine_handle-returning await_suspend() on both MSVC and Clang
//			// as this will provide ability to suspend the awaiting coroutine and
//			// resume another coroutine with a guaranteed tail-call to resume().
//			m_coroutine.resume();
//			return m_coroutine.promise().try_set_continuation(awaitingCoroutine);
//		}
//#endif
//	};

public:
	Flow() noexcept : m_coroutine(nullptr) {}

	explicit Flow(cppcoro::coroutine_handle<promise_type> coroutine) : m_coroutine(coroutine) {}

	Flow(Flow&& t) noexcept : m_coroutine(t.m_coroutine) {
		t.m_coroutine = nullptr;
	}

	/// Disable copy construction/assignment.
	Flow(const Flow&) = delete;
	Flow& operator=(const Flow&) = delete;

	/// Frees resources used by this Flow.
	~Flow() {
		if (m_coroutine)
			m_coroutine.destroy();
	}

	Flow& operator=(Flow&& other) noexcept {
		if (std::addressof(other) != this) {
			if (m_coroutine) {
				m_coroutine.destroy();
			}

			m_coroutine = other.m_coroutine;
			other.m_coroutine = nullptr;
		}

		return *this;
	}

	void start() {
		m_coroutine.resume();
	}

	/// \brief
	/// Query if the Flow result is complete.
	///
	/// Awaiting a Flow that is ready is guaranteed not to block/suspend.
//	bool is_ready() const noexcept {
//		return !m_coroutine || m_coroutine.done();
//	}

//	auto operator co_await() const & noexcept {
//		struct awaitable : awaitable_base {
//			using awaitable_base::awaitable_base;
//
//			decltype(auto) await_resume() {
//				if (!this->m_coroutine)
//					throw cppcoro::broken_promise{};
//
//				return this->m_coroutine.promise().result();
//			}
//		};
//
//		return awaitable{m_coroutine};
//	}
//
//	auto operator co_await() const && noexcept {
//		struct awaitable : awaitable_base {
//			using awaitable_base::awaitable_base;
//
//			decltype(auto) await_resume() {
//				if (!this->m_coroutine)
//					throw cppcoro::broken_promise{};
//
//				return std::move(this->m_coroutine.promise()).result();
//			}
//		};
//
//		return awaitable{m_coroutine};
//	}
//
//	auto when_ready() const noexcept {
//		struct awaitable : awaitable_base {
//			using awaitable_base::awaitable_base;
//
//			void await_resume() const noexcept {}
//		};
//
//		return awaitable{m_coroutine};
//	}
};

inline Flow flow_promise::get_return_object() noexcept {
	return Flow{ cppcoro::coroutine_handle<flow_promise>::from_promise(*this) };
}

// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------

struct my_coro_thread_pool {
	libv::mt::worker_thread_pool threads;

public:
	explicit my_coro_thread_pool(size_t n) : threads(n) {}

public:
	struct schedule_operation : std::suspend_always {
		my_coro_thread_pool* thread_pool;

		inline void await_suspend(cppcoro::coroutine_handle<> coro) const noexcept {
			thread_pool->threads.execute_async([coro = std::move(coro)] { coro.resume(); });
		}
//		inline void await_suspend(cppcoro::coroutine_handle<flow_promise> coro) const noexcept {
//			thread_pool->threads.execute_async([coro = std::move(coro)] { coro.resume(); });
//		}
	};

	struct schedule_operation_after : std::suspend_always {
		my_coro_thread_pool* thread_pool;
		std::chrono::steady_clock::duration after;

		inline void await_suspend(cppcoro::coroutine_handle<> coro) const noexcept {
			thread_pool->threads.execute_async([coro = std::move(coro)] { coro.resume(); }, after);
		}
//		inline void await_suspend(cppcoro::coroutine_handle<flow_promise> coro) const noexcept {
//			thread_pool->threads.execute_async([coro = std::move(coro)] { coro.resume(); }, after);
//		}
	};

	struct schedule_operation_at : std::suspend_always {
		my_coro_thread_pool* thread_pool;
		std::chrono::steady_clock::time_point at;

		inline void await_suspend(cppcoro::coroutine_handle<> coro) const noexcept {
			thread_pool->threads.execute_async([coro = std::move(coro)] { coro.resume(); }, at);
		}
//		inline void await_suspend(cppcoro::coroutine_handle<flow_promise> coro) const noexcept {
//			thread_pool->threads.execute_async([coro = std::move(coro)] { coro.resume(); }, at);
//		}
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

public:
	template <typename Coro>
	void execute_async_coro(Coro task) {
		threads.execute_async([t = std::move(task)] mutable {
			t.start();
//			t.resume();
		});
	}
	template <typename Coro>
	void execute_async_coro_alternative(Coro task) {
		threads.execute_async([t = std::move(task)] mutable {
			t();
		});
	}
//	void execute_async_coro(libv::unique_function<void()> task, std::chrono::steady_clock::duration after);
//	void execute_async_coro(libv::unique_function<void()> task, std::chrono::steady_clock::time_point at);
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

//int main() {
//	std::cout << "Hello coroutines" << " Thread: " << std::this_thread::get_id() << std::endl;
//	thread_pool threads(4);
//
//	cppcoro::sync_wait(consumer(threads));
//
//	std::cout << "stopping and joining" << " Thread: " << std::this_thread::get_id() << std::endl;
//}

// =================================================================================================

Flow task(int id, int count, thread_pool& tp) {
	std::cout << "Start Ticker. " << " Thread: " << std::this_thread::get_id() << std::endl;
	for (int i = 0; i < count; ++i) {
		std::cout << "Work A, ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl;
		co_await tp.schedule();
		std::cout << "Work B, ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl;
		co_await tp.schedule(std::chrono::microseconds(10));
//		std::cout << "Work C" << " Thread: " << std::this_thread::get_id() << std::endl;
//		co_yield i;
//		std::cout << "Work D" << " Thread: " << std::this_thread::get_id() << std::endl;
	}
}

//void task_entry(int count, thread_pool& tp) {
//cppcoro::task<> task_entry(int count, thread_pool& tp) {
//	auto t0 = task(0, count, tp);
//	auto t1 = task(1, count, tp);
//
////	cppcoro::coroutine_handle_t::from_promise(*this).resume();
//
////	co_await t0.when_ready();
////	co_await t0;
////	co_await t1;
//	co_await cppcoro::when_all_ready(std::move(t0), std::move(t1));
//}

//template <typename T>
//void exec(T& t) {
//	co_await t;
//}

// =================================================================================================

//int main() {
//	std::cout << "Hello coroutines" << " Thread: " << std::this_thread::get_id() << std::endl;
//	thread_pool threads(4);
//
////	cppcoro::sync_wait(consumer(threads));
////	cppcoro::sync_wait(ticker2(500, threads));
////	task_entry(500, threads);
//
////	auto t0 = task(0, 500, threads);
////	auto t1 = task(1, 500, threads);
//
////	exec(t0);
////	exec(t1);
////	cppcoro::sync_wait(t0);
////	cppcoro::sync_wait(t1);
////	cppcoro::schedule_on();
//
////	cppcoro::sync_wait(task_entry(500, threads));
//
////	task(0, 500, threads).start();
////	task(1, 500, threads).start();
//
////	auto t0 = task(0, 500, threads);
////	auto t1 = task(1, 500, threads);
////	t0.start();
////	t1.start();
//
//	threads.execute_async_coro(task(0, 500, threads));
//	threads.execute_async_coro(task(1, 500, threads));
//
//	std::this_thread::sleep_for(std::chrono::seconds(1));
////	threads.threads.wait_for_empty();
//
//	std::cout << "stopping and joining" << " Thread: " << std::this_thread::get_id() << std::endl;


//	for (const auto ta : alternative_idea(0, 0, 0)) {
//		std::cout << "ta:" << std::to_underlying(ta) << std::endl;
//	}
//}

// =================================================================================================

//auto foo() {
//	return [] {
//		auto ta = it*;
//		std::cout << "ta:" << std::to_underlying(ta) << std::endl;
//		threads.execute_async();
//	};
//}

#include <libv/mt/token_machine.hpp>
#include <libv/mt/worker_thread.hpp>
#include <mutex>
std::mutex cout_m;
thread_local int call_stack_depth = 0;
#define COUT(...) { auto lock = std::unique_lock(cout_m); std::cout << __VA_ARGS__; }
#define COUT_B(...) { auto lock = std::unique_lock(cout_m); std::cout << std::string(call_stack_depth++ * 4, ' ') << __VA_ARGS__; }
#define COUT_A(...) { auto lock = std::unique_lock(cout_m); std::cout << std::string(--call_stack_depth * 4, ' ') << __VA_ARGS__; }


enum class ThreadAffinity {
	GL,
	FS,
	CPU,
	Continue,
};

cppcoro::generator<ThreadAffinity> alternative_idea(int id, int i, int count) {
	COUT(" < Work ENTER  : " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << '-' << "/" << count << std::endl)

	co_yield ThreadAffinity::CPU;

	COUT(" < Work A  , ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl)

	co_yield ThreadAffinity::GL;

	COUT(" < Work B  , ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl)

	co_yield ThreadAffinity::FS;

	COUT(" < Work C  , ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl)

	co_yield ThreadAffinity::CPU;

	COUT(" < Work D.0, ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl)
	co_yield ThreadAffinity::Continue;
	COUT(" < Work D.1, ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl)
	co_yield ThreadAffinity::Continue;
	COUT(" < Work D.2, ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl)
	co_yield ThreadAffinity::Continue;
	COUT(" < Work D.3, ID: " << id << ", Thread: " << std::this_thread::get_id() << ", Step: " << i << "/" << count << std::endl)

//	if (error)
//		co_return;
}

struct CallFoo {
	libv::mt::worker_thread_pool& threads_cpu;
	libv::mt::worker_thread& threads_gl;
	libv::mt::token token;
	cppcoro::generator<ThreadAffinity> t;
	cppcoro::generator<ThreadAffinity>::iterator it;

	CallFoo(
			libv::mt::worker_thread_pool& threads_cpu,
			libv::mt::worker_thread& threads_gl,
			libv::mt::token&& token,
			cppcoro::generator<ThreadAffinity>&& t) :
		threads_cpu(threads_cpu),
		threads_gl(threads_gl),
		token(std::move(token)),
		t(std::move(t)) {

		COUT_B("before call foo INIT" << std::endl)
		init(); // Prime the co-routine to check what is the first thread it would like to have
		COUT_A("after  call foo INIT" << std::endl)
	}

	void schedule(ThreadAffinity ta) {
		switch (ta) {
		case ThreadAffinity::CPU:
			threads_cpu.execute_async(std::move(*this)); break;
		case ThreadAffinity::GL:
			threads_gl.execute_async(std::move(*this)); break;
		case ThreadAffinity::FS:
			threads_cpu.execute_async(std::move(*this)); break;
		case ThreadAffinity::Continue:
			; // Noop
		}
	}

	void init() {
		COUT_B("before it = t.begin()" << std::endl)
		it = t.begin(); // Primes the co-routine
		COUT_A("after  it = t.begin(), end: " << (it == t.end()) << std::endl)

		if (it == t.end())
			return;

		COUT_B("before *it INIT" << std::endl)
		ThreadAffinity ta = *it;
		COUT_A("after  *it INIT, ta: " << std::to_underlying(ta) << std::endl)

		schedule(ta);
	}

	void operator()() {
		while (true) {
			COUT_B("before *it" << std::endl)
			ThreadAffinity ta = *it;
			COUT_A("after  *it, ta: " << std::to_underlying(ta) << std::endl)
			COUT_B("before ++it" << std::endl)
			++it;
			COUT_A("after  ++it, end: " << (it == t.end()) << std::endl)

			if (it == t.end())
				return;

			if (ta == ThreadAffinity::Continue)
				continue;

			schedule(ta);
			break;
		}
	}
};

int main() {
	COUT("Hello coroutines" << " Thread: " << std::this_thread::get_id() << std::endl)

	libv::mt::token_machine tm;
	libv::mt::worker_thread_pool threads_cpu(4);
	libv::mt::worker_thread threads_gl;

	{
		COUT_B("before task create" << std::endl)
		cppcoro::generator<ThreadAffinity> task = alternative_idea(0, 0, 0);
		COUT_A("after  task create" << std::endl)
		COUT_B("before CallFoo create" << std::endl)
		CallFoo(threads_cpu, threads_gl, tm.create_token(), std::move(task));
		COUT_A("after  CallFoo create" << std::endl)
	}

	std::stop_source ss;
	tm.start_async_wait(ss);
	while (!ss.stop_requested())
		std::this_thread::yield();

//	threads.execute_async([it = task.begin(), t = std::move(task)] mutable {
//		ThreadAffinity ta;
//
//		ta = *it;
//		std::cout << "ta:" << std::to_underlying(ta) << std::endl; if (++it == t.end()) return;
//		ta = *it;
//		std::cout << "ta:" << std::to_underlying(ta) << std::endl; if (++it == t.end()) return;
//		ta = *it;
//		std::cout << "ta:" << std::to_underlying(ta) << std::endl; if (++it == t.end()) return;
//		ta = *it;
//		std::cout << "ta:" << std::to_underlying(ta) << std::endl; if (++it == t.end()) return;
//		ta = *it;
//		std::cout << "ta:" << std::to_underlying(ta) << std::endl; if (++it == t.end()) return;
//	});

//	for (const auto ta : task)
//		std::cout << "ta:" << std::to_underlying(ta) << std::endl;
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

//template <typename C>
//cppcoro::task<> example_test_network_protocol_task_2(C connection) {
//	int codec;
//	int con = codec + connection + intent_to_coro;
//	int* resource;
//
//	{
//		co_await con.receive()
//				[](RequestResourceSelect&& request) {
//					resource = server.find_resource(request->resource);
//					if (!resource)
//						con.send(ResponseResourceNotFound{});
//				},
//				[](RequestResourceDescription&& request) {
//					resource = server.find_resource(request->resource);
//					if (!resource)
//						con.send(ResponseResourceNotFound{});
//					else
//						con.send(ResponseResourceDescription{resource->size()});
//				}
//		);
//
//		while (con.connected) {
//			co_await con.receive(
//					[](RequestResourceSelect&& request) {
//						resource = server.find_resource(request->resource);
//						if (!resource)
//							con.send(ResponseResourceNotFound{});
//					},
//					[](RequestResourceDescription&& request) {
//						resource = server.find_resource(request->resource);
//						if (!resource)
//							con.send(ResponseResourceNotFound{});
//						else
//							con.send(ResponseResourceDescription{resource->size()});
//					},
//					[](RequestResourceData&& request) {
//						if (!resource) {
//							log.error("...");
//							co_return disconnect_state_error;
//						}
//
//						for (int i = request.offset; i < request.offset + request.amount; i += network_chunk_size)
//							// "SPAWN" ASYNC STATE to handle cancel
//							co_await con.send_with_queue(2, ResponseResourceChunk{i, resource->chunk(i)});
//					}
//			);
//		}
//	}
//}

//template <typename C>
//cppcoro::task<> example_test_network_protocol_task_3(C connection) {
//	int codec;
//	int con = codec + connection + intent_to_coro;
//	int* resource;
//
//	{
//		const auto message = co_await con.receive();
//
//		if (auto request = message.handle<RequestResourceSelect>()) {
//			resource = server.find_resource(request->resource);
//			if (!resource)
//				con.send(ResponseResourceNotFound{});
//		}
//		if (auto request = message.handle<RequestResourceDescription>()) {
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
//
//		while (con.connected) {
//			co_await con.receive(
//					[](RequestResourceSelect&& request) {
//						resource = server.find_resource(request->resource);
//						if (!resource)
//							con.send(ResponseResourceNotFound{});
//					},
//					[](RequestResourceDescription&& request) {
//						resource = server.find_resource(request->resource);
//						if (!resource)
//							con.send(ResponseResourceNotFound{});
//						else
//							con.send(ResponseResourceDescription{resource->size()});
//					},
//					[](RequestResourceData&& request) {
//						if (!resource) {
//							log.error("...");
//							co_return disconnect_state_error;
//						}
//
//						for (int i = request.offset; i < request.offset + request.amount; i += network_chunk_size)
//							// "SPAWN" ASYNC STATE to handle cancel
//							co_await con.send_with_queue(2, ResponseResourceChunk{i, resource->chunk(i)});
//					}
//			);
//		}
//	}
//}

// -------------------------------------------------------------------------------------------------
