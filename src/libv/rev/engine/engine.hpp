// Created by Vader on 2023.01.09..

#pragma once

// libv
// #include <libv/algo/erase_if_unstable.hpp>
// #include <libv/algo/sort.hpp>
// #include <libv/math/angle.hpp>
// #include <libv/math/constants.hpp>
// #include <libv/math/vec.hpp>
// #include <libv/utility/chrono.hpp>
// #include <libv/ui/chrono.hpp>
#include <libv/ui/fwd.hpp>
#include <libv/utility/hash_string.hpp>
// #include <libv/utility/random/xoroshiro128.hpp>
// #include <libv/math/quat.hpp>
#include <libv/mt/hardware_concurrency.hpp>
#include <libv/mt/worker_thread_pool.hpp>
// std
// #include <variant>
// #include <memory>
// #include <span>
// #include <random>
#include <unordered_map>
#include <vector>
// pro
#include <libv/rev/effect/particle.hpp>
#include <libv/rev/effect/particle_renderer.hpp>
#include <libv/rev/renderer/renderer_editor_grid.hpp>
#include <libv/rev/settings.hpp>


#include <libv/mt/thread_bulk.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/math/frustum.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class Engine {
private:
	libv::mt::worker_thread_pool threadPool{libv::mt::hardware_concurrency_or(4 + 2) - 2, "rev/cpu"};
	libv::mt::thread_bulk threads{libv::mt::hardware_concurrency_or(4 + 2) - 2};
// private:
public:
	libv::rev::ResourceManager resourceManager;

private:
public:
	libv::glr::UniformBuffer uniformStream{libv::gl::BufferUsage::StreamDraw};
	libv::rev::RendererParticle rendererParticle{resourceManager};
	libv::rev::RendererEditorGrid rendererEditorGrid{resourceManager};

private:
	// ParticleEngine particleEngine;

public:
	explicit Engine(libv::rev::Settings settings, Nexus& nexus) : resourceManager(std::move(settings), nexus) {
	}

public:
	void async_update(libv::time_point frameTime, libv::time_duration deltaTime) {
		// threadPool.execute_async([]{
			// 	// threads.execute_and_wait([] {
			// 	// 	update();
			// 	// });
			// 	// threads.execute_and_wait([] {
			// 	// 	prepare();
			// 	// });
		// });
	}
	void create(libv::gl::GL& gl) {
		rendererParticle.create(gl);
	}
	void destroy(libv::gl::GL& gl) {
		rendererParticle.destroy(gl);
	}
	void prepare_render(libv::gl::GL& gl) {
		resourceManager.update(gl);
	}
	// void join_update() {
	//
	// }
	// void async_prepare_render() {
	//
	// }
	// void join_prepare_render() {
	//
	// }
};

// // -------------------------------------------------------------------------------------------------
//
// class ParticleEngine {
// private:
// 	libv::mt::worker_thread_pool threadPool{4, "par/cpu"};
// 	libv::rev::ResourceManager resourceManager;
//
// private:
// 	libv::glr::UniformBuffer uniformStream{libv::gl::BufferUsage::StreamDraw};
// 	libv::rev::RendererParticle rendererParticle{resourceManager};
//
// private:
// 	std::vector<ParticleSet> sets;
//
// public:
// };

// -------------------------------------------------------------------------------------------------

// class Layer {
//
// };

// class Renderer {
//
// };

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
