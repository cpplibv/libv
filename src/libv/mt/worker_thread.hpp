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

class worker_thread {
	std::unique_ptr<class impl_worker_thread> self;

public:
	explicit worker_thread(std::string name = "unnamed");
	~worker_thread() noexcept;

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
