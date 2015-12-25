// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include <libv/ui/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// pro
#include <libv/ui/log.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::map<GLFWwindow*, Frame*> windowHandlers;

template <typename E, CapacitivSignal<const E&> Frame::* que, typename... Args>
void glfwCallback(GLFWwindow* window, Args... args) {
	try {
		(windowHandlers.at(window)->*que).fire(E(args...));
	} catch (const std::out_of_range& e) {
		LIBV_UI_EVENT_ERROR("Unhandeled event. No event handler (frame) assigned to this window.");
	}
}

void Frame::registerEventCallbacks(Frame* frame, GLFWwindow* window) {
	windowHandlers[window] = frame;

	glfwSetCharCallback(window, ::libv::ui::glfwCallback<EventChar, &Frame::eventQueChar, unsigned int>);
	glfwSetCharModsCallback(window, ::libv::ui::glfwCallback<EventCharMods, &Frame::eventQueCharMods, unsigned int, int>);
	glfwSetCursorEnterCallback(window, ::libv::ui::glfwCallback<EventCursorEnter, &Frame::eventQueCursorEnter, int>);
	glfwSetCursorPosCallback(window, ::libv::ui::glfwCallback<EventCursorPos, &Frame::eventQueCursorPos, double, double>);
	glfwSetDropCallback(window, ::libv::ui::glfwCallback<EventDrop, &Frame::eventQueDrop, int, const char**>);
	glfwSetFramebufferSizeCallback(window, ::libv::ui::glfwCallback<EventFramebufferSize, &Frame::eventQueFramebufferSize, int, int>);
	glfwSetKeyCallback(window, ::libv::ui::glfwCallback<EventKey, &Frame::eventQueKey, int, int, int, int>);
	glfwSetMouseButtonCallback(window, ::libv::ui::glfwCallback<EventMouseButton, &Frame::eventQueMouseButton, int, int, int>);
	glfwSetScrollCallback(window, ::libv::ui::glfwCallback<EventScroll, &Frame::eventQueScroll, double, double>);
	glfwSetWindowCloseCallback(window, ::libv::ui::glfwCallback<EventWindowClose, &Frame::eventQueWindowClose>);
	glfwSetWindowFocusCallback(window, ::libv::ui::glfwCallback<EventWindowFocus, &Frame::eventQueWindowFocus, int>);
	glfwSetWindowIconifyCallback(window, ::libv::ui::glfwCallback<EventWindowIconify, &Frame::eventQueWindowIconify, int>);
	glfwSetWindowPosCallback(window, ::libv::ui::glfwCallback<EventWindowPos, &Frame::eventQueWindowPos, int, int>);
	glfwSetWindowRefreshCallback(window, ::libv::ui::glfwCallback<EventWindowRefresh, &Frame::eventQueWindowRefresh>);
	glfwSetWindowSizeCallback(window, ::libv::ui::glfwCallback<EventWindowSize, &Frame::eventQueWindowSize, int, int>);
}

void Frame::unregisterEventCallbacks(GLFWwindow * window) {
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
	//sub for every monitor
	//glfwSetMonitorCallback(nullptr);
}

// -------------------------------------------------------------------------------------------------

void Frame::glfwCallback(const EventChar&) { }

void Frame::glfwCallback(const EventCharMods&) { }

void Frame::glfwCallback(const EventCursorEnter&) { }

void Frame::glfwCallback(const EventCursorPos&) { }

void Frame::glfwCallback(const EventDrop&) { }

void Frame::glfwCallback(const EventFramebufferSize& e) {
	if (size.xy() == e.size)
		return;

	size = ivec3(e.size, size.z);
	set(Property::Size, size);
	baseInvalidate();
}

void Frame::glfwCallback(const EventKey&) { }

void Frame::glfwCallback(const EventMouseButton&) { }

void Frame::glfwCallback(const EventScroll&) { }

void Frame::glfwCallback(const EventWindowClose&) { }

void Frame::glfwCallback(const EventWindowFocus&) {
//	LIBV_UI_TRACE("Not implemented yet.");
}

void Frame::glfwCallback(const EventWindowIconify& e) {
	minimalized = e.iconified;
}

void Frame::glfwCallback(const EventWindowPos& e) {
	pos = e.position;
}

void Frame::glfwCallback(const EventWindowRefresh&) {
}

void Frame::glfwCallback(const EventWindowSize&) {
	// EventFramebufferSize event handles this operation
}

// -------------------------------------------------------------------------------------------------

void Frame::initEvents() {
	eventQueChar.output(onChar);
	eventQueCharMods.output(onCharMods);
	eventQueCursorEnter.output(onCursorEnter);
	eventQueCursorPos.output(onCursorPos);
	eventQueDrop.output(onDrop);
	eventQueFramebufferSize.output(onFramebufferSize);
	eventQueKey.output(onKey);
	eventQueMouseButton.output(onMouseButton);
	eventQueScroll.output(onScroll);
	eventQueWindowClose.output(onWindowClose);
	eventQueWindowFocus.output(onWindowFocus);
	eventQueWindowIconify.output(onWindowIconify);
	eventQueWindowPos.output(onWindowPos);
	eventQueWindowRefresh.output(onWindowRefresh);
	eventQueWindowSize.output(onWindowSize);

	eventQueChar.output(&Frame::glfwCallback, this);
	eventQueCharMods.output(&Frame::glfwCallback, this);
	eventQueCursorEnter.output(&Frame::glfwCallback, this);
	eventQueCursorPos.output(&Frame::glfwCallback, this);
	eventQueDrop.output(&Frame::glfwCallback, this);
	eventQueFramebufferSize.output(&Frame::glfwCallback, this);
	eventQueKey.output(&Frame::glfwCallback, this);
	eventQueMouseButton.output(&Frame::glfwCallback, this);
	eventQueScroll.output(&Frame::glfwCallback, this);
	eventQueWindowClose.output(&Frame::glfwCallback, this);
	eventQueWindowFocus.output(&Frame::glfwCallback, this);
	eventQueWindowIconify.output(&Frame::glfwCallback, this);
	eventQueWindowPos.output(&Frame::glfwCallback, this);
	eventQueWindowRefresh.output(&Frame::glfwCallback, this);
	eventQueWindowSize.output(&Frame::glfwCallback, this);
}

void Frame::distributeEvents() {
	eventQueChar.flush();
	eventQueCharMods.flush();
	eventQueCursorEnter.flush();
	eventQueCursorPos.flush();
	eventQueDrop.flush();
	eventQueFramebufferSize.flush();
	eventQueKey.flush();
	eventQueMouseButton.flush();
	eventQueScroll.flush();
	eventQueWindowClose.flush();
	eventQueWindowFocus.flush();
	eventQueWindowIconify.flush();
	eventQueWindowPos.flush();
	eventQueWindowRefresh.flush();
	eventQueWindowSize.flush();
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv


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
//WindowPos
//WindowRefresh
//WindowSize
//Monitor

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
//WindowPos
//WindowRefresh
//WindowSize
//Monitor
