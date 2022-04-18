// Created by dbobula on 3/25/2022.

#include <space/surface/surface_canvas.hpp>

// std
#include <iostream>

//libv
#include <libv/utility/timer.hpp>
#include <libv/utility/index_spiral.hpp>
#include <libv/glr/queue.hpp>
//#include <src/libv/utility/screen_picker.hpp>
//#include <libv/rev/render_target.hpp>
//#include <libv/rev/post_processing.hpp>

//space
#include <space/log.hpp>
#include <space/surface/chunk.hpp>



namespace surface {


SurfaceCanvas::SurfaceCanvas(libv::ui::UI& ui) :
		renderer(ui)
//screen_picker(camera.picker({100, 100})),
//renderTarget({100, 100}, 4),
//postProcessing(renderer.resource_context.shader_loader, {100, 100})
{
	camera.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});
//		heightMap.storage(1, libv::vec2i{config.size, config.size});

	fileWatcher.subscribe_file("surface/noise_config.lua", [this](const libv::fsw::Event& event) {
//			auto lock = std::unique_lock(mutex);
		changed = true;
	});
	//postProcessing.vignetteIntensity(0.15f);
}

void SurfaceCanvas::attach() {
	focus();
}

//	virtual void update(libv::ui::time_duration delta_time) override {}
void SurfaceCanvas::render(libv::glr::Queue& glr) {
	renderer.prepare_for_render(glr);

//		screen_picker = camera.picker(canvas_size);
//		renderTarget.size(canvas_size.cast<int32_t>());
//		postProcessing.size(canvas_size.cast<int32_t>());

	const auto s_guard = glr.state.push_guard();

	glr.state.enableDepthTest();
	glr.state.depthFunctionLess();
	glr.state.enableDepthMask();

	glr.state.enableBlend();
	glr.state.blendSrc_SourceAlpha();
	glr.state.blendDst_One_Minus_SourceAlpha();

	//glr.state.enableCullFace();
	glr.state.disableCullFace();
	glr.state.cullBackFace();
	glr.state.frontFaceCCW();

	glr.state.clipPlanes(0);
	if (isPolygonFill)
		glr.state.polygonModeFill();
	else
		glr.state.polygonModeLine();

	glr.projection = camera.projection(canvas_size);
	glr.view = camera.view();
	glr.model = libv::mat4f::identity();

	//const auto eye = glr.eye();

	// Set framebuffer to the post-processing target
	//glr.framebuffer_draw(renderTarget.framebuffer());

	glr.setClearColor(0, 0, 0, 1);
	glr.clearColor();
	glr.clearDepth();

	// =================================================================================================
	if (changed) {
		libv::Timer timerChunkGen;
		ChunkGen chunkGen;
		auto script = libv::read_file_str_or_throw("surface/noise_config.lua");
		config = binding.getConfigFromLuaScript(script);
		renderer.debug.spheres.clear();
		chunks.clear();
		for (int i = 0; i < config.numChunks; ++i) {
			const auto chunkPosi = libv::index_spiral(i);
			const auto chunkPos = libv::vec2f(chunkPosi.x, chunkPosi.y);
			Chunk chunk = chunkGen.generateChunk(config, chunkPos);
			chunkGen.placeVegetation(chunk, config);
			fmt::print("TimerChunkGen: {:8.4f} ms", timerChunkGen.timed_ms().count());
			std::cout << std::endl;

			renderer.debug.spheres.emplace_back(space::RendererDebug::Sphere
					{{chunk.position, 0}, 0.1f, {1, 0, 0, 1}, 10, 10});

			if (config.visualization == Visualization::spheres) { //add features
				for (const auto& surfaceObjectStorage : chunk.featureList) {
					for (const auto& point : surfaceObjectStorage.points) {
						renderer.debug.spheres.emplace_back(point.position, point.size, point.color, 10, 10);
					}
				}
//						renderer.debug.build_triangles_mesh(renderer.debug.mesh_triangle);
			}
			chunks.emplace_back(std::move(chunk));
		}
		renderer.debug.spheres.emplace_back(space::RendererDebug::Sphere
				{{0.5f, 0, 0}, 0.25f, {1, 0, 0, 1}, 10, 10});
		renderer.debug.spheres.emplace_back(space::RendererDebug::Sphere
				{{0, 0.5f, 0}, 0.25f, {0, 1, 0, 1}, 10, 10});
		renderer.debug.spheres.emplace_back(space::RendererDebug::Sphere
				{{0, 0, 0.5f}, 0.25f, {0, 0, 1, 1}, 10, 10});
		renderer.debug.build_triangles_mesh(renderer.debug.mesh_triangle);
		if (config.mode == Mode::_3d) {
			renderer.surface.build_mesh(renderer.surface.mesh, chunks);
		}
//			else {
//				//TODO revive texture
//				heightMap = libv::glr::Texture2D::RGBA32F();
//				heightMap.storage(1, libv::vec2i{config.resolution, config.resolution});
//				heightMap.image(0, libv::vec2i{0, 0}, libv::vec2i{config.resolution, config.resolution}, chunk.getColors().data());
//			}


		changed = false;
	}

	config.mode == Mode::_3d ?
			renderer.surface.render(glr, renderer.resource_context.uniform_stream) :
			renderer.surfaceTexture.render(glr, renderer.resource_context.uniform_stream, heightMap);
	// render plant model
	if (config.visualization == Visualization::model)
		for (const auto& chunk : chunks) {
			for (const auto& feature : chunk.featureList) {
				for (const auto& point : feature.points) {
					const auto m2_guard = glr.model.push_guard();
					glr.model.translate(point.position);
					glr.model.scale(point.size * 0.01f);
					glr.model.rotate(libv::radian(libv::pi / 2), libv::vec3f(1, 0, 0));

					renderer.fleet.render(glr, renderer.resource_context.uniform_stream);
				}
			}
		}
	else
		renderer.debug.renderTriangles(glr, renderer.resource_context.uniform_stream);
}

// -------------------------------------------------------------------------------------------------


} // namespace surface
