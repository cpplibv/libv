// Project: libv, File: app/surface/surface/surface_canvas.cpp

// hpp
#include <surface/surface/surface_canvas.hpp>
// libv
#include <libv/algo/wildcard.hpp>
#include <libv/glr/queue.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <filesystem>
// pro
#include <surface/log.hpp>

#include <libv/gl/gl.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

inline libv::glr::Texture createTexture(const libv::vector_2D<float>& heatmap) {
	auto texture = libv::glr::Texture2D::R32F();
	texture.storage(1, libv::vec2z{heatmap.size_x(), heatmap.size_x()}.cast<int>());
	texture.set(libv::gl::MagFilter::Nearest);
	texture.set(libv::gl::MinFilter::Nearest);
	texture.set(libv::gl::Swizzle::Red, libv::gl::Swizzle::Red, libv::gl::Swizzle::Red, libv::gl::Swizzle::One);
	texture.image(0, libv::vec2i{0, 0}, heatmap.size().cast<int32_t>(), heatmap.data());
	return texture;
}

inline libv::glr::Texture createTexture(const libv::vector_2D<libv::vec3f>& heatmap) {
	auto texture = libv::glr::Texture2D::RGB32F();
	texture.storage(1, libv::vec2z{heatmap.size_x(), heatmap.size_x()}.cast<int>());
	texture.set(libv::gl::MagFilter::Nearest);
	texture.set(libv::gl::MinFilter::Nearest);
	texture.image(0, libv::vec2i{0, 0}, heatmap.size().cast<int32_t>(), heatmap.data());
	return texture;
}

inline libv::glr::Texture createTexture(const libv::vector_2D<libv::vec4f>& heatmap) {
	auto texture = libv::glr::Texture2D::RGBA32F();
	texture.storage(1, libv::vec2z{heatmap.size_x(), heatmap.size_x()}.cast<int>());
	texture.set(libv::gl::MagFilter::Nearest);
	texture.set(libv::gl::MinFilter::Nearest);
	texture.image(0, libv::vec2i{0, 0}, heatmap.size().cast<int32_t>(), heatmap.data());
	return texture;
}

// -------------------------------------------------------------------------------------------------

void Scene::renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum) {
	renderer.veggie.render(glr, uniform_stream, frustum);
}

// -------------------------------------------------------------------------------------------------

void SurfaceScene::build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) {
	for (const auto& chunk : chunks) {
		renderer.surface.addChunk(generation, chunk);
	}
}

void SurfaceScene::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum) {
	renderer.surface.render(glr, uniform_stream, frustum);
}

void SurfaceScene::buildVeggie(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) {
	for (const auto& chunk : chunks) {
		if (renderer.sprite.chunkGeneration(chunk->index) == generation)
			continue;

		for (const auto& veggie : chunk->veggies)
			renderer.sprite.add(veggie.id, veggie.pos, veggie.normal, veggie.surfaceNormal, veggie.rotation, veggie.scale, veggie.hsv_shift);

		renderer.sprite.commitChunk(generation, chunk->index);
	}
}

// -------------------------------------------------------------------------------------------------

void TextureScene::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum) {
	(void) frustum;
	auto s_guard = glr.state.push_guard();
	glr.state.disableDepthMask();
	glr.state.disableDepthTest();
	renderer.surfaceTexture.render(glr, uniform_stream);
}

void TextureScene::buildVeggie(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) {
	for (const auto& chunk : chunks) {
		if (renderer.sprite.chunkGeneration(chunk->index) == generation)
			continue;

		for (const auto& veggie : chunk->veggies)
			renderer.sprite.add(veggie.id, veggie.pos, veggie.normal, veggie.surfaceNormal, veggie.rotation, veggie.scale, veggie.hsv_shift);

		renderer.sprite.commitChunk(generation, chunk->index);
	}
}

// -------------------------------------------------------------------------------------------------

void HeightHeatMap::build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) {
	(void) generation;
//	rendererTexture.clear();
	for (const auto& chunk : chunks) {
		const auto heatMap = createTexture(chunk->height);
		renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
	}
}

// -------------------------------------------------------------------------------------------------

