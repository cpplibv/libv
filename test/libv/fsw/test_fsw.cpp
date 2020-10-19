// Project: libv.fsw, File: test/libv/fsw/test_fsw.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// std
#include <condition_variable>
#include <fstream>
#include <map>
#include <mutex>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <iostream>
// pro
#include <libv/fsw/watcher.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

struct Event {
	size_t watcher = 7777;
	std::string event;
};

template <typename T, typename Container = std::queue<T>>
struct Queue {
	Container queue;

	mutable std::mutex mutex;
	mutable std::condition_variable cv;

public:
	inline void push(const T& value) {
		std::unique_lock lock(mutex);
		queue.push(value);
		cv.notify_all();
	}

	inline void push(T&& value) {
		std::unique_lock lock(mutex);
		queue.push(std::move(value));
		cv.notify_all();
	}

public:
	inline void pull(T& out) {
		std::unique_lock lock(mutex);
		cv.wait(lock, [this]{ return !queue.empty(); });
		out = queue.front();
		queue.pop();
	}

	template<typename Rep, typename Period>
	inline bool pull_for(T& out, const std::chrono::duration<Rep, Period>& timeout) {
		std::unique_lock lock(mutex);
		auto success = cv.wait_for(lock, timeout, [this]{ return !queue.empty(); });
		if (success) {
			out = queue.front();
			queue.pop();
		}
		return success;
	}

	template<typename Clock, typename Duration>
	inline bool pull_until(T& out, const std::chrono::time_point<Clock, Duration>& attime) {
		std::unique_lock lock(mutex);
		auto success = cv.wait_until(lock, attime, [this]{ return !queue.empty(); }) == std::cv_status::no_timeout;
		if (success) {
			out = queue.front();
			queue.pop();
		}
		return success;
	}

public:
	inline bool empty() const {
		std::unique_lock lock(mutex);
		return queue.empty();
	}

	inline size_t size() const {
		std::unique_lock lock(mutex);
		return queue.size();
	}
};

// -------------------------------------------------------------------------------------------------

struct SubEntry {
	size_t id = 7777;
	libv::fsw::Watcher::token_type token;
};

struct FSWTester {
private:
	std::optional<libv::fsw::Watcher> watcher;

	Queue<Event> events;

	std::set<std::string> files;
	std::set<std::string> dirs;

//	std::map<size_t, libv::fsw::Watcher::token_type> tokens;
	size_t nextSubID = 0;

	std::chrono::milliseconds time_init    {1500};
	std::chrono::milliseconds time_sub_pre {1500};
	std::chrono::milliseconds time_sub_post{1500};
	std::chrono::milliseconds time_dir     { 500};
	std::chrono::milliseconds time_empty   { 500};
	std::chrono::milliseconds time_event   {5000};
	std::chrono::milliseconds time_dtor    {1000};

public:
	FSWTester() {
		watcher.emplace();
		std::this_thread::sleep_for(time_init);
	}

private:
	void _create_directory(std::filesystem::path path) {
		path = path.lexically_normal();
		if (path.empty())
			return;

		std::filesystem::path dir;

		for (const auto& i : path) {
			dir /= i;

			if (std::filesystem::exists(dir))
				continue;

			std::filesystem::create_directories(dir);
			dirs.emplace(dir.string());
		}

		std::this_thread::sleep_for(time_dir);
	}

	void _check_event(size_t expected_watcher, std::string expected_event) {
		Event event;
		const auto event_in_time = events.pull_for(event, time_event);

		CHECK(event_in_time);
		CHECK(event.watcher == expected_watcher);
		CHECK(event.event == expected_event);
	}

public:
	auto subscribe_file(std::string path) {
		std::this_thread::sleep_for(time_sub_pre);
		auto subID = nextSubID++;
		auto token = watcher->subscribe_file(path, [subID, this](const auto& event) {
			std::ostringstream os;
			os << event;
			std::cout << subID << " " << event << std::endl;

			events.push(Event{subID, std::move(os).str()});
		});
		std::this_thread::sleep_for(time_sub_post);
		return SubEntry{subID, token};
	}

	auto subscribe_directory(std::string path) {
		std::this_thread::sleep_for(time_sub_pre);
		auto subID = nextSubID++;
		auto token = watcher->subscribe_directory(path, [subID, this](const auto& event) {
			std::ostringstream os;
			os << event;
			std::cout << subID << " " << event << std::endl;

			events.push(Event{subID, std::move(os).str()});
		});
		std::this_thread::sleep_for(time_sub_post);
		return SubEntry{subID, token};
	}

//	void unsubscribe(std::string name) {
//		if (!tokens.contains(name))
//			throw std::runtime_error("Test failure: No subscription for: " + name);
//
//		watcher->unsubscribe(tokens.at(name));
//		tokens.erase(name);
//	}

