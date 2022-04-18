// Created by dbobula on 3/25/2022.

#include <space/surface/surface_viewer.hpp>

//libv
#include <libv/ui/settings.hpp>
//#include <libv/ui/component/label.hpp>
#include <libv/ctrl/feature_register.hpp>

//space
#include <space/surface/surface_canvas.hpp>
#include <space/view/camera_control.hpp>


namespace surface {
SurfaceViewer::SurfaceViewer() :
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
	frame.setSize(1024, 1024);
	frame.setAlwaysOnTop(true);

	space::CameraControl::register_controls(controls);
	space::CameraControl::bind_default_controls(controls);
	space::CameraControl2D::register_controls(controls);
	space::CameraControl2D::bind_default_controls(controls);
//		CanvasControl::register_controls(controls);
//		CanvasControl::bind_default_controls(controls);

//		libv::ui::Label label("helloLabel");
//		label.text("Surface app");
//		label.font_color(libv::vec4f(1, 1, 1, 1));
//		label.align_horizontal(libv::ui::AlignHorizontal::center);
//		label.align_vertical(libv::ui::AlignVertical::center);
//		ui.add(label);

	libv::ui::CanvasAdaptorT<SurfaceCanvas> canvas("canvas", ui);
	canvas.z_index_offset(-100);
	canvas.event().focus.connect([this, canvas](const libv::ui::EventFocus& e) mutable {
		if (is3DCamera) {
			if (e.gain()) {
				controls.context_leave_if_matches<space::CameraOrtho>(&canvas.object().camera2D);
				controls.context_enter<space::BaseCameraOrbit>(&canvas.object().camera3D);
			} else
				controls.context_leave_if_matches<space::BaseCameraOrbit>(&canvas.object().camera3D);
		} else {
			if (e.gain()) {
				controls.context_leave_if_matches<space::BaseCameraOrbit>(&canvas.object().camera3D);
				controls.context_enter<space::CameraOrtho>(&canvas.object().camera2D);
			} else
				controls.context_leave_if_matches<space::CameraOrtho>(&canvas.object().camera2D);
		}
	});

	frame.onContextUpdate.output([&](const auto&) {
		if (isCameraChanged)
			if (is3DCamera) {
				controls.context_leave_if_matches<space::CameraOrtho>(&canvas.object().camera2D);
				controls.context_enter<space::BaseCameraOrbit>(&canvas.object().camera3D);
			} else {
				controls.context_leave_if_matches<space::BaseCameraOrbit>(&canvas.object().camera3D);
				controls.context_enter<space::CameraOrtho>(&canvas.object().camera2D);
			}
	});

	controls.attach(frame);
	ui.attach(frame);
	ui.add(canvas);

	//switch between triangle fill and wireframe
	controls.feature_action<void>("surface.switch_polygon_mode", [](const auto&) {
		isPolygonFill = !isPolygonFill;
	});
	//switch between camera 3D and 2D
	controls.feature_action<void>("surface.switch_camera", [](const auto&) {
		is3DCamera = !is3DCamera;
		isCameraChanged = true;
	});
	controls.feature_action<void>("surface.normal_texture", [](const auto&) {
		currentHeatMap = HeatMapType::height;
		isTextureChanged = true;
	});
	controls.feature_action<void>("surface.temperature_texture", [](const auto&) {
		currentHeatMap = HeatMapType::temperature;
		isTextureChanged = true;
	});
	controls.feature_action<void>("surface.humidity_texture", [](const auto&) {
		currentHeatMap = HeatMapType::humidity;
		isTextureChanged = true;
	});
	controls.feature_action<void>("surface.fertility_texture", [](const auto&) {
		currentHeatMap = HeatMapType::fertility;
		isTextureChanged = true;
	});

	controls.bind("surface.switch_polygon_mode", "F2 [press]");
	controls.bind("surface.switch_camera", "F3 [press]");
	controls.bind("surface.normal_texture", "1 [press]");
	controls.bind("surface.temperature_texture", "2 [press]");
	controls.bind("surface.humidity_texture", "3 [press]");
	controls.bind("surface.fertility_texture", "4 [press]");
//	controls.bind("surface.temperature_heat_map", "1 [press]");

//		frame.onKey.output([canvas](const libv::input::EventKey& e) mutable {
//			if (e.keycode == libv::input::Keycode::H) {
//				canvas.object().camera.move_up(-1.f);
//			}
//		});
}

void SurfaceViewer::execute() {
	frame.show();
	frame.join();
}

} // namespace surface
