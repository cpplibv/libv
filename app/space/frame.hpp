// Project: libv, File: app/space/frame.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/frame/frame.hpp>
#include <libv/rev/shader_loader.hpp>
#include <libv/ui/event_hub.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/timer.hpp>
// pro
#include <space/camera_behaviour.hpp>
#include <space/canvas_behaviour.hpp>
#include <space/icon_set.hpp>
#include <space/log.hpp>
#include <space/render.hpp>
//#include <space/camera.hpp>
//#include <space/hud.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct SpaceFrame;
// TODO P1: Remove these hacks
inline void event_for_global_test_mode(const libv::Frame& frame, const libv::input::EventKey& e);
inline void event_for_ui_overlay_camera_hack(SpaceFrame& frame, const libv::input::EventKey& e);

// -------------------------------------------------------------------------------------------------

// TODO P1: Controls camera should only be placed into context if the canvas is focused
// TODO P1: Remove the F12 tracking manual workaround too for mode switching (Related to controls bypass)
// TODO P3: Arrow strip placement control from lua (or something lua related) (With auto reload and everything)

// -------------------------------------------------------------------------------------------------

struct SpaceFrame : public libv::Frame {
	libv::ui::UI ui;
	libv::ctrl::Controls controls;
	libv::Timer controls_timer; // <<< use frame loop to call controls

	SpaceFrame() :
			Frame("Space", 1280, 800),
//			Frame("Space", config.window_width, config.window_height),
			ui([] {
				libv::ui::Settings ui_settings;
				// TODO P1: Internalize used UI resources under space, currently: app/space/../../res/
				ui_settings.res_font.base_path = "../../res/font/";
				ui_settings.res_shader.base_path = "../../res/shader/";
				ui_settings.res_texture.base_path = "../../res/texture/";
				return ui_settings;
			}()) {

		setPosition(libv::Frame::FramePosition::center_current_monitor);
		setOpenGLProfile(libv::Frame::OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(libv::Frame::OpenGLSamples{4});
		setOpenGLRefreshRate(libv::Frame::OpenGLRefreshRate{1});
		setIcon(icon_set_iris_cyan());

		onContextUpdate.output([&](const auto&) {
			// shader_manager.update MUST run before any other render queue operation
			// OTHERWISE the not loaded uniform locations are attempted to be used and placed into the streams
			shader_manager.update(ui.gl());

			// TODO P1: Timer management should be smoother with control and frame attachment -> controls.attach(frame)
			controls.update(controls_timer.time());
		});

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

		shader_manager.attach_libv_ui_hub(ui.event_hub());
		ui.attach(*this);
		controls.attach(*this);

		onKey.output([this](const libv::input::EventKey& e) {
			// TODO P1: Remove hacks
			event_for_ui_overlay_camera_hack(*this, e);
			event_for_global_test_mode(*this, e);
		});
	}
};

// =================================================================================================

inline void event_for_global_test_mode(const libv::Frame& frame, const libv::input::EventKey& e) {
	const auto old_global_test_mode = global_test_mode;

	if (e.keycode == libv::input::Keycode::C && e.action != libv::input::Action::release) {
		const int32_t mode_count = 4;
		if (frame.isKeyPressed(libv::input::Keycode::ShiftLeft) || frame.isKeyPressed(libv::input::Keycode::ShiftRight))
			global_test_mode = global_test_mode == 0 ? mode_count - 1 : global_test_mode - 1;
		else
			global_test_mode = (global_test_mode + 1) % mode_count;
	}
	if (e.keycode == libv::input::Keycode::Backtick)
		global_test_mode = 0;
	if (e.keycode == libv::input::Keycode::Num1)
		global_test_mode = 1;
	if (e.keycode == libv::input::Keycode::Num2)
		global_test_mode = 2;
	if (e.keycode == libv::input::Keycode::Num3)
		global_test_mode = 3;

	log_space.info_if(old_global_test_mode != global_test_mode, "Test mode: {}", global_test_mode);
}

inline void event_for_ui_overlay_camera_hack(SpaceFrame& frame, const libv::input::EventKey& e) {
	// TODO P1: Ui focus for controls switch
	//			Hack workaround until ui focus does not operates control contexts
	if (e.keycode == libv::input::Keycode::F12 && e.action == libv::input::Action::press) {
		static int hack_camera_control_ui_mode = 0;
		hack_camera_control_ui_mode = (hack_camera_control_ui_mode + 1) % 3;
		if (hack_camera_control_ui_mode == 1)
			frame.controls.ignore_events(true);
		else
			frame.controls.ignore_events(false);
	}
}

// -------------------------------------------------------------------------------------------------


} // namespace app
