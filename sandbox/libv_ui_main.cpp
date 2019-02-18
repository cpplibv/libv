// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// ext
#include <GL/glew.h>
// libv
#include <libv/frame/frame.hpp>
#include <libv/glr/remote.hpp>
#include <libv/log/log.hpp>
#include <libv/parse/color.hpp>
// std
#include <iostream>
// pro
#include <libv/ui/component/image.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel.hpp>
#include <libv/ui/component/quad.hpp>
#include <libv/ui/component/stretch.hpp>
#include <libv/ui/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/ui.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger, "sandbox"};

class SandboxFrame : public libv::Frame {
private:
	libv::glr::Remote remote;
	libv::ui::UI ui;

private:
//	std::shared_ptr<libv::ui::Button> button;
	std::shared_ptr<libv::ui::Panel> panel0 = std::make_shared<libv::ui::Panel>();
	std::shared_ptr<libv::ui::Panel> panel1 = std::make_shared<libv::ui::Panel>();
	std::shared_ptr<libv::ui::Label> label0 = std::make_shared<libv::ui::Label>();
	std::shared_ptr<libv::ui::Label> label1 = std::make_shared<libv::ui::Label>();
	std::shared_ptr<libv::ui::Label> label2 = std::make_shared<libv::ui::Label>();
	std::shared_ptr<libv::ui::Quad> quad0 = std::make_shared<libv::ui::Quad>();
	std::shared_ptr<libv::ui::Stretch> stretch0 = std::make_shared<libv::ui::Stretch>();
	std::shared_ptr<libv::ui::Image> image0 = std::make_shared<libv::ui::Image>();

//private:
//	libv::ui::FontSize size = 12;

public:
	void create() {
		glewExperimental = true;
		if (GLenum err = glewInit() != GLEW_OK)
			log_sandbox.error("Failed to initialize glew: {}", glewGetErrorString(err));

		log_sandbox.debug("GL Vendor   {}", glGetString(GL_VENDOR));
		log_sandbox.debug("GL Renderer {}", glGetString(GL_RENDERER));
		log_sandbox.debug("GL Version  {}", glGetString(GL_VERSION));

		remote.create();
		remote.enableDebug();

		auto gl = remote.queue();
		ui.create(gl);
		remote.queue(std::move(gl));
		remote.render();
	}

	void render() {
		auto gl = remote.queue();

		gl.setClearColor(0.236f, 0.311f, 0.311f, 1.0f);
		gl.clearColor();
		gl.clearDepth();

		ui.update(gl);

		remote.queue(std::move(gl));
		remote.render();
	}

	void destroy() {
		auto gl = remote.queue();
		ui.destroy(gl);
		remote.queue(std::move(gl));
		remote.render();

		remote.destroy();
	}

public:
	SandboxFrame(const std::string& title, int32_t x, int32_t y) :
		Frame(title, x, y) {
		setPosition(POSITION_CENTER_CURRENT_MONITOR);
		setOpenGLProfile(OPENGL_PROFILE_CORE);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(4);
		ui.attach(*this);
		ui.setSize(1280.f, 800.f); // TODO P4: auto detect size changes

		label0->color(libv::parse::parse_color_or_throw("rgba(167, 152, 120, 100%)"));
		label0->setText("Hello, Label0!");
		label1->setText("Hello, Label1!");
		label2->setText("Hello, Label2!");
		label2->propertySize = libv::ui::parse_size_or_throw("C, C, C");
		quad0->color(libv::parse::parse_color_or_throw("rgba(134, 189, 111, 80%)"));
		stretch0->image(ui.context().texture2D("stretch_border.png"));
		stretch0->color(libv::parse::parse_color_or_throw("rgba(183, 190, 135, 100%)"));
		image0->image(ui.context().texture2D("atlas_ui_slate.png"));
		image0->propertySize = libv::ui::parse_size_or_throw("C, C, C");

		panel0->layout.alignHorizontal = libv::ui::Alignment::CENTER;
		panel0->layout.alignVertical = libv::ui::Alignment::CENTER;
		panel0->layout.orient = libv::ui::Orientation::TOP_TO_BOTTOM;
		panel0->add(label0);
		panel0->add(image0);
		panel0->add(panel1);
		panel0->add(label1);

		panel1->layout.alignHorizontal = libv::ui::Alignment::CENTER;
		panel1->layout.alignVertical = libv::ui::Alignment::CENTER;
		panel1->layout.orient = libv::ui::Orientation::LEFT_TO_RIGHT;
		panel1->add(quad0);
		panel1->add(label2);
		panel1->add(stretch0);

		ui.add(panel0);

		// TODO P1: String2D content size should not include last inter glyph spacing

		onKey.output([&](const libv::frame::EventKey& e) {
			if (e.action == libv::frame::Action::release)
				return;

			if (e.key == libv::frame::Key::Escape)
				closeDefault();

			if (e.key == libv::frame::Key::Backspace) {
				label0->string.pop_back();
				label2->string.pop_back();
				log_sandbox.trace("Pop back");
			}

			if (e.key == libv::frame::Key::Enter || e.key == libv::frame::Key::KPEnter) {
				label0->string.push_back("\n");
				label2->string.push_back("\n");
				log_sandbox.trace("Appending new line");
			}

			if (e.mods != libv::frame::KeyModifier::control)
				return;

			switch (e.key) {
			case libv::frame::Key::Num0:
				label0->string.setAlign(libv::ui::Anchor::Left);
				label2->string.setAlign(libv::ui::Anchor::Left);
				log_sandbox.trace("Set anchor to {}", "Left");
				break;

			case libv::frame::Key::Num1:
				label0->string.setAlign(libv::ui::Anchor::Center);
				label2->string.setAlign(libv::ui::Anchor::Center);
				log_sandbox.trace("Set anchor to {}", "Center");
				break;

			case libv::frame::Key::Num2:
				label0->string.setAlign(libv::ui::Anchor::Right);
				label2->string.setAlign(libv::ui::Anchor::Right);
				log_sandbox.trace("Set anchor to {}", "Right");
				break;

			case libv::frame::Key::Num3:
				label0->string.setAlign(libv::ui::Anchor::Justify);
				label2->string.setAlign(libv::ui::Anchor::Justify);
				log_sandbox.trace("Set anchor to {}", "Justify");
				break;

			default:
				break;
			}
		});
		onChar.output([&](const libv::frame::EventChar& e) {
			label0->string.push_back(e.utf8);
			label2->string.push_back(e.utf8);
			log_sandbox.trace("Append string {}", e.utf8);
		});
		onContextInitialization.output([&](const libv::frame::EventContextInitialization&) {
			create();
		});
		onContextRefresh.output([&](const libv::frame::EventContextRefresh&) {
			render();
		});
		onContextTerminate.output([&](const libv::frame::EventContextTerminate&) {
			destroy();
		});
	}
	~SandboxFrame() {
		disconnectAll();
	}
};

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	std::cout << libv::logger;
//	libv::logger.allow("libv.ui");
//	libv::logger.deny();

	{
		SandboxFrame frame("UI sandbox", 1280, 800);
		frame.show();
		frame.join();
	}

	return 0;
}
