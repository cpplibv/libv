// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// ext
#include <GL/glew.h>
// libv
#include <libv/frame/frame.hpp>
#include <libv/glr/remote.hpp>
#include <libv/log/log.hpp>
// std
#include <iostream>
// pro
#include <libv/lua/lua.hpp>
#include <libv/ui/ui_lua.hpp>
#include <libv/ui/ui.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger, "sandbox"};

class SandboxFrame : public libv::Frame {
private:
	libv::glr::Remote remote;
	libv::ui::UI ui;
	libv::lua::State lua;

private:
	std::shared_ptr<libv::ui::ComponentBase> lua_component;

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

		auto gl = remote.queue();
		ui.create(gl);
		remote.queue(std::move(gl));
		remote.render();
	}

	void render() {
		auto gl = remote.queue();

		gl.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
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
	SandboxFrame() :
		Frame("UI sandbox", 1280, 800),
		lua(libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::ui)) {

		setPosition(POSITION_CENTER_CURRENT_MONITOR);
		setOpenGLProfile(OPENGL_PROFILE_CORE);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(4);
		ui.attach(*this);
		ui.setSize(1280.f, 800.f); // TODO P4: auto detect size changes

		lua_component = libv::ui::script_file(ui, lua, "res/script/sandbox_ui.lua");
//		ui.add(lua_component);

		onKey.output([&](const libv::frame::EventKey& e) {
			if (e.action == libv::frame::Action::release)
				return;

			if (e.key == libv::frame::Key::Escape)
				closeDefault();

			if (e.key == libv::frame::Key::Backspace) {
//				label0->string.pop_back();
//				label0->invalidate(libv::ui::Flag::invalidLayout);
				log_sandbox.trace("Pop back");
			}

			if (e.key == libv::frame::Key::Enter || e.key == libv::frame::Key::KPEnter) {
//				label0->string.push_back("\n");
//				label0->invalidate(libv::ui::Flag::invalidLayout);
				log_sandbox.trace("Appending new line");
			}

			if (e.mods != libv::frame::KeyModifier::control)
				return;

			switch (e.key) {
			case libv::frame::Key::Num0:
//				label0->properties.align = libv::ui::Anchor::Left;
				log_sandbox.trace("Set anchor to {}", "Left");
				break;

			case libv::frame::Key::Num1:
//				label0->properties.align = libv::ui::Anchor::Center;
				log_sandbox.trace("Set anchor to {}", "Center");
				break;

			case libv::frame::Key::Num2:
//				label0->properties.align = libv::ui::Anchor::Right;
				log_sandbox.trace("Set anchor to {}", "Right");
				break;

			case libv::frame::Key::Num3:
//				label0->properties.align = libv::ui::Anchor::Justify;
				log_sandbox.trace("Set anchor to {}", "Justify");
				break;

			default:
				break;
			}
		});
		onChar.output([&](const libv::frame::EventChar& e) {
//			label0->string.push_back(e.utf8);
//			label0->invalidate(libv::ui::Flag::invalidLayout);
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
		SandboxFrame frame;
		frame.show();
		frame.join();
	}

	return 0;
}
