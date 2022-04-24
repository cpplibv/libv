// Created by dbobula on 3/25/2022.

#include <surface/surface/surface_canvas.hpp>

// std
#include <iostream>

//libv
//#include <libv/utility/timer.hpp>
#include <libv/utility/index_spiral.hpp>
#include <libv/glr/queue.hpp>

//space
#include <surface/log.hpp>
#include <surface/surface/chunk.hpp>


namespace surface {

SurfaceCanvas::SurfaceCanvas(libv::ui::UI& ui) :
		renderer(ui)
{
	camera3D.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});

	fileWatcher.subscribe_file("surface/noise_config.lua", [this](const libv::fsw::Event&) {
		changed = true;
	});

	surface = std::make_unique<Surface>();
	currentScene = createScene(currentHeatMap);
}

std::unique_ptr<Scene> SurfaceCanvas::createScene(SceneType scene) {
	switch (scene) {
	case SceneType::_3d:
		return std::make_unique<SurfaceScene>(renderer, renderer.resource_context);
	case SceneType::height:
		return std::make_unique<HeightHeatMap>(renderer, renderer.resource_context);
	case SceneType::temperature:
		return std::make_unique<TemperatureHeatMap>(renderer, renderer.resource_context);
	case SceneType::humidity:
		return std::make_unique<HumidityHeatMap>(renderer, renderer.resource_context);
	case SceneType::fertility:
		return std::make_unique<FertilityHeatMap>(renderer, renderer.resource_context);
	case SceneType::biome:
		return std::make_unique<BiomeHeatMap>(renderer, renderer.resource_context);
	}

	assert(false && "Invalid SceneType enum value");
	return nullptr;
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

	glr.setClearColor(0, 0, 0, 1);
	glr.clearColor();
	glr.clearDepth();
}

void SurfaceCanvas::update(libv::ui::time_duration delta_time) {
	(void) delta_time;

	if (changed) {
		changed = false;

		auto script = libv::read_file_str_or_throw("surface/noise_config.lua");
		auto conf = binding.getConfigFromLuaScript(script);

		surface->gen(std::move(conf));
//			buildChunks();
//			std::cout << "Building chunks has finished" << std::endl;
//			fmt::print("BuildChunks(): {:8.4f} ms", timerChunkGen.timed_ms().count());
//			std::cout << std::endl;
//			timerChunkGen.reset();
	}

	surfaceDirty = surface->update();
}

void SurfaceCanvas::render(libv::glr::Queue& glr) {
	setupRenderStates(glr);
	const auto s_guard = glr.state.push_guard();

	if (previousHeatMap != currentHeatMap) {
		previousHeatMap = currentHeatMap;
		//libv::Timer timerChunkGen;

		currentScene = createScene(currentHeatMap);
	}

	//render surface texture/_3d
	if (!surface->getChunks().empty()) {
		if (surfaceDirty) {
			currentScene->build(surface->getChunks());
	//		std::cout << "Building render objects has finished" << std::endl;
	//		fmt::print("currentScene->build(): {:8.4f} ms", timerChunkGen.timed_ms().count());
	//		std::cout << std::endl;
	//		timerChunkGen.reset();

	//		if (enableVegetation)  //there is veggies
			currentScene->buildVeggie(surface->getChunks());
		}

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
