// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include <libv/ui/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/fixed.hpp>
// pro
#include <libv/ui/log.hpp>


// TODO P3: Limit cursor pos event pre-fire to 1 (solution maybe to use unique signal que)

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::map<GLFWwindow*, Frame*> windowHandlers;

template <typename E, CapacitivSignal<const E&> Frame::* que, typename... Args>
void glfwCallback(GLFWwindow* window, Args... args) {
	try {
		(windowHandlers.at(window)->*que).fire(E(args...));
	} catch (const std::out_of_range& e) {
		LIBV_LOG_UI_EVENT_ERROR("Unhandeled event. No event handler (frame) assigned to this GLFW window.");
	}
}

template <typename E, CapacitivSignal<const E&> Frame::* que>
void glfwMousePositionCallback(GLFWwindow* window, double x, double y) {
	try {
		auto frame = windowHandlers.at(window);
		auto size = frame->getSize();
		(frame->*que).fire(E(x, size.y - y));
	} catch (const std::out_of_range& e) {
		LIBV_LOG_UI_EVENT_ERROR("Unhandeled event. No event handler (frame) assigned to this GLFW window.");
	}
}

void Frame::registerEventCallbacks(Frame* frame, GLFWwindow* window) {
	windowHandlers[window] = frame;

	glfwSetCharCallback(window, ::libv::ui::glfwCallback<EventChar, &Frame::eventQueChar, unsigned int>);
	glfwSetCharModsCallback(window, ::libv::ui::glfwCallback<EventCharMods, &Frame::eventQueCharMods, unsigned int, int>);
	glfwSetCursorEnterCallback(window, ::libv::ui::glfwCallback<EventMouseEnter, &Frame::eventQueMouseEnter, int>);
	glfwSetCursorPosCallback(window, ::libv::ui::glfwMousePositionCallback<EventMousePosition, &Frame::eventQueMousePosition>);
	glfwSetDropCallback(window, ::libv::ui::glfwCallback<EventDrop, &Frame::eventQueDrop, int, const char**>);
	glfwSetFramebufferSizeCallback(window, ::libv::ui::glfwCallback<EventFramebufferSize, &Frame::eventQueFramebufferSize, int, int>);
	glfwSetKeyCallback(window, ::libv::ui::glfwCallback<EventKey, &Frame::eventQueKey, int, int, int, int>);
	glfwSetMouseButtonCallback(window, ::libv::ui::glfwCallback<EventMouseButton, &Frame::eventQueMouseButton, int, int, int>);
	glfwSetScrollCallback(window, ::libv::ui::glfwCallback<EventMouseScroll, &Frame::eventQueMouseScroll, double, double>);
	glfwSetWindowCloseCallback(window, ::libv::ui::glfwCallback<EventWindowClose, &Frame::eventQueWindowClose>);
	glfwSetWindowFocusCallback(window, ::libv::ui::glfwCallback<EventWindowFocus, &Frame::eventQueWindowFocus, int>);
	glfwSetWindowIconifyCallback(window, ::libv::ui::glfwCallback<EventWindowIconify, &Frame::eventQueWindowIconify, int>);
	glfwSetWindowPosCallback(window, ::libv::ui::glfwCallback<EventWindowPosition, &Frame::eventQueWindowPosition, int, int>);
	glfwSetWindowRefreshCallback(window, ::libv::ui::glfwCallback<EventWindowRefresh, &Frame::eventQueWindowRefresh>);
	glfwSetWindowSizeCallback(window, ::libv::ui::glfwCallback<EventWindowSize, &Frame::eventQueWindowSize, int, int>);
}

void Frame::unregisterEventCallbacks(GLFWwindow* window) {
	glfwSetCharCallback(window, nullptr);
	glfwSetCursorEnterCallback(window, nullptr);
	glfwSetCursorPosCallback(window, nullptr);
	glfwSetFramebufferSizeCallback(window, nullptr);
	glfwSetKeyCallback(window, nullptr);
	glfwSetMouseButtonCallback(window, nullptr);
	glfwSetScrollCallback(window, nullptr);
	glfwSetWindowCloseCallback(window, nullptr);
	glfwSetWindowFocusCallback(window, nullptr);
	glfwSetWindowIconifyCallback(window, nullptr);
	glfwSetWindowPosCallback(window, nullptr);
	glfwSetWindowRefreshCallback(window, nullptr);
	glfwSetWindowSizeCallback(window, nullptr);

	windowHandlers.erase(window);
}

// -------------------------------------------------------------------------------------------------

