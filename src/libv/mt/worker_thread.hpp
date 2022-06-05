// Project: libv.mt, File: src/libv/mt/worker_thread.hpp

#pragma once

// libv
#include <libv/utility/unique_function.hpp>
// std
#include <chrono>
#include <string>
#include <memory>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

class impl_worker_thread;

class worker_thread {
	std::shared_ptr<impl_worker_thread> self;

public:
	explicit worker_thread(std::string name = "unnamed");
	inline worker_thread(const worker_thread&) noexcept = default;
	inline worker_thread& operator=(const worker_thread&) & noexcept = default;
	inline worker_thread(worker_thread&&) noexcept = default;
	inline worker_thread& operator=(worker_thread&&) & noexcept = default;
	~worker_thread() noexcept;

public:
	void execute_async(libv::unique_function<void()> task);
	void execute_async(libv::unique_function<void()> task, std::chrono::steady_clock::duration after);
	void execute_async(libv::unique_function<void()> task, std::chrono::steady_clock::time_point at);
	void stop();
	void join();
	[[nodiscard]] bool wait_for_empty() const;

public:
	[[nodiscard]] const std::string& name() const noexcept;
	[[nodiscard]] std::size_t task_queue_size() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
