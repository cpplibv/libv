// Project: libv, File: app/space/view/frame.hpp

#pragma once

// libv
#include <libv/frame/frame.hpp>
//#include <libv/frame/monitor.hpp>
// pro
#include <space/view/icon_set.hpp>
//#include <space/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct SpaceFrame : public libv::Frame {
	SpaceFrame() :
		Frame("Space", 1280, 800) {

		setPosition(libv::Frame::FramePosition::center_current_monitor);
		setOpenGLProfile(libv::Frame::OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(libv::Frame::OpenGLSamples{4});
		setOpenGLRefreshRate(libv::Frame::OpenGLRefreshRate{1});
		setIcon(icon_set_iris_cyan());

		onKey.output([this](const libv::input::EventKey& e) {
			if (e.keycode == libv::input::Keycode::Escape)
				closeForce();
		});

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

	~SpaceFrame() {
		disconnectAll();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
