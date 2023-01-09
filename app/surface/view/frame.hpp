// Project: libv, File: app/surface/view/frame.hpp

#pragma once

// libv
#include <libv/frame/frame.hpp>
//#include <libv/frame/monitor.hpp>
// pro
#include <libv/frame/icon_set.hpp>
//#include <surface/log.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct SurfaceFrame : public libv::Frame {
	SurfaceFrame() :
//		Frame("Surface", 1280, 800) {
		Frame("Surface", 1680, 1050) {

		setPosition(libv::Frame::FramePosition::center_current_monitor);
		setOpenGLProfile(libv::Frame::OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(libv::Frame::OpenGLSamples{4});
		setOpenGLRefreshRate(libv::Frame::OpenGLRefreshRate{1});
		setIcon(libv::frame::icon_set_iris_cyan());
//		setSwapInterval(0);

		onKey.output([this](const libv::input::EventKey& e) {
			if (e.keycode == libv::input::Keycode::Escape)
				closeForce();
//			if (e.keycode == libv::input::Keycode::KPAsterisk)
//				setSwapInterval(-1);
//			if (e.keycode == libv::input::Keycode::KPPlus)
//				setSwapInterval(0);
//			if (e.keycode == libv::input::Keycode::KPMinus)
//				setSwapInterval(1);
//			if (e.keycode == libv::input::Keycode::KPEnter)
//				setSwapInterval(2);
		});

		// TODO P1: Fix bug where setDisplayMode(DisplayMode::fullscreen_windowed) fails with assert (incorrect matrix)
		//		setDisplayMode(DisplayMode::fullscreen_windowed);

//		onSize.output([&](const libv::frame::EventSize& e) {
//			if (config.save_window_size) {
//				config.window_width = e.size.x;
//				config.window_height = e.size.y;
//			}
//		});

//		int frame_index = 0;
//		int frame_total = 6;
//		int num_row = 4; // TODO P5: math to keep good ratio based on total;
//		getCurrentMonitor().workAreaPosition;
//		const auto workAreaSize = workAreaSize;
//		setPosition(...)
//		setSize(...
//				workAreaSize.x / num_row,
//				workAreaSize.y / (frame_total / num_row
//		);
	}

	~SurfaceFrame() {
		disconnectAll();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
