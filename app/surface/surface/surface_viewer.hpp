// Created by dbobula on 3/25/2022.

#pragma once

// libv
#include <libv/ctrl/controls.hpp>
#include <libv/ui/component/fwd.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/ui.hpp>
// pro
#include <surface/view/frame.hpp>
#include <surface/surface/surface_canvas.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

class SurfaceViewer {
public:
//	libv::rev::RenderTarget renderTarget;
//	libv::rev::PostProcessing postProcessing;
	surface::SurfaceFrame frame;
	libv::ctrl::Controls controls;
	libv::ui::UI ui;

	libv::ui::CanvasAdaptorT<SurfaceCanvas> canvas;

	libv::ui::PanelAnchor mainLayers;
	libv::ui::Label sceneTitle;
	libv::ui::Label configName;

//	surface::Renderer renderer;
//	surface::CameraPlayer camera;
//	surface::CameraPlayer::screen_picker screen_picker;

public:
	explicit SurfaceViewer(const std::string& configPath, const std::string& configFolder);

private:
	void initUI(const std::string& configPath);

public:
	void execute();
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
