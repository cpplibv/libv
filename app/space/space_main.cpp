// Project: libv, File: app/space/space_main.cpp, Author: Császár Mátyás [Vader]

// ext
#include <fmt/chrono.h>
//#include <fmt/format.h>
//#include <clip/clip.h>
// libv
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/frame/frame.hpp>
#include <libv/math/distance/intersect.hpp>
#include <libv/rev/shader.hpp>
#include <libv/rev/shader_loader.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/event_hub.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/timer.hpp>
//#include <libv/glr/layout_to_string.hpp>
//#include <libv/lua/lua.hpp>
//#include <libv/math/ray.hpp>
//#include <libv/utility/hex_dump.hpp>

// std
#include <iostream>
// pro
#include <space/camera.hpp>
#include <space/camera_behaviour.hpp>
#include <space/command.hpp>
#include <space/hud.hpp>
#include <space/icon_set.hpp>
#include <space/log.hpp>
#include <space/make_shader_error_overlay.hpp>
#include <space/network_client.hpp>
#include <space/network_server.hpp>
#include <space/playout.hpp>
#include <space/render.hpp>
#include <space/state.hpp>


// -------------------------------------------------------------------------------------------------

void register_controls(libv::ctrl::FeatureRegister controls) {
//		libv::ctrl::scale_group sg_translate{
//				.impulse = 0.1,
//				.time = 1.0,
//				.mouse = 1.0 / 600.0,
//				.scroll = 0.1,
//				.gp_analog = 1.0,
//				.js_analog = 1.0
//		};

	controls.feature_action<app::SpaceCanvas>("space.add_fleet_by_mouse", [](const auto& arg, app::SpaceCanvas& ctx) {
		(void) arg;

		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		std::cout << "mouse_local_coord: " << mouse_local_coord << std::endl;
		std::cout << "world_coord: " << world_coord << std::endl;

		if (!ctx.state.fleets.empty()) {
			ctx.playout_delay_buffer.queue<app::CommandFleetMove>(
					static_cast<app::FleetID>(ctx.state.fleets.size() - 1),
					world_coord
			);
		}

		ctx.playout_delay_buffer.queue<app::CommandFleetSpawn>(world_coord);
	});

	controls.feature_action<app::SpaceCanvas>("space.warp_camera_to_mouse", [](const auto& arg, app::SpaceCanvas& ctx) {
		(void) arg;

		const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
		const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
		const auto mouse_ray_pos = ctx.camera.eye();
		const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

		std::cout << "world_coord: " << world_coord << std::endl;

		const auto playerID = app::PlayerID{0};
		ctx.playout_delay_buffer.queue<app::CommandCameraWarpTo>(playerID, world_coord);

		ctx.camera.warp_to(world_coord); // <<< Move this line to CommandCameraWarpTo apply
	});
}

void bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("space.add_fleet_by_mouse", "Ctrl + LMB [press]");
	controls.bind("space.warp_camera_to_mouse", "Z");
}

// -------------------------------------------------------------------------------------------------

// TODO P1: Controls camera should only be placed into context if the canvas is focused
// TODO P1: Remove the F12 tracking manual workaround too for mode switching (Related to controls bypass)
// TODO P3: Arrow strip placement control from lua (or something lua related) (With auto reload and everything)

struct SpaceFrame : public libv::Frame {
	libv::ui::UI ui;
	libv::ctrl::Controls controls;
	libv::Timer timer; // <<< use frame loop to call controls

	SpaceFrame() :
		Frame("Space", 1280, 800),
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
		setIcon(app::icon_set_iris_cyan());

		app::CameraBehaviour::register_controls(controls);
		app::CameraBehaviour::bind_default_controls(controls);
		register_controls(controls);
		bind_default_controls(controls);

		onContextUpdate.output([&](const auto&) {
			// shader_manager.update MUST run before any other render queue operation
			// OTHERWISE the not loaded uniform locations are attempted to be used and placed into the streams
			shader_manager.update(ui.gl());

			// TODO P1: Timer management should be smoother with control and frame attachment -> controls.attach(frame)
			controls.update(timer.time());
		});

		onKey.output([this](const libv::input::EventKey& e) {
			if (e.keycode == libv::input::Keycode::Escape)
				closeForce();
		});

		shader_manager.attach_libv_ui_hub(ui.event_hub());
		ui.attach(*this);
		controls.attach(*this);
	}
};

// -------------------------------------------------------------------------------------------------

int main() {
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");

	std::cout << libv::logger_stream;
	app::log_space.info("Hello Space!");

	// Change working directory
	std::filesystem::current_path("app/space/");

	SpaceFrame frame;

	app::CameraPlayer camera_main;
	app::CameraPlayer camera_mini;
	camera_main.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});
	camera_mini.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});

	frame.onKey.output([&](const libv::input::EventKey& e) {
		// TODO P1: Ui focus base camera context switch
		//			Hack workaround until ui focus does not operates control contexts
		if (e.keycode == libv::input::Keycode::F12 && e.action == libv::input::Action::press) {
			static int hack_camera_control_ui_mode = 0;
			hack_camera_control_ui_mode = (hack_camera_control_ui_mode + 1) % 3;
			if (hack_camera_control_ui_mode == 1)
				frame.controls.context_leave<app::BaseCameraOrbit>();
			else
				frame.controls.context_enter<app::BaseCameraOrbit>(&camera_main);
		}

		// TODO P1: Shortcut to save camera position and reload it upon restart
		//          > Requires persistence
		// TODO P1: Persist auto runtime hook options
		//          > Requires persistence
		// TODO P1: Auto runtime hook option for random uniform variables
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
//		app::log_space.info("Test mode: {}", global_test_mode);
	});

	// TODO P3: State object are all around, tidy up
	app::SpaceState space_state;
	app::SpaceSession space_session;
	app::PlayoutDelayBuffer playout_delay_buffer;
	std::optional<app::NetworkServer> server;
	std::optional<app::NetworkClient> client;

//	std::cout << libv::glr::layout_to_string<UniformLayoutMatrices>("Sphere") << std::endl;

	std::optional<libv::ui::CanvasAdaptorT<app::SpaceCanvas>> canvas_main; // TODO P1: Make HUD an object, store UI components that has to accessed, create API around it (Having to access UI components is fine)

	frame.ui.add(app::create_hud( // TODO P1: Too many argument passed in: Use a better communication channel (Maybe event bus)
			playout_delay_buffer,
			space_session,
			space_state,
			client,
			server,
			camera_main,
			camera_mini,

			canvas_main // out
	));

	frame.controls.context_enter<app::BaseCameraOrbit>(&camera_main); // TODO P4: <app::BaseCameraOrbit> Question mark? Context variables and inheritance?
	frame.controls.context_enter<app::SpaceCanvas>(&canvas_main->object()); // TODO P1: Enter / leave on canvas focus-unfocus

	frame.show();
	frame.join();

	return EXIT_SUCCESS;
}
