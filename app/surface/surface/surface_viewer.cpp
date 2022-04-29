// Created by dbobula on 3/25/2022.

// hpp
#include <surface/surface/surface_viewer.hpp>
// libv
#include <libv/ctrl/binding.hpp>
#include <libv/ctrl/binding_register.hpp>
#include <libv/ctrl/feature.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/event/event_overlay.hpp>
#include <libv/ui/settings.hpp>
// pro
#include <surface/log.hpp>
#include <surface/view/camera_control.hpp>
#include <surface/view/overlay_shader_error.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

SurfaceViewer::SurfaceViewer(const std::string& configPath) :
		ui([] {
			libv::ui::Settings settings;
			// TODO P1: Internalize used UI resources under surface, currently: app/surface/../../res/
			settings.res_font.base_path = "../../res/font/";
			settings.res_shader.base_path = "../../res/shader/";
			settings.res_texture.base_path = "../../res/texture/";

			settings.res_font.restict_under_base = false; // TODO P1: Should go away with proper res folder
			settings.res_shader.restict_under_base = false; // TODO P1: Should go away with proper res folder
			settings.res_texture.restict_under_base = false; // TODO P1: Should go away with proper res folder

			settings.track_style_scripts = true;
			return settings;
		}()),
		canvas("canvas", ui, controls, configPath),
		mainLayers("layers"),
		sceneTitle("sceneTitle"){

//	frame.setSize(1024, 1024);
//	frame.setAlwaysOnTop(true);

	surface::CameraControl::register_controls(controls);
	surface::CameraControl::bind_default_controls(controls);
	surface::CameraControl2D::register_controls(controls);
	surface::CameraControl2D::bind_default_controls(controls);
//		CanvasControl::register_controls(controls);
//		CanvasControl::bind_default_controls(controls);

	controls.feature_action<void>("surface.refresh_surface", [](const auto&) {
		refresh = true;
	});
	controls.feature_action<void>("surface.toggle_freeze", [](const auto&) {
		freeze = !freeze;
	});
	controls.feature_action<void>("surface.toggle_biome_blending", [](const auto&) {
		blendBiomes = !blendBiomes;
		changed = true;
	});
	controls.feature_action<void>("surface.toggle_wireframe", [](const auto&) {
		enableWireframe = !enableWireframe;
	});
	controls.feature_action<void>("surface.toggle_vegetation", [](const auto&) {
		enableVegetation = !enableVegetation;
	});
	controls.feature_action<void>("surface.cycle_camera", [](const auto&) {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			currentCameraMode = CameraMode::_3d;
			break;
		case CameraMode::_3d:
			currentCameraMode = CameraMode::_2d;
			break;
		}
	});
	controls.feature_action<void>("surface.cycle_config", [this](const auto&) {
		refresh = true;
		configName.text(canvas.object().cycleConfig());
	});
	controls.feature_action<void>("surface.toggle_grid", [](const auto&) {
		enableGrid = !enableGrid;
	});
	controls.feature_action<void>("surface.toggle_fog", [](const auto&) {
		enableFog = !enableFog;
	});
	controls.feature_action<void>("surface.toggle_skybox", [](const auto&) {
		enableSkybox = !enableSkybox;
	});

	controls.feature_action<void>("surface.3d", [this](const auto&) {
		log_surface.info("Changing scene to: 3D Surface");
		sceneTitle.text("3D Surface");
		currentScene = SceneType::_3d;
		currentCameraMode = CameraMode::_3d;
	});
	controls.feature_action<void>("surface.height_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Height");
		sceneTitle.text("Texture: Height");
		currentScene = SceneType::height;
		currentCameraMode = CameraMode::_2d;
	});
	controls.feature_action<void>("surface.temperature_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Temperature");
		sceneTitle.text("Texture: Temperature");
		currentScene = SceneType::temperature;
		currentCameraMode = CameraMode::_2d;
	});
	controls.feature_action<void>("surface.humidity_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Humidity");
		sceneTitle.text("Texture: Humidity");
		currentScene = SceneType::humidity;
		currentCameraMode = CameraMode::_2d;
	});
	controls.feature_action<void>("surface.fertility_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Fertility");
		sceneTitle.text("Texture: Fertility");
		currentScene = SceneType::fertility;
		currentCameraMode = CameraMode::_2d;
	});
	controls.feature_action<void>("surface.biome_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Biome");
		sceneTitle.text("Texture: Biome");
		currentScene = SceneType::biome;
		currentCameraMode = CameraMode::_2d;
	});
