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
		currentConfigPath_(std::move(configPath_)),
		renderer(ui) {

	fileWatcher.subscribe_directory("config/", [this](const libv::fsw::Event& event) {
		auto lock = std::unique_lock(mutex);
		if (event.path.generic_string() == currentConfigPath_)
			changed = true;
	});

	fileWatcher.subscribe_directory("../../res/shader/", [this](const libv::fsw::Event& event) {
		if (event.path.filename().generic_string().starts_with("sprite_"))
			forceRebake = true;
	});

	surface = std::make_unique<Surface>();
	activeScene = createScene(currentScene);
}

std::string SurfaceCanvas::cycleConfig() {
	auto lock = std::unique_lock(mutex);

	const auto filter_pattern = "**.lua";

	bool next = false;
	bool first = true;
	std::string firstConfig;
	const auto dir = "config/";
	for (const auto& entry : std::filesystem::directory_iterator(dir)) {
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
			currentConfigPath_ = filepath;
			return currentConfigPath_;
		}

		if (filepath == currentConfigPath_) {
			next = true;
		}
	}

	assert(not first && "Given directory is empty of lua config files");
	currentConfigPath_ = firstConfig;
	return currentConfigPath_;
}

std::unique_ptr<Scene> SurfaceCanvas::createScene(SceneType scene) {
	switch (scene) {
	case SceneType::_3d:
		return std::make_unique<SurfaceScene>(renderer);
	case SceneType::height:
		return std::make_unique<HeightHeatMap>(renderer);
	case SceneType::temperature:
		return std::make_unique<TemperatureHeatMap>(renderer);
	case SceneType::humidity:
		return std::make_unique<HumidityHeatMap>(renderer);
	case SceneType::fertility:
		return std::make_unique<FertilityHeatMap>(renderer);
	case SceneType::biome:
		return std::make_unique<BiomeHeatMap>(renderer);
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

//void SurfaceCanvas::updateVisibleChunks() {
//	visibleChunks.clear();
//	const auto& frustum = cameraManager.getCameraFrustum(canvas_size);
//	for (const auto& chunk : surface->getChunks()) {
//		//Red check: throw away
//		//Green check: generate and render
//		const auto pos = libv::vec3f(chunk->position, 0.f);
//		if (frustum.sphereInFrustum(pos, chunk->size.length() / 2.f) != Frustum::Position::OUTSIDE) {
//			visibleChunks.emplace_back(chunk->index);
//		}
//		//Yellow check: lowprio generate, build but not render
//	}
//}

void SurfaceCanvas::update(libv::ui::time_duration delta_time) {
	(void) delta_time;

	cameraManager.update();

	if (!freeze)
		cameraFrustum = cameraManager.getCameraFrustum(canvas_size);

	if (refresh) {
		refresh = false;
		renderer.veggie.clear();
		renderer.surface.clear();

		activeScene = createScene(currentScene);

		changed = true;
	}

	if (changed) {
		changed = false;
		auto lock = std::unique_lock(mutex); // To guard the currentConfigPath read
		auto script = libv::read_file_str_or_throw(currentConfigPath_);
		auto conf = binding.getConfigFromLuaScript(script);

		conf->blendBiomes = blendBiomes;

		renderer.sprite.clear();

		renderer.sky.fogIntensity = conf->fogIntensity;
		renderer.sky.fogColor = conf->fogColor;
		renderer.sprite.fogIntensity = conf->fogIntensity;
		renderer.sprite.fogColor = conf->fogColor;
		renderer.surface.fogIntensity = conf->fogIntensity;
		renderer.surface.fogColor = conf->fogColor;
		renderer.veggie.fogIntensity = conf->fogIntensity;
		renderer.veggie.fogColor = conf->fogColor;

		surface->gen(std::move(conf));
	}

	renderer.sky.fogEnabled = enableFog;
	renderer.sprite.fogEnabled = enableFog;
	renderer.surface.fogEnabled = enableFog;
	renderer.veggie.fogEnabled = enableFog;

	surfaceDirty = surface->update();
}

void SurfaceCanvas::render(libv::glr::Queue& glr) {
	setupRenderStates(glr);
	const auto s_guard = glr.state.push_guard();

	if (previousScene != currentScene) {
		previousScene = currentScene;
		activeScene = createScene(currentScene);
		//build mesh
		activeScene->build(surface->getGeneration(), surface->getChunks());
		activeScene->buildVeggie(surface->getGeneration(), surface->getChunks());
	}
//	if (previousScene != currentScene) {
//		previousScene = currentScene;
//		activeScene = createScene(currentScene);
//	}

	if (surfaceDirty) {
		if (!std::exchange(initializedSprites, true)) {
			const auto treeID = renderer.sprite.registerSprite("tree_01.vm4", 0.2f / 66.85f);
			const auto stoneID = renderer.sprite.registerSprite("stone_01.vm4", 1.0f / 250.f);
//			renderer.sprite.registerSprite("fighter_01_eltanin.0006_med.fixed.game.vm4", 1.0f / 66.85f);
//			renderer.sprite.registerSprite("projectile_missile_01_hellfire.0005_med.game.vm4", 1.0f / 66.85f);
//			renderer.sprite.registerSprite("tank_01_rocket_ring.0031_med.game.vm4", 1.0f / 66.85f);
//			renderer.sprite.registerSprite("building_delnan_16P_tex.0038_med.game.vm4", 1.0f / 66.85f);
			renderer.sprite.bakeSprites(renderer.resource_context.loader, glr, renderer.resource_context.uniform_stream);

//			for (int x = 0; x < 10; ++x) {
//				for (int y = 0; y < 10; ++y) {
//					const auto xf = static_cast<float>(x);
//					const auto yf = static_cast<float>(y);
//					renderer.sprite.add(x % 2 == 0, {xf, yf, 0.f});
////					renderer.sprite.add(0, {xf, yf, 0.f}, hsv_shift, normal, rotation);
//				}
//			}
		}

		//build mesh
		activeScene->build(surface->getGeneration(), surface->getChunks());
		activeScene->buildVeggie(surface->getGeneration(), surface->getChunks());

	} else if (forceRebake.exchange(false))
		renderer.sprite.bakeSprites(renderer.resource_context.loader, glr, renderer.resource_context.uniform_stream);

	//render surface texture/_3d
	activeScene->render(glr, renderer.resource_context.uniform_stream, cameraFrustum);

	if (currentScene == SceneType::_3d && enableSkybox)
		renderer.sky.render(glr, renderer.resource_context.uniform_stream);

	// render plant model/debug
	if (enableVegetation) {
		activeScene->renderVeggie(glr, renderer.resource_context.uniform_stream, cameraFrustum);
		renderer.sprite.render(glr, renderer.resource_context.uniform_stream);
	}

	{
		for (int i = 0; i < 10; ++i) {
			auto m_guard = glr.model.push_guard();
			glr.model.translate(-1.f, 1.0f * static_cast<float>(i), 0.f);
			glr.model.scale(1.f / 66.85f);
			tree_01.render(glr, renderer.resource_context.uniform_stream);
		}
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
