// Project: libv.mt, File: src/libv/mt/worker_thread_pool.hpp, Author: Császár Mátyás [Vader]

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

class worker_thread_pool {
	std::unique_ptr<class impl_worker_thread_pool> self;

public:
	explicit worker_thread_pool(size_t n, std::string name = "unnamed-pool");
	~worker_thread_pool() noexcept;

public:
	void execute_async(libv::unique_function<void()> task);
	void execute_async(libv::unique_function<void()> task, std::chrono::steady_clock::duration after);
	void execute_async(libv::unique_function<void()> task, std::chrono::steady_clock::time_point at);
	void stop();
	void join();

public:
	[[nodiscard]] const std::string& name() const noexcept;
	[[nodiscard]] size_t task_queue_size() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
