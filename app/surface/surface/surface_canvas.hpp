// Created by dbobula on 3/25/2022.

#pragma once

// libv
#include <libv/fsw/watcher.hpp>
#include <libv/glr/texture.hpp>
#include <libv/ui/component/canvas.hpp>
// pro
#include <surface/surface/lua_binding.hpp>
#include <surface/view/camera.hpp>
#include <surface/view/render/renderer.hpp>
#include <surface/view/render/renderer_surface.hpp>
#include <surface/view/render/renderer_surface_texture.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

enum class CameraMode {
	_2d,
	_3d,
};

inline CameraMode currentCameraMode = CameraMode::_3d;
inline CameraMode previousCameraMode = currentCameraMode;

enum class SceneType {
	_3d,

	height,
	temperature,
	humidity,
	fertility,
	biome,

//	distribution,
};

inline SceneType currentHeatMap = SceneType::_3d;
inline SceneType previousHeatMap = currentHeatMap;

//inline bool hasSceneChanged = true;

inline bool enableWireframe = false;
inline bool enableVegetation = true;
inline bool enableGrid = true;

// -------------------------------------------------------------------------------------------------

struct Scene {
protected:
//	libv::glr::UniformBuffer& uniform_stream;
//	libv::glr::Queue& glr;
	Renderer& renderer;

public:
	explicit Scene(Renderer& renderer) : renderer(renderer) {}


	virtual void build(const std::vector<Chunk>& chunks) = 0;
	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) = 0;
	virtual void buildVeggie(const std::vector<Chunk>& chunks) = 0;

//	virtual void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) = 0;
	void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
		renderer.debug.render(glr, uniform_stream);
	}

	virtual ~Scene() = default;
};

struct SurfaceScene : Scene {
	RendererSurface rendererSurface;

	SurfaceScene(Renderer& renderer, RendererResourceContext& rctx) : Scene(renderer), rendererSurface(rctx) {
	}

	virtual void build(const std::vector<Chunk>& chunks) override {
		for (const auto& chunk : chunks) {
			rendererSurface.addChunk(chunk);
		}
	}

	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override {
		rendererSurface.render(glr, uniform_stream);
	}

	virtual void buildVeggie(const std::vector<Chunk>& chunks) override {
		for (const auto& chunk : chunks)
			for (const auto& veggie : chunk.veggies)
				renderer.debug.add_debug_sphere(veggie.pos, veggie.size, veggie.color, 10, 10);
	}

//	virtual void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override {
//		renderer.debug.render(glr, uniform_stream);
//	}
};

inline libv::glr::Texture createTexture(const libv::vector_2D<float>& heatmap) {
	auto texturef = libv::glr::Texture2D::R32F();
	texturef.storage(1, libv::vec2z{heatmap.size_x(), heatmap.size_x()}.cast<int>());
	texturef.set(libv::gl::MagFilter::Nearest);
	texturef.set(libv::gl::MinFilter::Nearest);
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
	RendererSurfaceTexture rendererTexture;

	explicit TextureScene(Renderer& renderer, RendererResourceContext& rctx) : Scene(renderer), rendererTexture(rctx) {
	}

	virtual void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override {
		rendererTexture.render(glr, uniform_stream);
	}

	virtual void buildVeggie(const std::vector<Chunk>& chunks) override {
		for (const auto& chunk : chunks)
			for (const auto& veggie : chunk.veggies)
				renderer.debug.add_debug_sphere(libv::vec3f{xy(veggie.pos), -2.5f}, veggie.size, veggie.color, 10, 10);
	}

//	virtual void renderVeggie(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override {
//		renderer.debug.render(glr, uniform_stream);
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

	virtual void build(const std::vector<Chunk>& chunks) override {
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(getColors(chunk.height));
			rendererTexture.addTexture(heatMap, chunk.position, chunk.size);
		}
	}
};

struct BiomeHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(const std::vector<Chunk>& chunks) override {
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(chunk.biomeMap);
			rendererTexture.addTexture(heatMap, chunk.position, chunk.size);
		}
	}
};

struct TemperatureHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(const std::vector<Chunk>& chunks) override {
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(chunk.temperature);
			rendererTexture.addTexture(heatMap, chunk.position, chunk.size);
		}
	}
};

struct HumidityHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(const std::vector<Chunk>& chunks) override {
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(chunk.humidity);
			rendererTexture.addTexture(heatMap, chunk.position, chunk.size);
		}
	}
};

struct FertilityHeatMap : TextureScene {
	using TextureScene::TextureScene;

	virtual void build(const std::vector<Chunk>& chunks) override {
		for (const auto& chunk : chunks) {
			const auto heatMap = createTexture(chunk.fertility);
			rendererTexture.addTexture(heatMap, chunk.position, chunk.size);
		}
	}
};


//TODO: revive distribution texture
//struct TemperatureHumidityDistributionHeatMap : TextureScene {
//
//	virtual void build(const std::vector<Chunk>& chunks) override {
//		const auto size = chunk.humidity.size_y() + 1;
//		libv::vector_2D<float> result{size, size};
//		result.fill(0.f);
//
//		for (const auto& chunk : chunks) {
//			for (size_t y = 0; y < chunk.humidity.size_y(); ++y) {
//				for (size_t x = 0; x < chunk.humidity.size_x(); ++x) {
//					result(
//							std::clamp(static_cast<size_t>(std::max(0.f, chunk.humidity(x, y)) * static_cast<float>(config.resolution)), 0uz, result.size_x() - 1),
//							std::clamp(static_cast<size_t>(std::max(0.f, chunk.temperature(x, y)) * static_cast<float>(config.resolution)), 0uz, result.size_y() - 1)
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
	//scene
	surface::CameraPlayer camera3D;
	surface::CameraOrtho camera2D;
//	CameraPlayer::screen_picker screen_picker;

private:
//	libv::rev::RenderTarget renderTarget;
//	libv::rev::PostProcessing postProcessing;
//	std::vector<libv::glr::Texture2D::RGBA32F> heightMapTextures;
//	std::mutex mutex;
	//canvas/rendering/surface
	surface::Renderer renderer;
	//surface
	std::vector<Chunk> chunks;
	Config config;
	std::unique_ptr<Scene> currentScene;
	//vezerles
	std::atomic<bool> changed = true;
	SurfaceLuaBinding binding;
	libv::fsw::Watcher fileWatcher;
public:
	explicit SurfaceCanvas(libv::ui::UI& ui);
private:
//	libv::vector_2D<float> buildSurfaceTexture();
//	libv::glr::Texture buildTexture(const Chunk& chunk);
	void addGizmo();
	//hate member functions working on members
	void buildChunks();
//	void buildRenderObject(const Chunk& chunk);
//	void buildRenderObjects();
//	void clearRenderObjects();
	virtual void attach() override;
	void setupRenderStates(libv::glr::Queue& glr);
	virtual void render(libv::glr::Queue& glr) override;

//	void render(Scene scene, VeggieVisualization visualization);
//	void render(SurfaceRenderer renderer, VeggieRenderer veggieRenderer);
// scene.render()
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
