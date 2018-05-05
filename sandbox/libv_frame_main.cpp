// File: main.cpp, Created on 2013. augusztus 16. 11:59, Author: Vader

// std
#include <iostream>
// pro
#include <libv/frame/frame.hpp>
#include <libv/frame/log.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger, "sandbox"};

using libv::frame::Key;

void noisyEvents(libv::Frame& frame) {
	const auto pretty_print_to_log = [](const auto& event) {
		log_sandbox.trace("Event: {}", event.toPrettyString());
	};

	frame.onChar.output(pretty_print_to_log);
	frame.onCharMods.output(pretty_print_to_log);
	frame.onDrop.output(pretty_print_to_log);
	frame.onFramebufferSize.output(pretty_print_to_log);
	frame.onKey.output(pretty_print_to_log);
	frame.onMouseButton.output(pretty_print_to_log);
	frame.onMouseEnter.output(pretty_print_to_log);
	frame.onMousePosition.output(pretty_print_to_log);
	frame.onMouseScroll.output(pretty_print_to_log);
	frame.onWindowFocus.output(pretty_print_to_log);
	frame.onWindowIconify.output(pretty_print_to_log);
	frame.onWindowPosition.output(pretty_print_to_log);
	frame.onWindowRefresh.output(pretty_print_to_log);
	frame.onWindowSize.output(pretty_print_to_log);

	frame.onCloseRequest.output(pretty_print_to_log);

	frame.onContextInitialization.output(pretty_print_to_log);
	frame.onContextRefresh.output(pretty_print_to_log);
	frame.onContextTerminate.output(pretty_print_to_log);
}

class TestFrame : public libv::Frame {
public:
	TestFrame(const std::string& title) : Frame(title, 1200, 750) {
		setPosition(POSITION_CENTER_CURRENT_MONITOR);
		setCloseOperation(ON_CLOSE_DISPOSE);
		setOpenGLProfile(OPENGL_PROFILE_CORE);
		setOpenGLVersion(3, 3);
		show();

		noisyEvents(*this);
		onKey.output([this](const auto& e) {
			if (e.key == Key::Escape)
				this->closeDefault();
			if (e.key == Key::X)
				this->setDecoration(!this->isDecorated());
			if (e.key == Key::C)
				this->setPosition(POSITION_CENTER_CURRENT_MONITOR);
		});
	}
};

int main(int, char**) {
	std::cout << libv::logger;

//	libv::log.allow("libv.frame.event");
//	libv::log.deny();

	TestFrame f1("TestFrame");
	f1.join();

	return 0;
}
