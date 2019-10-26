// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/panel_viewer.hpp>
// ext
#include <fmt/format.h>
#include <boost/container/flat_set.hpp>
//#include <range/v3/view/enumerate.hpp>
// libv
#include <libv/parse/color.hpp>
#include <libv/ui/component/image.hpp> // testing only, remove it
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/label_image.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/scroll_bar.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/quad.hpp> // testing only, remove it
#include <libv/ui/component/stretch.hpp> // testing only, remove it
#include <libv/ui/context_ui.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/shader/shader_font.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
#include <libv/ui/shader/shader_image.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
#include <libv/ui/shader/shader_quad.hpp> // There is a way to work around this include when using context().shader<T>, find it and implement it.
#include <libv/utility/generic_path.hpp>
#include <libv/utility/overload.hpp>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/ui/quick_file_picker.hpp>
#include <vm4_viewer/version.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

PanelViewer::PanelViewer(BaseComponent& parent) : libv::ui::PanelFloat(parent, "VM4Viewer") { }

void PanelViewer::key(libv::input::Key key) {
	if (!isAttached())
		return;

	static int16_t i = 11;
	if (key == libv::input::Key::Q) i++;
	if (key == libv::input::Key::A) i--;

	if (picker_files != nullptr)
		picker_files->key(key);

	{
		auto style = context().style("vm4pv.info.bar");
		style->set("font_size", libv::ui::FontSize{i});
	}

	{
		auto style = context().style("vm4pv.button");
		auto v = style->get_or_throw<libv::ui::Size>("size");
		v[0].pixel = (static_cast<float>(i)-11.f) * 2.f;
		v[1].pixel = (static_cast<float>(i)-11.f) * 5.f;
		style->set("size", v);
	}

	log_app.trace("Font size {}", i);
}

