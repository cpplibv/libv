// Project: libv.mt, File: sandbox/libv_coro_mt_main.cpp

// hpp
// #include <catch2/catch_test_macros.hpp>
// std
//#include <chrono>
//#include <thread>
// pro
// #include <libv/mt/coro_affinity_task.hpp>
//#include <libv/mt/binary_latch.hpp>
//#include <libv/mt/worker_thread.hpp>
//#include <libv/mt/worker_thread_pool.hpp>
#include <libv/mt/mutex_spinlock.hpp>


// =================================================================================================


#include <cppcoro/generator.hpp>

#include <libv/mt/thread_number.hpp>
#include <libv/mt/ticket_machine.hpp>
#include <libv/mt/worker_thread.hpp>
#include <libv/mt/worker_thread_pool.hpp>

#include <chrono>
#include <coroutine>
#include <iostream>
#include <string>
#include <thread>

#include <iostream>
#include <mutex>
#include <thread>



// -------------------------------------------------------------------------------------------------

std::mutex cout_m;
thread_local int call_stack_depth = 0;
// #define COUT(...) { auto lock = std::unique_lock(cout_m); std::cout << __VA_ARGS__; }
// #define COUT_B(...) { auto lock = std::unique_lock(cout_m); std::cout << std::string(call_stack_depth++ * 4, ' ') << __VA_ARGS__; }
// #define COUT_A(...) { auto lock = std::unique_lock(cout_m); std::cout << std::string(--call_stack_depth * 4, ' ') << __VA_ARGS__; }
#define COUT(...) {  }
#define COUT_B(...) {  }
#define COUT_A(...) {  }

enum class ThreadAffinity {
	GL,
	FS,
	CPU,
	Continue,
};


// =================================================================================================

// -------------------------------------------------------------------------------------------------

using namespace libv::mt;

// -------------------------------------------------------------------------------------------------

#include <vector>

#include <fmt/printf.h>
#include <fmt/chrono.h>
#include <libv/algo/linear_find.hpp>
#include <libv/utility/chrono.hpp>
//#include <deque>

class MultithreadTimeline {
	struct Entry {
		const void* object_id;
		std::thread::id thread_id;
		std::string name;
//		std::chrono::steady_clock::time_point start;
		std::chrono::system_clock::time_point start;
		std::chrono::system_clock::time_point stop;
	};

//	std::mutex mutex;
	libv::mutex_spinlock mutex;

//	libv::spin
	std::vector<Entry> entries;
//	std::deque<Entry> entries;

//	struct Entry {
//		const void* object_id;
//		std::string name;
////		std::chrono::steady_clock::time_point start;
//		std::chrono::system_clock::time_point start;
//		std::chrono::system_clock::time_point stop;
//	};
//
//	struct Track {
////		std::string track_name;
//		std::thread::id thread_id;
//		std::vector<Entry> entries;
//	};

//	std::mutex_spin_lock
//	std::mutex mutex;
//	std::vector<Track> tracks;

public:
	MultithreadTimeline() {
		entries.reserve(16 * 1024);
	}

public:
	void add(const void* object_id, std::string name, std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point stop) {
		const auto thread_id = std::this_thread::get_id();
//		fmt::print("start {} {}\n", std::bit_cast<uint64_t>(thread_id), object_id);
		auto lock = std::unique_lock(mutex);

		entries.emplace_back(object_id, thread_id, std::move(name), start, stop);

//		auto* track = libv::linear_find_optional(tracks, thread_id, &Track::thread_id);
//		if (!track)
//			track = &tracks.emplace_back(thread_id);
//
//		auto& entry = track->entries.emplace_back(object_id, std::move(name));
//		entry.start = std::chrono::system_clock::now();
	}
//	void start(const void* object_id, std::string name) {
//		const auto thread_id = std::this_thread::get_id();
//		fmt::print("start {} {}\n", std::bit_cast<uint64_t>(thread_id), object_id);
//		auto lock = std::unique_lock(mutex);
//
//		auto* track = libv::linear_find_optional(tracks, thread_id, &Track::thread_id);
//		if (!track)
//			track = &tracks.emplace_back(thread_id);
//
//		auto& entry = track->entries.emplace_back(object_id, std::move(name));
//		entry.start = std::chrono::system_clock::now();
//	}
//
//	void stop(const void* object_id) {
//		const auto thread_id = std::this_thread::get_id();
//		fmt::print("stop  {} {}\n", std::bit_cast<uint64_t>(thread_id), object_id);
//		const auto now = std::chrono::system_clock::now();
//
//		auto lock = std::unique_lock(mutex);
//
//		auto* track = libv::linear_find_optional(tracks, thread_id, &Track::thread_id);
//		if (!track) {
//			assert(false && "Internal error: thread_id was not started");
//			return;
//		}
//
//		auto* entry = libv::linear_find_last_optional(track->entries, object_id, &Entry::object_id);
//		if (!entry) {
//			assert(false && "Internal error: object_id was not started");
//			return;
//		}
//
//		entry->stop = now;
//	}

