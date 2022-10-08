// Project: libv, File: app/star/game/game_client.cpp

// hpp
#include <star/game/game_client.hpp>
// libv
//#include <libv/mt/worker_thread.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
//#include <libv/ctrl/binding_register.hpp>
//#include <libv/ctrl/feature_register.hpp>
//#include <libv/ui/event/event_overlay.hpp>
//#include <libv/ui/event_hub.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
#include <libv/ui/component/scene_container.hpp>
// pro
#include <star/game/config/client_config.hpp>
#include <star/game/config/config.hpp>
#include <star/game/control/requests.hpp>
#include <star/game/game_client_frame.hpp>
#include <star/game/scene/scenes.hpp>
#include <star/log.hpp>
//#include <star/game/view/camera_control.hpp>
//#include <star/game/view/canvas_control.hpp>


//#include <libv/ui/context/context_state.hpp>



namespace star {

// -------------------------------------------------------------------------------------------------

struct ImplGameClient {
//	libv::mt::worker_thread scheduler_{"scheduler"};
	libv::Nexus nexus_;

	std::shared_ptr<ClientConfig> config_;
//	Config<ClientConfigGroup> config_;
//	ConfigManager configManager{scheduler_, nexus_};
//	Config<ClientConfig> config_;
//	std::shared_ptr<ClientConfig> config_;
//	std::shared_ptr<ClientConfig> configManager{scheduler_, nexus_};

//	auto nexus = libv::Nexus();
//	auto config = star::ClientConfig::loadFromJSON(scheduler, nexus, libv::read_file_or_throw(arg_config.value()));

	libv::ui::UI ui;

	GameClientFrame frame;

//	libv::ctrl::Controls controls;
//
//	Renderer renderer{ui};
//
//	GameThread game_thread{ui, nexus};
//
//	User user;

	inline ImplGameClient(libv::ui::Settings&& ui_settings, const std::filesystem::path& configFilepath) :
			config_(make_config<ClientConfig>(nexus_, configFilepath)),
			ui(ui_settings) {}
};

// -------------------------------------------------------------------------------------------------

GameClient::GameClient(const std::filesystem::path& configFilepath) :
	self(std::make_unique<ImplGameClient>(
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
			}(),
			configFilepath
	)) {

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

//	controls.attach(self->frame);
	self->ui.attach(self->frame);

	self->ui.load_style_script_file("style.lua");

	init_ui();
}

GameClient::~GameClient() {
	unregister_nexus();
}

void GameClient::register_nexus() {
	self->nexus_.object_view_set<ClientConfig>(self->config_.get());

	self->nexus_.connect_global<RequestClientExit>(this, [this](const RequestClientExit&) {
		self->frame.closeDefault();
	});

	// Setup Logging
	self->config_->development.logging_trace_ui.subscribe_and_call(this, [](const ConfigEntry<bool>::Change& event) {
		libv::logger_stream.below("libv.ui", libv::Logger::Severity::Info, event.entry.value());
	});
}

void GameClient::unregister_nexus() {
	self->config_->unsubscribe(this);

	self->nexus_.disconnect_all(this);
	self->nexus_.object_view_remove<ClientConfig>();

	assert(self->nexus_.num_tracked() == 0);
	assert(self->nexus_.num_object() == 0);
}

void GameClient::init_ui() {
	auto msc = libv::ui::SceneContainer("sc-main");
	msc.identifier("main");
	msc.assign(createSceneMainMenu(self->nexus_));
	self->ui.add(std::move(msc));
	self->ui.event().before_update([this] {
		self->config_->update();
	});

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

std::shared_ptr<ClientConfig> GameClient::config() const noexcept {
	return self->config_;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
