// Project: libv, File: app/space/game_instance.cpp

// hpp
#include <space/game_instance.hpp>
// libv
#include <libv/ctrl/feature_register.hpp>
#include <libv/ui/event/event_overlay.hpp>
#include <libv/ui/event_hub.hpp>
#include <libv/ui/settings.hpp>
// pro
#include <space/view/camera_control.hpp>
#include <space/view/canvas_control.hpp>
#include <space/view/scene_main_control.hpp>
#include <space/internal_events.hpp>
#include <space/log.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------

GameInstance::GameInstance() :
	ui([] {
		libv::ui::Settings settings;
		// TODO P1: Internalize used UI resources under space, currently: app/space/../../res/
		settings.res_font.base_path = "../../res/font/";
		settings.res_shader.base_path = "../../res/shader/";
		settings.res_texture.base_path = "../../res/texture/";

		settings.res_font.restict_under_base = false; // TODO P1: Should go away with proper res folder
		settings.res_shader.restict_under_base = false; // TODO P1: Should go away with proper res folder
		settings.res_texture.restict_under_base = false; // TODO P1: Should go away with proper res folder

		settings.track_style_scripts = true;
		return settings;
	}()) {

	register_nexus();
	CameraControl::register_controls(controls);
	CameraControl::bind_default_controls(controls);
	CanvasControl::register_controls(controls);
	CanvasControl::bind_default_controls(controls);
	SceneMainControl::register_controls(controls);
	SceneMainControl::bind_default_controls(controls);

	frame.onKey.output([this](const libv::input::EventKey& e) {
		// TODO P1: Remove this, global test variables should be a generic concept
		event_for_global_test_mode(this->frame, e);
	});

	ui.event().global.connect_system<libv::ui::EventOverlay>([this](const libv::ui::EventOverlay& event) {
		log_space.info("Controls intercepted: {}", event.controls_intercepted());
		// TODO P1: A more seamless integration of UI and Controls would be nice
		controls.ignore_events(event.controls_intercepted());
	});

	controls.attach(frame);
	ui.attach(frame);

//	ui.load_style_script(libv::read_file_or_throw("style.lua"));
	ui.load_style_script_file("style.lua");
}

GameInstance::~GameInstance() {
	unregister_nexus();
}

void GameInstance::register_nexus() {
	nexus.connect<mc::RequestNameChange>(this, [this](const mc::RequestNameChange& event) {
		user.name = event.name;
		nexus.broadcast<mc::OnNameChange>();
	});
}

void GameInstance::unregister_nexus() {
	nexus.disconnect_all(this);
}

void GameInstance::execute() {
	frame.show();
	frame.join();
}

void GameInstance::enterSinglePlayer() {
	scene_main.openSP();
}

void GameInstance::enterMultiPlayerClient(std::string server_address, uint16_t server_port) {
	scene_main.openMPClient(std::move(server_address), server_port);
}

void GameInstance::enterMultiPlayerServer(uint16_t port) {
	scene_main.openMPServer(port);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