	void print(std::chrono::system_clock::time_point t0) {
//		for (const auto& track : tracks) {
//			const auto thread_number = std::bit_cast<uint64_t>(track.thread_id);
//			for (const auto& entry : track.entries) {
//				fmt::print("{} {} {} {}\n", thread_number, entry.name,
//						(entry.start - t0),
//						(entry.stop - t0)
//				);
//			}
//		}
		for (const auto& entry : entries) {
			const auto thread_number = std::bit_cast<uint64_t>(entry.thread_id);
			fmt::print("{} {} {} {}\n", thread_number, entry.name,
					(entry.start - t0),
					(entry.stop - t0)
			);
		}
	}
};

// -------------------------------------------------------------------------------------------------

cppcoro::generator<ThreadAffinity> alternative_idea(int id, int i, int count) {
	(void) id;
	(void) i;
	(void) count;
	COUT(" < Work ENTER  : " << id << ", Thread: " << libv::thread_number() << ", Step: " << '-' << "/" << count << std::endl)

	co_yield ThreadAffinity::CPU;
	std::this_thread::yield();

	COUT(" < Work A  , ID: " << id << ", Thread: " << libv::thread_number() << ", Step: " << i << "/" << count << std::endl)

	co_yield ThreadAffinity::GL;

	COUT(" < Work B  , ID: " << id << ", Thread: " << libv::thread_number() << ", Step: " << i << "/" << count << std::endl)

	co_yield ThreadAffinity::FS;

	COUT(" < Work C  , ID: " << id << ", Thread: " << libv::thread_number() << ", Step: " << i << "/" << count << std::endl)

	co_yield ThreadAffinity::CPU;

	COUT(" < Work D.0, ID: " << id << ", Thread: " << libv::thread_number() << ", Step: " << i << "/" << count << std::endl)
	co_yield ThreadAffinity::Continue;
	COUT(" < Work D.1, ID: " << id << ", Thread: " << libv::thread_number() << ", Step: " << i << "/" << count << std::endl)
	co_yield ThreadAffinity::Continue;
	COUT(" < Work D.2, ID: " << id << ", Thread: " << libv::thread_number() << ", Step: " << i << "/" << count << std::endl)
	co_yield ThreadAffinity::Continue;
	COUT(" < Work D.3, ID: " << id << ", Thread: " << libv::thread_number() << ", Step: " << i << "/" << count << std::endl)

//	if (error)
//		co_return;
}

struct Context {
	MultithreadTimeline& timeline;
	libv::mt::worker_thread_pool& threads_cpu;
	libv::mt::worker_thread& threads_gl;
};

struct CallData {
	Context& context;
	libv::mt::ticket token;
	cppcoro::generator<ThreadAffinity> t;
	cppcoro::generator<ThreadAffinity>::iterator it;

	CallData(
			Context& context,
			libv::mt::ticket&& token,
			cppcoro::generator<ThreadAffinity>&& t) :
		context(context),
		token(std::move(token)),
		t(std::move(t)) {

	}
};

struct CallFoo {
	std::unique_ptr<CallData> data;

	CallFoo(
			Context& context,
			libv::mt::ticket&& token,
			cppcoro::generator<ThreadAffinity>&& t) :
			data(std::make_unique<CallData>(context, std::move(token), std::move(t))) {

		COUT_B("before call foo INIT" << std::endl)
		init(); // Prime the co-routine to check what is the first thread it would like to have
		COUT_A("after  call foo INIT" << std::endl)
	}

