// Project: libv, File: app/star/game/game_client.cpp

// hpp
#include <star/game/game_client.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/ctrl/binding_register.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/math/exp_moving_avg.hpp>
#include <libv/sun/camera_control.hpp>
#include <libv/sun/ui/overlay_shader_error.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/scene_container.hpp>
#include <libv/ui/event/event_overlay.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/utility/timer.hpp>
#include <wish/resource_path.hpp>
// pro
#include <star/game/config/client_config.hpp>
#include <star/game/control/requests.hpp>
#include <star/game/game_client_frame.hpp>
#include <star/game/scene/scenes.hpp>
#include <star/log.hpp>
#include <star/version.hpp>

//#include <libv/mt/worker_thread.hpp>
//#include <libv/ui/event/event_overlay.hpp>
//#include <libv/ui/event_hub.hpp>
//#include <star/game/view/camera_control.hpp>
//#include <star/game/view/canvas_control.hpp>
//#include <libv/ui/context/context_state.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct ImplGameClient {
	libv::Nexus nexus_;

//	libv::mt::worker_thread scheduler_{"scheduler"};
//	GameThread game_thread{ui, nexus};

//	Config<ClientConfigGroup> config_;
//	ConfigManager configManager{scheduler_, nexus_};
//	Config<ClientConfig> config_;
//	std::shared_ptr<ClientConfig> config_;
//	std::shared_ptr<ClientConfig> configManager{scheduler_, nexus_};
//	auto config = star::ClientConfig::loadFromJSON(scheduler, nexus, libv::read_file_or_throw(arg_config.value()));
	std::shared_ptr<ClientConfig> config_;
//	User user;
	libv::ctrl::Controls controls;

//	Renderer renderer{ui};

	GameClientFrame frame;
	libv::ui::UI ui;

	explicit inline ImplGameClient(const std::filesystem::path& configFilepath) :
			config_(make_config<ClientConfig>(nexus_, configFilepath)),
			ui([&] {
				libv::ui::Settings settings;

				// settings.nexus = nexus_;

				// TODO P1: Internalize used UI resources under star, currently: app/star/../../res/
				settings.res_font.base_path = "../../res/font/";
				settings.res_shader.base_path = "../../res/shader/";
				settings.res_texture.base_path = "../../res/texture/";

				settings.res_font.restrict_under_base = false; // TODO P1: Should go away with proper res folder
				settings.res_shader.restrict_under_base = false; // TODO P1: Should go away with proper res folder
				settings.res_texture.restrict_under_base = false; // TODO P1: Should go away with proper res folder

				settings.track_style_scripts = true;
				return settings;
			}()) {
	}
};

// -------------------------------------------------------------------------------------------------

GameClient::GameClient(bool devMode, const std::filesystem::path& configFilepath) :
		self(std::make_unique<ImplGameClient>(configFilepath)) {

	register_controls();
	register_nexus();

	self->ui.attach(self->frame);
	self->ui.load_style_script_file(wish::resource_path("res/style.lua"));

	init_ui(devMode);
}

GameClient::~GameClient() {
	unregister_nexus();
}

void GameClient::register_controls() {
	libv::sun::CameraControl::register_controls(self->controls);
	libv::sun::CameraControl::bind_default_controls(self->controls);
//	CanvasControl::register_controls(controls);
//	CanvasControl::bind_default_controls(controls);

	self->ui.event().global.connect_system<libv::ui::EventOverlay>([this](const libv::ui::EventOverlay& event) {
		log_star.info("Controls intercepted: {}", event.controls_intercepted());
		// TODO P1: A more seamless integration of UI and Controls would be nice
		self->controls.ignore_events(event.controls_intercepted());
	});

	self->controls.attach(self->frame);
}

void GameClient::register_nexus() {
	self->nexus_.object_view_set<ClientConfig>(self->config_.get());
	self->nexus_.object_view_set<libv::ctrl::Controls>(&self->controls);

	self->nexus_.connect_global<RequestClientExit>(this, [this](const RequestClientExit&) {
		self->frame.closeDefault();
	});

	// Setup configs
	self->config_->graphics.vsync_enable.subscribe_and_call(this, [this](const ConfigEntry<bool>::Change& event) {
		self->frame.setSwapInterval(event.entry.value() ? 1 : 0);
	});

	self->config_->development.always_on_top.subscribe_and_call(this, [this](const ConfigEntry<bool>::Change& event) {
		self->frame.setAlwaysOnTop(event.entry.value());
	});

	self->config_->development.logging_trace_ui.subscribe_and_call(this, [](const ConfigEntry<bool>::Change& event) {
		libv::logger_stream.below("libv.ui", libv::Logger::Severity::Info, event.entry.value());
	});
}

void GameClient::unregister_nexus() {
	self->config_->unsubscribe(this);

	self->nexus_.disconnect_all(this);
	self->nexus_.object_view_remove<ClientConfig>();
	self->nexus_.object_view_remove<libv::ctrl::Controls>();

	assert(self->nexus_.num_tracked() == 0);
	assert(self->nexus_.num_object() == 0);
}

libv::ui::Component GameClient::overlay_version(bool devMode) {
	return libv::ui::Label::nsa("version", "overlay.version", fmt::format("Star {}{}", build.version_number, devMode ? " [Dev]" : ""));
}

libv::ui::Component GameClient::overlay_fps() {
	static constexpr auto fpsUpdatePeriod = std::chrono::milliseconds{250};

	// TODO P4: Implement proper frame statistics
	//			min, max, avg, window min, window max, window avg, 1% low
	//  		(including integration with UI and Game state statistics)
	auto fps = libv::ui::Label::ns("fps", "overlay.fps");
	self->ui.event().global_before_update([
			fps,
			frameTimer = libv::Timer{},
			updateTimer = libv::Timer{},
			avg = libv::exp_moving_avgf(60, 1, 0.8f)] mutable {
		const auto dt = frameTimer.timef_s().count();
		avg.add(1.0f / dt, dt);
		if (updateTimer.elapsed() < fpsUpdatePeriod)
			return;
		updateTimer.reset();
		fps.text(fmt::format("{:6.1f} FPS", avg.value()));
	});
	return fps;
}

void GameClient::init_ui(bool devMode) {
	auto layers = libv::ui::PanelAnchor("layers");

	auto msc = layers.add_n<libv::ui::SceneContainer>("sc-main");
	msc.identifier("main");
	// msc.assign(createSceneMainMenu(self->nexus_));
	// msc.assign(createSceneCredits(self->nexus_));
	msc.assign(createSceneSurface(self->nexus_));

	// auto fps = layers.add_ns<libv::ui::Label>("version", "overlay.fps");

	self->ui.add(std::move(layers));
	self->ui.add(overlay_version(devMode));
	self->ui.add(overlay_fps());
	self->ui.add(libv::sun::overlay_shader_error(true));
	// self->ui.event().global_before_update([this, fps, t = libv::Timer{}, avg = libv::exp_moving_avg<float>(60, 1, 0.8f)] mutable {
	// 	self->config_->update();
	//
	// 	// TODO P4: Implement proper statistics gathering (including integration with UI and Game state statistics)
	// 	const auto dt = t.timef_s().count();
	// 	avg.add(1.0f / dt, dt);
	// 	fps.text(fmt::format("{:6.1f} FPS", avg.value()));
	// });

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
