// Project: libv, File: app/star/game/game_client.cpp

// hpp
#include <star/game/game_client.hpp>
// libv
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
//#include <libv/ctrl/binding_register.hpp>
//#include <libv/ctrl/feature_register.hpp>
//#include <libv/ui/event/event_overlay.hpp>
//#include <libv/ui/event_hub.hpp>
//#include <libv/utility/read_file.hpp>
// pro
#include <star/game/client_config.hpp>
#include <star/game/control/requests.hpp>
#include <star/game/game_client_frame.hpp>
#include <star/game/scene/scene_main_menu.hpp>
#include <star/game/scene/scene_root.hpp>
//#include <star/game/view/camera_control.hpp>
//#include <star/game/view/canvas_control.hpp>
//#include <star/log.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct ImplGameClient {
	std::shared_ptr<ClientConfig> settings_;

	GameClientFrame frame;

	libv::Nexus nexus_;
//	libv::ctrl::Controls controls;

	libv::ui::UI ui;

//	Renderer renderer{ui};
//
//	GameThread game_thread{ui, nexus};
//
//	User user;

	inline ImplGameClient(std::shared_ptr<ClientConfig>&& settings, libv::ui::Settings&& ui_settings) :
			settings_(std::move(settings)),
			ui(ui_settings) {}
};

// -------------------------------------------------------------------------------------------------

GameClient::GameClient(const std::filesystem::path& settingsFilepath) :
	self(std::make_unique<ImplGameClient>(
			std::make_shared<ClientConfig>(),
			[] {
				libv::ui::Settings settings;
				// TODO P1: Internalize used UI resources under star, currently: app/star/../../res/
				settings.res_font.base_path = "../../res/font/";
				settings.res_shader.base_path = "../../res/shader/";
				settings.res_texture.base_path = "../../res/texture/";

				settings.res_font.restict_under_base = false; // TODO P1: Should go away with proper res folder
				settings.res_shader.restict_under_base = false; // TODO P1: Should go away with proper res folder
				settings.res_texture.restict_under_base = false; // TODO P1: Should go away with proper res folder

				settings.track_style_scripts = true;
				return settings;
			}()
	)) {

//	settings->loadJSON(libv::read_file_or_throw(settingsFilepath));

	register_nexus();
//	CameraControl::register_controls(controls);
//	CameraControl::bind_default_controls(controls);
//	CanvasControl::register_controls(controls);
//	CanvasControl::bind_default_controls(controls);
//
//	ui.event().global.connect_system<libv::ui::EventOverlay>([this](const libv::ui::EventOverlay& event) {
//		log_star.info("Controls intercepted: {}", event.controls_intercepted());
//		// TODO P1: A more seamless integration of UI and Controls would be nice
//		controls.ignore_events(event.controls_intercepted());
//	});
//
//	controls.attach(self->frame);
	self->ui.attach(self->frame);

//	ui.load_style_script(libv::read_file_or_throw("style.lua"));
//	ui.load_style_script(libv::read_file_and_track_or_throw("style.lua"));
	self->ui.load_style_script_file("style.lua");

	init_ui();
}

GameClient::~GameClient() {
	unregister_nexus();
	// For the sake of forward declared unique_ptr
}

void GameClient::register_nexus() {
//	nexus_.connect<mc::RequestNameChange>(this, [this](const mc::RequestNameChange& event) {
//		user.name = event.name;
//		nexus_.broadcast<mc::OnNameChange>();
//	});
	self->nexus_.connect<RequestClientExit>(this, [this](const RequestClientExit&) {
		self->frame.closeDefault();
	});
}

void GameClient::unregister_nexus() {
	self->nexus_.disconnect_all(this);
}

void GameClient::init_ui() {
	self->ui.add(SceneRoot());
	self->ui.event().global.fire<SwitchPrimaryScene>(createSceneMainMenu(*this));

//	frame.onKey.output([&](const libv::input::EventKey& e) {
//		if (e.keycode == libv::input::Keycode::F2 && e.action == libv::input::Action::press)
//			ui.foreach_recursive_component([](const libv::ui::Component& component) {
//				fmt::print("{}: {}\n", component.path(), to_string(component.flags()));
//			});
//	});
}

void GameClient::run() {
	self->frame.show();
	self->frame.join();
}

libv::Nexus& GameClient::nexus() noexcept {
	return self->nexus_;
}

const std::shared_ptr<ClientConfig>& GameClient::settings() const noexcept {
	return self->settings_;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
