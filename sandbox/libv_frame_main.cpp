// File: main.cpp, Created on 2013. augusztus 16. 11:59, Author: Vader

// libv
#include <libv/input/inputs.hpp>
// std
#include <cmath>
#include <iostream>
#include <thread>
// pro
#include <libv/frame/frame.hpp>
#include <libv/frame/icon.hpp>
#include <libv/frame/log.hpp>
#include <libv/frame/monitor.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

using libv::input::Key;

libv::frame::Icon generateIcon(size_t size, libv::vec4uc color) {
	libv::frame::Icon result;

	result.size.x = static_cast<int>(size);
	result.size.y = static_cast<int>(size);
	result.pixels.resize(size * size);

	const double radius = static_cast<double>(size - 1);

	for (size_t x = 0; x < size; ++x)
		for (size_t y = 0; y < size; ++y)
			if (std::sqrt(static_cast<double>(x * x + y * y)) < radius)
				result.pixels[y * size + x] = color;

	return result;
}

void noisyEvents(libv::Frame& frame) {
	const auto pretty_print_to_log = [](const auto& event) {
		log_sandbox.trace("Event: {}", event.toPrettyString());
	};

	frame.onChar.output(pretty_print_to_log);
	frame.onDrop.output(pretty_print_to_log);
	frame.onFramebufferSize.output(pretty_print_to_log);
	frame.onKey.output(pretty_print_to_log);
	frame.onMouseButton.output(pretty_print_to_log);
	frame.onMouseEnter.output(pretty_print_to_log);
	frame.onMousePosition.output(pretty_print_to_log);
	frame.onMouseScroll.output(pretty_print_to_log);
	frame.onWindowFocus.output(pretty_print_to_log);
	frame.onWindowMaximize.output(pretty_print_to_log);
	frame.onWindowMinimize.output(pretty_print_to_log);
	frame.onWindowPosition.output(pretty_print_to_log);
	frame.onWindowRefresh.output(pretty_print_to_log);
	frame.onWindowSize.output(pretty_print_to_log);

	frame.onCloseRequest.output(pretty_print_to_log);

	frame.onContextCreate.output(pretty_print_to_log);
//	frame.onContextUpdate.output(pretty_print_to_log);
	frame.onContextDestroy.output(pretty_print_to_log);
}

template <typename Rep, typename Per, typename F>
void executeAfter(std::chrono::duration<Rep, Per> delay, F&& func) {
	std::thread t([delay, f = std::forward<F>(func)]{
		std::this_thread::sleep_for(delay);
		std::forward<decltype(f)>(f)();
	});
	t.detach();
}

class TestFrame : public libv::Frame {
public:
	TestFrame(const std::string& title) : Frame(title, 1280, 960) {
		setPosition(FramePosition::center_current_monitor);
		setCloseOperation(CloseOperation::dispose);
		setOpenGLProfile(OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		show();

		noisyEvents(*this);
		onKey.output([this](const auto& e) {
			if (e.action != libv::input::Action::release)
				return;

			if (e.key == Key::Escape)
				closeDefault();

			// --- 0 row

			if (e.key == Key::Backtick)
				clearIcon();
			if (e.key == Key::Num1)
				setIcon(std::vector<libv::frame::Icon>{generateIcon(16, {255, 0, 0, 255}), generateIcon(32, {255, 63, 127, 255}), generateIcon(48, {255, 127, 255, 255})});
			if (e.key == Key::Num2)
				setIcon(std::vector<libv::frame::Icon>{generateIcon(16, {0, 255, 0, 255}), generateIcon(32, {127, 255, 63, 255}), generateIcon(48, {255, 255, 127, 255})});

			// --- Q row

			if (e.key == Key::Q)
				executeAfter(std::chrono::seconds{5}, [this]{ this->show(); });
			if (e.key == Key::W)
				executeAfter(std::chrono::seconds{5}, [this]{ this->hide(); });
			if (e.key == Key::E)
				executeAfter(std::chrono::seconds{5}, [this]{ this->maximize(); });
			if (e.key == Key::R)
				executeAfter(std::chrono::seconds{5}, [this]{ this->minimize(); });
			if (e.key == Key::T)
				executeAfter(std::chrono::seconds{5}, [this]{ this->restore(); });
			if (e.key == Key::Y)
				executeAfter(std::chrono::seconds{5}, [this]{ this->focus(); });
			if (e.key == Key::U)
				executeAfter(std::chrono::seconds{5}, [this]{ this->requestAttention(); });

			if (e.key == Key::I)
				setCursorMode(CursorMode::normal);
			if (e.key == Key::O)
				setCursorMode(CursorMode::hidden);
			if (e.key == Key::P)
				setCursorMode(CursorMode::disabled);

			if (e.key == Key::BracketOpen)
				setClipboardString("Your clipboard now belongs to us.");
			if (e.key == Key::BracketClose)
				log_sandbox.trace("getClipboardString: {}", getClipboardString());

			// --- A row

			if (e.key == Key::A)
				setDisplayMode(libv::Frame::DisplayMode::windowed);
			if (e.key == Key::S)
				setDisplayMode(libv::Frame::DisplayMode::borderless);
			if (e.key == Key::D)
				setDisplayMode(libv::Frame::DisplayMode::fullscreen);

			if (e.key == Key::F)
				log_sandbox.trace("getFrameSize: {}", getFrameSize());
			if (e.key == Key::G)
				log_sandbox.trace("getSize: {}", getSize());
			if (e.key == Key::H)
				log_sandbox.trace("getPosition: {}", getPosition());

			if (e.key == Key::J)
				setAlwaysOnTop(true);
			if (e.key == Key::K)
				setAlwaysOnTop(false);

			if (e.key == Key::L)
				setFocusOnShow(true);
			if (e.key == Key::Semicolon)
				setFocusOnShow(false);

			if (e.key == Key::Apostrophe)
				for (const auto& [_, monitor] : libv::frame::Monitor::monitors) {
					log_sandbox.trace("{} pos {}, work pos {}, work size {}, content scale {}, physical mm {}", monitor.name, monitor.position, monitor.workAreaPosition, monitor.workAreaSize, monitor.contentScale, monitor.physicalSizeMM);
					log_sandbox.trace("     >> size {}, rate {}, bits {}", monitor.currentVideoMode.size, monitor.currentVideoMode.refreshRate, monitor.currentVideoMode.colorBits);
					for (const auto& mode : monitor.videoModes)
						log_sandbox.trace("        size {}, rate {}, bits {}", mode.size, mode.refreshRate, mode.colorBits);
				}

			// --- Z row

			if (e.key == Key::Z)
				setResizable(!isResizable());

			if (e.key == Key::X)
				setDecoration(!isDecorated());

			if (e.key == Key::C)
				setPosition(FramePosition::center_current_monitor);
			if (e.key == Key::V)
				setPosition(FramePosition::center_primary_monitor);

			if (e.key == Key::B)
				setSizeLimit(200, 150, 400, 300);
			if (e.key == Key::N)
				setSizeLimit(-1, -1, 400, 300);
			if (e.key == Key::M)
				setSizeLimit(-1, -1, -1, -1);

			if (e.key == Key::Comma)
				setAspectRatio(16, 9);
			if (e.key == Key::Period)
				setAspectRatio(-1, -1);
		});
	}
};

int main(int, char**) {
	std::cout << libv::logger_stream;

//	libv::log.allow("libv.frame.event");
//	libv::log.deny();

	TestFrame f1("TestFrame");
	f1.join();

	return 0;
}
