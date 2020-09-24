// Project: libv.lua, File: sandbox/libv_ui_lua_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/frame/frame.hpp>
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
#include <libv/log/log.hpp>
// std
#include <iostream>
// pro
#include <libv/lua/lua.hpp>
#include <libv/ui/ui_lua.hpp>
#include <libv/ui/ui.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

class SandboxFrame : public libv::Frame {
private:
	libv::ui::UI ui;
	libv::lua::State lua;

private:
	std::shared_ptr<libv::ui::CoreComponent> lua_component;

public:
	SandboxFrame() :
		Frame("UI sandbox", 1280, 800),
		lua(libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::ui)) {

		setPosition(FramePosition::center_current_monitor);
		setOpenGLProfile(OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(OpenGLSamples{4});
		ui.attach(*this);
		ui.setSize(1280.f, 800.f); // TODO P4: auto detect size changes

		lua_component = libv::ui::script_file(ui, lua, "res/script/sandbox_ui.lua");
//		ui.add(lua_component);

		onKey.output([&](const libv::input::EventKey& e) {
			if (e.action == libv::input::Action::release)
				return;

			if (e.keycode == libv::input::Keycode::Escape)
				closeDefault();
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
