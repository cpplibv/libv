// Created by dbobula on 3/25/2022.

#include <surface/surface/surface_canvas.hpp>

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
#include <surface/log.hpp>
#include <surface/surface/chunk.hpp>


namespace surface {

SurfaceCanvas::SurfaceCanvas(libv::ui::UI& ui) :
		renderer(ui)
//screen_picker(camera.picker({100, 100})),
//renderTarget({100, 100}, 4),
//postProcessing(renderer.resource_context.shader_loader, {100, 100})
{
	camera3D.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});

	fileWatcher.subscribe_file("surface/noise_config.lua", [this](const libv::fsw::Event&) {
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

	switch (currentCameraMode) {
	case CameraMode::_2d:
		glr.projection = camera2D.projection(canvas_size);
		glr.view = camera2D.view();
		break;
	case CameraMode::_3d:
		// TODO: get current position from the other camera type
		glr.projection = camera3D.projection(canvas_size);
		glr.view = camera3D.view();
		break;
	default:
		assert(false && "Invalid CameraMode enum value");
	}

	glr.model = libv::mat4f::identity();

	// Set framebuffer to the post-processing target
	//glr.framebuffer_draw(renderTarget.framebuffer());

	glr.setClearColor(0, 0, 0, 1);
	glr.clearColor();
	glr.clearDepth();
}

//libv::vector_2D<float> SurfaceCanvas::buildSurfaceTexture() {
//	const auto size = config.resolution + 1;
//	libv::vector_2D<float> result{size, size};
//	result.fill(0.f);
//
//	for (const auto& chunk : chunks) {
//		for (size_t y = 0; y < chunk.humidity.size_y(); ++y) {
//			for (size_t x = 0; x < chunk.humidity.size_x(); ++x) {
//				result(
//						std::clamp(static_cast<size_t>(std::max(0.f, chunk.humidity(x, y)) * static_cast<float>(config.resolution)), 0uz, result.size_x() - 1),
//						std::clamp(static_cast<size_t>(std::max(0.f, chunk.temperature(x, y)) * static_cast<float>(config.resolution)), 0uz, result.size_y() - 1)
//				) += 0.005f;
//			}
//		}
//	}
//
//	return result;
//}

//libv::glr::Texture SurfaceCanvas::buildTexture(const Chunk& chunk) {
//
//	if (currentHeatMap == SceneType::height || currentHeatMap == SceneType::biome) {
//		auto texture = libv::glr::Texture2D::RGBA32F();
//		texture.storage(1, libv::vec2z{config.resolution + 1, config.resolution + 1}.cast<int>());
//		texture.set(libv::gl::MagFilter::Nearest);
//		texture.set(libv::gl::MinFilter::Nearest);
//		texture.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
//		if (currentHeatMap == SceneType::height)
//			texture.image(0, libv::vec2i{0, 0}, libv::vec2z{config.resolution + 1, config.resolution + 1}.cast<int>(), chunk.getColors(chunk.height).data());
//		else if (currentHeatMap == SceneType::biome)
//			texture.image(0, libv::vec2i{0, 0}, chunk.biomeMap.size().cast<int32_t>(), chunk.biomeMap.data());
//
//		return texture;
//	} else {
//		auto texturef = libv::glr::Texture2D::R32F();
//		texturef.storage(1, libv::vec2z{config.resolution + 1, config.resolution + 1}.cast<int>());
//		texturef.set(libv::gl::MagFilter::Nearest);
//		texturef.set(libv::gl::MinFilter::Nearest);
////		texturef.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
//		if (currentHeatMap == SceneType::temperature)
//			texturef.image(0, libv::vec2i{0, 0}, chunk.temperature.size().cast<int32_t>(), chunk.temperature.data());
//		else if (currentHeatMap == SceneType::humidity)
//			texturef.image(0, libv::vec2i{0, 0}, chunk.humidity.size().cast<int32_t>(), chunk.humidity.data());
//		else if (currentHeatMap == SceneType::fertility)
//			texturef.image(0, libv::vec2i{0, 0}, chunk.fertility.size().cast<int32_t>(), chunk.fertility.data());
//
//		return texturef;
//	}

//	renderer.surfaceTexture.addTexture(heightMap, chunk.position);
//}

//void SurfaceCanvas::buildRenderObject(const Chunk& chunk) {
//	if (is3DCamera) {
//		renderer.surface.addChunk(chunk);
//	} else {
//		const auto heatMap = buildTexture(chunk);
//		renderer.surfaceTexture.addTexture(heatMap, chunk.position);
//	}
//
//	//add features
//	//TODO: only build when necessary
////	if (withVegetation) {
//	if (config.visualization == Visualization::spheres) {
//		for (const auto& veggie : chunk.veggies) {
//			if (is3DCamera)
//				renderer.debug.add_debug_sphere(veggie.pos, veggie.size, veggie.color, 10, 10);
//			else
//				renderer.debug.add_debug_sphere(libv::vec3f{xy(veggie.pos), -2.5f}, veggie.size, veggie.color, 10, 10);
//		}
//	}
////	}
//
//}

//void SurfaceCanvas::clearRenderObjects() {
//	renderer.surface.clear();
//	renderer.surfaceTexture.clear();
//	renderer.debug.clear_spheres();
//}

void SurfaceCanvas::addGizmo() {
	renderer.debug.add_debug_sphere(
			{0.7f, 0, 0}, 0.15f, {1, 0, 0, 1});
	renderer.debug.add_debug_sphere(
			{0, 0.7f, 0}, 0.15f, {0, 1, 0, 1});
	renderer.debug.add_debug_sphere(
			{0, 0, 0.7f}, 0.15f, {0, 0, 1, 1});
}

//void SurfaceCanvas::buildRenderObjects() {
//	clearRenderObjects();
//	addGizmo();
//
//	if (currentHeatMap == SceneType::distribution && !is3DCamera) {
//		const auto data = buildSurfaceTexture();
//
//		auto texturef = libv::glr::Texture2D::R32F();
//		texturef.storage(1, data.size().cast<int32_t>());
//		texturef.set(libv::gl::MagFilter::Nearest);
//		texturef.set(libv::gl::MinFilter::Nearest);
////		texturef.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
//		texturef.image(0, libv::vec2i{0, 0}, data.size().cast<int32_t>(), data.data());
//
//		renderer.surfaceTexture.addTexture(texturef, {-5.f, -5.f}, {10.f, 10.f});
//	} else {
//		for (const auto& chunk : chunks) {
//			buildRenderObject(chunk);
//		}
//	}
//}


void SurfaceCanvas::buildChunks() {
	libv::Timer timerChunkGen;
	ChunkGen chunkGen;
	auto script = libv::read_file_str_or_throw("surface/noise_config.lua");
	config = binding.getConfigFromLuaScript(script);

	//getChunk, render (availability alapjan)
	chunks.clear();
	for (size_t i = 0; i < config.numChunks; ++i) {
		const auto chunkIndex = libv::index_spiral(i).cast<int32_t>();
		Chunk chunk = chunkGen.generateChunk(config, chunkIndex);
		chunkGen.placeVegetation(chunk, config);
		// more log needed
		fmt::print("TimerChunkGen: {:8.4f} ms", timerChunkGen.timed_ms().count());
		std::cout << std::endl;

		chunks.emplace_back(std::move(chunk));
	}
}

//	virtual void update(libv::ui::time_duration delta_time) override {}
void SurfaceCanvas::render(libv::glr::Queue& glr) {
	setupRenderStates(glr);
	//		screen_picker = camera.picker(canvas_size);
//		renderTarget.size(canvas_size.cast<int32_t>());
//		postProcessing.size(canvas_size.cast<int32_t>());
	const auto s_guard = glr.state.push_guard();


	if (hasSceneChanged || changed) {
		hasSceneChanged = false;
		libv::Timer timerChunkGen;
		if (changed) {
			buildChunks();
			changed = false;

			std::cout << "Building chunks has finished" << std::endl;
			fmt::print("BuildChunks(): {:8.4f} ms", timerChunkGen.timed_ms().count());
			std::cout << std::endl;

			timerChunkGen.reset();
		}


		if (currentHeatMap == SceneType::_3d) {
			currentScene = std::make_unique<SurfaceScene>(renderer, renderer.resource_context);
		} else if (currentHeatMap == SceneType::height) {
			currentScene = std::make_unique<HeightHeatMap>(renderer, renderer.resource_context);
		} else if (currentHeatMap == SceneType::temperature) {
			currentScene = std::make_unique<TemperatureHeatMap>(renderer, renderer.resource_context);
		} else if (currentHeatMap == SceneType::humidity) {
			currentScene = std::make_unique<HumidityHeatMap>(renderer, renderer.resource_context);
//		} else if (currentHeatMap == SceneType::fertility) {
//			auto newScene = fert(renderer, renderer.resource_context);
		} else if (currentHeatMap == SceneType::biome) {
			currentScene = std::make_unique<BiomeHeatMap>(renderer, renderer.resource_context);
		} else {
			throw std::runtime_error("Unsupported scene type");
		}

		currentScene->build(chunks);
//		std::cout << "Building render objects has finished" << std::endl;
//		fmt::print("currentScene->build(): {:8.4f} ms", timerChunkGen.timed_ms().count());
//		std::cout << std::endl;
//		timerChunkGen.reset();

		if (withVegetation)
			currentScene->buildVeggie(chunks);

//		std::cout << "Building veggie has finished" << std::endl;
//		fmt::print("currentScene->buildVeggie(): {:8.4f} ms", timerChunkGen.timed_ms().count());
//		std::cout << std::endl;
	}


	//render surface texture/_3d
	currentScene->render(glr, renderer.resource_context.uniform_stream);

	// render plant model/debug
	if (withVegetation) {
//		if (config.visualization == Visualization::model)
//			for (const auto& chunk : chunks) {
//				for (const auto& veggie : chunk.veggies) {
//					const auto m2_guard = glr.model.push_guard();
//					glr.model.translate(veggie.pos);
//					glr.model.scale(veggie.size * 0.01f);
//					glr.model.rotate(libv::radian(libv::pi / 2), libv::vec3f(1, 0, 0));
//
//					renderer.fleet.render(glr, renderer.resource_context.uniform_stream);
//				}
//			}
//		else
		currentScene->renderVeggie(glr, renderer.resource_context.uniform_stream);

//		renderer.debug.render(glr, renderer.resource_context.uniform_stream);
	}

}

// -------------------------------------------------------------------------------------------------


} // namespace surface
