// Created by dbobula on 3/25/2022.

#pragma once

//libv
#include <libv/ui/component/canvas.hpp>
#include <libv/glr/texture.hpp>
#include <libv/fsw/watcher.hpp>

//space
#include <surface/view/camera.hpp>
#include <surface/view/render/renderer.hpp>

//surface
#include <surface/surface/lua_binding.hpp>


namespace surface {
inline bool isPolygonFill = true;
inline bool withVegetation = true;
inline bool is3DCamera = true;
inline bool isCameraChanged = true;
inline bool hasSceneChanged = true;

enum class SceneType {
	_3d,
	height,
	temperature,
	humidity,
	fertility,
	biome,

	distribution,
};

inline SceneType currentHeatMap = SceneType::height;

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
