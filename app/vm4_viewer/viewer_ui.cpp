// Project: libv, File: app/vm4_viewer/viewer_ui.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <vm4_viewer/viewer_ui.hpp>
// libv
#include <libv/parse/color.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_state.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/utility/concat.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/vm4/load.hpp>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/scene/camera_behaviour.hpp>
#include <vm4_viewer/ui/quick_file_picker.hpp>
#include <vm4_viewer/version.hpp>
#include <vm4_viewer/viewer_info_panel.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

ViewerUI::ViewerUI(CoreComponent& parent) :
	libv::ui::PanelFloat(parent, "VM4Viewer"),
	scene(shader_loader) {
	scene.camera.orbit_to(15.f);
	scene.camera.rotate_x(libv::to_rad(45.f));
	scene.camera.rotate_z(libv::to_rad(45.f));

	// <<< P95: app_vm4v: control integration
	auto fr = libv::ctrl::FeatureRegister{controls};
	CameraBehaviour::register_controls(fr);
	CameraBehaviour::bind_controls(controls);
	controls.context_enter(&scene.camera);
//	controls.context_enter(&scene.camera2);
}

void ViewerUI::update(libv::ui::time_duration elapsed_time_t) {
	if (not isFocused())
		return;

	controls.update(elapsed_time_t);
}

void ViewerUI::load(const std::string& path) {
	// libv::erase_stable(config.recent_projects, project);
	// config.recent_projects.emplace_back(project);

	// libv::erase_stable(config.recent_models, model);
	// config.recent_models.emplace_back(model);

	libv::Timer timer;

	log_app.info("Loading  {}...", path);
	auto file = libv::read_file_or_throw(path);
	const auto time_load = timer.timef_us().count();

	log_app.info("Parsing  {}...", path);
	auto model_vm4 = libv::vm4::load_or_throw(file);
	const auto time_parse = timer.timef_us().count();

	log_app.info("Updating {}...", path);
	info_panel->update(model_vm4);
	const auto time_update = timer.timef_us().count();

	log_app.info("Applying {}...", path);
	scene.model.emplace(std::move(model_vm4));
	scene.reset_camera();
	const auto time_apply = timer.timef_us().count();

	log_app.info("Processed {} in: Load {}us, Parse {}us, Update {}us, Apply {}us. Total: {}us.",
			path, time_load, time_parse, time_update, time_apply,
			time_load + time_parse + time_update, time_apply);
}

// -------------------------------------------------------------------------------------------------

//void ViewerUI::register_controls(libv::ctrl::FeatureRegister& controls) {
//	CameraBehaviour::register_controls(controls);
//}

// -------------------------------------------------------------------------------------------------

void ViewerUI::onKey(const libv::ui::EventKey& event) {
	if (not isFocused())
		return;

	// TODO P2: libv.ui: Better access to modifiers
	const auto shift = context().state.key_pressed(libv::input::Keycode::ShiftLeft) || context().state.key_pressed(libv::input::Keycode::ShiftRight);

	if (event.keycode == libv::input::Keycode::F && shift)
		scene.reset_camera();
	else if (event.keycode == libv::input::Keycode::F)
		scene.focus_camera();

	controls.input(event);
	event.stop_propagation();
}

void ViewerUI::onFocus(const libv::ui::EventFocus& event) {
	log_app.info("ViewerUI Focus {}", event.focus);
}

void ViewerUI::onMouseButton(const libv::ui::EventMouseButton& event) {
	focus();

	// <<< P95: app_vm4v: control integration
	controls.input(libv::input::EventMouseButton{event.button, event.action});
	event.stop_propagation();
}

void ViewerUI::onMouseMovement(const libv::ui::EventMouseMovement& event) {
	if (not isFocused())
		return;

	// <<< P95: app_vm4v: control integration
	controls.input(libv::input::EventMousePosition{libv::vec2d{event.mouse_position}});
	event.stop_propagation();
}

void ViewerUI::onMouseScroll(const libv::ui::EventMouseScroll& event) {
	if (not isFocused())
		return;

	// <<< P95: app_vm4v: control integration
	controls.input(libv::input::EventMouseScroll{libv::vec2d{event.scroll_movement}});
	event.stop_propagation();
}