void PanelViewer::doAttach() {
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
		auto style = context().style("vm4pv.button");
		style->set("align", libv::ui::AlignHorizontal::Center);
		style->set("align_vertical", libv::ui::AlignVertical::Center);
		style->set("bg_image", context().texture2D("separator_bar_256x16.png"));
		style->set("font", context().font("consola.ttf"));
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(170, 2%, 90%, 100%)"));
		style->set("font_size", libv::ui::FontSize{12});
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
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

	{
		auto style = context().style("vm4pv.info");
		style->set("align", libv::ui::AlignHorizontal::Left);
		style->set("align_vertical", libv::ui::AlignVertical::Top);
		style->set("orientation", libv::ui::Orientation::TOP_TO_BOTTOM);
		style->set("anchor_target", libv::ui::Anchor{0.0f, 1.0f, 0.0f});
		style->set("anchor_parent", libv::ui::Anchor{0.0f, 1.0f, 0.0f});
		style->set("size", libv::ui::parse_size_or_throw("d, 100%"));
	}

	{
		auto style = context().style("vm4pv.scroll-show");
		style->set("align", libv::ui::AlignHorizontal::Left);
		style->set("align_vertical", libv::ui::AlignVertical::Top);
		style->set("orientation", libv::ui::Orientation::LEFT_TO_RIGHT);
		style->set("anchor_target", libv::ui::ANCHOR_CENTER_RIGHT);
		style->set("anchor_parent", libv::ui::ANCHOR_CENTER_RIGHT);
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("vm4pv.scroll-bar");
		style->set("orientation", libv::ui::Orientation::BOTTOM_TO_TOP);
		style->set("size", libv::ui::parse_size_or_throw("8px, 200px"));

		style->set("bg_color", libv::parse::parse_color_or_throw("hsva(120, 30%, 90%, 100%)"));
		style->set("bar_color", libv::parse::parse_color_or_throw("hsva(240, 30%, 90%, 100%)"));

//		style->set("style_arrow", context().style("vm4pv.scroll-bar-fit:arrow"));
//		style->set("style_track", context().style("vm4pv.scroll-bar-fit:track"));
//		style->set("style_bar", context().style("vm4pv.scroll-bar-fit:bar"));

//		style->set("scroll_arrows", true);
//		style->set("scale_bar_relative", true);
//		style->set("min_scroll_size", 22.f);
//		style->set("min_scroll_ratio", 0.f);
	}

	{
		auto style = context().style("vm4pv.scroll-show2");
		style->set("align", libv::ui::AlignHorizontal::Left);
		style->set("align_vertical", libv::ui::AlignVertical::Top);
		style->set("orientation", libv::ui::Orientation::TOP_TO_BOTTOM);
		style->set("anchor_target", libv::ui::ANCHOR_BOTTOM_CENTER);
		style->set("anchor_parent", libv::ui::ANCHOR_BOTTOM_CENTER);
		style->set("size", libv::ui::parse_size_or_throw("d, d"));
	}

	{
		auto style = context().style("vm4pv.scroll-bar2");
		style->set("orientation", libv::ui::Orientation::LEFT_TO_RIGHT);
		style->set("size", libv::ui::parse_size_or_throw("200px, 8px"));

		style->set("bg_color", libv::parse::parse_color_or_throw("hsva(120, 30%, 90%, 100%)"));
		style->set("bar_color", libv::parse::parse_color_or_throw("hsva(240, 30%, 90%, 100%)"));

//		style->set("style_arrow", context().style("vm4pv.scroll-bar-fit:arrow"));
//		style->set("style_track", context().style("vm4pv.scroll-bar-fit:track"));
//		style->set("style_bar", context().style("vm4pv.scroll-bar-fit:bar"));

//		style->set("scroll_arrows", true);
//		style->set("scale_bar_relative", true);
//		style->set("min_scroll_size", 22.f);
//		style->set("min_scroll_ratio", 0.f);
	}

//	{
//		auto style = context().style("vm4pv.scroll-bar:arrow");
//		style->set("size", libv::ui::parse_size_or_throw("22px, 22px"));
//		style->set("image", context().texture2D("separator_bar_256x16.png"));
//	}
//
//	{
//		auto style = context().style("vm4pv.scroll-bar:track");
//		style->set("size", libv::ui::parse_size_or_throw("1r, 1r"));
//		style->set("image", context().texture2D("separator_bar_256x16.png"));
//	}
//
//	{
//		auto style = context().style("vm4pv.scroll-bar:bar");
//		style->set("image", context().texture2D("separator_bar_256x16.png"));
//	}

//	{
//		auto style = context().style("vm4pv.file_list");
//		style->set("align", libv::ui::AlignHorizontal::Left);
//		style->set("align_vertical", libv::ui::AlignVertical::Top);
//		style->set("orientation", libv::ui::Orientation::TOP_TO_BOTTOM);
//		style->set("anchor_parent", libv::ui::Anchor{0.5f, 0.5f, 0.0f});
//		style->set("anchor_target", libv::ui::Anchor{0.5f, 0.5f, 0.0f});
//		style->set("font_size", libv::ui::FontSize{11});
//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry");
//		style->set("font_size", libv::ui::FontSize{11});
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry.selected");
//		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 0%, 100%, 100%)"));
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry.vm");
//		style->set("font_color", libv::parse::parse_color_or_throw("hsva(120, 22%, 90%, 100%)"));
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry.importable");
//		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 22%, 90%, 100%)"));
//	}
//
//	{
//		auto style = context().style("vm4pv.file_list.entry.other");
//		style->set("font_color", libv::parse::parse_color_or_throw("hsva(0, 0%, 50%, 100%)"));
//	}

//	{
//		auto style = context().style("vm4pv.info.separator");
//		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
//		style->set("bg_image", context().texture2D("separator_bar_256x16.png"));
//		style->set("size", libv::ui::parse_size_or_throw("1r, 8px"));
//	}

	{
		auto style = context().style("vm4pv.info.bar");
		style->set("align", libv::ui::AlignHorizontal::Center);
		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("bg_image", context().texture2D("separator_bar_256x16.png"));
		style->set("font_color", libv::parse::parse_color_or_throw("hsva(170, 2%, 90%, 100%)"));
		style->set("font_size", libv::ui::FontSize{11});
		style->set("size", libv::ui::parse_size_or_throw("1r D, D"));
	}

//	{
//		auto style = context().style("vm4pv.info.title");
//		style->set("font_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
//		style->set("font_size", libv::ui::FontSize{12});
//		style->set("size", libv::ui::parse_size_or_throw("d, d"));
//	}
//
//	{
//		auto style = context().style("vm4pv.info.border");
//		style->set("color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
//		style->set("image_shader", context().shaderImage());
//		style->set("image", context().texture2D("stretch_border.png"));
//		style->set("size", libv::ui::parse_size_or_throw("1r, 1r"));
//	}

	{
		auto style = context().style("vm4pv.info.slim_border");
		style->set("bg_color", libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
		style->set("bg_image", context().texture2D("slate_line_edge.png"));
		style->set("bg_shader", context().shaderImage());
		style->set("size", libv::ui::parse_size_or_throw("1r, 1r"));
	}

	// -------------------------------------------------------------------------------------------------

	{
		label_version = std::make_shared<libv::ui::Label>(*this, "version");
		label_version->style(context().style("vm4pv.label_version"));
		label_version->text(app::full_version);
		add(label_version);
	}

	{
		picker_files = std::make_shared<app::ui::QuickFilePicker>(*this, "picker");
		picker_files->style(context().style("vm4pv.file_list"));
		add(picker_files);
	}

	{
		auto tmp_panel = std::make_shared<libv::ui::PanelLine>(*this, "scroll-show");
		tmp_panel->style(context().style("vm4pv.scroll-show"));
		add(tmp_panel);

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(+110); tmp->value_min(+10); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(+110); tmp->value_min(-10); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(-110); tmp->value_min(+10); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(-110); tmp->value_min(-10); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(+10); tmp->value_min(+110); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(+10); tmp->value_min(-110); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(-10); tmp->value_min(+110); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(-10); tmp->value_min(-110); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(100); tmp->value_min(100); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(0); tmp->value_min(0); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(100); tmp->value_min(0); tmp->value_range(0);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(100); tmp->value_min(0); tmp->value_range(200);
			tmp_panel->add(tmp);
		}
		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(0); tmp->value_min(100); tmp->value_range(0);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(0); tmp->value_max(0); tmp->value_min(100); tmp->value_range(200);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(10); tmp->value_max(210); tmp->value_min(10); tmp->value_range(20); tmp->value_step(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar"));
			tmp->value(10); tmp->value_max(200); tmp->value_min(0); tmp->value_range(20); tmp->value_step(20);
			tmp_panel->add(tmp);
		}
	}

	{
		auto tmp_panel = std::make_shared<libv::ui::PanelLine>(*this, "scroll-show2");
		tmp_panel->style(context().style("vm4pv.scroll-show2"));
		add(tmp_panel);

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(5); tmp->value_max(10); tmp->value_min(0); tmp->value_range(0);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(+110); tmp->value_min(+10); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(+110); tmp->value_min(-10); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(-110); tmp->value_min(+10); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(-110); tmp->value_min(-10); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(+10); tmp->value_min(+110); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(+10); tmp->value_min(-110); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(-10); tmp->value_min(+110); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(-10); tmp->value_min(-110); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(100); tmp->value_min(100); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(0); tmp->value_min(0); tmp->value_range(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(100); tmp->value_min(0); tmp->value_range(0);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(100); tmp->value_min(0); tmp->value_range(200);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(0); tmp->value_min(100); tmp->value_range(0);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(0); tmp->value_max(0); tmp->value_min(100); tmp->value_range(200);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(10); tmp->value_max(210); tmp->value_min(10); tmp->value_range(20); tmp->value_step(20);
			tmp_panel->add(tmp);
		}

		{
			auto tmp = std::make_shared<libv::ui::ScrollBar>(*this, "scroll-bar");
			tmp->style(context().style("vm4pv.scroll-bar2"));
			tmp->value(10); tmp->value_max(200); tmp->value_min(0); tmp->value_range(20); tmp->value_step(20);
			tmp_panel->add(tmp);
		}
	}

	{
		label_version = std::make_shared<libv::ui::Label>(*this, "help");
		label_version->style(context().style("vm4pv.label_help"));
		label_version->text(
				"Strafe camera - WASD\n"
				"Lift camera - RF\n"
				"Roll camera - QE\n"
				"Zoom camera - TG"
		);
		add(label_version);
	}

	{
		panel_left = std::make_shared<libv::ui::PanelLine>(*this, "info");
		panel_left->style(context().style("vm4pv.info"));
		add(panel_left);

		{
			button_close = std::make_shared<libv::ui::Button>(*panel_left, "close");
			button_close->style(context().style("vm4pv.button"));
			button_close->text("Close");
			button_close->event_mouseButton(this, [this](const libv::ui::Button::EMouseButton& event) {
				log_app.trace("Close Click {} {}", libv::input::to_string(event.button),  libv::input::to_string(event.action));
			});
			button_close->event_mouseMovement(this, [this](const libv::ui::Button::EMouseMovement& event) {
				log_app.trace("Close Movement {} {} {} {}", event.mouse_position, event.mouse_movement, event.enter, event.leave);
			});
			button_close->event_mouseScroll(this, [this](const libv::ui::Button::EMouseScroll& event) {
				log_app.trace("Close Scroll {} {}", event.scroll_position, event.scroll_movement);
			});
			button_close->event_submit(this, [this](const libv::ui::Button::ESubmit&) {
				log_app.trace("Close Submit");
			});

			panel_left->add(button_close);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>(*panel_left, "bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->text("Project");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>(*panel_left, "border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>(*panel_left, "bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->text("File");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>(*panel_left, "border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Button>(*panel_left, "button1");
			temp->style(context().style("vm4pv.button"));
			temp->text("Button1");
			temp->event_mouseButton(this, [this](const libv::ui::Button::EMouseButton& event) {
				log_app.trace("Button1 Click {} {}", libv::input::to_string(event.button),  libv::input::to_string(event.action));
			});
			temp->event_mouseMovement(this, [this](const libv::ui::Button::EMouseMovement& event) {
				log_app.trace("Button1 Movement {} {} {} {}", event.mouse_position, event.mouse_movement, event.enter, event.leave);
			});
			temp->event_mouseScroll(this, [this](const libv::ui::Button::EMouseScroll& event) {
				log_app.trace("Button1 Scroll {} {}", event.scroll_position, event.scroll_movement);
			});
			temp->event_submit(this, [this](const libv::ui::Button::ESubmit&) {
				log_app.trace("Button1 Submit");
			});

			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>(*panel_left, "bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->text("Model");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>(*panel_left, "border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>(*panel_left, "bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->text("Node");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>(*panel_left, "border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::LabelImage>(*panel_left, "bar");
			temp->style(context().style("vm4pv.info.bar"));
			temp->text("Material");
			panel_left->add(temp);
		} {
			auto temp = std::make_shared<libv::ui::Stretch>(*panel_left, "border");
			temp->style(context().style("vm4pv.info.slim_border"));
			panel_left->add(temp);
		}

//		{
//			auto temp = std::make_shared<libv::ui::Label>("text");
//			temp->set(temp->property.align, libv::ui::AlignHorizontal::Right);
//			temp->set(temp->property.font, context().font("consola.ttf"));
//			temp->set(temp->property.font_color, libv::parse::parse_color_or_throw("rgba(255, 255, 255, 100%)"));
//			temp->set(temp->property.font_shader, context().shaderFont());
//			temp->set(temp->property.font_size, libv::ui::FontSize{12});
//			temp->text("Model");
//
//			auto& child = panel_left->add(temp);
//			child.ptr->set(child.property.size, libv::ui::parse_size_or_throw("d, d"));
//		}
//
//		{
//			auto temp = std::make_shared<libv::ui::Quad>();
//			temp->set(temp->property.color, libv::parse::parse_color_or_throw("hsva(120°, 30%, 100%, 50%)"));
//
//			auto& child = panel_left->add(temp);
//			child.ptr->set(child.property.size, libv::ui::parse_size_or_throw("100%, 50px"));
//		}
//
//		{
//			auto temp = std::make_shared<libv::ui::Image>("separator");
//			temp->style(context().style("vm4pv.info.separator"));
//			panel_left->add(temp);
//		}
//
//		{
//			auto temp = std::make_shared<libv::ui::Stretch>("border");
//			temp->style(context().style("vm4pv.info.border"));
//			panel_left->add(temp);
//		}
	}
}

void PanelViewer::init() { }

// TODO P5: libv.vm4: include BSO and BSR for each node(?) / LOD(?) / mesh(?) / model
// TODO P5: libv.vm4: include AABB for each node(?) / LOD(?) / mesh(?) / model
//
//void PanelViewer::find_bounding_sphere() {
//	// Find (BSO) Bounding sphere origin and (BSR) Bounding sphere radius
//	const auto& node = scene.model.nodes[0];
//
//	auto referencePointW = libv::vec3f{};
//	auto referenceDistanceWSQ = 0.f;
//
//	for (const auto& meshID : node.meshIDs) {
//		const auto& mesh = scene.model.meshes[meshID];
//		const auto indexBegin = mesh.baseIndex;
//		const auto indexEnd = mesh.baseIndex + mesh.numIndices;
//
//		for (size_t index = indexBegin; index < indexEnd; ++index) {
//			const auto vertexID = scene.model.indices[index] + mesh.baseVertex;
//			const auto vertexPositionM = scene.model.vertices[vertexID].position;
//			const auto vertexPositionW = parentTransformation * node.transformation * vertexPositionM;
//
//			const auto vertexDistanceWSQ = (vertexPositionW - referencePointW).lengthSQ();
//			if (vertexDistanceWSQ > referenceDistanceWSQ) {
//				referencePointW
//
//			}
//		}
//	}
//
//	for (const auto& childID : node.childrenIDs)
//		referencePoint = findFarestVertex(scene.model.nodes[childID], referencePoint);
//
////	for (const auto& node : scene.model.nodes)
////	for (const auto& node : scene.model.nodes)
//}

void PanelViewer::update_model() {
	//	libv::erase(config.recent_projects, project);
	//	config.recent_projects.emplace_back(project);
//	scene.model;
}

void PanelViewer::update_camera() {
//	scene.camera;
}

void PanelViewer::update_ui() {
//	this->in
}

void PanelViewer::update_info() {
//	//		panel_info.property.set(context().style("vm4_viewer.info"));
//
//	label_name.property.set(context().style("vm4_viewer.info.name"));
//	label_name.text(fmt::format(
//			"Name: {}",
//			scene.model.name
//	));
//
//	label_file.property.set(context().style("vm4_viewer.info.file"));
//	label_file.text(fmt::format(
//			"File:      {}"
//			"Version:   {}"
//			"Hash:      {}"
//			"Last edit: {}",
//			// TODO P5: Implement vm4 viewer addition file information display
//			"Not Implemented Yet",
//			"Not Implemented Yet",
//			"Not Implemented Yet",
//			"Not Implemented Yet"
//	));
//
//	label_info.property.set(context().style("vm4_viewer.info.info"));
//	label_info.text(fmt::format(
//			"Vertex count:       {:9}\n"
//			"Index count:        {:9}\n"
//			"LOD count:          {:9}\n"
//			"Material count:     {:9}\n"
//			"Node count:         {:9}\n"
//			"Mesh count:         {:9}\n"
//			"Animation count:    {:9}\n"
//			"Anim.Channel count: {:9}",
//			scene.model.vertices.size(),
//			scene.model.indices.size(),
//			scene.model.lods.size(),
//			scene.model.materials.size(),
//			scene.model.nodes.size(),
//			scene.model.meshes.size(),
//			scene.model.animations.size(),
//			scene.model.animationChannels.size()
//	));
//
//	int i = 0;
//	label_lod.clear();
//	for (const auto& lod : scene.model.lods) {
//		auto& label = label_lod.emplace_back();
//
//		label.property.set(context().style("vm4_viewer.info.lod"));
//		label.text(fmt::format(
//				"Level: {}\n"
//				"Range far:   {:12.6f}\n"
//				"Range near:  {:12.6f}\n"
//				"Root nodeID: {}\n"
//				"Root node:   {}",
//				// TODO P5: Implement grid layout (? or grid text component; ? or expanding space text element)
//				// TODO P5: Implement component link, clicking root node should jump to node info
//				i++,
//				lod.rangeFar,
//				lod.rangeNear,
//				lod.rootNodeID,
//				scene.model.nodes[lod.rootNodeID].name
//		));
//	}
//
//	i = 0;
//	label_material.clear();
//	for (const auto& material : scene.model.materials) {
//		auto& label = label_material.emplace_back();
//
//		label.property.set(context().style("vm4_viewer.info.material"));
//
//		std::ostringstream ss;
//		ss << fmt::format(
//				"ID: {}\n"
//				"Name: {}\n"
//				"Shader: {}\n",
//				i++,
//				material.name,
//				material.shader
//		);
//		for (const auto& [key, data] : material.property) {
//			std::visit([&key, &ss](const auto& value) {
//				ss << fmt::format("\t{:20}: {}\n", key, value);
//			}, data);
//		}
//
//		label.text(std::move(ss).str());
//	}
//


	// =================================================================================================

	//		for (const auto& [level, lod] : scene.model.lods | ranges::view::enumerate) {
	//			auto& label = label_lod.emplace_back();
	//			label.text(fmt::format(
	//					"Level: {}\n"
	//					"Range far:   {:12.6f}\n"
	//					"Range near:  {:12.6f}\n"
	//					"Root nodeID: {}\n",
	//					"Root node:   {}",
	//					// TODO P5: Implement grid layout (? or grid text component)
	//					// TODO P5: Implement component link, clicking root node should jump to node info
	//					level,
	//					lod.rangeFar,
	//					lod.rangeNear,
	//					lod.rootNodeID,
	//					scene.model.nodes[lod.rootNodeID].name
	//			));
	//		}

	//		for (const auto& material : scene.model.materials) {
	//			material.name;
	//			material.property.size();
	//			material.shader;
	//		}
	//
	//		for (const auto& node : scene.model.nodes) {
	//			node.name;
	//			node.parentID;
	//			node.childrenIDs.size();
	//			scene.model.nodes[node.parentID];
	//			for (const auto& childID : node.childrenIDs) {
	//				scene.model.nodes[childID];
	//			}
	//			node.meshIDs.size();
	//			for (const auto& meshID : node.meshIDs) {
	//				scene.model.meshes[meshID];
	//			}
	//			node.transformation;
	//		}
	//
	//		for (const auto& mesh : scene.model.meshes) {
	//			mesh.name;
	//			mesh.baseIndex;
	//			mesh.baseVertex;
	//			mesh.materialID;
	//			scene.model.materials[mesh.materialID];
	//			mesh.numIndices;
	//		}
	//
	//		for (const auto& animation : scene.model.animations) {
	//			(void) animation;
	//		}
	//
	//		for (const auto& animationChannel : scene.model.animationChannels) {
	//			(void) animationChannel;
	//		}
}


//void PanelViewer::create() {
//	add(panel_left);
//	add(panel_help);
//
//	panel_left->add(label_info);
//	label_info->text("Hello world!");
//	label_info->property.font

//	const auto style_label_01 = ui.context().style("style-label-01");
//	style_label_01->set("color", libv::parse::parse_color_or_throw("rgba(167, 152, 120, 100%)"));
//	style_label_01->set("font", ui.context().font("consola.ttf"));
//	style_label_01->set("align", libv::ui::Anchor::Left);
//
//	label0->property.set(style_label_01);
//	label0->text("Hello, Label0!");
//	label1->property.set(style_label_01);
//	label1->text("Hello, Label1!");
//	label2->property.set(style_label_01);
//	label2->text("Hello, Label2!");
//	label2->propertySize = libv::ui::parse_size_or_throw("dynamic, dynamic");
//
//	quad0->color(libv::parse::parse_color_or_throw("rgba(134, 189, 111, 80%)"));
//	stretch0->image(ui.context().texture2D("stretch_border.png"));
//	stretch0->color(libv::parse::parse_color_or_throw("rgba(183, 190, 135, 100%)"));
//	image0->image(ui.context().texture2D("atlas_ui_slate.png"));
//	image0->propertySize = libv::ui::parse_size_or_throw("dynamic, dynamic");

//		panel0->layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		panel0->layout.alignVertical = libv::ui::Alignment::CENTER;
//		panel0->layout.orient = libv::ui::Orientation::TOP_TO_BOTTOM;
//		panel0->add(label0);
//		panel0->add(image0);
//		panel0->add(panel1);
//		panel0->add(label1);
//
//		panel1->layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		panel1->layout.alignVertical = libv::ui::Alignment::CENTER;
//		panel1->layout.orient = libv::ui::Orientation::LEFT_TO_RIGHT;
//		panel1->add(quad0);
//		panel1->add(label2);
//		panel1->add(stretch0);
//
//		ui.add(panel0);
//}
//
//void PanelViewer::render() {
//}
//
//void PanelViewer::destroy() {
//}

// -------------------------------------------------------------------------------------------------

} // namespace app
