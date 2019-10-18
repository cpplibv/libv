// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// ext
#include <GL/glew.h>
// libv
#include <libv/frame/frame.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
#include <libv/log/log.hpp>
#include <libv/parse/color.hpp>
// std
#include <iostream>
// pro
#include <libv/ui/component/image.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/quad.hpp>
#include <libv/ui/component/stretch.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/ui.hpp>


// -------------------------------------------------------------------------------------------------

// button	 			Clickable elements.
// sprite-button	 	A button that displays an image rather than text.
// checkbox	 			Clickable elements with a cross in the middle that can be turned off or on.
// flow					Invisible containers that lay out children either horizontally or vertically.
// frame	 			Grey semi-transparent boxes that contain other elements. They have a caption, and, just like flows, they lay out children either horizontally or vertically.
// label	 			A piece of text.
// progressbar	 		Indicate progress by displaying a partially filled bar.
// table	 			An invisible container that lays out children in a specific number of columns. Column width is given by the largest element contained.
// textfield	 		Boxes of text the user can type in.
// radiobutton	 		Identical to checkbox except circular.
// sprite	 			An element that shows an image.
// scroll-pane	 		Similar to a flow but includes the ability to show and use scroll bars.
// drop-down	 		A drop down list of other elements.
// list-box	 			A list of other elements.
// camera	 			A camera that shows the game at the given position on the given surface.
// choose-elem-button	A button that lets the player pick one of an: item, entity, tile, or signal similar to the filter-select window.
// text-box				A multi-line text box that supports selection and copy-paste.
// slider				A number picker.
// entity-preview		A preview of an entity.
// split-pane
// tab-pane

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

class SandboxFrame : public libv::Frame {
private:
	libv::glr::Remote remote;
	libv::ui::UI ui;

private:
//	std::shared_ptr<libv::ui::Button> button;
	std::shared_ptr<libv::ui::PanelLine> panel0 = std::make_shared<libv::ui::PanelLine>(ui.root());
	std::shared_ptr<libv::ui::PanelLine> panel1 = std::make_shared<libv::ui::PanelLine>(ui.root());
	std::shared_ptr<libv::ui::Label> label0 = std::make_shared<libv::ui::Label>(ui.root());
	std::shared_ptr<libv::ui::Label> label1 = std::make_shared<libv::ui::Label>(ui.root());
	std::shared_ptr<libv::ui::Label> label2 = std::make_shared<libv::ui::Label>(ui.root());
	std::shared_ptr<libv::ui::Quad> quad0 = std::make_shared<libv::ui::Quad>(ui.root());
	std::shared_ptr<libv::ui::Stretch> stretch0 = std::make_shared<libv::ui::Stretch>(ui.root());
	std::shared_ptr<libv::ui::Image> image0 = std::make_shared<libv::ui::Image>(ui.root());

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
//		ui.destroy(gl);
//		remote.queue(std::move(gl));
//		remote.render();

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
		ui.setSize(1280.f, 800.f); // TODO P4: auto detect size changes

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
//
//		onKey.output([&](const libv::frame::EventKey& e) {
//			if (e.action == libv::input::Action::release)
//				return;
//
//			if (e.key == libv::input::Key::Escape)
//				closeDefault();
//
//			if (e.key == libv::input::Key::Backspace) {
//				label0->string.pop_back();
//				label2->string.pop_back();
//				label0->flag(libv::ui::Flag::invalidLayout);
//				label2->flag(libv::ui::Flag::invalidLayout);
//				log_sandbox.trace("Pop back");
//			}
//
//			if (e.key == libv::input::Key::Enter || e.key == libv::input::Key::KPEnter) {
//				label0->string.push_back("\n");
//				label2->string.push_back("\n");
//				label0->flag(libv::ui::Flag::invalidLayout);
//				label2->flag(libv::ui::Flag::invalidLayout);
//				log_sandbox.trace("Appending new line");
//			}
//
//			if (e.mods != libv::input::KeyModifier::control)
//				return;
//
//			switch (e.key) {
//			case libv::input::Key::Num0:
//				label0->properties.align = libv::ui::AlignHorizontal::Left;
//				label2->properties.align = libv::ui::AlignHorizontal::Left;
//				log_sandbox.trace("Set anchor to {}", "Left");
//				break;
//
//			case libv::input::Key::Num1:
//				label0->properties.align = libv::ui::AlignHorizontal::Center;
//				label2->properties.align = libv::ui::AlignHorizontal::Center;
//				log_sandbox.trace("Set anchor to {}", "Center");
//				break;
//
//			case libv::input::Key::Num2:
//				label0->properties.align = libv::ui::AlignHorizontal::Right;
//				label2->properties.align = libv::ui::AlignHorizontal::Right;
//				log_sandbox.trace("Set anchor to {}", "Right");
//				break;
//
//			case libv::input::Key::Num3:
//				label0->properties.align = libv::ui::AlignHorizontal::Justify;
//				label2->properties.align = libv::ui::AlignHorizontal::Justify;
//				log_sandbox.trace("Set anchor to {}", "Justify");
//				break;
//
//			default:
//				break;
//			}
//		});
		onChar.output([&](const libv::input::EventChar& e) {
//			label0->string.push_back(e.utf8);
//			label2->string.push_back(e.utf8);
//			label0->flag(libv::ui::Flag::invalidLayout);
//			label2->flag(libv::ui::Flag::invalidLayout);
			log_sandbox.trace("Append string {}", e.utf8.data());
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