void BiomeHeatMap::build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) {
	(void) generation;
//	rendererTexture.clear();
	for (const auto& chunk : chunks) {
		const auto heatMap = createTexture(chunk->color);
		renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
	}
}

// -------------------------------------------------------------------------------------------------

void TemperatureHeatMap::build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) {
	(void) generation;
//	rendererTexture.clear();
	for (const auto& chunk : chunks) {
		const auto heatMap = createTexture(chunk->temperature);
		renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
	}
}

// -------------------------------------------------------------------------------------------------

void HumidityHeatMap::build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) {
	(void) generation;
//	rendererTexture.clear();
	for (const auto& chunk : chunks) {
		const auto heatMap = createTexture(chunk->humidity);
		renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
	}
}

// -------------------------------------------------------------------------------------------------

void FertilityHeatMap::build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) {
	(void) generation;
//	rendererTexture.clear();
	for (const auto& chunk : chunks) {
		const auto heatMap = createTexture(chunk->fertility);
		renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
	}
}

// -------------------------------------------------------------------------------------------------

//	void TemperatureHumidityDistributionHeatMap::build(const std::vector<std::shared_ptr<Chunk>>& chunks) {
//		rendererTexture.clear();
//		const auto size = chunk->humidity.size_y() + 1;
//		libv::vector_2D<float> result{size, size};
//		result.fill(0.f);
//
//		for (const auto& chunk : chunks) {
//			for (size_t y = 0; y < chunk->humidity.size_y(); ++y) {
//				for (size_t x = 0; x < chunk->humidity.size_x(); ++x) {
//					result(
//							std::clamp(static_cast<size_t>(std::max(0.f, chunk->humidity(x, y)) * static_cast<float>(config.resolution)), 0uz, result.size_x() - 1),
//							std::clamp(static_cast<size_t>(std::max(0.f, chunk->temperature(x, y)) * static_cast<float>(config.resolution)), 0uz, result.size_y() - 1)
//					) += 0.005f;
//				}
//			}
//		}
//
//		auto texture = libv::glr::Texture2D::R32F();
//		texture.storage(1, result.size().cast<int32_t>());
//		texture.set(libv::gl::MagFilter::Nearest);
//		texture.set(libv::gl::MinFilter::Nearest);
////		texture.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
//		texture.image(0, libv::vec2i{0, 0}, result.size().cast<int32_t>(), result.data());
//
//		renderer.addTexture(texture, {-5.f, -5.f}, {10.f, 10.f});
//	}

// =================================================================================================

SurfaceCanvas::SurfaceCanvas(libv::ui::UI& ui, libv::ctrl::Controls& controls, std::string configPath_, std::string configFolder) :
		cameraManager(controls),
		currentConfigPath_(std::move(configPath_)),
		configFolder(std::move(configFolder)),
		renderer(ui),
		binding(this->configFolder) {

	fileWatcher.subscribe_directory(this->configFolder, [this](const libv::fsw::Event& event) {
		auto lock = std::unique_lock(mutex);
		if (event.path.generic_string() == currentConfigPath_)
			changed = true;
	});

	fileWatcher.subscribe_directory("res/shader/", [this](const libv::fsw::Event& event) {
		if (event.path.filename().generic_string().starts_with("sprite_"))
			forceRebake = true;
	});

	int32_t nextSpriteID = 0;
	spriteMappings.emplace_back(nextSpriteID++, "oak", "tree_01_normalized_2.vm4", 0.2f / 66.85f);
	spriteMappings.emplace_back(nextSpriteID++, "pine", "tree_pine_05.vm4", 2.f);
	spriteMappings.emplace_back(nextSpriteID++, "palm", "tree_palm_02.vm4", 1.0f / 20.f);
	spriteMappings.emplace_back(nextSpriteID++, "rock", "stone_01.vm4", 1.0f / 250.f);
	spriteMappings.emplace_back(nextSpriteID++, "cactus_a", "tree_cactus_a_03.vm4", 1.0f / 10.f);
	spriteMappings.emplace_back(nextSpriteID++, "cactus_b", "tree_cactus_b_04.vm4", 1.0f / 7.f);

	//	renderer.sprite.registerSprite("building_delnan_16P_tex.0038_med.game.vm4", 1.0f / 66.85f);
	//	renderer.sprite.registerSprite("fighter_01_eltanin.vm4", 1.0f / 66.85f);
	//	renderer.sprite.registerSprite("projectile_missile_01_hellfire.0005_med.game.vm4", 1.0f / 66.85f);
	//	renderer.sprite.registerSprite("tank_01_rocket_ring.0031_med.game.vm4", 1.0f / 66.85f);
	//	renderer.sprite.registerSprite("test_sphere.vm4", 5.f / 100.f);
	//	renderer.sprite.registerSprite("test_tree_cone.vm4", 1.0f / 70.0f);
	//	renderer.sprite.registerSprite("tree_01_normalized.vm4", 0.2f / 66.85f);
	//	renderer.sprite.registerSprite("tree_01.vm4", 0.2f / 66.85f);

	surface = std::make_unique<Surface>();
	activeScene = createScene(currentScene);
}

