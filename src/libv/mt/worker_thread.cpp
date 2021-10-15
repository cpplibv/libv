// Project: libv.mt, File: src/libv/mt/worker_thread.cpp

// hpp
#include <libv/mt/worker_thread.hpp>
// std
#include <thread>
// pro
#include <libv/mt/basic_worker_thread.hxx>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

class impl_worker_thread : public basic_worker_thread<std::jthread> {
public:
	using basic_worker_thread::basic_worker_thread;
};

// -------------------------------------------------------------------------------------------------

worker_thread::worker_thread(std::string new_name) :
	self(std::make_unique<impl_worker_thread>(std::move(new_name))) {}

worker_thread::~worker_thread() noexcept = default; // For the sake of forward declared unique_ptr

// -------------------------------------------------------------------------------------------------

void worker_thread::execute_async(libv::unique_function<void()> func) {
	self->execute_async(std::move(func));
}

void worker_thread::execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::duration after) {
	self->execute_async(std::move(func), after);
}

void worker_thread::execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::time_point at) {
	self->execute_async(std::move(func), at);
}

void worker_thread::stop() {
	self->stop();
}

void worker_thread::join() {
	self->join();
}

const std::string& worker_thread::name() const noexcept {
	return self->name();
}

size_t worker_thread::task_queue_size() const noexcept {
	return self->task_queue_size();
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
