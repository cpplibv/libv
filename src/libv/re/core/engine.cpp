// Project: libv.re, File: src/libv/re/core/engine.cpp

#include <libv/re/core/engine.hpp>

#include <libv/fsw/watcher.hpp>
#include <libv/mt/queue_busy.hpp>
#include <libv/mt/ticket_machine.hpp>
#include <libv/mt/worker_thread_pool.hpp>
#include <libv/re/core/destroy_queue.hpp>
#include <libv/re/core/render_library.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/material_loader.hpp>
#include <libv/re/resource/model_loader.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/re/resource/texture_loader.hpp>
#include <libv/re/settings.hpp>
#include <libv/utility/memory/storage.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/utility/timer.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

bool re_storage_active = false;
libv::storage<Engine> re_storage;
Engine& r = re_storage.value();

// -------------------------------------------------------------------------------------------------

struct ImplEngine {
	// libv::gl::GL gl; // In future GL ownership will be moved to libv.re from libv.ui

	libv::mt::ticket_machine pendingFrameBlockingWork;
	libv::mt::worker_thread_pool threadFS{2, "re/fs"};
	libv::mt::worker_thread_pool threadCPU{8, "re/cpu"};
	libv::mt::queue_busy<libv::unique_function<void()>> queueGL;

	Settings settings;

	libv::Nexus nexus;
	libv::fsw::Watcher fsw;

	DestroyQueue destroy;

	ShaderLoader shader;     // Uses: nexus, fsw
	TextureLoader texture;   // Uses: nexus, fsw
	MaterialLoader material; // Uses: nexus, fsw, shader, texture
	ModelLoader model;       // Uses: nexus, fsw, shader, texture, material

	explicit ImplEngine(Settings settings, libv::Nexus& nexus, libv::GL& gl) :
		settings(std::move(settings)),
		nexus(nexus),
		shader(this->settings, nexus, gl),
		texture(this->settings, nexus, gl),
		material(this->settings, nexus, gl),
		model(this->settings, nexus, gl) { }
};

// -------------------------------------------------------------------------------------------------

Engine::Engine(Settings settings, libv::Nexus& nexus, libv::GL& gl) :
	self(std::make_unique<ImplEngine>(std::move(settings), nexus, gl)),
	settings(self->settings),
	nexus(self->nexus),
	fsw(self->fsw),
	gl(gl), // gl(self->gl),
	destroy(self->destroy),
	shader(self->shader),
	texture(self->texture),
	material(self->material),
	model(self->model) {

	shader.init();
	texture.init();
	material.init();
	model.init();
}

Engine::~Engine() {
	// Teardown sequence:
	// - Precondition: EVERY scene and handler have already been destroyed by the client code
	// - Finish outstanding tasks (async operations, broadcast events from the destruction of the scenes)
	// - Destroy each loader:
	// 		- Destroy a loader that no one else uses
	// 		- Place a ticket into the GL queue to process until the queue is flushed
	// 		- Process the GL queue
	// - Process the destroy queue

	libv::Timer timer;

	// Finish outstanding tasks
	self->threadFS.stop();
	self->threadFS.join();

	self->threadCPU.stop();
	self->threadCPU.join();

	self->queueGL.emplace([t = blockNextFrame()] {});
	auto numTasks = processQueueGL(gl);

	// Destroy loaders
	self->model.reset();
	self->queueGL.emplace([t = blockNextFrame()] {});
	numTasks += processQueueGL(gl);

	self->material.reset();
	self->queueGL.emplace([t = blockNextFrame()] {});
	numTasks += processQueueGL(gl);

	self->texture.reset();
	self->queueGL.emplace([t = blockNextFrame()] {});
	numTasks += processQueueGL(gl);

	self->shader.reset();
	self->queueGL.emplace([t = blockNextFrame()] {});
	numTasks += processQueueGL(gl);

	// Process destroy queue
	self->destroy.gl_destroy(gl);

	log_re.trace("Engine teardown took:{: 6.3f}ms for {} task", timer.timef_ms().count(), numTasks);
}

// -------------------------------------------------------------------------------------------------

uint32_t Engine::processQueueGL(libv::GL& gl) {
	(void) gl;

	std::stop_source stopSource;
	auto wait_guard = self->pendingFrameBlockingWork.async_wait_guard(stopSource);

	int completedTasks = 0;
	// Busy waiting, while we wait for the pending blocking operations to finish, we process the res queue
	while (auto task = self->queueGL.try_wait_pop(stopSource.get_token())) {
		++completedTasks;
		try {
			(*task)();
		} catch (const std::exception& ex) {
			log_re.error("Exception occurred in GL task processing: {}", ex.what());
		}
	}

	return completedTasks;
}

void Engine::preRenderUpdate(libv::GL& gl) {
	libv::Timer timer;

	const auto numTasks = processQueueGL(gl);

	if (numTasks != 0)
		log_re.trace("GL task queue processing took:{: 6.3f}ms for {} task", timer.timef_ms().count(), numTasks);
}

void Engine::postFrameUpdate(libv::GL& gl) {
	destroy.gl_destroy(gl);
}

void Engine::queueTaskCPU(libv::unique_function<void()> task) {
	self->threadCPU.execute_async(std::move(task));
}

void Engine::queueTaskCPU(libv::unique_function<void()> task, std::chrono::steady_clock::duration after) {
	self->threadCPU.execute_async(std::move(task), after);
}

void Engine::queueTaskFS(libv::unique_function<void()> task) {
	self->threadFS.execute_async(std::move(task));
}

void Engine::queueTaskGL(libv::unique_function<void()> task) {
	self->queueGL.emplace(std::move(task));
}

libv::mt::ticket Engine::blockNextFrame() {
	return self->pendingFrameBlockingWork.create_ticket();
}

// void async_update(libv::time_point frameTime, libv::time_duration deltaTime) {
// 	// threadPool.execute_async([]{
// 		// 	// threads.execute_and_wait([] {
// 		// 	// 	update();
// 		// 	// });
// 		// 	// threads.execute_and_wait([] {
// 		// 	// 	prepare();
// 		// 	// });
// 	// });
// }
// void create(libv::GL& gl) {
// 	// rendererParticle.create(gl);
// }
// void destroy(libv::GL& gl) {
// 	// rendererParticle.destroy(gl);
// }
// void prepare_render(libv::GL& gl) {
// 	// resourceManager.update(gl);
// }
// void join_update() {
//
// }
// void async_prepare_render() {
//
// }
// void join_prepare_render() {
//
// }

// -------------------------------------------------------------------------------------------------

RenderLibrary::RenderLibrary(Settings settings, libv::Nexus& nexus, libv::GL& gl) {
	if (re_storage_active)
		throw std::runtime_error("libv::re::RenderLibrary was already initialized");
	re_storage_active = true;
	log_re.trace("Initialize Render Library");
	re_storage.create(std::move(settings), nexus, gl);
}

RenderLibrary::~RenderLibrary() {
	log_re.trace("Terminate Render Library");
	re_storage.destroy();
	re_storage_active = false;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
