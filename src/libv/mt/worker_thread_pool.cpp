// Project: libv.mt, File: src/libv/mt/worker_thread_pool.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/mt/worker_thread_pool.hpp>
// pro
#include <libv/mt/basic_worker_thread.lpp>
#include <libv/mt/thread_group.hpp>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

class impl_worker_thread_pool : public basic_worker_thread<libv::mt::thread_group> {
public:
	using basic_worker_thread::basic_worker_thread;
};

// -------------------------------------------------------------------------------------------------

worker_thread_pool::worker_thread_pool(size_t n, std::string new_name) :
	self(std::make_unique<impl_worker_thread_pool>(std::move(new_name), n)) {}

worker_thread_pool::~worker_thread_pool() noexcept = default; // For the sake of forward declared unique_ptr

// -------------------------------------------------------------------------------------------------

void worker_thread_pool::execute_async(libv::unique_function<void()> func) {
	self->execute_async(std::move(func));
}

void worker_thread_pool::execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::duration after) {
	self->execute_async(std::move(func), after);
}

void worker_thread_pool::execute_async(libv::unique_function<void()> func, std::chrono::steady_clock::time_point at) {
	self->execute_async(std::move(func), at);
}

void worker_thread_pool::stop() {
	self->stop();
}

void worker_thread_pool::join() {
	self->join();
}

const std::string& worker_thread_pool::name() const noexcept {
	return self->name();
}

size_t worker_thread_pool::task_queue_size() const noexcept {
	return self->task_queue_size();
}

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
