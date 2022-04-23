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
	if (enableWireframe)
		glr.state.polygonModeLine();
	else
		glr.state.polygonModeFill();

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

//void SurfaceCanvas::addGizmo() {
//	renderer.debug.add_debug_sphere(
//			{0.7f, 0, 0}, 0.15f, {1, 0, 0, 1});
//	renderer.debug.add_debug_sphere(
//			{0, 0.7f, 0}, 0.15f, {0, 1, 0, 1});
//	renderer.debug.add_debug_sphere(
//			{0, 0, 0.7f}, 0.15f, {0, 0, 1, 1});
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


	if (previousHeatMap != currentHeatMap || changed) {
		previousHeatMap = currentHeatMap;
		libv::Timer timerChunkGen;
		if (changed) {
			buildChunks();
			changed = false;

			std::cout << "Building chunks has finished" << std::endl;
			fmt::print("BuildChunks(): {:8.4f} ms", timerChunkGen.timed_ms().count());
			std::cout << std::endl;

			timerChunkGen.reset();
		}

		switch (currentHeatMap) {
		case SceneType::_3d:
			currentScene = std::make_unique<SurfaceScene>(renderer, renderer.resource_context);
			break;
		case SceneType::height:
			currentScene = std::make_unique<HeightHeatMap>(renderer, renderer.resource_context);
			break;
		case SceneType::temperature:
			currentScene = std::make_unique<TemperatureHeatMap>(renderer, renderer.resource_context);
			break;
		case SceneType::humidity:
			currentScene = std::make_unique<HumidityHeatMap>(renderer, renderer.resource_context);
			break;
		case SceneType::fertility:
//			auto newScene = fert(renderer, renderer.resource_context);
			currentScene = std::make_unique<FertilityHeatMap>(renderer, renderer.resource_context);
			break;
		case SceneType::biome:
			currentScene = std::make_unique<BiomeHeatMap>(renderer, renderer.resource_context);

			break;
		}

		currentScene->build(chunks);
//		std::cout << "Building render objects has finished" << std::endl;
//		fmt::print("currentScene->build(): {:8.4f} ms", timerChunkGen.timed_ms().count());
//		std::cout << std::endl;
//		timerChunkGen.reset();

//		if (enableVegetation)
		currentScene->buildVeggie(chunks);

//		std::cout << "Building veggie has finished" << std::endl;
//		fmt::print("currentScene->buildVeggie(): {:8.4f} ms", timerChunkGen.timed_ms().count());
//		std::cout << std::endl;
	}


	//render surface texture/_3d
	currentScene->render(glr, renderer.resource_context.uniform_stream);

	// render plant model/debug
	if (enableVegetation) {
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

	if (currentHeatMap == SceneType::_3d && enableGrid) {
		const auto s_guard = glr.state.push_guard();
		glr.state.polygonModeFill(); // In case we are wireframe mode, force poly fill for the grid
		glr.state.disableDepthMask();
		renderer.editorGrid.render(glr, renderer.resource_context.uniform_stream);
	}
}

// -------------------------------------------------------------------------------------------------


} // namespace surface