// -------------------------------------------------------------------------------------------------

void ViewerUI::doAttach() {
	set(property.snapToEdge, libv::ui::SnapToEdge{false});
	set(property.squish, libv::ui::Squish{false});

	{
		auto style = context().style("vm4pv");
		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("bg_shader", context().shaderImage());
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_shader", context().shaderFont());
		style->set("font_size", libv::ui::FontSize{11});
		style->set("quad_shader", context().shaderQuad());
	}

	{
		auto style = context().style("vm4pv.label_version");
		style->set("align", libv::ui::AlignHorizontal::right);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("anchor_target", libv::ui::Anchor{1.0f, 0.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{1.0f, 0.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("vm4pv.info_panel");
		style->set("anchor_target", libv::ui::Anchor{0.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{0.0f, 1.0f, 0.0f});
//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
		style->set("size", libv::ui::parse_size_or_throw("350px, d"));
	}

	{
		auto style = context().style("vm4pv.file_list");
		style->set("align", libv::ui::AlignHorizontal::left);
		style->set("align_vertical", libv::ui::AlignVertical::top);
		style->set("orientation", libv::ui::Orientation::down);
		style->set("anchor_target", libv::ui::Anchor{0.5f, 0.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{0.5f, 0.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
		style->set("font_size", libv::ui::FontSize{11});
	}

	{
		auto style = context().style("vm4pv.label_help");
		style->set("align", libv::ui::AlignHorizontal::right);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("anchor_target", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("overlay.notification");
		style->set("align", libv::ui::AlignHorizontal::center);
		style->set("align_vertical", libv::ui::AlignVertical::top);
		style->set("orientation", libv::ui::Orientation::down);
//		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 35%)"));
//		style->set("bg_shader", context().shaderQuad());
//		style->set("font", context().font("consola.ttf"));
//		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
//		style->set("font_size", libv::ui::FontSize{16});
		style->set("anchor_target", libv::ui::ANCHOR_TOP_CENTER);
		style->set("anchor_parent", libv::ui::ANCHOR_TOP_CENTER);
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	} {
		auto style = context().style("overlay.notification.bubble");
		style->set("align", libv::ui::AlignHorizontal::center);
		style->set("align_vertical", libv::ui::AlignVertical::center);
//		style->set("orientation", libv::ui::Orientation::down);
		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 35%)"));
		style->set("bg_shader", context().shaderQuad());
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{16});
//		style->set("anchor_target", libv::ui::ANCHOR_TOP_CENTER);
//		style->set("anchor_parent", libv::ui::ANCHOR_TOP_CENTER);
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	} {
		auto style = context().style("overlay.notification.text");
//		style->set("align", libv::ui::AlignHorizontal::center);
//		style->set("align_vertical", libv::ui::AlignVertical::top);
//		style->set("orientation", libv::ui::Orientation::down);
//		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 35%)"));
//		style->set("bg_shader", context().shaderQuad());
//		style->set("font", context().font("consola.ttf"));
//		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
//		style->set("font_size", libv::ui::FontSize{16});
//		style->set("anchor_target", libv::ui::ANCHOR_TOP_CENTER);
//		style->set("anchor_parent", libv::ui::ANCHOR_TOP_CENTER);
//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	} {
		auto style = context().style("overlay.shader_error");
		style->set("align", libv::ui::AlignHorizontal::left);
		style->set("align_vertical", libv::ui::AlignVertical::center);
		style->set("orientation", libv::ui::Orientation::down);
//		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 0, 0, 20%)"));
//		style->set("bg_shader", context().shaderQuad());
//		style->set("font", context().font("consola.ttf"));
//		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
//		style->set("font_size", libv::ui::FontSize{12});
		style->set("anchor_target", libv::ui::ANCHOR_CENTER_CENTER);
		style->set("anchor_parent", libv::ui::ANCHOR_CENTER_CENTER);
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	} {
		auto style = context().style("overlay.shader_error.text");
		style->set("align_horizontal", libv::ui::AlignHorizontal::left);
		style->set("align_vertical", libv::ui::AlignVertical::center);
//		style->set("orientation", libv::ui::Orientation::down);
//		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 0, 0, 20%)"));
//		style->set("bg_shader", context().shaderQuad());
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(255, 141, 145, 100%)"));
		style->set("font_shader", context().shaderFont());
		style->set("font_size", libv::ui::FontSize{12});
//		style->set("anchor_target", libv::ui::ANCHOR_CENTER_CENTER);
//		style->set("anchor_parent", libv::ui::ANCHOR_CENTER_CENTER);
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	} {
		auto style = context().style("overlay.shader_inspector");
		style->set("align", libv::ui::AlignHorizontal::left);
		style->set("align_vertical", libv::ui::AlignVertical::center);
		style->set("orientation", libv::ui::Orientation::down);
		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(0, 0, 0, 40%)"));
		style->set("bg_shader", context().shaderQuad());
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{12});
		style->set("anchor_target", libv::ui::ANCHOR_CENTER_RIGHT);
		style->set("anchor_parent", libv::ui::ANCHOR_CENTER_RIGHT);
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	// -------------------------------------------------------------------------------------------------

	{
		info_panel = std::make_shared<ViewerInfoPanel>(*this, "info_panel");
		info_panel->style(context().style("vm4pv.info_panel"));
		add(info_panel);
	} {
		label_version = std::make_shared<libv::ui::Label>(*this, "version");
		label_version->style(context().style("vm4pv.label_version"));
		label_version->text(app::full_version);
		add(label_version);
	} {
		picker_files = std::make_shared<app::ui::QuickFilePicker>(*this, "picker");
		picker_files->style(context().style("vm4pv.file_list"));
		picker_files->event_pick(this, [this](const app::ui::QuickFilePicker::EventPick& event) {
			this->load(event.component.value());
		});
		add(picker_files);
	} {
		label_version = std::make_shared<libv::ui::Label>(*this, "help");
		label_version->style(context().style("vm4pv.label_help"));
		label_version->text(
				"Strafe camera - WASD\n"
				"Lift camera - YH\n"
				"Roll camera - QE\n"
				"Zoom camera - TG\n"
				"Focus camera - F\n"
				"Reset camera - Shift+F"
		);
		add(label_version);
	} {
		overlay_notification = std::make_shared<app::ui::OverlayNotification>(*this);
		overlay_notification->style(context().style("overlay.notification"));
		add(overlay_notification);
	} {
		overlay_shader_error = std::make_shared<app::ui::OverlayShaderError>(*this);
		overlay_shader_error->style(context().style("overlay.shader_error"));
		add(overlay_shader_error);
	} {
		overlay_shader_inspector = std::make_shared<app::ui::OverlayShaderInspector>(*this);
		overlay_shader_inspector->style(context().style("overlay.shader_inspector"));
		add(overlay_shader_inspector);
	}

	watchFocus(true);
	watchKey(true);
	watchMouse(libv::ui::Flag::mask_watchMouse);
}

void ViewerUI::doRender(libv::ui::ContextRender& ctx) {
	ctx.gl.callback([this](libv::gl::GL& gl) {
		const auto success = [this](const app::ShaderReportSuccess& report) {
			auto name = report.shader.name();
			auto ntf_msg = libv::concat("Shader reload success: ", name);

//			this->overlay_notification->info(std::move(ntf_msg), std::chrono::seconds{2});
			this->overlay_shader_error->error_remove(std::move(name));
		};

		const auto failure = [this](const app::ShaderReportFailure& report) {
			auto name = report.shader.name();
			auto ntf_msg = libv::concat("Shader reload failure: ", name, ":\n", report.reason);

//			this->overlay_notification->error(std::move(ntf_msg), std::chrono::seconds{5});
			this->overlay_shader_error->error_add(std::move(name), report.reason);
		};

		this->shader_loader.update(gl, success, failure);
	});

	update(context().state.time_delta());

	scene.render(ctx.gl, layout_size2());
}

// -------------------------------------------------------------------------------------------------

} // namespace app
