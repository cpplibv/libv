// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// libv
#include <libv/gl/gl_fwd.hpp>
#include <libv/mt/queue_busy.hpp>
#include <libv/mt/token_machine.hpp>
//#include <libv/mt/worker_thread.hpp>
#include <libv/mt/worker_thread_pool.hpp>
#include <libv/utility/timer.hpp>
#include <libv/utility/unique_function.hpp>
// pro
#include <libv/rev/log.hpp>
#include <libv/rev/settings.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct InternalResourceManager {
//	libv::Nexus nexus;
//	libv::fsw::Watcher fsw;
	Settings settings;

private:
	libv::mt::token_machine pending_blocking_work;
//	libv::mt::worker_thread thread_fs{"res/fs"};
//	libv::mt::worker_thread_pool thread_cpu{2, "res/cpu"};
	libv::mt::worker_thread_pool thread_fs{2, "res/fs"};
	libv::mt::worker_thread_pool thread_cpu{4, "res/cpu"};
	libv::mt::queue_busy<libv::unique_function<void()>> queue_res_;

public:
	explicit inline InternalResourceManager(Settings&& settings) :
		settings(std::move(settings)) {
	}

	void update(libv::gl::GL& gl) {
		(void) gl;
		libv::Timer timer;

		std::stop_source stop_source;
		auto wait_guard = pending_blocking_work.async_wait_guard(stop_source);

		int num_res_update = 0;
		// Busy waiting, while we wait for the pending blocking operations to finish, we process the res queue
		while (auto task = queue_res_.try_wait_pop(stop_source.get_token())) {
			++num_res_update;
			try {
				(*task)();
			} catch (const std::exception& ex) {
				log_rev.error("Exception occurred in resource update: {}", ex.what());
			}
		}

		if (num_res_update != 0)
			log_rev.trace("Main loop resource update took:{: 6.3f}ms for {} resource", timer.timef_ms().count(), num_res_update);
	}

public:
	template <typename F>
	void queue_fs(F&& task_) {
		thread_fs.execute_async([
					task = std::forward<F>(task_),
					token = pending_blocking_work.create_token()]() mutable {
			task();
		});
	}

	template <typename F>
	void queue_cpu(F&& task_) {
		thread_cpu.execute_async([
					task = std::forward<F>(task_),
					token = pending_blocking_work.create_token()]() mutable {
			task();
		});
	}

	void queue_res(libv::unique_function<void()> task) {
		queue_res_.emplace(std::move(task));
	}
};

// -------------------------------------------------------------------------------------------------

// TODO P4: libv.rev.res: Coroutines could improve the readability of multi-staged / multi-threaded resource loading
//
//	void processResource() {
//		irm->queue_fs ([] {
//			processFS(std::move(res), path, mov);
//
//			irm->queue_cpu([] {
//				processCPU(std::move(res), data, mov);
//
//				irm->queue_res([] {
//					processRES(std::move(res), model, mov);
//				});
//			});
//		});
//	}
//
//	Flow processResource(InternalModel_ptr res, std::shared_ptr<InternalModelLoader> loader_sp) {
//		co_await irm->queue_fs();
//		processFS(res, path, mov);
//
//		co_await irm->queue_cpu();
//		processCPU(res, data, mov);
//
//		co_await irm->queue_res();
//		processRES(res, model, mov);
//	}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
