// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// ext
#include <GL/glew.h>
// libv
#include <libv/frame/frame.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
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
#include <libv/ui/component/label_image.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_full.hpp>
#include <libv/ui/component/panel_grid.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/quad.hpp>
#include <libv/ui/component/scroll_bar.hpp>
#include <libv/ui/component/scroll_pane.hpp>
#include <libv/ui/component/stretch.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/ui.hpp>
//#include <libv/ui/style.hpp>


// -------------------------------------------------------------------------------------------------


inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

class SandboxFrame : public libv::Frame {
private:
	libv::glr::Remote remote;
	libv::ui::UI ui;

private:
	libv::ui::Button button0;
	libv::ui::Button button1;
	libv::ui::Button button2;
	libv::ui::Button button3;
	libv::ui::Button button;
	libv::ui::Image image;
	libv::ui::InputField input_field;
	libv::ui::InputField input_field2;
	libv::ui::Label label;
	libv::ui::LabelImage label_image1;
	libv::ui::LabelImage label_image2;
	libv::ui::PanelFloat panel_float;
	libv::ui::PanelFull panel_full;
	libv::ui::PanelGrid panel_grid;
	libv::ui::PanelLine panel_line;
	libv::ui::PanelLine panel_line_scrolled;
	libv::ui::Stretch stretch;
	libv::ui::Quad quad;
	libv::ui::ScrollBar scroll_bar_x;
	libv::ui::ScrollBar scroll_bar_y;
	libv::ui::ScrollArea scroll_area;

//	std::shared_ptr<libv::ui::Quad> quad0 = std::make_shared<libv::ui::Quad>(ui.root());
//	std::shared_ptr<libv::ui::Stretch> stretch0 = std::make_shared<libv::ui::Stretch>(ui.root());

public:
	void create() {
		glewExperimental = true;
		if (GLenum err = glewInit() != GLEW_OK)
			log_sandbox.error("Failed to initialize glew: {}", glewGetErrorString(err));

		log_sandbox.debug("GL Vendor     {}", glGetString(GL_VENDOR));
		log_sandbox.debug("GL Renderer   {}", glGetString(GL_RENDERER));
		log_sandbox.debug("GL Version    {}", glGetString(GL_VERSION));

		remote.create();
		remote.enableDebug();

//		auto gl = remote.queue();
//		ui.create(gl);
//		remote.queue(std::move(gl));
//		remote.execute();
	}

	void render() {
		auto gl = remote.queue();

		gl.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
		gl.clearColor();
		gl.clearDepth();

		ui.update(gl);

		remote.queue(std::move(gl));
		remote.execute();
	}

