// Created by dbobula on 3/25/2022.

#pragma once

// libv
#include <libv/fsw/watcher.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/queue.hpp>
#include <libv/ui/component/canvas.hpp>
// std
#include <mutex>
// pro
#include <surface/surface/lua_binding.hpp>
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
//	libv::glr::UniformBuffer& uniform_stream;
//	libv::glr::Queue& glr;
	Renderer& renderer;

public:
	explicit Scene(Renderer& renderer) : renderer(renderer) {}

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) = 0;
	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Frustum& frustum) = 0;
//	virtual void clear() = 0;
//	virtual void clear(size_t count) = 0;
	virtual void buildVeggie(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) = 0;

//	virtual void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) = 0;
	void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Frustum& frustum) {
		renderer.veggie.render(glr, uniform_stream, frustum);
	}

	virtual ~Scene() = default;
};

struct SurfaceScene : Scene {
	using Scene::Scene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override {
//		renderer.surface.clear();
		for (const auto& chunk : chunks) {
			renderer.surface.addChunk(generation, chunk);
		}
	}

	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Frustum& frustum) override {
		renderer.surface.render(glr, uniform_stream, frustum);
	}

	virtual void buildVeggie(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override {
		int type = 0;
		for (const auto& chunk : chunks)
			for (const auto& veggie : chunk->veggies) {
				type = (type + 1) % 2;
				renderer.sprite.add(type, veggie.pos, veggie.normal, veggie.rotation, veggie.scale, veggie.hsv_color_shift);
//				renderer.sprite.add(0, veggie.pos, veggie.normal, veggie.rotation, veggie.scale, veggie.hsv_color_shift);
			}
	}

//	virtual void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override {
//		rendererVeggie.render(glr, uniform_stream, frustum);
//	}
};

inline libv::glr::Texture createTexture(const libv::vector_2D<float>& heatmap) {
	auto texturef = libv::glr::Texture2D::R32F();
	texturef.storage(1, libv::vec2z{heatmap.size_x(), heatmap.size_x()}.cast<int>());
	texturef.set(libv::gl::MagFilter::Nearest);
	texturef.set(libv::gl::MinFilter::Nearest);
	texturef.set(libv::gl::Swizzle::Red, libv::gl::Swizzle::Red, libv::gl::Swizzle::Red, libv::gl::Swizzle::One);
	texturef.image(0, libv::vec2i{0, 0}, heatmap.size().cast<int32_t>(), heatmap.data());
	return texturef;
}

inline libv::glr::Texture createTexture(const libv::vector_2D<libv::vec4f>& heatmap) {
	auto texturef = libv::glr::Texture2D::RGBA32F();
	texturef.storage(1, libv::vec2z{heatmap.size_x(), heatmap.size_x()}.cast<int>());
	texturef.set(libv::gl::MagFilter::Nearest);
	texturef.set(libv::gl::MinFilter::Nearest);
	texturef.image(0, libv::vec2i{0, 0}, heatmap.size().cast<int32_t>(), heatmap.data());
	return texturef;
}

struct TextureScene : Scene {
	using Scene::Scene;

	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Frustum& frustum) override {
		(void) frustum;
		auto s_guard = glr.state.push_guard();
		glr.state.disableDepthMask();
		glr.state.disableDepthTest();
		renderer.surfaceTexture.render(glr, uniform_stream);
	}

	virtual void buildVeggie(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override {
		for (const auto& chunk : chunks)
			for (const auto& veggie : chunk->veggies) {
				renderer.sprite.add(0, veggie.pos, veggie.normal, veggie.rotation, veggie.scale, veggie.hsv_color_shift);
			}
	}

//	virtual void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override {
//		rendererVeggie.render(glr, uniform_stream);
//	}

};

//inline libv::vector_2D<float> getFloats(const libv::vector_2D<SurfacePoint>& points_) {
//	libv::vector_2D<float> heatmap{points_.size()};
////	colors.reserve(points_.size_x() * points_.size_y());
//	for (size_t y = 0; y < points_.size_y(); ++y)
//		for (size_t x = 0; x < points_.size_x(); ++x)
//			heatmap(x, y) = points_(x, y).color.z;
//	return heatmap;
//}

inline libv::vector_2D<libv::vec4f> getColors(const libv::vector_2D<SurfacePoint>& points_) {
	libv::vector_2D<libv::vec4f> heatmap{points_.size()};
//	colors.reserve(points_.size_x() * points_.size_y());
	for (size_t y = 0; y < points_.size_y(); ++y)
		for (size_t x = 0; x < points_.size_x(); ++x)
			heatmap(x, y) = points_(x, y).color;
	return heatmap;
}

struct HeightHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override {
		(void) generation;
//		rendererTexture.clear();
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(getColors(chunk->height));
			renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
		}
	}
};

struct BiomeHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override {
		(void) generation;
//		rendererTexture.clear();
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(chunk->biomeMap);
			renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
		}
	}
};

struct TemperatureHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override {
		(void) generation;
//		rendererTexture.clear();
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(chunk->temperature);
			renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
		}
	}
};

struct HumidityHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override {
		(void) generation;
//		rendererTexture.clear();
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(chunk->humidity);
			renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
		}
	}
};

struct FertilityHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(int generation, const std::vector<std::shared_ptr<Chunk>>& chunks) override {
		(void) generation;
//		rendererTexture.clear();
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(chunk->fertility);
			renderer.surfaceTexture.addTexture(heatMap, chunk->index, chunk->position, chunk->size);
		}
	}
};


//TODO: revive distribution texture
//struct TemperatureHumidityDistributionHeatMap : TextureScene {
//
//	virtual void build(const std::vector<std::shared_ptr<Chunk>>& chunks) override {
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
//		auto texturef = libv::glr::Texture2D::R32F();
//		texturef.storage(1, result.size().cast<int32_t>());
//		texturef.set(libv::gl::MagFilter::Nearest);
//		texturef.set(libv::gl::MinFilter::Nearest);
////		texturef.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
//		texturef.image(0, libv::vec2i{0, 0}, result.size().cast<int32_t>(), result.data());
//
//		renderer.addTexture(texturef, {-5.f, -5.f}, {10.f, 10.f});
//	}
//};

class SurfaceCanvas : public libv::ui::CanvasBase {
public:
	CameraManager cameraManager;

private:
	std::mutex mutex;
	std::string currentConfigPath_;

private:
	surface::Renderer renderer;

	SurfaceLuaBinding binding;
//	std::string configPaths;
	libv::fsw::Watcher fileWatcher;

private:
	std::unique_ptr<Scene> activeScene;
	std::unique_ptr<Surface> surface;
//	std::vector<libv::vec2i> visibleChunks;
	Frustum cameraFrustum;
//	std::vector<Surface> surface;
	bool surfaceDirty = false;
	bool initializedSprites = false;

	libv::rev::Model tree_01 = renderer.resource_context.loader.model.load("tree_01.vm4");

public:
	explicit SurfaceCanvas(libv::ui::UI& ui, libv::ctrl::Controls& controls, std::string configPath);
	std::string cycleConfig();

private:
	[[nodiscard]] std::unique_ptr<Scene> createScene(SceneType scene);

private:
	virtual void attach() override;
	void setupRenderStates(libv::glr::Queue& glr);
//	void updateVisibleChunks();
//	void setConfig();
	virtual void render(libv::glr::Queue& glr) override;
	virtual void update(libv::ui::time_duration delta_time) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
