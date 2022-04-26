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

SurfaceCanvas::SurfaceCanvas(libv::ui::UI& ui, libv::ctrl::Controls& controls) :
		cameraManager(controls),
		renderer(ui) {

	configPaths.emplace_back("config/noise_config.lua");
	configPaths.emplace_back("config/noise_config2.lua");
	configPaths.emplace_back("config/noise_config3.lua");

	setConfig();

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

void SurfaceCanvas::setConfig() {
	configCnt %= configPaths.size();
	currentConfigPath = configPaths[configCnt];

	fileWatcher.subscribe_file(currentConfigPath, [this](const libv::fsw::Event&) {
		changed = true;
	});
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

	glr.projection = cameraManager.projection(canvas_size);
	glr.view = cameraManager.view();

	glr.model = libv::mat4f::identity();

	glr.setClearColor(0, 0, 0, 1);
	glr.clearColor();
	glr.clearDepth();
}

void SurfaceCanvas::update(libv::ui::time_duration delta_time) {
	(void) delta_time;

	cameraManager.update();

	if (configChanged) {
		configChanged = false;
		configCnt++;
		setConfig();
	}

	if (refresh) {
		refresh = false;
		currentScene = createScene(currentHeatMap);
		changed = true;
	}

	if (changed) {
		changed = false;

		auto script = libv::read_file_str_or_throw(currentConfigPath);
		auto conf = binding.getConfigFromLuaScript(script);

		surface->gen(std::move(conf));
	}

	surfaceDirty = surface->update();
}

void SurfaceCanvas::render(libv::glr::Queue& glr) {
	setupRenderStates(glr);
	const auto s_guard = glr.state.push_guard();

	if (previousHeatMap != currentHeatMap) {
		previousHeatMap = currentHeatMap;
		currentScene = createScene(currentHeatMap);
//		if (surfaceDirty) {
		//build mesh
		currentScene->build(surface->getChunks());
		currentScene->buildVeggie(surface->getChunks());
//		}
	}

	if (!surface->getChunks().empty()) {
		if (surfaceDirty) {
			//build mesh
			currentScene->build(surface->getChunks());
			currentScene->buildVeggie(surface->getChunks());
		}

		//render surface texture/_3d
		currentScene->render(glr, renderer.resource_context.uniform_stream);

		// render plant model/debug
		if (enableVegetation)
			currentScene->renderVeggie(glr, renderer.resource_context.uniform_stream);
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
