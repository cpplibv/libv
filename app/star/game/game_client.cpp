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
#include <libv/re/library.hpp>
#include <libv/res/resource_path.hpp>
#include <libv/sun/camera_control.hpp>
#include <libv/sun/ui/overlay_shader_error.hpp>
#include <libv/sun/ui/overlay_resource_error.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/scene_container.hpp>
#include <libv/ui/event/event_overlay.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/utility/timer.hpp>
// std
#include <optional>
// pro
#include <star/game/config/client_config.hpp>
#include <star/game/control/controllers.hpp>
#include <star/game/control/requests.hpp>
#include <star/game/game_client_frame.hpp>
#include <star/game/scene/scenes.hpp>
#include <star/game/version.hpp>
#include <star/log.hpp>

// debug
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/panel_line.hpp>

#include <libv/ui/component_system/switch_scene.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct ImplGameClient {
	libv::Nexus nexus_;

	//	Scheduler scheduler{"client-scheduler"};

	//	Config<ClientConfigGroup> config_;
	//	ConfigManager configManager{scheduler_, nexus_};
	//	Config<ClientConfig> config_;
	//	std::shared_ptr<ClientConfig> config_;
	//	std::shared_ptr<ClientConfig> configManager{scheduler_, nexus_};
	//	auto config = star::ClientConfig::loadFromJSON(scheduler, nexus, libv::read_file_or_throw(arg_config.value()));
	std::shared_ptr<ClientConfig> config_;

	//	User user;
	libv::ctrl::Controls controls;

	GameClientFrame frame;

	std::optional<libv::re::RenderLibrary> renderLibrary;
	libv::ui::UI ui;

	explicit inline ImplGameClient(const std::filesystem::path& configFilepath) :
			config_(make_config<ClientConfig>(nexus_, configFilepath)),
			ui(nexus_, [&] {
				libv::ui::Settings settings;
				settings.use_libv_res_resource_path = true;
				settings.res_font.base_path = "res/font/";
				settings.res_font.restrict_under_base = true;
				settings.res_shader.base_path = "res/shader/";
				settings.res_shader.restrict_under_base = true;
				settings.res_texture.base_path = "res/texture/";
				settings.res_texture.restrict_under_base = true;

				// settings.resStyle.resourceMappings.emplace_back("", "res/");
				settings.resStyle.trackFiles = true;
				return settings;
			}()) {
	}
};

// -------------------------------------------------------------------------------------------------

GameClient::GameClient(bool devMode, const std::filesystem::path& configFilepath) :
		self(std::make_unique<ImplGameClient>(configFilepath)) {

	register_controls();
	register_nexus();
	register_renderLibrary();

	self->ui.attach(self->frame);
	self->ui.attachControls(self->controls);
	// self->ui.loadStyleFile("style/error_overlay.lua");
	self->ui.loadStyleFile("style/main.lua");
	self->ui.loadStyleFile("style/game.lua");

	init_ui(devMode);

	self->frame.onContextUpdate.output([this](const libv::frame::EventContextUpdate&) {
		self->config_->update();
	});
}

GameClient::~GameClient() {
	unregister_nexus();

	// Move out the nexus and check if everyone cleaned itself up
	auto nexus = std::move(self->nexus_);
	self.reset();
	assert(nexus.num_tracked() == 0);
	assert(nexus.num_object() == 0);
}

void GameClient::register_controls() {
	libv::sun::CameraControl::register_controls(self->controls);
	libv::sun::CameraControl::bind_default_controls(self->controls, 1);
	registerSPSessionControls(self->controls);

	self->ui.event().global.connect_system<libv::ui::EventOverlay>([this](const libv::ui::EventOverlay& event) {
		log_star.info("Controls intercepted: {}", event.controls_intercepted());
		// TODO P1: A more seamless integration of UI and Controls would be nice
		self->controls.ignore_events(event.controls_intercepted());
	});
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

void GameClient::register_renderLibrary() {
	// Render Library will most likely be moved into the UI at some point
	self->ui.subscribe([this](const libv::ui::EventGLCreate& event) {
		self->renderLibrary.emplace([&] {
				libv::re::Settings settings;
				// settings.model.trackFiles = devMode;
				// settings.texture.trackFiles = devMode;
				// settings.shader.trackFiles = devMode;
				return settings;
			}(),
			self->nexus_,
			event.gl);
	});
	self->ui.subscribe([this](const libv::ui::EventGLDestroy& event) {
		(void) event;
		self->renderLibrary.reset();
	});
}

void GameClient::unregister_nexus() {
	self->config_->unsubscribe(this);

	self->nexus_.disconnect_all(this);
	self->nexus_.object_view_remove<ClientConfig>();
	self->nexus_.object_view_remove<libv::ctrl::Controls>();
}

libv::ui::Component GameClient::overlay_version(bool devMode) {
	return libv::ui::Label::nsa("version", "overlay.version", fmt::format("Star {}{}", build.version_number, devMode ? " [Dev]" : ""));
}

libv::ui::Component GameClient::overlay_fps() {
	static constexpr auto fpsUpdatePeriod = std::chrono::milliseconds{250};

	// 	// TODO P4: Implement proper statistics gathering (including integration with UI and Game state statistics)
	// 	const auto dt = t.timef_s().count();
	// 	avg.add(1.0f / dt, dt);
	// 	fps.text(fmt::format("{:6.1f} FPS", avg.value()));
	// TODO P4: Implement proper frame statistics
	//			min, max, avg, window min, window max, window avg, 1% low, 0.1% low
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

	auto msc = layers.add_na<libv::ui::SceneContainer>("sc-main", "main");
	msc.event().attach.connect([this](const libv::ui::SceneContainer& signal) {
		// Switch to the first scene on attach so the Render Library is already initalized
		// libv::ui::switchParentScene("main", signal, createSceneMainMenu(self->nexus_));
		// libv::ui::switchParentScene("main", signal, createSceneCredits(self->nexus_)); // Shortcut during development
		// libv::ui::switchParentScene("main", signal, createSceneSettings(self->nexus_)); // Shortcut during development
		// libv::ui::switchParentScene("main", signal, createSceneSurface(self->nexus_)); // Shortcut during development
		libv::ui::switchParentScene("main", signal, createSceneSPSession(self->nexus_)); // Shortcut during development
	});

	self->ui.add(std::move(layers));
	self->ui.add(overlay_version(devMode));
	self->ui.add(overlay_fps());
	self->ui.add(libv::sun::overlay_resource_error(true));

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