std::string SurfaceCanvas::cycleConfig() {
	auto lock = std::unique_lock(mutex);

	const auto filter_pattern = "**.lua";

	bool next = false;
	bool first = true;
	std::string firstConfig;
	for (const auto& entry : std::filesystem::directory_iterator(configFolder)) {
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
	focus(libv::ui::FocusMode::active);
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
	libv::Timer timer;

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
		auto conf = binding.getConfigFromLuaScript(script, spriteMappings);

		conf->blendBiomes = blendBiomes;

		renderer.sky.fogIntensity = conf->fogIntensity;
		renderer.sky.fogColor = conf->fogColor;
//		renderer.sky.sunColor = conf->sunColor;
//		renderer.sky.sunDirection = conf->sunDirection;
		renderer.sprite.fogIntensity = conf->fogIntensity;
		renderer.sprite.fogColor = conf->fogColor;
		renderer.sprite.sunColor = conf->sunColor;
		renderer.sprite.sunDirection = conf->sunDirection;
		renderer.surface.fogIntensity = conf->fogIntensity;
		renderer.surface.fogColor = conf->fogColor;
		renderer.surface.sunColor = conf->sunColor;
		renderer.surface.sunDirection = conf->sunDirection;
		renderer.veggie.fogIntensity = conf->fogIntensity;
		renderer.veggie.fogColor = conf->fogColor;
//		renderer.veggie.sunColor = conf->sunColor;
//		renderer.veggie.sunDirection = conf->sunDirection;

		renderer.sky.texture = renderer.resource_context.loader.texture.load(conf->skyboxTexture);

		surface->gen(std::move(conf));
	}

	renderer.sky.fogEnabled = enableFog;
	renderer.sprite.fogEnabled = enableFog;
	renderer.surface.fogEnabled = enableFog;
	renderer.veggie.fogEnabled = enableFog;

	surfaceDirty = surface->update(cameraManager.position(), cameraManager.forward());

//	auto t0 = timer.timef_ms().count();
//	log_surface.trace_if(t0 > 0.01f, "{:25}:{:8.4f} ms", "Canvas update took SUM", t0);
}

