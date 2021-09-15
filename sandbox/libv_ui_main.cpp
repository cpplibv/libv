// Project: libv.ui, File: sandbox/libv_ui_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/frame/frame.hpp>
//#include <libv/input/event.hpp>
//#include <libv/input/input.hpp>
#include <libv/log/log.hpp>
//#include <libv/parse/color.hpp>
// std
#include <iostream>
// pro
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/image.hpp>
#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_full.hpp>
#include <libv/ui/component/panel_grid.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/panel_status_line.hpp>
#include <libv/ui/component/scroll_bar.hpp>
#include <libv/ui/component/scroll_pane.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/ui.hpp>
//#include <libv/ui/style.hpp>

#include <libv/ui/attach_state.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

class SandboxFrame : public libv::Frame {
private:
	libv::ui::UI ui;

private:
	libv::ui::Button button0;
	libv::ui::Button button1;
	libv::ui::Button button2;
	libv::ui::Button button3;
	libv::ui::Button button4;
	libv::ui::Button button5;
	libv::ui::Button button;
	libv::ui::Image image;
	libv::ui::InputField input_field0;
	libv::ui::InputField input_field1;
	libv::ui::InputField input_field2;
	libv::ui::Label label;
	libv::ui::Label label_image1;
	libv::ui::Label label_image2;
	libv::ui::PanelFloat panel_float;
	libv::ui::PanelFull panel_full;
	libv::ui::PanelGrid panel_grid;
	libv::ui::PanelLine panel_line;
	libv::ui::PanelLine panel_line_scrolled;
	libv::ui::Image stretch;
	libv::ui::Image quad;
	libv::ui::ScrollBar scroll_bar_x;
	libv::ui::ScrollBar scroll_bar_y;
	libv::ui::ScrollBar scroll_bar_ix;
	libv::ui::ScrollBar scroll_bar_iy;
	libv::ui::ScrollArea scroll_area_outer;
	libv::ui::ScrollArea scroll_area_inner;
	libv::ui::PanelStatusLine status_log;

public:
	SandboxFrame() :
		Frame("UI Sandbox", 1280, 800) {
		setPosition(FramePosition::center_current_monitor);
		setOpenGLProfile(OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(OpenGLSamples{4});
		ui.attach(*this);

		//

		button.text("Hello World!");
		button.event().submit([](const libv::ui::EventSubmit&) {
			log_sandbox.info("Button submitted");
		});
		button.event().submit([](libv::ui::Button& component, const libv::ui::EventSubmit& event) {
			(void) event;
			log_sandbox.info("Button pressed {}", component.path());
			component.text(component.text() + ".");
			event.stop_propagation(); // stop propagation so 'connect' handler is not called
		});
		button.event().submit.connect([](const libv::ui::EventSubmit&) {
			log_sandbox.info("connect"); // never called as propagation in stopped
		});
		button.event().submit.connect_system([](const libv::ui::EventSubmit&) {
			log_sandbox.info("connect_system"); // always called as system handler
		});
		button.event().submit.connect_system_front([](const libv::ui::EventSubmit&) {
			log_sandbox.info("connect_system_front"); // always called as system handler
		});

		button0.text("Button 0!");
		button1.text("Button 1!");
		button1.background(libv::ui::Background::texture({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D("separator_bar_256x16.png")));

		button2.background(libv::ui::Background::color({1.f, 1.f, 1.f, 0.4f}));
		button2.text("Button 2!");
		button2.anchor(libv::ui::Anchor::top_left);
		button2.size(libv::ui::parse_size_or_throw("33%, 33%"));

		button3.background(libv::ui::Background::color({1.f, 1.f, 1.f, 0.4f}));
		button3.text("Button 3!");
		button3.anchor(libv::ui::Anchor::bottom_right);
		button3.size(libv::ui::parse_size_or_throw("33%, 33%"));

		button4.background(libv::ui::Background::color({1.f, 1.f, 1.f, 0.4f}));
		button4.text("Button 4!");
		button4.size(libv::ui::parse_size_or_throw("D, D"));
		button4.background(libv::ui::Background::texture({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D("separator_bar_256x16.png")));

		label.text("Label");
		label.align_horizontal(libv::ui::AlignHorizontal::center);
		label.align_vertical(libv::ui::AlignVertical::center);

		label_image1.text("Label image");
		label_image1.background(libv::ui::Background::texture({1.f, 1.f, 1.f, 1.0f}, ui.context().texture2D("separator_bar_256x16.png")));

		label_image2.text("Label image2");
		label_image1.background(libv::ui::Background::texture({1.f, 1.f, 1.f, 0.5f}, ui.context().texture2D("separator_bar_256x16.png")));

		image.background(libv::ui::Background::texture({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D("separator_bar_256x16.png")));
		image.size(libv::ui::parse_size_or_throw("25%, 50px"));

		input_field0.background(libv::ui::Background::texture({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D("gray256.png")));
		input_field1.background(libv::ui::Background::texture({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D("gray256.png")));
		input_field2.background(libv::ui::Background::texture({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D("gray256.png")));

		input_field0.text("Input field 0");
		input_field0.event().change([](auto& component, const auto&) {
			log_sandbox.info("Input field 0 {} changed to {}", component.path(), component.text());
		});
		input_field0.event().caret([](auto& component, const auto&) {
			log_sandbox.info("Input field 0 {} caret moved to {}", component.path(), component.caret());
		});
		input_field0.event().submit([](auto& component, const auto&) {
			log_sandbox.info("Input field 0 {} submitted", component.path());
		});
		input_field0.size(libv::ui::parse_size_or_throw("D, 50px"));
//		input_field0.size(libv::ui::parse_size_or_throw("D, D")); // <<< P92: cascades nan

		input_field1.text("Input field 1 line 1\nInput field 1 line 2\nInput field 1 line 3\nInput field 1 line 4\nInput field 1 line 5");
		input_field1.event().change([](auto& component, const auto&) {
			log_sandbox.info("Input field 1 {} changed to {}", component.path(), component.text());
		});
		input_field1.event().caret([](auto& component, const auto&) {
			log_sandbox.info("Input field 1 {} caret moved to {}", component.path(), component.caret());
		});
		input_field1.event().submit([](auto& component, const auto&) {
			log_sandbox.info("Input field 1 {} submitted", component.path());
		});
		input_field1.size(libv::ui::parse_size_or_throw("D, D"));

		input_field2.text("Input field 2");
		input_field2.event().change([](auto& component, const auto&) {
			log_sandbox.info("Input field 2 {} changed to {}", component.path(), component.text());
		});
		input_field2.event().caret([](auto& component, const auto&) {
			log_sandbox.info("Input field 2 {} caret moved to {}", component.path(), component.caret());
		});
		input_field2.event().submit([](auto& component, const auto&) {
			log_sandbox.info("Input field 2 {} submitted", component.path());
		});
		input_field2.size(libv::ui::parse_size_or_throw("D, D"));

		stretch.background(libv::ui::Background::border({1.f, 1.f, 1.f, 1.f}, ui.context().texture2D("stretch_border.png")));

		quad.background(libv::ui::Background::color({.8f, .5f, .5f, 1.f}));
		quad.anchor(libv::ui::Anchor::center_center);
		quad.size(libv::ui::parse_size_or_throw("0.5r, 0.5r"));

		panel_line_scrolled.orientation(libv::ui::Orientation::down);
		panel_line_scrolled.anchor(libv::ui::Anchor::top_left);

		scroll_bar_x.bar_image(ui.context().texture2D("separator_bar_256x16.png"));
		scroll_bar_x.bar_color({0.8f, 0.2f, 0.2f, 1.0f});
		scroll_bar_x.orientation(libv::ui::Orientation::right);
		scroll_bar_x.value_min(-500);
		scroll_bar_x.value_max(500);
		scroll_bar_x.value(0);
		scroll_bar_x.event().change([this](auto& component, const auto& event) {
			log_sandbox.info("Scroll bar X {} changed to {} with change {}", component.path(), component.value(), event.change);
			scroll_area_outer.area_position({static_cast<float>(component.value()), scroll_area_outer.area_position().y});
		});

		scroll_bar_y.bar_image(ui.context().texture2D("separator_bar_256x16.png"));
		scroll_bar_y.bar_color({0.2f, 0.8f, 0.2f, 1.0f});
		scroll_bar_y.orientation(libv::ui::Orientation::up);
		scroll_bar_y.value_min(-1000);
		scroll_bar_y.value_max(1000);
		scroll_bar_y.value(0);
		scroll_bar_y.event().change([this](auto& component, const auto& event) {
			log_sandbox.info("Scroll bar Y {} changed to {} with change {}", component.path(), component.value(), event.change);
			scroll_area_outer.area_position({scroll_area_outer.area_position().x, static_cast<float>(component.value())});
		});

		scroll_bar_ix.bar_image(ui.context().texture2D("separator_bar_256x16.png"));
		scroll_bar_ix.bar_color({0.8f, 0.2f, 1.0f, 1.0f});
		scroll_bar_ix.orientation(libv::ui::Orientation::right);
		scroll_bar_ix.value_min(-500);
		scroll_bar_ix.value_max(500);
		scroll_bar_ix.value(0);
		scroll_bar_ix.event().change([this](auto& component, const auto& event) {
			log_sandbox.info("Scroll bar X {} changed to {} with change {}", component.path(), component.value(), event.change);
			scroll_area_inner.area_position({static_cast<float>(component.value()), scroll_area_inner.area_position().y});
		});

		scroll_bar_iy.bar_image(ui.context().texture2D("separator_bar_256x16.png"));
		scroll_bar_iy.bar_color({0.2f, 0.8f, 1.0f, 1.0f});
		scroll_bar_iy.orientation(libv::ui::Orientation::up);
		scroll_bar_iy.value_min(-1000);
		scroll_bar_iy.value_max(1000);
		scroll_bar_iy.value(0);
		scroll_bar_iy.event().change([this](auto& component, const auto& event) {
			log_sandbox.info("Scroll bar Y {} changed to {} with change {}", component.path(), component.value(), event.change);
			scroll_area_inner.area_position({scroll_area_inner.area_position().x, static_cast<float>(component.value())});
		});

		scroll_area_outer.content(panel_line_scrolled);
		scroll_area_outer.mode(libv::ui::ScrollAreaMode::vertical);
		scroll_area_outer.size(libv::ui::parse_size_or_throw("50%, 3r"));
		scroll_area_outer.anchor(libv::ui::Anchor::center_center);
		scroll_area_outer.event().area([](auto& component, const auto& event) {
			log_sandbox.info("Scroll area outer size {} changed to {}", event.old_size, component.area_size());
		});

		scroll_area_inner.content(input_field1);
		scroll_area_inner.mode(libv::ui::ScrollAreaMode::vertical);
		scroll_area_inner.size(libv::ui::parse_size_or_throw("50%, 50px"));
		scroll_area_inner.anchor(libv::ui::Anchor::center_center);

		{
			libv::ui::Label tmp;
			tmp.text(
				"And I will strike 0 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 0.\n"
				"And I will strike 1 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 1.\n"
				"And I will strike 2 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 2.\n"
				"And I will strike 3 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 3."
			);
			tmp.size(libv::ui::parse_size_or_throw("D, D"));
			panel_line_scrolled.add(tmp);
		}
		panel_line_scrolled.add(scroll_area_inner);
//		panel_line_scrolled.anchor(libv::ui::Anchor::top_left);
//		panel_line_scrolled.anchor(libv::ui::Anchor::bottom_right);
		panel_line_scrolled.add(input_field2);
		panel_line_scrolled.add(button4);
		{
			libv::ui::Label tmp;
			tmp.text(
				"222222222222trike 0 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 0.\n"
				"222222222222trike 1 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 1.\n"
				"222222222222trike 2 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 2.\n"
				"222222222222trike 3 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 3."
			);
			tmp.size(libv::ui::parse_size_or_throw("D, D"));
			panel_line_scrolled.add(tmp);
		} {
			libv::ui::Label tmp;
			tmp.text(
				"333333333333trike 0 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 0.\n"
				"333333333333trike 1 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 1.\n"
				"333333333333trike 2 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 2.\n"
				"333333333333trike 3 down upon thee with great vengeance and furious anger (those who would attempt to poison and destroy my brothers 3."
			);
			tmp.size(libv::ui::parse_size_or_throw("D, D"));
			panel_line_scrolled.add(tmp);
		}
		panel_line_scrolled.add(image);

		status_log.orientation(libv::ui::Orientation::down);

		button3.event().submit([this] {
			if (button3.anchor() != libv::ui::Anchor::center_center)
				button3.anchor(libv::ui::Anchor::center_center);
			else
				button3.anchor(libv::ui::Anchor::bottom_right);
		});

		button5.text("Fill status log");
		button5.align_horizontal(libv::ui::AlignHorizontal::center);
		button5.align_vertical(libv::ui::AlignVertical::center);
		button5.event().submit(status_log, [this] {
			for (int i = 0; i < 5; ++i) {
				const auto t = std::chrono::seconds(i + 1);
				libv::ui::Button tmp;
				tmp.background(libv::ui::Background::color({0.7f, 0.7f, 0.5f, 1.0f}));
				tmp.align_horizontal(libv::ui::AlignHorizontal::center);
				tmp.align_vertical(libv::ui::AlignVertical::center);
//				tmp.size(libv::ui::parse_size_or_throw("150px, 10px"));
				tmp.margin({static_cast<float>(i) * 15.f, 1, static_cast<float>(i) * 15.f, 3});
				tmp.text(fmt::format("Status entry {} for {}s", i, t.count()));
				tmp.event().submit([](libv::ui::Button& btn) {
					btn.markRemove();
				});
				status_log.add(i, tmp, t);
			}
		});

		panel_full.add(button1);

		panel_float.add(button2);
		panel_float.add(button3);

		panel_grid.column_count(5);
		panel_grid.padding({5, 5, 5, 10});
//		panel_grid.padding({5, 5, 5, 50});
//		panel_grid.padding({5, 5, 5, 5});
		panel_grid.orientation2(libv::ui::Orientation2::RIGHT_DOWN);
		panel_grid.spacing2(libv::ui::Spacing2{1, 2});
		panel_grid.size(libv::ui::parse_size_or_throw("1r, 3r"));

		panel_grid.add(button0);
		panel_grid.add(panel_full);
		panel_grid.add(panel_float);
		panel_grid.add(input_field0);
		panel_grid.add(label_image1);
		panel_grid.add(quad);
		panel_grid.add(stretch);
		panel_grid.add(scroll_bar_x);
		panel_grid.add(scroll_bar_y);
		panel_grid.add(scroll_bar_ix);
		panel_grid.add(scroll_bar_iy);
		panel_grid.add(label);
		panel_grid.add(button5);

		panel_line.orientation(libv::ui::Orientation::down);
//		panel_line.orientation(libv::ui::Orientation::up);
		panel_line.padding({20, 10, 20, 10});
//		panel_line.padding({5, 5, 5, 5});
		panel_line.spacing(libv::ui::Spacing{6});
//		panel_line.spacing(libv::ui::Spacing{2});
		panel_line.add(panel_grid);
		panel_line.add(status_log);
		panel_line.add(button);
		panel_line.add(scroll_area_outer);
		panel_line.add(label_image2);

		struct State {
			libv::ui::PanelLine component;

			explicit State(libv::ui::PanelLine component) :
				component(std::move(component)) {
			}
		};
		libv::ui::attach_state<State>(panel_line)(panel_line);

		ui.add(panel_line);


		// -------------------------------------------------------------------------------------------------

//		This was a test case for padding_pattern background

//		ui.style("space.hud-bar.mp.panel") {
//			anchor = "top_center",
//					orientation = "right",
//					size = "D, D",
//					spacing = 5,
//					margin = 5,
//
//					orientation = "down",
//					align_horizontal = "center",
//					align_vertical = "center",
//		}
//
//		ui.style("space.hud-bar.mp.iris-lbl") {
//			align_horizontal = "center",
//					--align_horizontal = "left",
//					--align_horizontal = "right",
//					--align_horizontal = "justify",
//					--align_horizontal = "justify-all",
//					align_vertical = "center",
//					--align_vertical = "bottom",
//					--align_vertical = "top",
//					--align_vertical = "justify",
//					--align_vertical = "justify-all",
//					font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
//					--background = "color: red";
//			--background = "color:" .. colors.hud_light_bg_color;
//			--background = "texture: atlas_ui_slate.png";
//			--background = "texture: hexagon_metal_0001_diffuse.dds";
//			--background = "pattern: hexagon_metal_0001_diffuse.dds";
//			--background = "texture: separator_bar_256x16.png";
//			--background = "texture: stretch_border.png";
//			--background = "border: stretch_border.png";
//			--background = "pattern: stretch_border.png";
//			--background = "padding_pattern: padding_stripes.png";
//			background = "padding_pattern: stripes.png";
//			--size = "200px, 200px",
//					--size = "100px, 100px",
//					--padding = { 40, 7 },
//					--padding = {  0, 7 },
//					--size = "6px D 1r, 10px D",
//					--size = "6px D 1r, 16px",
//					--size = "6px D 1r, 19px",
//
//					--size = "6px D 1r, 1px D",
//					--size = "6px D 1r, 2px D",
//					--size = "6px D 1r, 4px D",
//					size = "6px D 1r, 8px D",
//
//					--size = "6px D 1r, D",
//					--size = "6px D 1r, 20px",
//					--size = "4pxD, D",
//		}

//		libv::ui::Label lbl_iris("mp-title-iris");
//		lbl_iris.style("space.hud-bar.mp.iris-lbl");
//		lbl_iris.text("Iris");
//		mp_bar.add(lbl_iris);
//
//		libv::ui::Label lbl_iris0("mp-title-iris");
//		lbl_iris0.style("space.hud-bar.mp.iris-lbl");
//		lbl_iris0.text("First Line");
//		mp_bar.add(lbl_iris0);
//
//		libv::ui::Label lbl_iris1("mp-title-iris");
//		lbl_iris1.style("space.hud-bar.mp.iris-lbl");
//		lbl_iris1.text("");
//		mp_bar.add(lbl_iris1);
//
//		libv::ui::Label lbl_iris2("mp-title-iris");
//		lbl_iris2.style("space.hud-bar.mp.iris-lbl");
//		lbl_iris2.text("First Line\nSecond Line");
//		mp_bar.add(lbl_iris2);
//
//		libv::ui::Label lbl_iris3("mp-title-iris");
//		lbl_iris3.style("space.hud-bar.mp.iris-lbl");
//		lbl_iris3.text("Iris\nSecond Line");
//		mp_bar.add(lbl_iris3);

		// -------------------------------------------------------------------------------------------------

//		This was a test case for layout invalidation on style reloading, would be nice to move it into a unit test

//		ui.style("space.hud-bar.mp.iris-lbl") {
//			align_horizontal = "center",
//			align_vertical = "center",
//			font_color = "rgba(1.0, 1.0, 1.0, 1.0)",
//			--background = "color: red";
//			--background = "color:" .. colors.hud_light_bg_color;
//			--background = "texture: atlas_ui_slate.png";
//			--background = "texture: hexagon_metal_0001_diffuse.dds";
//			--background = "texture: separator_bar_256x16.png";
//			--background = "texture: stretch_border.png";
//			background = "border: stretch_border.png";
//			--size = "200px, 200px",
//			--size = "300px, 300px",
//			--size = "100px, 100px",
//			size = "100px, 100px",
//			--size = "50px, 50px",
//			--size = "10pxD, 4pxD",
//		}

//		libv::ui::PanelLine xp("xp"); {
//			libv::ui::Label lbl_iris("inner");
//			lbl_iris.style("space.hud-bar.mp.iris-lbl");
//			lbl_iris.text("Inner");
//			xp.add(lbl_iris);
//		}
//		xp.size(libv::ui::parse_size_or_throw("100px, 100px"));
//		mp_bar.add(xp);
//
//		libv::ui::PanelLine xp2("xp2"); {
//			libv::ui::PanelLine xp3("xp3"); {
//				libv::ui::Label lbl_iris("PX PX Inner");
//				lbl_iris.style("space.hud-bar.mp.iris-lbl");
//				lbl_iris.text("PX PX\nInner");
//				xp3.add(lbl_iris);
//			}
//			xp3.size(libv::ui::parse_size_or_throw("100px, 100px"));
//			xp2.add(xp3);
//		}
//		xp2.size(libv::ui::parse_size_or_throw("100px, 100px"));
//		mp_bar.add(xp2);
//
//		libv::ui::PanelLine xp5("xp5"); {
//			libv::ui::PanelLine xp6("xp6"); {
//				libv::ui::Label lbl_iris("D  PX Inner");
//				lbl_iris.style("space.hud-bar.mp.iris-lbl");
//				lbl_iris.text("D  PX\nInner");
//				xp6.add(lbl_iris);
//			}
//			xp6.size(libv::ui::parse_size_or_throw("100px, 100px"));
//			xp5.add(xp6);
//		}
//		xp5.size(libv::ui::parse_size_or_throw("D, D"));
//		mp_bar.add(xp5);
//
//		libv::ui::PanelLine xp7("xp7"); {
//			libv::ui::PanelLine xp8("xp8"); {
//				libv::ui::Label lbl_iris("PX D  Inner");
//				lbl_iris.style("space.hud-bar.mp.iris-lbl");
//				lbl_iris.text("PX D \nInner");
//				xp8.add(lbl_iris);
//			}
//			xp8.size(libv::ui::parse_size_or_throw("D, D"));
//			xp7.add(xp8);
//		}
//		xp7.size(libv::ui::parse_size_or_throw("100px, 100px"));
//		mp_bar.add(xp7);
//
//		libv::ui::PanelLine xp9("xp9"); {
//			libv::ui::PanelLine xp10("xp10"); {
//				libv::ui::Label lbl_iris("D  D  Inner");
//				lbl_iris.style("space.hud-bar.mp.iris-lbl");
//				lbl_iris.text("D  D \nInner");
//				xp10.add(lbl_iris);
//			}
//			xp10.size(libv::ui::parse_size_or_throw("D, D"));
//			xp9.add(xp10);
//		}
//		xp9.size(libv::ui::parse_size_or_throw("D, D"));
//		mp_bar.add(xp9);
//
//		libv::ui::Label lbl_iris("outer");
//		lbl_iris.style("space.hud-bar.mp.iris-lbl");
//		lbl_iris.text("Outer");
//		mp_bar.add(lbl_iris);

		// -------------------------------------------------------------------------------------------------


//		libv::ui::PanelLine mouse_stack;
//		foreach_mouse_over([](const auto& arg) {
//			libv::ui::Label lbl;
//			lbl.text(fmt::format("{}: {:<60} [{:>9}-{:>9}]", arg.order, arg.component.path(), arg.cornerBL, arg.cornerTR));
//			mouse_stack.add(lbl);
//		});
//
//		libv::ui::Table table;
//		foreach_mouse_over([](const auto& arg) {
//			table[-1][0] = arg.component.path();
//			table[-1][1] = arg.order;
//			table[-1][2] = arg.cornerBL;
//			table[-1][3] = arg.cornerTR;
//		});



//		const auto style_label_01 = ui.context().style("style-label-01");
//		style_label_01->set("color", libv::parse::parse_color_or_throw("rgba(167, 152, 120, 100%)"));
//		style_label_01->set("font", ui.context().font("consola.ttf"));
////		style_label_01->set("align", libv::ui::Anchor::Left);
//
//		label0->properties.set(style_label_01);
//		label0->setText("Hello, Label0!");
//		label1->properties.set(style_label_01);
//		label1->setText("Hello, Label1!");
//		label2->properties.set(style_label_01);
//		label2->setText("Hello, Label2!");
//		label2->propertySize = libv::ui::parse_size_or_throw("dynamic, dynamic");
//
//		quad0->color(libv::parse::parse_color_or_throw("rgba(134, 189, 111, 80%)"));
//		stretch0->image(ui.context().texture2D("stretch_border.png"));
//		stretch0->color(libv::parse::parse_color_or_throw("rgba(183, 190, 135, 100%)"));
//		image0->image(ui.context().texture2D("atlas_ui_slate.png"));
//		image0->propertySize = libv::ui::parse_size_or_throw("dynamic, dynamic");
//
//		panel0->layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		panel0->layout.alignVertical = libv::ui::Alignment::CENTER;
//		panel0->layout.orient = libv::ui::Orientation::down;
//		panel0->add(label0);
//		panel0->add(image0);
//		panel0->add(panel1);
//		panel0->add(label1);
//
//		panel1->layout.alignHorizontal = libv::ui::Alignment::CENTER;
//		panel1->layout.alignVertical = libv::ui::Alignment::CENTER;
//		panel1->layout.orient = libv::ui::Orientation::right;
//		panel1->add(quad0);
//		panel1->add(label2);
//		panel1->add(stretch0);
//
//		ui.add(panel0);

		onKey.output([&](const libv::input::EventKey& e) {
			if (e.action == libv::input::Action::release)
				return;

			if (e.keycode == libv::input::Keycode::Escape)
				closeDefault();

//			if (e.keycode == libv::input::Keycode::Backspace) {
//				label0->string.pop_back();
//				label2->string.pop_back();
//				label0->flag(libv::ui::Flag::invalidLayout);
//				label2->flag(libv::ui::Flag::invalidLayout);
//				log_sandbox.trace("Pop back");
//			}

			if (e.keycode == libv::input::Keycode::Enter || e.keycode == libv::input::Keycode::KPEnter) {
				log_sandbox.trace("Appending new line");
				label.text(label.text() + "\n");
			}

			if (isKeyPressed(libv::input::Keycode::ShiftLeft) || isKeyPressed(libv::input::Keycode::ShiftRight)) {
				if (e.keycode == libv::input::Keycode::F7) {
					log_sandbox.trace("AlignVertical::bottom");
					label.align_vertical(libv::ui::AlignVertical::bottom);
					input_field0.align_vertical(libv::ui::AlignVertical::bottom);
					status_log.align_vertical(libv::ui::AlignVertical::bottom);
				}
				if (e.keycode == libv::input::Keycode::F8) {
					log_sandbox.trace("AlignVertical::center");
					label.align_vertical(libv::ui::AlignVertical::center);
					input_field0.align_vertical(libv::ui::AlignVertical::center);
					status_log.align_vertical(libv::ui::AlignVertical::center);
				}
				if (e.keycode == libv::input::Keycode::F9) {
					log_sandbox.trace("AlignVertical::top");
					label.align_vertical(libv::ui::AlignVertical::top);
					input_field0.align_vertical(libv::ui::AlignVertical::top);
					status_log.align_vertical(libv::ui::AlignVertical::top);
				}
				if (e.keycode == libv::input::Keycode::F10) {
					log_sandbox.trace("AlignVertical::justify");
					label.align_vertical(libv::ui::AlignVertical::justify);
					input_field0.align_vertical(libv::ui::AlignVertical::justify);
					status_log.align_vertical(libv::ui::AlignVertical::justify);
				}
				if (e.keycode == libv::input::Keycode::F11) {
					log_sandbox.trace("AlignVertical::justify_all");
					label.align_vertical(libv::ui::AlignVertical::justify_all);
					input_field0.align_vertical(libv::ui::AlignVertical::justify_all);
					status_log.align_vertical(libv::ui::AlignVertical::justify_all);
				}
			} else {
				if (e.keycode == libv::input::Keycode::F7) {
					log_sandbox.trace("AlignHorizontal::left");
					label.align_horizontal(libv::ui::AlignHorizontal::left);
					input_field0.align_horizontal(libv::ui::AlignHorizontal::left);
					status_log.align_horizontal(libv::ui::AlignHorizontal::left);
				}
				if (e.keycode == libv::input::Keycode::F8) {
					log_sandbox.trace("AlignHorizontal::center");
					label.align_horizontal(libv::ui::AlignHorizontal::center);
					input_field0.align_horizontal(libv::ui::AlignHorizontal::center);
					status_log.align_horizontal(libv::ui::AlignHorizontal::center);
				}
				if (e.keycode == libv::input::Keycode::F9) {
					log_sandbox.trace("AlignHorizontal::right");
					label.align_horizontal(libv::ui::AlignHorizontal::right);
					input_field0.align_horizontal(libv::ui::AlignHorizontal::right);
					status_log.align_horizontal(libv::ui::AlignHorizontal::right);
				}
				if (e.keycode == libv::input::Keycode::F10) {
					log_sandbox.trace("AlignHorizontal::justify");
					label.align_horizontal(libv::ui::AlignHorizontal::justify);
					input_field0.align_horizontal(libv::ui::AlignHorizontal::justify);
					status_log.align_horizontal(libv::ui::AlignHorizontal::justify);
				}
				if (e.keycode == libv::input::Keycode::F11) {
					log_sandbox.trace("AlignHorizontal::justify_all");
					label.align_horizontal(libv::ui::AlignHorizontal::justify_all);
					input_field0.align_horizontal(libv::ui::AlignHorizontal::justify_all);
					status_log.align_horizontal(libv::ui::AlignHorizontal::justify_all);
				}
			}

			if (e.keycode == libv::input::Keycode::KPNum0) {
				button.event().submit.fire({});
				ui.context().broadcast(std::string("Test string passed as global event"));
			}
		});
		label.event().global.connect<std::string>([](const std::string& e){
			log_sandbox.trace("global.connect {}", e);
		});
		label.event().global.connect<std::string>([](libv::ui::Label& label, const std::string& e){
			log_sandbox.trace("global.connect {} {}", e, label.path());
		});
		onChar.output([&](const libv::input::EventChar& e) {
//			label0->string.push_back(e.utf8);
//			label2->string.push_back(e.utf8);
//			label0->flag(libv::ui::Flag::invalidLayout);
//			label2->flag(libv::ui::Flag::invalidLayout);
			log_sandbox.trace("Append string {}", e.utf8);
			label.text(label.text() + e.utf8);
		});
	}
	~SandboxFrame() {
		disconnectAll();
	}
};

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	// For CLion console
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");

	std::cout << libv::logger_stream;
//	libv::logger_stream.allow("libv.ui");
//	libv::logger_stream.deny();

	{
		SandboxFrame frame;
		frame.show();
		frame.join();
	}

	return 0;
}
