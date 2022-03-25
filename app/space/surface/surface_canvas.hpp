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

class SurfaceCanvas : public libv::ui::CanvasBase {
public:
	space::CameraPlayer camera;
//	CameraPlayer::screen_picker screen_picker;

private:
//	libv::rev::RenderTarget renderTarget;
//	libv::rev::PostProcessing postProcessing;
	space::Renderer renderer;
	libv::glr::Texture2D::RGBA32F heightMap;
	std::vector <Chunk> chunks;
//	std::mutex mutex;
	std::atomic<bool> changed = true;
	SurfaceLuaBinding binding;
	Config config;
	libv::fsw::Watcher fileWatcher;
public:
	explicit SurfaceCanvas(libv::ui::UI& ui);
private:
	void setupRenderStates(libv::glr::Queue& glr);
	virtual void attach() override;
	virtual void render(libv::glr::Queue& glr) override;
};
}
