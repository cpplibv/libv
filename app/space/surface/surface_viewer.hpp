// Created by dbobula on 3/25/2022.

#pragma once

//libv
#include <libv/ctrl/controls.hpp>
#include <libv/ui/ui.hpp>

//space
#include <space/view/frame.hpp>



namespace surface {

class SurfaceViewer {
public:
//	libv::rev::RenderTarget renderTarget;
//	libv::rev::PostProcessing postProcessing;
	space::SpaceFrame frame;

	libv::ctrl::Controls controls;

	libv::ui::UI ui;

//	space::Renderer renderer;
//	space::CameraPlayer camera;
//	space::CameraPlayer::screen_picker screen_picker;

public:
	SurfaceViewer();
	void execute();
};

} // namespace surface