// These event callbacks called after the first iteration of the loop after the event occurred.
// These are the realization of an event on the frame object itself. Most event is noop due to lack
// of impact on the frame.

void Frame::glfwCallback(const EventChar&) { }

void Frame::glfwCallback(const EventCharMods&) { }

void Frame::glfwCallback(const EventDrop&) { }

void Frame::glfwCallback(const EventFramebufferSize& e) {
	size = e.size;
}

void Frame::glfwCallback(const EventKey& e) {
	keyStates[to_value(e.key)] = (e.action != GLFW_RELEASE) ? KeyState::pressed : KeyState::released;
}

void Frame::glfwCallback(const EventMouseButton& e) {
	mouseStates[to_value(e.button)] = (e.action != GLFW_RELEASE) ? KeyState::pressed : KeyState::released;
}

void Frame::glfwCallback(const EventMouseEnter&) { }

void Frame::glfwCallback(const EventMousePosition& e) {
	mousePosition = (static_cast<uint64_t>
			(convert_to_s_24_8(e.position.x)) << 32) | convert_to_s_24_8(e.position.y);
}

void Frame::glfwCallback(const EventMouseScroll& e) {
	auto old = getScrollPosition();
	scrollPosition = (static_cast<uint64_t>
			(convert_to_s_24_8(e.offset.x + old.x)) << 32) | convert_to_s_24_8(e.offset.y + old.y);
}

void Frame::glfwCallback(const EventWindowClose&) { }

void Frame::glfwCallback(const EventWindowFocus&) { }

void Frame::glfwCallback(const EventWindowIconify& e) {
	minimized = e.iconified;
}

void Frame::glfwCallback(const EventWindowPosition& e) {
	position = e.position;
}

void Frame::glfwCallback(const EventWindowRefresh&) { }

void Frame::glfwCallback(const EventWindowSize&) { }

// -------------------------------------------------------------------------------------------------

void Frame::initEvents() {
	eventQueChar.output(&Frame::glfwCallback, this);
	eventQueCharMods.output(&Frame::glfwCallback, this);
	eventQueDrop.output(&Frame::glfwCallback, this);
	eventQueFramebufferSize.output(&Frame::glfwCallback, this);
	eventQueKey.output(&Frame::glfwCallback, this);
	eventQueMouseButton.output(&Frame::glfwCallback, this);
	eventQueMouseEnter.output(&Frame::glfwCallback, this);
	eventQueMousePosition.output(&Frame::glfwCallback, this);
	eventQueMouseScroll.output(&Frame::glfwCallback, this);
	eventQueWindowClose.output(&Frame::glfwCallback, this);
	eventQueWindowFocus.output(&Frame::glfwCallback, this);
	eventQueWindowIconify.output(&Frame::glfwCallback, this);
	eventQueWindowPosition.output(&Frame::glfwCallback, this);
	eventQueWindowRefresh.output(&Frame::glfwCallback, this);
	eventQueWindowSize.output(&Frame::glfwCallback, this);

	eventQueChar.output(onChar);
	eventQueCharMods.output(onCharMods);
	eventQueDrop.output(onDrop);
	eventQueFramebufferSize.output(onFramebufferSize);
	eventQueKey.output(onKey);
	eventQueMouseButton.output(onMouseButton);
	eventQueMouseEnter.output(onMouseEnter);
	eventQueMousePosition.output(onMousePosition);
	eventQueMouseScroll.output(onMouseScroll);
	eventQueWindowClose.output(onWindowClose);
	eventQueWindowFocus.output(onWindowFocus);
	eventQueWindowIconify.output(onWindowIconify);
	eventQueWindowPosition.output(onWindowPosition);
	eventQueWindowRefresh.output(onWindowRefresh);
	eventQueWindowSize.output(onWindowSize);
}

void Frame::distributeEvents() {
	eventQueChar.flush();
	eventQueCharMods.flush();
	eventQueDrop.flush();
	eventQueFramebufferSize.flush();
	eventQueKey.flush();
	eventQueMouseButton.flush();
	eventQueMouseEnter.flush();
	eventQueMousePosition.flush();
	eventQueMouseScroll.flush();
	eventQueWindowClose.flush();
	eventQueWindowFocus.flush();
	eventQueWindowIconify.flush();
	eventQueWindowPosition.flush();
	eventQueWindowRefresh.flush();
	eventQueWindowSize.flush();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv


//Char
//CharMods
//CursorEnter
//CursorPos
//Drop
//FramebufferSize
//Key
//MouseButton
//Scroll
//WindowClose
//WindowFocus
//WindowIconify
//WindowPosition
//WindowRefresh
//WindowSize
//Monitor
