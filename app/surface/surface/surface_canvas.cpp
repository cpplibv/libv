// Created by dbobula on 3/25/2022.

// hpp
#include <surface/surface/surface_canvas.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/algo/wildcard.hpp>
// pro
#include <surface/log.hpp>
//std
#include <filesystem>


namespace surface {

// -------------------------------------------------------------------------------------------------

SurfaceCanvas::SurfaceCanvas(libv::ui::UI& ui, libv::ctrl::Controls& controls, std::string configPath_) :
		cameraManager(controls),
		currentConfigPath(std::move(configPath_)),
		renderer(ui) {

	fileWatcher.subscribe_directory("config", [this](const libv::fsw::Event&) {
		changed = true;
	});

	surface = std::make_unique<Surface>();
	activeScene = createScene(currentScene);
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

	glr.projection = cameraManager.projection(canvas_size);
	glr.view = cameraManager.view();

	glr.model = libv::mat4f::identity();

	glr.setClearColor(0, 0, 0, 1);
	glr.clearColor();
	glr.clearDepth();
}

std::string SurfaceCanvas::cycleConfigs() {
	const auto filter_pattern = "**.lua";

	bool next = false;
	bool first = true;
	std::string firstConfig;
	const auto dir = "config/";
	for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
		if (not entry.is_regular_file())
			continue;

		auto filepath = entry.path().generic_string();

		if (not libv::match_wildcard_glob(filepath, filter_pattern))
			continue;

		if (first) {
			first = false;
			firstConfig = filepath;
		}

		if (next) {
			currentConfigPath = filepath;
			return currentConfigPath;
		}

		if (filepath == currentConfigPath) {
			next = true;
		}
	}

	assert(not first && "Given directory is empty of lua config files");
	currentConfigPath = firstConfig;
	return currentConfigPath;
}

void SurfaceCanvas::update(libv::ui::time_duration delta_time) {
	(void) delta_time;

	cameraManager.update();

	if (refresh) {
		refresh = false;
		activeScene = createScene(currentScene);
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

	if (previousScene != currentScene) {
		previousScene = currentScene;
		activeScene = createScene(currentScene);
		//build mesh
		activeScene->build(surface->getGenCnt(), surface->getChunks());
		activeScene->buildVeggie(surface->getGenCnt(), surface->getChunks());
	}

	if (surfaceDirty) {
		//build mesh
		activeScene->build(surface->getGenCnt(), surface->getChunks());
		activeScene->buildVeggie(surface->getGenCnt(), surface->getChunks());
	}

	//render surface texture/_3d
	activeScene->render(glr, renderer.resource_context.uniform_stream);

	// render plant model/debug
	if (enableVegetation)
		activeScene->renderVeggie(glr, renderer.resource_context.uniform_stream);

	if (enableSkybox) {
		renderer.sky.render(glr, renderer.resource_context.uniform_stream);
	}

	if (currentScene == SceneType::_3d && enableGrid) {
		const auto s_guard = glr.state.push_guard();
		glr.state.polygonModeFill(); // In case we are wireframe mode, force poly fill for the grid
		glr.state.disableDepthMask();
		renderer.editorGrid.render(glr, renderer.resource_context.uniform_stream);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