void SurfaceCanvas::render(libv::glr::Queue& glr) {
//	libv::Timer timerSum;
//	libv::Timer timer;
//	log_surface.trace("Start render");
//	const auto time = [&timer](std::string_view description) {
//		auto t0 = timer.timef_ms().count();
//		log_surface.trace_if(t0 > 0.01f, "{:25}:{:8.4f} ms", description, t0);
//		timer.reset();
//	};

	setupRenderStates(glr);
//	time("SetupRenderStates");

	const auto s_guard = glr.state.push_guard();

	if (previousScene != currentScene) {
		previousScene = currentScene;
		activeScene = createScene(currentScene);
		//build mesh
		// <<< build / buildVeggie called twice? build mesh happens in here and in surfaceDirty too
		activeScene->build(surface->getGeneration(), surface->getActiveChunks());
		activeScene->buildVeggie(surface->getGeneration(), surface->getActiveChunks());
	}
//	time("Scene Change");

	if (surfaceDirty) {
		if (!std::exchange(initializedSprites, true)) {

			for (const auto& spriteMapping : spriteMappings) {
				const auto id = renderer.sprite.registerSprite(spriteMapping.path, spriteMapping.scale);
				assert(id == spriteMapping.id);
				(void) id;
			}

			renderer.sprite.bakeSprites(renderer.resource_context.loader, glr, renderer.resource_context.uniform_stream);
		}

		//build mesh
		activeScene->build(surface->getGeneration(), surface->getActiveChunks());
//		time("surfaceDirty build");
		activeScene->buildVeggie(surface->getGeneration(), surface->getActiveChunks());
//		time("surfaceDirty buildVeggie");

	} else if (forceRebake.exchange(false)) {
		renderer.sprite.bakeSprites(renderer.resource_context.loader, glr, renderer.resource_context.uniform_stream);
//		time("forceRebake");
	}

	// render surface texture/_3d
	activeScene->render(glr, renderer.resource_context.uniform_stream, cameraFrustum);
//	time("activeScene->render");

	if (currentScene == SceneType::_3d && enableSkybox) {
		renderer.sky.render(glr, renderer.resource_context.uniform_stream);
//		time("renderer.sky.render");
	}

	// render plant model/debug
	if (enableVegetation) {
		activeScene->renderVeggie(glr, renderer.resource_context.uniform_stream, cameraFrustum);
		renderer.sprite.render(glr, renderer.resource_context.uniform_stream, cameraFrustum);
//		time("renderer.sprite.render");
	}

//	{
//		const auto s2_guard = glr.state.push_guard();
//		glr.state.disableCullFace();
//
//		for (int i = 0; i < 10; ++i) {
//			auto m_guard = glr.model.push_guard();
//			glr.model.translate(-1.f, 1.0f * static_cast<float>(i), 0.f);
//			glr.model.scale(1.f / 66.85f);
//			tree_01.render(glr, renderer.resource_context.uniform_stream);
//		}
//		for (int i = 0; i < 10; ++i) {
//			auto m_guard = glr.model.push_guard();
//			glr.model.translate(2.f, 1.0f * static_cast<float>(i), 0.f);
//			glr.model.scale(5.f * 5.f / 100.f);
//			tree_02.render(glr, renderer.resource_context.uniform_stream);
//		}
//		for (int i = 0; i < 10; ++i) {
//			auto m_guard = glr.model.push_guard();
//			glr.model.translate(5.f, 1.0f * static_cast<float>(i), 0.f);
//			glr.model.scale(1.f / 66.85f);
//			tree_01_n.render(glr, renderer.resource_context.uniform_stream);
//		}
//		for (int i = 0; i < 10; ++i) {
//			auto m_guard = glr.model.push_guard();
//			glr.model.translate(8.f, 1.0f * static_cast<float>(i), 0.f);
//			glr.model.scale(5.f * 1.0f / 250.f);
//			tree_03.render(glr, renderer.resource_context.uniform_stream);
//		}
//		for (int i = 0; i < 10; ++i) {
//			auto m_guard = glr.model.push_guard();
//			glr.model.translate(11.f, 1.0f * static_cast<float>(i), 0.f);
//			glr.model.scale(5.f * 1.0f / 20.f);
//			tree_04.render(glr, renderer.resource_context.uniform_stream);
//		}
//		for (int i = 0; i < 10; ++i) {
//			auto m_guard = glr.model.push_guard();
//			glr.model.translate(14.f, 1.0f * static_cast<float>(i), 0.f);
//			glr.model.scale(5.f * 1.0f / 10.f);
//			tree_05.render(glr, renderer.resource_context.uniform_stream);
//		}
//	}
//	time("Test models render");

	if (currentScene == SceneType::_3d && enableGrid) {
		const auto s_guard = glr.state.push_guard();
		glr.state.polygonModeFill(); // In case we are wireframe mode, force poly fill for the grid
		glr.state.disableDepthMask();
		renderer.editorGrid.render(glr, renderer.resource_context.uniform_stream);
//		time("renderer.editorGrid.render");
	}

//	auto tSum = timerSum.timef_ms().count();
//	log_surface.trace_if(tSum > 0.01f, "{:25}:{:8.4f} ms", "Canvas render took SUM", tSum);
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
