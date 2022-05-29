// Project: libv.frame, File: src/libv/frame/impl_frame.lpp, Author: Császár Mátyás [Vader]

#pragma once

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <boost/container/flat_set.hpp>
#include <fmt/format.h>
// libv
#include <libv/input/input.hpp>
#include <libv/mt/executor_thread.hpp>
// std
#include <mutex>
// pro
#include <libv/frame/core.hxx>
#include <libv/frame/events.hpp>
#include <libv/frame/events_variant.hpp>
#include <libv/frame/icon.hpp>
#include <libv/frame/log.hpp>


class GLFWwindow;

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

class ImplFrame : public libv::Trackable {
public:
	std::atomic<bool> forcedClose{false};

public:
	GLFWwindow* window = nullptr;
	GLFWwindow* shareWindow = nullptr;

	libv::ExecutorThread context;
	std::atomic<bool> showWasCalled{false};

	std::shared_ptr<Core> core = getCoreInstance();

public:
	int32_t swapInterval = 1;
	libv::vec2i aspectRatio = {-1, -1};
	libv::vec2f contentScale = {1, 1};
	libv::vec2i framebufferSize;
	libv::vec2i position = {};
	libv::vec2i size;
	libv::vec4i frameSize = {}; /// x - left, y - top, z - right, w - bottom
	libv::vec2i sizeLimitMax = {-1, -1};
	libv::vec2i sizeLimitMin = {-1, -1};

public:
	std::vector<GLFWimage> iconsGLFW;
	std::vector<Icon> icons;

public:
	Frame::OpenGLProfile openGLProfile = Frame::OpenGLProfile::compat;
	Frame::OpenGLRefreshRate openGLRefreshRate = Frame::OpenGLRefreshRate::dont_care;
	Frame::OpenGLSamples openGLSamples = Frame::OpenGLSamples::dont_care;
	int openGLVersionMajor = 3;
	int openGLVersionMinor = 3;
	bool openGLForwardCompat = true;

	bool decorated = true;
	bool resizable = true;
	bool alwaysOnTop = false;

	bool hidden = true;
	bool maximized = false;
	bool minimized = false;

	bool initialFocus = true;
	bool focusOnShow = true;

	Frame::CloseOperation defaultCloseOperation = Frame::CloseOperation::dispose;
	Frame::CursorMode cursorMode = Frame::CursorMode::normal;
	Frame::DisplayMode displayMode = Frame::DisplayMode::windowed;

	std::string title;

public:
	std::mutex frameState_m; /// Guards every state that is observable by getters
	boost::container::flat_set<libv::input::Keycode> pressedKeys;
	boost::container::flat_set<libv::input::Scancode> pressedScancodes;
	boost::container::flat_set<libv::input::MouseButton> pressedMouseButtons;

	libv::vec2d mousePosition;
	libv::vec2d scrollPosition;

	// TODO P2: libv.frame: Joystick/Gamepad support: Store states

public:
	std::mutex eventQueue_m; /// glfw documentation states that not just pulling even can cause event callbacks to fire, meaning unknown threads could enter
	std::vector<Event> eventQueue;

public:
	ImplFrame(std::string title, libv::vec2i size) :
		context(fmt::format("Frame thread of {}", title)),
		size(size),
		title(std::move(title)) {

		eventQueue.reserve(16);
		pressedKeys.reserve(16);
		pressedMouseButtons.reserve(16);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