	void schedule(ThreadAffinity ta) {
		switch (ta) {
		case ThreadAffinity::CPU:
			data->context.threads_cpu.execute_async(std::move(*this)); break;
		case ThreadAffinity::GL:
			data->context.threads_gl.execute_async(std::move(*this)); break;
		case ThreadAffinity::FS:
			data->context.threads_cpu.execute_async(std::move(*this)); break;
		case ThreadAffinity::Continue:
			; // Noop
		}
	}

	void init() {
		COUT_B("before it = t.begin()" << std::endl)
		const auto t_start = std::chrono::system_clock::now();
		data->it = data->t.begin(); // Primes the co-routine
		const auto t_stop = std::chrono::system_clock::now();
		data->context.timeline.add(data.get(), "JOB NAME", t_start, t_stop);
//		timeline.start(&timeline, "JOB NAME");
//		it = t.begin(); // Primes the co-routine
//		timeline.stop(&timeline);
		COUT_A("after  it = t.begin(), end: " << (it == t.end()) << std::endl)

		if (data->it == data->t.end())
			return;

		COUT_B("before *it INIT" << std::endl)
		ThreadAffinity ta = *data->it;
		COUT_A("after  *it INIT, ta: " << std::to_underlying(ta) << std::endl)

		schedule(ta);
	}

	void operator()() {
		while (true) {
			COUT_B("before *it" << std::endl)
			ThreadAffinity ta = *data->it;
			COUT_A("after  *it, ta: " << std::to_underlying(ta) << std::endl)
			COUT_B("before ++it" << std::endl)
			const auto t_start = std::chrono::system_clock::now();
			++data->it; // Reenters the coroutine
			const auto t_stop = std::chrono::system_clock::now();
			data->context.timeline.add(data.get(), "JOB NAME", t_start, t_stop);
//			timeline.start(&timeline, "JOB NAME");
//			++it; // Reenters the coroutine
//			timeline.stop(&timeline);
			COUT_A("after  ++it, end: " << (it == t.end()) << std::endl)

			if (data->it == data->t.end())
				return;

			if (ta == ThreadAffinity::Continue)
				continue;

			schedule(ta);
			break;
		}
	}
};

// -------------------------------------------------------------------------------------------------

int main() {
	MultithreadTimeline timeline;

//	libv::mt::worker_thread worker;

//	CHECK(av0.value() == av1.value());
	// CHECK(not av2.has_value()); // Should fail with assert as we are reading from a moved out object

	COUT("Hello coroutines" << " Thread: " << std::this_thread::get_id() << std::endl)

	libv::mt::ticket_machine tm;
	libv::mt::worker_thread_pool threads_cpu(4);
	libv::mt::worker_thread threads_gl;

	Context context{timeline, threads_cpu, threads_gl};

	const auto t0 = std::chrono::system_clock::now();

	for (int i = 0; i < 1000; ++i) {
		COUT_B("before task create" << std::endl)
		cppcoro::generator<ThreadAffinity> task = alternative_idea(0, 0, 0);
		COUT_A("after  task create" << std::endl)
		COUT_B("before CallFoo create" << std::endl)
		CallFoo(context, tm.create_ticket(), std::move(task));
//		auto task = std::make_unique<CallFoo>(timeline, threads_cpu, threads_gl, tm.create_ticket(), std::move(task));
//		task->start();
		COUT_A("after  CallFoo create" << std::endl)
	}

	std::stop_source ss;
	tm.start_async_wait(ss);
	while (!ss.stop_requested())
		std::this_thread::yield();

	const auto t1 = std::chrono::system_clock::now();

	std::cout << std::endl;
	std::cout << "===========================" << std::endl;
	fmt::print("Test took: {}us\n", libv::time_duration(t1 - t0).count() * 1000.0 * 1000.0);
	std::cout << std::endl;

	timeline.print(t0);

	std::cout << std::endl;
	fmt::print("Test took: {}us\n", libv::time_duration(t1 - t0).count() * 1000.0 * 1000.0);
	std::cout << std::endl;
}

// -------------------------------------------------------------------------------------------------