	void empty_check() {
		std::this_thread::sleep_for(time_empty);
		CHECK(events.size() == 0);
	}

	void create_check(const SubEntry& watcher, std::string expected_path) {
		_check_event(watcher.id, "Create " + expected_path);
	}

	void remove_check(const SubEntry& watcher, std::string expected_path) {
		_check_event(watcher.id, "Remove " + expected_path);
	}

	void modify_check(const SubEntry& watcher, std::string expected_path) {
		_check_event(watcher.id, "Modify " + expected_path);
	}

	void rename_check(const SubEntry& watcher, std::string expected_path, std::string expected_old_path) {
		_check_event(watcher.id, "Rename " + expected_path + " was " + expected_old_path);
	}

	void create_file(std::string path) {
		path = std::filesystem::path(path).lexically_normal().string();

		if (std::filesystem::exists(path))
			throw std::runtime_error("Test failure: File already exists: " + path);

		_create_directory(std::filesystem::path(path).remove_filename());

		std::ofstream f(path);
		files.emplace(std::move(path));
	}

	void modify_file(std::string path) {
		path = std::filesystem::path(path).lexically_normal().string();

		if (!files.contains(path))
			throw std::runtime_error("Test failure: File is not part of the test: " + path);

		std::ofstream f(path, std::ios::out | std::ios::app);
		f << "-";
	}

	void create_directory(std::string path) {
		path = std::filesystem::path(path).lexically_normal().string();

		if (std::filesystem::exists(path))
			throw std::runtime_error("Test failure: Directory already exists: " + path);

		_create_directory(path);
	}

	void remove_file(std::string path) {
		path = std::filesystem::path(path).lexically_normal().string();

		if (!files.contains(path))
			throw std::runtime_error("Test failure: File is not part of the test: " + path);

		std::filesystem::remove(path);
		files.erase(path);
	}

	~FSWTester() {
		std::this_thread::sleep_for(time_dtor);

		watcher.reset(); // To release file / directory locks

		CHECK(events.size() == 0);

		for (const auto& file : files) {
			std::filesystem::remove(file);
		}
		for (const auto& dir : dirs) {
			std::filesystem::remove_all(dir);
		}
	}
};

} // namespace

// =================================================================================================

TEST_CASE("single file watching", "[libv.fsw.watcher]") {
	FSWTester test;

	SECTION("subscribe first") {
		auto w0 = test.subscribe_file("a.txt");
		test.empty_check();

		test.create_file("a.txt");
		test.create_check(w0, "a.txt");

		test.modify_file("a.txt");
		test.modify_check(w0, "a.txt");

		test.remove_file("a.txt");
		test.remove_check(w0, "a.txt");
	}

	SECTION("create first") {
		test.create_file("a.txt");
		test.empty_check();

		auto w0 = test.subscribe_file("a.txt");
		test.empty_check();

		test.modify_file("a.txt");
		test.modify_check(w0, "a.txt");

		test.remove_file("a.txt");
		test.remove_check(w0, "a.txt");
	}

	SECTION("create mod first") {
		test.create_file("a.txt");
		test.empty_check();

		test.modify_file("a.txt");
		test.empty_check();

		auto w0 = test.subscribe_file("a.txt");
		test.empty_check();

		test.remove_file("a.txt");
		test.remove_check(w0, "a.txt");
	}

	SECTION("create mod rem first") {
		test.create_file("a.txt");
		test.empty_check();

		test.modify_file("a.txt");
		test.empty_check();

		test.remove_file("a.txt");
		test.empty_check();

		auto w0 = test.subscribe_file("a.txt");
		test.empty_check();

		(void) w0;
	}
}

