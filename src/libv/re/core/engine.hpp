// Project: libv.re, File: src/libv/re/core/engine.hpp

#pragma once

#include <libv/fsw/fwd.hpp>
#include <libv/gl/fwd.hpp>
#include <libv/mt/fwd.hpp>
#include <libv/re/fwd.hpp>
#include <libv/utility/nexus_fwd.hpp>
#include <libv/utility/unique_function.hpp>

#include <chrono>
#include <memory>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------
// OpenGL 4.5 is required with the following extensions:
// Optional: EXT_texture_filter_anisotropic (not yet used, but planned)
// Required: ARB_shader_draw_parameters (not yet used, but planned)

class ImplEngine;
class Engine {
private:
	std::unique_ptr<ImplEngine> self;

public:
	Settings& settings;

	libv::Nexus& nexus;
	libv::fsw::Watcher& fsw;

	libv::GL& gl;
	DestroyQueue& destroy;

	ShaderLoader& shader;
	TextureLoader& texture;
	MaterialLoader& material;
	ModelLoader& model;

public:
	Engine(Settings settings, libv::Nexus& nexus, libv::GL& gl);
	~Engine();

private:
	uint32_t processQueueGL(libv::GL& gl);

public:
	void preRenderUpdate(libv::GL& gl);
	void postFrameUpdate(libv::GL& gl);
	// These might be moved into their own 'context' type header as scheduler
	void queueTaskCPU(libv::unique_function<void()> task);
	void queueTaskCPU(libv::unique_function<void()> task, std::chrono::steady_clock::duration after);
	void queueTaskFS(libv::unique_function<void()> task);
	void queueTaskGL(libv::unique_function<void()> task);
	libv::mt::ticket blockNextFrame();

	// I think destroy() will be required to handle single "frame" + immediate cleanup renders properly
	// scenes and every object have to be destroyed first, destroy() can only cleanup GL resources
	// after that happens. I think this will not possible with a single pass.
	// (UNLESS, scenes can be weak owned by the engine)
	// void destroy();

	// void async_update(libv::time_point frameTime, libv::time_duration deltaTime);
	// void create(libv::GL& gl);
	// void destroy(libv::GL& gl);
	// void prepare_render(libv::GL& gl);
	// void join_update();
	// void async_prepare_render();
	// void join_prepare_render();
};

// -------------------------------------------------------------------------------------------------

extern Engine& r;

} // namespace re
} // namespace libv --------------------------------------------------------------------------------
namespace libv {

using libv::re::r;

} // namespace libv --------------------------------------------------------------------------------