	void destroy() {
		auto gl = remote.queue();
		ui.destroy(gl);
		remote.queue(std::move(gl));
		remote.execute();

		remote.destroy();
	}

public:
	SandboxFrame() :
		Frame("UI sandbox", 1280, 800) {
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
		});

		button0.text("Button 0!");
		button1.text("Button 1!");
		button1.image(ui.context().texture2D("separator_bar_256x16.png"));

		button2.text("Button 2!");
		button2.anchor(libv::ui::Anchor::top_left);
		button2.size(libv::ui::parse_size_or_throw("33%, 33%"));
		button3.text("Button 3!");
		button3.anchor(libv::ui::Anchor::bottom_right);
		button3.size(libv::ui::parse_size_or_throw("33%, 33%"));

		label.text("Label");
		label.align_horizontal(libv::ui::AlignHorizontal::center);
		label.align_vertical(libv::ui::AlignVertical::center);

		label_image1.text("Label image");
		label_image1.image(ui.context().texture2D("separator_bar_256x16.png"));

		label_image2.text("Label image2");
		label_image2.image(ui.context().texture2D("separator_bar_256x16.png"));
		label_image2.color({1.f, 1.f, 1.f, 0.5f});

		image.image(ui.context().texture2D("separator_bar_256x16.png"));
		image.size(libv::ui::parse_size_or_throw("25%, 50px"));

		input_field.text("Input field");
		input_field.event().change([](auto& component, const auto&) {
			log_sandbox.info("Input field {} changed to {}", component.path(), component.text());
		});
		input_field.event().caret([](auto& component, const auto&) {
			log_sandbox.info("Input field {} caret moved to {}", component.path(), component.caret());
		});
		input_field.event().submit([](auto& component, const auto&) {
			log_sandbox.info("Input field {} submitted", component.path());
		});

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

		stretch.image(ui.context().texture2D("stretch_border.png"));

		quad.color({.8f, .5f, .5f, 1.f});
		quad.anchor(libv::ui::Anchor::center_center);
		quad.size(libv::ui::parse_size_or_throw("0.5r, 0.5r"));

		panel_line_scrolled.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

		scroll_bar_x.bar_image(ui.context().texture2D("separator_bar_256x16.png"));
		scroll_bar_x.bar_color({0.8f, 0.2f, 0.2f, 1.0f});
		scroll_bar_x.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
		scroll_bar_x.value_min(-500);
		scroll_bar_x.value_max(500);
		scroll_bar_x.value(0);
		scroll_bar_x.event().change([this](auto& component, const auto& event) {
			log_sandbox.info("Scroll bar X {} changed to {} with change {}", component.path(), component.value(), event.change);
			scroll_area.area_position({static_cast<float>(component.value()), scroll_area.area_position().y});
		});

		scroll_bar_y.bar_image(ui.context().texture2D("separator_bar_256x16.png"));
		scroll_bar_y.bar_color({0.2f, 0.8f, 0.2f, 1.0f});
		scroll_bar_y.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
		scroll_bar_y.value_min(-500);
		scroll_bar_y.value_max(500);
		scroll_bar_y.value(0);
		scroll_bar_y.event().change([this](auto& component, const auto& event) {
			log_sandbox.info("Scroll bar Y {} changed to {} with change {}", component.path(), component.value(), event.change);
			scroll_area.area_position({scroll_area.area_position().x, static_cast<float>(component.value())});
		});

		scroll_area.content(panel_line_scrolled);
		scroll_area.mode(libv::ui::ScrollAreaMode::vertical);
		scroll_area.size(libv::ui::parse_size_or_throw("50%, 1r"));
		scroll_area.anchor(libv::ui::Anchor::center_center);
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
		panel_line_scrolled.add(input_field2);
		input_field2.size(libv::ui::parse_size_or_throw("D, D"));
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

		panel_full.add(button1);

		panel_float.add(button2);
		panel_float.add(button3);

		panel_grid.column_count(3);
		panel_grid.padding({5, 5, 5, 10});
		panel_grid.orientation2(libv::ui::Orientation2::RIGHT_DOWN);
		panel_grid.add(button0);
		panel_grid.add(panel_full);
		panel_grid.add(panel_float);
		panel_grid.add(input_field);
		panel_grid.add(label_image1);
		panel_grid.add(quad);
		panel_grid.add(stretch);
		panel_grid.add(scroll_bar_x);
		panel_grid.add(scroll_bar_y);

		panel_line.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
		panel_line.padding({20, 10, 20, 10});
		panel_line.add(panel_grid);
		panel_line.add(button);
		panel_line.add(scroll_area);
		panel_line.add(label);
		panel_line.add(label_image2);

		ui.add(panel_line);

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

			if (e.keycode == libv::input::Keycode::F7) {
				log_sandbox.trace("AlignVertical::top");
				label.align_vertical(libv::ui::AlignVertical::top);
				input_field.align_vertical(libv::ui::AlignVertical::top);
			}

			if (e.keycode == libv::input::Keycode::F8) {
				log_sandbox.trace("AlignVertical::center");
				label.align_vertical(libv::ui::AlignVertical::center);
				input_field.align_vertical(libv::ui::AlignVertical::center);
			}

			if (e.keycode == libv::input::Keycode::F9) {
				log_sandbox.trace("AlignVertical::bottom");
				label.align_vertical(libv::ui::AlignVertical::bottom);
				input_field.align_vertical(libv::ui::AlignVertical::bottom);
			}

			if (e.keycode == libv::input::Keycode::F10) {
				log_sandbox.trace("AlignVertical::justify");
				label.align_vertical(libv::ui::AlignVertical::justify);
				input_field.align_vertical(libv::ui::AlignVertical::justify);
			}

			if (e.keycode == libv::input::Keycode::F11) {
				log_sandbox.trace("AlignVertical::justify_all");
				label.align_vertical(libv::ui::AlignVertical::justify_all);
				input_field.align_vertical(libv::ui::AlignVertical::justify_all);
			}
		});
		onChar.output([&](const libv::input::EventChar& e) {
//			label0->string.push_back(e.utf8);
//			label2->string.push_back(e.utf8);
//			label0->flag(libv::ui::Flag::invalidLayout);
//			label2->flag(libv::ui::Flag::invalidLayout);
			log_sandbox.trace("Append string {}", e.utf8.data());
			label.text(label.text() + e.utf8.data());
		});
		onContextCreate.output([&](const libv::frame::EventContextCreate&) {
			create();
		});
		onContextUpdate.output([&](const libv::frame::EventContextUpdate&) {
			render();
		});
		onContextDestroy.output([&](const libv::frame::EventContextDestroy&) {
			destroy();
		});
	}
	~SandboxFrame() {
		disconnectAll();
	}
};

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	system("chcp 65001"); // !!! For Clion terminal
	std::filesystem::current_path(std::filesystem::current_path().parent_path()); // !!! For Clion editor
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n"); // !!! For Clion console

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
