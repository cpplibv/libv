// Created by dbobula on 3/25/2022.

#pragma once

//libv
#include <libv/ui/component/canvas.hpp>
#include <libv/glr/texture.hpp>
#include <libv/fsw/watcher.hpp>

//space
#include <space/view/camera.hpp>
#include <space/view/render/renderer.hpp>

//surface
#include <space/surface/lua_binding.hpp>


namespace surface {
inline bool isPolygonFill = true;
inline bool is3DCamera = true;
inline bool isCameraChanged = true;
inline bool isTextureChanged = true;

enum class HeatMapType {
	height,
	temperature,
	humidity,
	fertility,
	biome,

	distribution,
};

inline HeatMapType currentHeatMap = HeatMapType::height;

class SurfaceCanvas : public libv::ui::CanvasBase {
public:
	//scene
	space::CameraPlayer camera3D;
	space::CameraOrtho camera2D;
//	CameraPlayer::screen_picker screen_picker;

private:
//	libv::rev::RenderTarget renderTarget;
//	libv::rev::PostProcessing postProcessing;
//	std::vector<libv::glr::Texture2D::RGBA32F> heightMapTextures;
//	std::mutex mutex;
	//canvas/rendering/surface
	space::Renderer renderer;
	//surface
	std::vector<Chunk> chunks;
	Config config;
	//vezerles
	std::atomic<bool> changed = true;
	SurfaceLuaBinding binding;
	libv::fsw::Watcher fileWatcher;
public:
	explicit SurfaceCanvas(libv::ui::UI& ui);
private:
	libv::vector_2D<float> buildSurfaceTexture();
	libv::glr::Texture buildTexture(const Chunk& chunk);
	void addGizmo();
	//hate member functions working on members
	void buildChunks();
	void buildRenderObject(const Chunk& chunk);
	void buildRenderObjects();
	void clearRenderObjects();
	virtual void attach() override;
	void setupRenderStates(libv::glr::Queue& glr);
	virtual void render(libv::glr::Queue& glr) override;
};
}
