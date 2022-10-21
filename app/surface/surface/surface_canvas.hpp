// Created by dbobula on 3/25/2022.

#pragma once

// libv
#include <libv/fsw/watcher.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/queue.hpp>
#include <libv/ui/component/canvas.hpp>
// std
#include <map>
#include <mutex>
// pro
#include <surface/surface/lua_binding.hpp>
#include <surface/surface/sprite_mapping.hpp>
#include <surface/surface/surface.hpp>
#include <surface/view/camera_manager.hpp>
#include <surface/view/render/renderer.hpp>


#include <libv/rev/model.hpp>
#include <libv/rev/resource_manager.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

enum class SceneType {
	_3d,

	height,
	temperature,
	humidity,
	fertility,
	biome,

//	distribution,
};

inline SceneType currentScene = SceneType::_3d;
inline SceneType previousScene = currentScene;

inline bool enableWireframe = false;
inline bool enableVegetation = true;
inline bool enableGrid = false;
inline bool enableFog = true;
inline bool enableSkybox = true;

inline bool refresh = false;
inline std::atomic<bool> changed = true;
inline bool freeze = false;
inline bool blendBiomes = true;

inline std::atomic<bool> forceRebake = false;


// -------------------------------------------------------------------------------------------------

struct Scene {
protected:
	Renderer& renderer;

public:
	explicit inline Scene(Renderer& renderer) : renderer(renderer) {}

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) = 0;
	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum) = 0;
	virtual void buildVeggie(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) = 0;

	void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum);

	virtual ~Scene() = default;
};

struct SurfaceScene : Scene {
	using Scene::Scene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override;
	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum) override;
	virtual void buildVeggie(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override;
};

struct TextureScene : Scene {
	using Scene::Scene;

	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum) override;
	virtual void buildVeggie(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override;
};

struct HeightHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override;
};

struct BiomeHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override;
};

struct TemperatureHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override;
};

struct HumidityHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override;
};

struct FertilityHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override;
};

// TODO P5: revive distribution texture
//struct TemperatureHumidityDistributionHeatMap : TextureScene {
//	using TextureScene::TextureScene;
//
//	virtual void build(const std::vector<std::shared_ptr<Chunk>>& chunks) override;
//};

// -------------------------------------------------------------------------------------------------

class SurfaceCanvas : public libv::ui::CanvasBase {
public:
	CameraManager cameraManager;

private:
	std::mutex mutex;
	std::string currentConfigPath_;
	std::string configFolder;

private:
	Renderer renderer;

	SurfaceLuaBinding binding;
	libv::fsw::Watcher fileWatcher;

private:
	std::unique_ptr<Scene> activeScene;
	std::unique_ptr<Surface> surface;

	libv::frustum cameraFrustum;
	bool surfaceDirty = false;
	bool initializedSprites = false;

private:
	std::vector<SpriteMappingEntry> spriteMappings;

//private:
//	libv::rev::Model tree_01 = renderer.resource_context.loader.model.load("tree_01.vm4");
//	libv::rev::Model tree_01_n = renderer.resource_context.loader.model.load("tree_01_normalized_2.vm4");
//	libv::rev::Model tree_02 = renderer.resource_context.loader.model.load("test_sphere.vm4");
//	libv::rev::Model tree_03 = renderer.resource_context.loader.model.load("stone_01.vm4");
//	libv::rev::Model tree_04 = renderer.resource_context.loader.model.load("tree_palm_02.vm4");
//	libv::rev::Model tree_05 = renderer.resource_context.loader.model.load("tree_cactus_a_03.vm4");

public:
	explicit SurfaceCanvas(libv::ui::UI& ui, libv::ctrl::Controls& controls, std::string configPath, std::string configFolder);
	std::string cycleConfig();

private:
	[[nodiscard]] std::unique_ptr<Scene> createScene(SceneType scene);

private:
	virtual void attach() override;
	void setupRenderStates(libv::glr::Queue& glr);
	virtual void render(libv::glr::Queue& glr) override;
	virtual void update(libv::ui::time_duration delta_time) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
