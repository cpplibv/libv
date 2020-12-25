// Project: libv.math, File: test/libv/math/test_snap.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// std
#include <chrono>
// pro
#include <libv/mt/async_value.hpp>
#include <libv/mt/binary_latch.hpp>
#include <libv/mt/worker_thread.hpp>
#include <libv/mt/worker_thread_pool.hpp>


// -------------------------------------------------------------------------------------------------

int load_async_42_stoppable(std::stop_token stop_token) {
	int i = 0;
	while (!stop_token.stop_requested() && ++i != 42)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	return i;
}

int load_async_42() {
	std::this_thread::sleep_for(std::chrono::milliseconds(10) * 42);
	return 42;
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("async_value ctor semantics", "[libv.mt.async_value]") {
	libv::mt::worker_thread worker;

	libv::mt::async_value<int> av0;
	av0.load(worker, load_async_42);

	// 0 copy to 1
	// 0 move to 2
	// 1 copy to 0
	// 2 move to 1
	libv::mt::async_value<int> av1(av0);
	libv::mt::async_value<int> av2(std::move(av0));
	av0 = av1;
	av1 = std::move(av2);

	while (av0.loading())
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

	REQUIRE(av0.has_value());
	REQUIRE(av1.has_value());
	CHECK(av0.value() == 42);
	CHECK(av0.value() == av1.value());
	CHECK(not av2.has_value());
}

TEST_CASE("async_value complete loading with worker_thread", "[libv.mt.async_value]") {
	libv::mt::worker_thread worker;

	libv::mt::async_value<int> av0;
	libv::mt::async_value<int> av1;

	av0.load(worker, load_async_42);
	av1.load(worker, load_async_42_stoppable);

	while (av0.loading())
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	while (av1.loading())
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

	REQUIRE(av0.has_value());
	REQUIRE(av1.has_value());
	CHECK(av0.value() == 42);
	CHECK(av1.value() == 42);
}

TEST_CASE("async_value complete loading with worker_thread_pool", "[libv.mt.async_value]") {
	libv::mt::worker_thread_pool worker(4);

	libv::mt::async_value<int> av0;
	libv::mt::async_value<int> av1;

	av0.load(worker, load_async_42);
	av1.load(worker, load_async_42_stoppable);

	while (av0.loading())
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	while (av1.loading())
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	REQUIRE(av0.has_value());
	REQUIRE(av1.has_value());
	CHECK(av0.value() == 42);
	CHECK(av1.value() == 42);
}

TEST_CASE("async_value load cancellation before entering load function", "[libv.mt.async_value]") {
	libv::mt::worker_thread worker;

	// Trap the worker thread with a fake task
	libv::binary_latch trap_in;
	libv::binary_latch trap_out;
	worker.execute_async([&] {
		trap_in.raise();
		trap_out.wait();
	});
	trap_in.wait();

	// Start loading
	bool load_was_called_av0 = false;
	bool load_was_called_av1 = false;

	libv::mt::async_value<int> av0;
	libv::mt::async_value<int> av1;
	av0.load(worker, [&] {
		load_was_called_av0 = true;
		return 42;
	});
	av1.load(worker, [&] {
		load_was_called_av1 = true;
		return 42;
	});
	av0.cancel();
	// Not canceling av1, only av0

	// Let the worker thread lose
	trap_out.raise();
	worker.stop();
	worker.join();

	CHECK(not load_was_called_av0);
	CHECK(not av0.has_value());
	CHECK(not av0.loading());

	CHECK(load_was_called_av1);
	REQUIRE(av1.has_value());
	CHECK(av1.value() == 42);
}

TEST_CASE("async_value load cancellation after entering load function", "[libv.mt.async_value]") {
	libv::mt::worker_thread worker;

	// Trap the worker thread with a fake task
	libv::binary_latch trap_in;
	libv::binary_latch trap_out;
	worker.execute_async([&] {
		trap_in.raise();
		trap_out.wait();
	});
	trap_in.wait();

	// Start loading
	libv::mt::async_value<int> av0;

	bool load_was_called_av0 = false;
	libv::binary_latch load_trap_in;
	libv::binary_latch load_trap_out;
	av0.load(worker, [&](std::stop_token st) {
		load_was_called_av0 = true;

		load_trap_in.raise();
		CHECK(not st.stop_requested());

		load_trap_out.wait();
		CHECK(st.stop_requested());

		return 42;
	});

	// Let the worker thread lose
	trap_out.raise();

	load_trap_in.wait();
	av0.cancel();
	load_trap_out.raise();

	// Join
	worker.stop();
	worker.join();

	CHECK(load_was_called_av0);
	CHECK(not av0.has_value());
	CHECK(not av0.loading());
}

// -------------------------------------------------------------------------------------------------