TEST_CASE("double file watching", "[libv.fsw.watcher]") {
	FSWTester test;

	SECTION("subscribe first") {
		auto w0 = test.subscribe_file("a.txt");
		auto w1 = test.subscribe_file("a.txt");
		test.empty_check();

		test.create_file("a.txt");
		test.create_check(w0, "a.txt");
		test.create_check(w1, "a.txt");

		test.modify_file("a.txt");
		test.modify_check(w0, "a.txt");
		test.modify_check(w1, "a.txt");

		test.remove_file("a.txt");
		test.remove_check(w0, "a.txt");
		test.remove_check(w1, "a.txt");
	}

	SECTION("create first") {
		test.create_file("a.txt");
		test.empty_check();

		auto w0 = test.subscribe_file("a.txt");
		auto w1 = test.subscribe_file("a.txt");
		test.empty_check();

		test.modify_file("a.txt");
		test.modify_check(w0, "a.txt");
		test.modify_check(w1, "a.txt");

		test.remove_file("a.txt");
		test.remove_check(w0, "a.txt");
		test.remove_check(w1, "a.txt");
	}

	SECTION("create mod first") {
		test.create_file("a.txt");
		test.empty_check();

		test.modify_file("a.txt");
		test.empty_check();

		auto w0 = test.subscribe_file("a.txt");
		auto w1 = test.subscribe_file("a.txt");
		test.empty_check();

		test.remove_file("a.txt");
		test.remove_check(w0, "a.txt");
		test.remove_check(w1, "a.txt");
	}

	SECTION("create mod rem first") {
		test.create_file("a.txt");
		test.empty_check();

		test.modify_file("a.txt");
		test.empty_check();

		test.remove_file("a.txt");
		test.empty_check();

		auto w0 = test.subscribe_file("a.txt");
		auto w1 = test.subscribe_file("a.txt");
		test.empty_check();

		(void) w0;
		(void) w1;
	}
}

TEST_CASE("multi file no interference watching", "[libv.fsw.watcher]") {
	FSWTester test;

	auto w0 = test.subscribe_file("a.txt");
	auto w1 = test.subscribe_file("b.txt");

	test.create_file("a.txt");
	test.create_check(w0, "a.txt");

	test.modify_file("a.txt");
	test.modify_check(w0, "a.txt");

	test.remove_file("a.txt");
	test.remove_check(w0, "a.txt");

	(void) w1;
}

TEST_CASE("two file watching with and without present folders", "[libv.fsw.watcher]") {
	FSWTester test;
	std::string path;

	bool efsw_first_create_in_empty_dir_workaround = false;

	SECTION("tmp/") {
		path = "tmp/";
		efsw_first_create_in_empty_dir_workaround = true;
	}
	SECTION("tmp/a/") {
		path = "tmp/a/";
	}
	SECTION("tmp/a/a/") {
		path = "tmp/a/a/";
	}

	SECTION("tmp/ with full dir") {
		path = "tmp/";
		test.create_directory("tmp/");
	}
	SECTION("tmp/a/ with full dir") {
		path = "tmp/a/";
		test.create_directory("tmp/a/");
	}
	SECTION("tmp/a/a/ with full dir") {
		path = "tmp/a/a/";
		test.create_directory("tmp/a/a/");
	}

	SECTION("tmp/a/ with tmp/") {
		path = "tmp/a/";
		test.create_directory("tmp/");
		efsw_first_create_in_empty_dir_workaround = true;
	}
	SECTION("tmp/a/a/ with tmp/") {
		path = "tmp/a/a/";
		test.create_directory("tmp/");
	}
	SECTION("tmp/a/a/ with tmp/a/") {
		path = "tmp/a/a/";
		test.create_directory("tmp/a/");
		efsw_first_create_in_empty_dir_workaround = true;
	}

	auto w0 = test.subscribe_file(path + "a.txt");
	auto w1 = test.subscribe_file(path + "b.txt");

	test.create_file(path + "a.txt");
	if (!efsw_first_create_in_empty_dir_workaround)
		test.create_check(w0, path + "a.txt");
	test.modify_file(path + "a.txt");
	test.modify_check(w0, path + "a.txt");
	test.remove_file(path + "a.txt");
	test.remove_check(w0, path + "a.txt");

	test.create_file(path + "b.txt");
	test.create_check(w1, path + "b.txt");
	test.modify_file(path + "b.txt");
	test.modify_check(w1, path + "b.txt");
	test.remove_file(path + "b.txt");
	test.remove_check(w1, path + "b.txt");
}

TEST_CASE("subscribe_directory", "[libv.fsw.watcher]") {
	FSWTester test;
	std::string path;

	SECTION("tmp/ with full dir") {
		path = "tmp/";
		test.create_directory("tmp/");
	}
	SECTION("tmp/a/ with full dir") {
		path = "tmp/a/";
		test.create_directory("tmp/a/");
	}
	SECTION("tmp/a/a/ with full dir") {
		path = "tmp/a/a/";
		test.create_directory("tmp/a/a/");
	}

	auto wd = test.subscribe_directory(path);

	test.create_file(path + "a.txt");
	test.create_check(wd, path + "a.txt");
	test.create_file(path + "b.txt");
	test.create_check(wd, path + "b.txt");
}

