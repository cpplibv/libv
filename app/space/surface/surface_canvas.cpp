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
	camera3D.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});

	fileWatcher.subscribe_file("surface/noise_config.lua", [this](const libv::fsw::Event& event) {
//			auto lock = std::unique_lock(mutex);
		changed = true;
	});
	//postProcessing.vignetteIntensity(0.15f);
}

void SurfaceCanvas::attach() {
	focus();
}

void SurfaceCanvas::setupRenderStates(libv::glr::Queue& glr) {
	renderer.prepare_for_render(glr);

	glr.state.enableDepthTest();
	glr.state.depthFunctionLess();
	glr.state.enableDepthMask();

	glr.state.enableBlend();
	glr.state.blendSrc_SourceAlpha();
	glr.state.blendDst_One_Minus_SourceAlpha();

	glr.state.enableCullFace();
//	glr.state.disableCullFace();
	glr.state.cullBackFace();
	glr.state.frontFaceCCW();

	glr.state.clipPlanes(0);
	if (isPolygonFill)
		glr.state.polygonModeFill();
	else
		glr.state.polygonModeLine();

	if (is3DCamera) {
		//TODO: get current position from the other camera type
//		if (isCameraChanged)
//			camera3D.look_at(camera2D.position(), libv::vec3f{xy(camera2D.position()),0});
		glr.projection = camera3D.projection(canvas_size);
		glr.view = camera3D.view();
	} else {
//		camera2D.look_at(camera3D.orbit_point(), libv::vec3f{xy(camera3D.orbit_point()), 0});
//		if (isCameraChanged)
//			camera2D.position(camera3D.eye());
		glr.projection = camera2D.projection(canvas_size);
		glr.view = camera2D.view();

	}

	glr.model = libv::mat4f::identity();

	// Set framebuffer to the post-processing target
	//glr.framebuffer_draw(renderTarget.framebuffer());

	glr.setClearColor(0, 0, 0, 1);
	glr.clearColor();
	glr.clearDepth();
}

libv::glr::Texture2D::RGBA32F SurfaceCanvas::buildTexture(const Chunk& chunk) {
	auto texture = libv::glr::Texture2D::RGBA32F();
	texture.storage(1, libv::vec2i{config.resolution + 1, config.resolution + 1});
	texture.set(libv::gl::MagFilter::Nearest);
	texture.set(libv::gl::MinFilter::Nearest);
	texture.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
	if (currentHeatMap == HeatMapType::height) {
		texture.image(0, libv::vec2i{0, 0}, libv::vec2i{config.resolution + 1, config.resolution + 1}, chunk.getColors(chunk.height).data());
	} else if (currentHeatMap == HeatMapType::temperature) {
		texture.image(0, libv::vec2i{0, 0}, libv::vec2i{config.resolution + 1, config.resolution + 1}, chunk.getColors(chunk.temperature).data());
	} else if (currentHeatMap == HeatMapType::humidity) {
		texture.image(0, libv::vec2i{0, 0}, libv::vec2i{config.resolution + 1, config.resolution + 1}, chunk.getColors(chunk.humidity).data());
	} else if (currentHeatMap == HeatMapType::fertility) {
		texture.image(0, libv::vec2i{0, 0}, libv::vec2i{config.resolution + 1, config.resolution + 1}, chunk.getColors(chunk.fertility).data());
	} else if (currentHeatMap == HeatMapType::surface) {
		texture.image(0, libv::vec2i{0, 0}, libv::vec2i{config.resolution + 1, config.resolution + 1}, chunk.getColors(chunk.surface).data());
	}
	return texture;

//	renderer.surfaceTexture.addTexture(heightMap, chunk.position);
}

void SurfaceCanvas::buildRenderObject(const Chunk& chunk) {
	if (is3DCamera) {
		renderer.surface.addChunk(chunk);
	} else {
		const auto heatMap = buildTexture(chunk);
		renderer.surfaceTexture.addTexture(heatMap, chunk.position);
	}
}

void SurfaceCanvas::clearRenderObjects() {
	renderer.surface.clear();
	renderer.surfaceTexture.clear();
//	renderer.debug.clear_spheres();
}

void SurfaceCanvas::buildRenderObjects() {
	clearRenderObjects();

	for (const auto& chunk : chunks) {
		buildRenderObject(chunk);
	}
}

void SurfaceCanvas::addGizmo() {
	renderer.debug.add_debug_sphere(
			{0.7f, 0, 0}, 0.15f, {1, 0, 0, 1});
	renderer.debug.add_debug_sphere(
			{0, 0.7f, 0}, 0.15f, {0, 1, 0, 1});
	renderer.debug.add_debug_sphere(
			{0, 0, 0.7f}, 0.15f, {0, 0, 1, 1});
}

void SurfaceCanvas::buildChunks() {
	libv::Timer timerChunkGen;
	ChunkGen chunkGen;
	auto script = libv::read_file_str_or_throw("surface/noise_config.lua");
	config = binding.getConfigFromLuaScript(script);
	clearRenderObjects();
	renderer.debug.clear_spheres();

	//getChunk, render (availability alapjan)
	chunks.clear();
	for (int i = 0; i < config.numChunks; ++i) {
		const auto chunkPos = libv::vec2f(libv::index_spiral(i).cast<float>());
		Chunk chunk = chunkGen.generateChunk(config, chunkPos);
		chunkGen.placeVegetation(chunk, config);
		// more log needed
		fmt::print("TimerChunkGen: {:8.4f} ms", timerChunkGen.timed_ms().count());
		std::cout << std::endl;

		renderer.debug.add_debug_sphere({chunk.position, 0}, 0.1f, {1, 0, 0, 1}, 10, 10);

		buildRenderObject(chunk);
		//add features
		if (config.visualization == Visualization::spheres) {
			for (const auto& surfaceObjectStorage : chunk.featureList) {
				for (const auto& point : surfaceObjectStorage.points) {
					renderer.debug.add_debug_sphere(point.position, point.size, point.color, 10, 10);
				}
			}
		}
		chunks.emplace_back(std::move(chunk));
	}
	addGizmo();
}


//	virtual void update(libv::ui::time_duration delta_time) override {}
void SurfaceCanvas::render(libv::glr::Queue& glr) {
	setupRenderStates(glr);
	//		screen_picker = camera.picker(canvas_size);
//		renderTarget.size(canvas_size.cast<int32_t>());
//		postProcessing.size(canvas_size.cast<int32_t>());
	const auto s_guard = glr.state.push_guard();
	if (changed) {
		buildChunks();
		changed = false;
	}

	if (isCameraChanged || isTextureChanged) {
		buildRenderObjects();
		isCameraChanged = false;
	}

//	if (isTextureChanged) {
//		const auto heatMap = buildTexture(chunk);
//		renderer.surfaceTexture.addTexture(heatMap, chunk.position);
//		isTextureChanged = false;
//	}

	//render surface texture/_3d
	if (is3DCamera) {
		renderer.surface.render(glr, renderer.resource_context.uniform_stream);
	} else {
		renderer.surfaceTexture.render(glr, renderer.resource_context.uniform_stream);
	}

	// render plant model/debug
	if (is3DCamera) {
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
			renderer.debug.render(glr, renderer.resource_context.uniform_stream);
	}
}

// -------------------------------------------------------------------------------------------------


} // namespace surface
