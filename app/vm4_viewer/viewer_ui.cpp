// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/viewer_ui.hpp>
// libv
#include <libv/input/event.hpp>
#include <libv/parse/color.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/context_render.hpp>
#include <libv/ui/context_state.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
#include <libv/vm4/load.hpp>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/ui/quick_file_picker.hpp>
#include <vm4_viewer/version.hpp>
#include <vm4_viewer/viewer_info_panel.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

ViewerUI::ViewerUI(BaseComponent& parent) : libv::ui::PanelFloat(parent, "VM4Viewer") {
	scene.camera.translateZoom(-15.f);
	scene.camera.rotateX(libv::to_rad(45.f));
	scene.camera.rotateZ(libv::to_rad(45.f));
}

void ViewerUI::update(libv::ui::time_duration elapsed_time_t) {
	if (not isFocused())
		return;

	const auto elapsed_time = static_cast<float>(elapsed_time_t.count());
	const auto zoom = -scene.camera.zoom();

	const auto keyboard_spin_speed = libv::to_rad(60.f); // 60°/s
	const auto keyboard_move_speed = zoom * 0.5f + 1.0f;
	const auto keyboard_zoom_scale = zoom * 0.2f + 0.1f;

	if (context().state.key_pressed(libv::input::Key::W))
		scene.camera.rotateX(keyboard_spin_speed * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::S))
		scene.camera.rotateX(-keyboard_spin_speed * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::A))
		scene.camera.rotateZ(keyboard_spin_speed * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::D))
		scene.camera.rotateZ(-keyboard_spin_speed * elapsed_time);

	if (context().state.key_pressed(libv::input::Key::Q))
		scene.camera.rotateY(keyboard_spin_speed * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::E))
		scene.camera.rotateY(-keyboard_spin_speed * elapsed_time);

	if (context().state.key_pressed(libv::input::Key::Up))
		scene.camera.translateY(-keyboard_move_speed * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::Down))
		scene.camera.translateY(keyboard_move_speed * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::Right))
		scene.camera.translateX(keyboard_move_speed * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::Left))
		scene.camera.translateX(-keyboard_move_speed * elapsed_time);

	if (context().state.key_pressed(libv::input::Key::Y))
		scene.camera.translateZ(keyboard_move_speed * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::H))
		scene.camera.translateZ(-keyboard_move_speed * elapsed_time);

	if (context().state.key_pressed(libv::input::Key::T))
		scene.camera.translateZoom(-keyboard_zoom_scale * elapsed_time);
	if (context().state.key_pressed(libv::input::Key::G))
		scene.camera.translateZoom(+keyboard_zoom_scale * elapsed_time);
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

bool ViewerUI::onKey(const libv::input::EventKey& event) {
	// TODO P0: Proper mouse shield and focus travers (remove two line below)
	if (event.key == libv::input::Key::Tab)
		focus();

	if (not isFocused())
		return false;

	if (event.key == libv::input::Key::F && (event.mods & libv::input::KeyModifier::shift) != libv::input::KeyModifier::none)
		scene.reset_camera();
	else if (event.key == libv::input::Key::F)
		scene.focus_camera();

	log_app.info("ViewerUI Key {} {} {} {}", libv::input::to_string(event.action), libv::input::to_string(event.key), libv::to_value(event.mods), event.scancode);
	return true;
}

void ViewerUI::onFocus(const libv::ui::EventFocus& event) {
	log_app.info("ViewerUI Focus {}", event.focus);
}

bool ViewerUI::onMouseButton(const libv::ui::EventMouseButton& event) {
	// TODO P0: proper mouse shielding, and gain focus here too
	//	focus();

	log_app.info("ViewerUI Button {} {}", libv::input::to_string(event.action), libv::input::to_string(event.button));
	return true;
}

bool ViewerUI::onMouseMovement(const libv::ui::EventMouseMovement& event) {
	if (not isFocused())
		return false;

	log_app.info("ViewerUI Movement {} {} {} {}", event.enter, event.leave, event.mouse_movement, event.mouse_position);

	const auto zoom = -scene.camera.zoom();
	const auto mouse_spin_speed = libv::to_rad(60.f) * 0.0025f; // 60°/400px
	const auto mouse_move_speed = zoom * 0.0006f + 0.05f;

	if (context().state.mouse_pressed(libv::input::Mouse::Left) && context().state.mouse_pressed(libv::input::Mouse::Right)) {
		scene.camera.translateZoom(-event.mouse_movement.y * mouse_move_speed);

	} else if (context().state.mouse_pressed(libv::input::Mouse::Right)) {
		scene.camera.translateX(-event.mouse_movement.x * mouse_move_speed);
		scene.camera.translateY(event.mouse_movement.y * mouse_move_speed);

	} else if (context().state.mouse_pressed(libv::input::Mouse::Left)) {
		scene.camera.rotateZ(event.mouse_movement.x * mouse_spin_speed);
		scene.camera.rotateX(-event.mouse_movement.y * mouse_spin_speed);
	}

	return true;
}

bool ViewerUI::onMouseScroll(const libv::ui::EventMouseScroll& event) {
	if (not isFocused())
		return false;

	log_app.info("ViewerUI Scroll {} {}", event.scroll_movement, event.scroll_position);

	const auto zoom = -scene.camera.zoom();
	const auto mouse_scroll_scale = zoom * 0.08f + 0.1f;

	scene.camera.translateZoom(event.scroll_movement.y * mouse_scroll_scale);

	return true;
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
		style->set("align", libv::ui::AlignHorizontal::Right);
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
		style->set("align", libv::ui::AlignHorizontal::Left);
		style->set("align_vertical", libv::ui::AlignVertical::Top);
		style->set("orientation", libv::ui::Orientation::TOP_TO_BOTTOM);
		style->set("anchor_target", libv::ui::Anchor{0.5f, 0.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{0.5f, 0.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
		style->set("font_size", libv::ui::FontSize{11});
	}

	{
		auto style = context().style("vm4pv.label_help");
		style->set("align", libv::ui::AlignHorizontal::Right);
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("rgba(187, 191, 195, 75%)"));
		style->set("font_size", libv::ui::FontSize{10});
		style->set("anchor_target", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{1.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	// -------------------------------------------------------------------------------------------------

	{
		info_panel = std::make_shared<ViewerInfoPanel>(*this, "info_panel");
		info_panel->style(context().style("vm4pv.info_panel"));
		add(info_panel);
	}

	{
		label_version = std::make_shared<libv::ui::Label>(*this, "version");
		label_version->style(context().style("vm4pv.label_version"));
		label_version->text(app::full_version);
		add(label_version);
	}

	{
		picker_files = std::make_shared<app::ui::QuickFilePicker>(*this, "picker");
		picker_files->style(context().style("vm4pv.file_list"));
		picker_files->event_pick(this, [this](const app::ui::QuickFilePicker::EventPick& event) {
			this->load(event.component.value());
		});
		add(picker_files);
	}

	{
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
	}

	watchFocus(true);
	watchKey(true);
	watchMouse(libv::ui::Flag::mask_watchMouse);
}

void ViewerUI::doRender(libv::ui::ContextRender& ctx) {
	update(context().state.time_delta());
	scene.render(ctx.gl, size2());
}

// -------------------------------------------------------------------------------------------------

} // namespace app