TEST_CASE("no dup event on in stacked folders", "[libv.fsw.watcher]") {
	FSWTester test;

	bool efsw_first_create_in_empty_dir_workaround = false;

	SECTION("0 dir") {
	}
	SECTION("1 dir") {
		test.create_directory("tmp/");
	}
	SECTION("2 dir") {
		test.create_directory("tmp/tmp/");
	}
	SECTION("3 dir") {
		test.create_directory("tmp/tmp/tmp/");
		efsw_first_create_in_empty_dir_workaround = true;
	}

	auto w0 = test.subscribe_file("tmp/tmp/tmp/a.txt");
	auto w1 = test.subscribe_file("tmp/tmp/a.txt");
	auto w2 = test.subscribe_file("tmp/a.txt");
	auto w3 = test.subscribe_file("a.txt");

	test.create_file("tmp/tmp/tmp/a.txt");
	if (!efsw_first_create_in_empty_dir_workaround)
		test.create_check(w0, "tmp/tmp/tmp/a.txt");

	test.create_file("tmp/tmp/a.txt");
	test.create_check(w1, "tmp/tmp/a.txt");

	test.create_file("tmp/a.txt");
	test.create_check(w2, "tmp/a.txt");

	test.create_file("a.txt");
	test.create_check(w3, "a.txt");
}

TEST_CASE("no dup event on out stacked folders", "[libv.fsw.watcher]") {
	FSWTester test;

	bool efsw_first_create_in_empty_dir_workaround = false;

	SECTION("0 dir") {
	}
	SECTION("1 dir") {
		test.create_directory("tmp/");
	}
	SECTION("2 dir") {
		test.create_directory("tmp/tmp/");
	}
	SECTION("3 dir") {
		test.create_directory("tmp/tmp/tmp/");
		efsw_first_create_in_empty_dir_workaround = true;
	}

	auto w3 = test.subscribe_file("a.txt");
	auto w2 = test.subscribe_file("tmp/a.txt");
	auto w1 = test.subscribe_file("tmp/tmp/a.txt");
	auto w0 = test.subscribe_file("tmp/tmp/tmp/a.txt");

	test.create_file("tmp/tmp/tmp/a.txt");
	if (!efsw_first_create_in_empty_dir_workaround)
		test.create_check(w0, "tmp/tmp/tmp/a.txt");

	test.create_file("tmp/tmp/a.txt");
	test.create_check(w1, "tmp/tmp/a.txt");

	test.create_file("tmp/a.txt");
	test.create_check(w2, "tmp/a.txt");

	test.create_file("a.txt");
	test.create_check(w3, "a.txt");
}

TEST_CASE("scenario 0", "[libv.fsw.watcher]") {
	FSWTester test;

	bool efsw_first_create_in_empty_dir_workaround = false;

	SECTION("no dir") {
	}
	SECTION("half dir") {
		test.create_directory("tmp/a/p");
		test.create_directory("tmp/b/p");
	}
	SECTION("full dir") {
		test.create_directory("tmp/p/p");
		test.create_directory("tmp/p/b");
		test.create_directory("tmp/a/p");
		test.create_directory("tmp/b/p");

		efsw_first_create_in_empty_dir_workaround = true;
	}

	auto w0 = test.subscribe_file("tmp/p/p/a.txt");
	auto w1 = test.subscribe_file("tmp/p/p/b.txt");
	auto w2 = test.subscribe_file("tmp/p/a.txt");
	auto w3 = test.subscribe_file("tmp/p/b.txt");
	auto w4 = test.subscribe_file("tmp/a.txt");
	auto w5 = test.subscribe_file("a.txt");

	test.create_file("tmp/p/p/a.txt");
	if (!efsw_first_create_in_empty_dir_workaround)
		test.create_check(w0, "tmp/p/p/a.txt");

	test.create_file("tmp/p/p/b.txt");
	test.create_check(w1, "tmp/p/p/b.txt");

	test.create_file("tmp/a.txt");
	test.create_check(w4, "tmp/a.txt");

	test.create_file("tmp/p/b.txt");
	test.create_check(w3, "tmp/p/b.txt");

	test.create_file("tmp/p/a.txt");
	test.create_check(w2, "tmp/p/a.txt");

	test.create_file("a.txt");
	test.create_check(w5, "a.txt");
}

// -------------------------------------------------------------------------------------------------