//	controls.feature_action<void>("surface.distribution_texture", [](const auto&) {
//		log_surface.info("Changing scene to: Texture distribution");
//		currentScene = SceneType::distribution;
//		hasSceneChanged = true;
//		currentCameraMode = CameraMode::_2d;
//	});

	controls.feature_binary<void>("surface.show_controls", [this](libv::ctrl::arg_binary arg) {
		static constexpr auto helpComponentName = "controls-help";
		if (arg.value) {
			std::ostringstream os;

			std::size_t maxFeatureLength = 0;
			controls.foreach_bindings([&](const libv::ctrl::Binding& binding) {
				maxFeatureLength = std::max(maxFeatureLength, binding.feature_name().size());
			});

			controls.foreach_bindings([&](const libv::ctrl::Binding& binding) {
//				os << fmt::format("{:{}} : {}\n", binding.feature_name(), maxFeatureLength, binding.sequence().to_string_symbol());
				os << fmt::format("{:{}} : {}\n", binding.feature_name(), maxFeatureLength, binding.sequence().to_string_name());
			});

			auto text = std::move(os).str();
			if (!text.empty())
				text.pop_back(); // Discard the last \n character

			libv::ui::Label label(helpComponentName);
			label.font_color({1, 1, 1, 1});
			//label.style("overlay.controls-help.lbl");
			label.text(std::move(text));
			label.background(libv::ui::Background::color({0, 0, 0, 0.5f}));

			mainLayers.add(label);
		} else {
			mainLayers.remove(helpComponentName);
		}
	});

	ui.event().global.connect_system<libv::ui::EventOverlay>([this](const libv::ui::EventOverlay& event) {
		log_surface.info("Controls intercepted: {}", event.controls_intercepted());
		controls.ignore_events(event.controls_intercepted());
	});

	controls.bind("surface.refresh_surface", "R [press]");
	controls.bind("surface.show_controls", "F1");
	controls.bind("surface.toggle_wireframe", "F2 [press]");
	controls.bind("surface.cycle_camera", "F3 [press]");
	controls.bind("surface.toggle_vegetation", "F4 [press]");
	controls.bind("surface.toggle_grid", "F5 [press]");
	controls.bind("surface.toggle_fog", "F6 [press]");
	controls.bind("surface.toggle_skybox", "F7 [press]");
	controls.bind("surface.toggle_freeze", "F8 [press]");
	controls.bind("surface.toggle_biome_blending", "F9 [press]");

	controls.bind("surface.cycle_config", "C [press]");

	controls.bind("surface.3d", "1 [press]");
	controls.bind("surface.height_texture", "2 [press]");
	controls.bind("surface.temperature_texture", "3 [press]");
	controls.bind("surface.humidity_texture", "4 [press]");
	controls.bind("surface.fertility_texture", "5 [press]");
	controls.bind("surface.biome_texture", "6 [press]");
//	controls.bind("surface.distribution_texture", "7 [press]");

	initUI(configPath);

	controls.attach(frame);
	ui.attach(frame);
	ui.add(mainLayers);

}

void SurfaceViewer::initUI(const std::string& configPath) {
	canvas.z_index_offset(-100);

	canvas.event().focus.connect([this](const libv::ui::EventFocus& e) mutable {
		canvas.object().cameraManager.enableControls(e.gain());
	});

	mainLayers.add(canvas);

	libv::ui::Label label("version_lbl");
	label.text("v1.0");
	label.font_color(libv::vec4f(0.7f, 0.7f, 0.7f, 1.0f));
	label.align_horizontal(libv::ui::AlignHorizontal::right);
	label.align_vertical(libv::ui::AlignVertical::bottom);
	label.margin(8, 0);
	mainLayers.add(label);

	sceneTitle.text("3D Surface");
	sceneTitle.font_color(libv::vec4f(1, 1, 1, 1));
	sceneTitle.align_horizontal(libv::ui::AlignHorizontal::center);
	sceneTitle.align_vertical(libv::ui::AlignVertical::top);
	sceneTitle.margin(6);
	mainLayers.add(sceneTitle);

	configName.text(configPath);
	configName.font_color(libv::vec4f(1, 1, 1, 1));
	configName.align_horizontal(libv::ui::AlignHorizontal::left);
	configName.align_vertical(libv::ui::AlignVertical::top);
	configName.margin(6);
	mainLayers.add(configName);

	mainLayers.add(overlay_shader_error());
}

void SurfaceViewer::execute() {
	frame.show();
	frame.join();
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
