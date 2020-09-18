// Project: libv.frame, File: sandbox/libv_frame_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
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

using libv::input::Keycode;

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
	frame.onContentScale.output(pretty_print_to_log);
	frame.onDrop.output(pretty_print_to_log);
	frame.onFocus.output(pretty_print_to_log);
	frame.onFramebufferSize.output(pretty_print_to_log);
	frame.onKey.output(pretty_print_to_log);
	frame.onMaximize.output(pretty_print_to_log);
	frame.onMinimize.output(pretty_print_to_log);
	frame.onMouseButton.output(pretty_print_to_log);
	frame.onMouseEnter.output(pretty_print_to_log);
//	frame.onMousePosition.output(pretty_print_to_log);
	frame.onMouseScroll.output(pretty_print_to_log);
	frame.onPosition.output(pretty_print_to_log);
	frame.onRefresh.output(pretty_print_to_log);
	frame.onSize.output(pretty_print_to_log);

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

		noisyEvents(*this);

		show();

		onKey.output([this](const libv::input::EventKey& e) {
			if (e.action != libv::input::Action::release)
				return;

			if (e.keycode == Keycode::Escape)
				closeDefault();

			// --- 0 row

			if (e.keycode == Keycode::Backtick)
				clearIcon();
			if (e.keycode == Keycode::Num1)
				setIcon(std::vector<libv::frame::Icon>{generateIcon(16, {255, 0, 0, 255}), generateIcon(32, {255, 63, 127, 255}), generateIcon(48, {255, 127, 255, 255})});
			if (e.keycode == Keycode::Num2)
				setIcon(std::vector<libv::frame::Icon>{generateIcon(16, {0, 255, 0, 255}), generateIcon(32, {127, 255, 63, 255}), generateIcon(48, {255, 255, 127, 255})});

			// --- Q row

			if (e.keycode == Keycode::Q)
				executeAfter(std::chrono::seconds{5}, [this]{ this->show(); });
			if (e.keycode == Keycode::W)
				executeAfter(std::chrono::seconds{5}, [this]{ this->hide(); });
			if (e.keycode == Keycode::E)
				executeAfter(std::chrono::seconds{5}, [this]{ this->maximize(); });
			if (e.keycode == Keycode::R)
				executeAfter(std::chrono::seconds{5}, [this]{ this->minimize(); });
			if (e.keycode == Keycode::T)
				executeAfter(std::chrono::seconds{5}, [this]{ this->restore(); });
			if (e.keycode == Keycode::Y)
				executeAfter(std::chrono::seconds{5}, [this]{ this->focus(); });
			if (e.keycode == Keycode::U)
				executeAfter(std::chrono::seconds{5}, [this]{ this->requestAttention(); });

			if (e.keycode == Keycode::I)
				setCursorMode(CursorMode::normal);
			if (e.keycode == Keycode::O)
				setCursorMode(CursorMode::hidden);
			if (e.keycode == Keycode::P)
				setCursorMode(CursorMode::disabled);

			if (e.keycode == Keycode::BracketOpen)
				setClipboardString("Your clipboard now belongs to us.");
			if (e.keycode == Keycode::BracketClose)
				log_sandbox.trace("getClipboardString: {}", getClipboardString());

			if (e.keycode == Keycode::Backslash)
				for (const auto& [_, monitor] : libv::frame::Monitor::monitors) {
					log_sandbox.trace("{} pos {}, work pos {}, work size {}, content scale {}, physical mm {}", monitor.name, monitor.position, monitor.workAreaPosition, monitor.workAreaSize, monitor.contentScale, monitor.physicalSizeMM);
					log_sandbox.trace("     >> size {}, rate {}, bits {}", monitor.currentVideoMode.size, monitor.currentVideoMode.refreshRate, monitor.currentVideoMode.colorBits);
					for (const auto& mode : monitor.videoModes)
						log_sandbox.trace("        size {}, rate {}, bits {}", mode.size, mode.refreshRate, mode.colorBits);
				}

			// --- A row

			if (e.keycode == Keycode::A)
				setDisplayMode(libv::Frame::DisplayMode::windowed);
			if (e.keycode == Keycode::S)
				setDisplayMode(libv::Frame::DisplayMode::borderless_maximized);
			if (e.keycode == Keycode::D)
				setDisplayMode(libv::Frame::DisplayMode::fullscreen);
			if (e.keycode == Keycode::F)
				setDisplayMode(libv::Frame::DisplayMode::fullscreen_windowed);

			if (e.keycode == Keycode::G)
				log_sandbox.trace("getFrameSize: {}", getFrameSize());
			if (e.keycode == Keycode::H)
				log_sandbox.trace("getSize: {}", getSize());
			if (e.keycode == Keycode::J)
				log_sandbox.trace("getPosition: {}", getPosition());

			if (e.keycode == Keycode::K)
				setAlwaysOnTop(true);
			if (e.keycode == Keycode::L)
				setAlwaysOnTop(false);

			if (e.keycode == Keycode::Semicolon)
				setFocusOnShow(true);
			if (e.keycode == Keycode::Apostrophe)
				setFocusOnShow(false);

			// --- Z row

			if (e.keycode == Keycode::Z)
				setResizable(!isResizable());

			if (e.keycode == Keycode::X)
				setDecoration(!isDecorated());

			if (e.keycode == Keycode::C)
				setPosition(FramePosition::center_current_monitor);
			if (e.keycode == Keycode::V)
				setPosition(FramePosition::center_primary_monitor);

			if (e.keycode == Keycode::B)
				setSizeLimit(200, 150, 400, 300);
			if (e.keycode == Keycode::N)
				setSizeLimit(-1, -1, 400, 300);
			if (e.keycode == Keycode::M)
				setSizeLimit(-1, -1, -1, -1);

			if (e.keycode == Keycode::Comma)
				setAspectRatio(16, 9);
			if (e.keycode == Keycode::Period)
				setAspectRatio(-1, -1);
		});
	}
};

int main(int, char**) {
	std::cout << libv::logger_stream;

//	libv::log.allow("libv.frame.event");
//	libv::log.deny();

	libv::frame::Monitor::onMonitor.output([](const auto& event) {
		log_sandbox.trace("Event: {}", event.toPrettyString());
	});

	{
		TestFrame f1("TestFrame1");
		TestFrame f2("TestFrame2");
		f1.join();
		f2.join();
	} {
		TestFrame f3("TestFrame3");
		f3.join();
	}

	return 0;
}
