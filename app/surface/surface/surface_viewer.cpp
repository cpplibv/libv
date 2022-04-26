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
#include <surface/surface/surface_canvas.hpp>
#include <surface/view/camera_control.hpp>
#include <surface/view/overlay_shader_error.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

SurfaceViewer::SurfaceViewer() :
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
		mainLayers("layers"),
		sceneTitle("sceneTitle") {

//	frame.setSize(1024, 1024);
//	frame.setAlwaysOnTop(true);

	surface::CameraControl::register_controls(controls);
	surface::CameraControl::bind_default_controls(controls);
	surface::CameraControl2D::register_controls(controls);
	surface::CameraControl2D::bind_default_controls(controls);
//		CanvasControl::register_controls(controls);
//		CanvasControl::bind_default_controls(controls);

	controls.feature_action<void>("surface.refresh_surface", [](const auto&) {
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
	controls.feature_action<void>("surface.toggle_grid", [](const auto&) {
		enableGrid = !enableGrid;
	});

	controls.feature_action<void>("surface.3d", [this](const auto&) {
		log_surface.info("Changing scene to: 3D Surface");
		sceneTitle.text("3D Surface");
		currentHeatMap = SceneType::_3d;
		currentCameraMode = CameraMode::_3d;
	});
	controls.feature_action<void>("surface.height_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Height");
		sceneTitle.text("Texture: Height");
		currentHeatMap = SceneType::height;
		currentCameraMode = CameraMode::_2d;
	});
	controls.feature_action<void>("surface.temperature_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Temperature");
		sceneTitle.text("Texture: Temperature");
		currentHeatMap = SceneType::temperature;
		currentCameraMode = CameraMode::_2d;
	});
	controls.feature_action<void>("surface.humidity_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Humidity");
		sceneTitle.text("Texture: Humidity");
		currentHeatMap = SceneType::humidity;
		currentCameraMode = CameraMode::_2d;
	});
	controls.feature_action<void>("surface.fertility_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Fertility");
		sceneTitle.text("Texture: Fertility");
		currentHeatMap = SceneType::fertility;
		currentCameraMode = CameraMode::_2d;
	});
	controls.feature_action<void>("surface.biome_texture", [this](const auto&) {
		log_surface.info("Changing scene to: Texture Biome");
		sceneTitle.text("Texture: Biome");
		currentHeatMap = SceneType::biome;
		currentCameraMode = CameraMode::_2d;
	});
//	controls.feature_action<void>("surface.distribution_texture", [](const auto&) {
//		log_surface.info("Changing scene to: Texture distribution");
//		currentHeatMap = SceneType::distribution;
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

			mainLayers.add(label);
		} else {
			mainLayers.remove(helpComponentName);
		}
	});

	ui.event().global.connect_system<libv::ui::EventOverlay>([this](const libv::ui::EventOverlay& event) {
		log_surface.info("Controls intercepted: {}", event.controls_intercepted());
		controls.ignore_events(event.controls_intercepted());
	});

	controls.bind("surface.refresh_surface", "R");
	controls.bind("surface.show_controls", "F1");
	controls.bind("surface.toggle_wireframe", "F2 [press]");
	controls.bind("surface.cycle_camera", "F3 [press]");
	controls.bind("surface.toggle_vegetation", "F4 [press]");
	controls.bind("surface.toggle_grid", "F5 [press]");

	controls.bind("surface.3d", "1 [press]");
	controls.bind("surface.height_texture", "2 [press]");
//	controls.bind("surface.normal_texture", "?? [press]");
	controls.bind("surface.temperature_texture", "3 [press]");
	controls.bind("surface.humidity_texture", "4 [press]");
	controls.bind("surface.fertility_texture", "5 [press]");
	controls.bind("surface.biome_texture", "6 [press]");
//	controls.bind("surface.distribution_texture", "7 [press]");

	initUI();

	controls.attach(frame);
	ui.attach(frame);
	ui.add(mainLayers);
}

void SurfaceViewer::initUI() {
	libv::ui::CanvasAdaptorT<SurfaceCanvas> canvas("canvas", ui, controls);
	canvas.z_index_offset(-100);

	canvas.event().focus.connect([this, canvas](const libv::ui::EventFocus& e) mutable {
		canvas.object().cameraManager.enableControls(e.gain());
	});

	mainLayers.add(canvas);

	libv::ui::Label label("version_lbl");
	label.text("v1.0");
	label.font_color(libv::vec4f(0.8f, 0.8f, 0.8f, 0.6f));
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

	mainLayers.add(overlay_shader_error());
}

void SurfaceViewer::execute() {
	frame.show();
	frame.join();
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
