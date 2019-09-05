// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/math/fixed_point.hpp>
// pro
#include <libv/frame/log.hpp>
#include <libv/frame/impl_frame.lpp>


// TODO P3: Limit cursor position event pre-fire to 1 (solution maybe to use unique signal queue)

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

// <<< P4: multi thread safety?
std::map<GLFWwindow*, Frame*> windowHandlers;

template <typename E, auto queue>
struct DispatchGLFWEvent {
	template <typename... Args>
	static inline void call(GLFWwindow* window, Args... args) {
		try {
			Frame& frame = *windowHandlers.at(window);
//			frame.realizeGLFWCallback(E(args...));
			(frame.self.get()->*queue).fire(E(args...));

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}

	static inline void mouse(GLFWwindow* window, double x, double y) {
		try {
			Frame& frame = *windowHandlers.at(window);
			auto size = frame.getSize();
//			frame.realizeGLFWCallback(E(x, size.y - y - 1));
			(frame.self.get()->*queue).fire(E(x, size.y - y - 1));

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}
};

void Frame::registerEventCallbacks(Frame* frame, GLFWwindow* window) {
	windowHandlers[window] = frame;

	glfwSetCharCallback           (window, DispatchGLFWEvent<EventChar           , &ImplFrame::eventQueChar           >::call);
	glfwSetCursorEnterCallback    (window, DispatchGLFWEvent<EventMouseEnter     , &ImplFrame::eventQueMouseEnter     >::call);
	glfwSetCursorPosCallback      (window, DispatchGLFWEvent<EventMousePosition  , &ImplFrame::eventQueMousePosition  >::mouse);
	glfwSetDropCallback           (window, DispatchGLFWEvent<EventDrop           , &ImplFrame::eventQueDrop           >::call);
	glfwSetFramebufferSizeCallback(window, DispatchGLFWEvent<EventFramebufferSize, &ImplFrame::eventQueFramebufferSize>::call);
	glfwSetKeyCallback            (window, DispatchGLFWEvent<EventKey            , &ImplFrame::eventQueKey            >::call);
	glfwSetMouseButtonCallback    (window, DispatchGLFWEvent<EventMouseButton    , &ImplFrame::eventQueMouseButton    >::call);
	glfwSetScrollCallback         (window, DispatchGLFWEvent<EventMouseScroll    , &ImplFrame::eventQueMouseScroll    >::call);
	glfwSetWindowFocusCallback    (window, DispatchGLFWEvent<EventWindowFocus    , &ImplFrame::eventQueWindowFocus    >::call);
	glfwSetWindowIconifyCallback  (window, DispatchGLFWEvent<EventWindowIconify  , &ImplFrame::eventQueWindowIconify  >::call);
	glfwSetWindowPosCallback      (window, DispatchGLFWEvent<EventWindowPosition , &ImplFrame::eventQueWindowPosition >::call);
	glfwSetWindowRefreshCallback  (window, DispatchGLFWEvent<EventWindowRefresh  , &ImplFrame::eventQueWindowRefresh  >::call);
	glfwSetWindowSizeCallback     (window, DispatchGLFWEvent<EventWindowSize     , &ImplFrame::eventQueWindowSize     >::call);

//	glfwSetCharCallback(window, ::libv::frame::glfwCallback<EventChar, &ImplFrame::eventQueChar, unsigned int>);
//	glfwSetCursorEnterCallback(window, ::libv::frame::glfwCallback<EventMouseEnter, &ImplFrame::eventQueMouseEnter, int>);
//	glfwSetCursorPosCallback(window, ::libv::frame::glfwMousePositionCallback<EventMousePosition, &ImplFrame::eventQueMousePosition>);
//	glfwSetDropCallback(window, ::libv::frame::glfwCallback<EventDrop, &ImplFrame::eventQueDrop, int, const char**>);
//	glfwSetFramebufferSizeCallback(window, ::libv::frame::glfwCallback<EventFramebufferSize, &ImplFrame::eventQueFramebufferSize, int, int>);
//	glfwSetKeyCallback(window, ::libv::frame::glfwCallback<EventKey, &ImplFrame::eventQueKey, int, int, int, int>);
//	glfwSetMouseButtonCallback(window, ::libv::frame::glfwCallback<EventMouseButton, &ImplFrame::eventQueMouseButton, int, int, int>);
//	glfwSetScrollCallback(window, ::libv::frame::glfwCallback<EventMouseScroll, &ImplFrame::eventQueMouseScroll, double, double>);
//	glfwSetWindowFocusCallback(window, ::libv::frame::glfwCallback<EventWindowFocus, &ImplFrame::eventQueWindowFocus, int>);
//	glfwSetWindowIconifyCallback(window, ::libv::frame::glfwCallback<EventWindowIconify, &ImplFrame::eventQueWindowIconify, int>);
//	glfwSetWindowPosCallback(window, ::libv::frame::glfwCallback<EventWindowPosition, &ImplFrame::eventQueWindowPosition, int, int>);
//	glfwSetWindowRefreshCallback(window, ::libv::frame::glfwCallback<EventWindowRefresh, &ImplFrame::eventQueWindowRefresh>);
//	glfwSetWindowSizeCallback(window, ::libv::frame::glfwCallback<EventWindowSize, &ImplFrame::eventQueWindowSize, int, int>);
}

void Frame::unregisterEventCallbacks(GLFWwindow* window) {
	glfwSetCharCallback(window, nullptr);
	glfwSetCursorEnterCallback(window, nullptr);
	glfwSetCursorPosCallback(window, nullptr);
	glfwSetDropCallback(window, nullptr);
	glfwSetFramebufferSizeCallback(window, nullptr);
	glfwSetKeyCallback(window, nullptr);
	glfwSetMouseButtonCallback(window, nullptr);
	glfwSetScrollCallback(window, nullptr);
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

void Frame::realizeGLFWCallback(const EventChar&) { }

void Frame::realizeGLFWCallback(const EventDrop&) { }

void Frame::realizeGLFWCallback(const EventFramebufferSize&) { }

void Frame::realizeGLFWCallback(const EventKey& e) {
	self->keyStates[to_value(e.key)] = (e.action != libv::input::Action::release) ? libv::input::KeyState::pressed : libv::input::KeyState::released;
}

void Frame::realizeGLFWCallback(const EventMouseButton& e) {
	self->mouseStates[to_value(e.button)] = (e.action != libv::input::Action::release) ? libv::input::KeyState::pressed : libv::input::KeyState::released;
}

void Frame::realizeGLFWCallback(const EventMouseEnter&) { }

void Frame::realizeGLFWCallback(const EventMousePosition& e) {
	self->mousePosition = (static_cast<uint64_t>
			(convert_to_s24_8(e.position.x)) << 32) | convert_to_s24_8(e.position.y);
}

void Frame::realizeGLFWCallback(const EventMouseScroll& e) {
	auto old = getScrollPosition();
	self->scrollPosition = (static_cast<uint64_t>
			(convert_to_s24_8(e.offset.x + old.x)) << 32) | convert_to_s24_8(e.offset.y + old.y);
}

void Frame::realizeGLFWCallback(const EventWindowFocus&) { }

void Frame::realizeGLFWCallback(const EventWindowIconify& e) {
	self->minimized = e.iconified;
}

void Frame::realizeGLFWCallback(const EventWindowPosition& e) {
	self->position = e.position;
}

void Frame::realizeGLFWCallback(const EventWindowRefresh&) { }

void Frame::realizeGLFWCallback(const EventWindowSize& e) {
	self->size = e.size;
}

// -------------------------------------------------------------------------------------------------

void Frame::initEventQueues() {
	self->eventQueChar.output(&Frame::realizeGLFWCallback, this);
	self->eventQueDrop.output(&Frame::realizeGLFWCallback, this);
	self->eventQueFramebufferSize.output(&Frame::realizeGLFWCallback, this);
	self->eventQueKey.output(&Frame::realizeGLFWCallback, this);
	self->eventQueMouseButton.output(&Frame::realizeGLFWCallback, this);
	self->eventQueMouseEnter.output(&Frame::realizeGLFWCallback, this);
	self->eventQueMousePosition.output(&Frame::realizeGLFWCallback, this);
	self->eventQueMouseScroll.output(&Frame::realizeGLFWCallback, this);
	self->eventQueWindowFocus.output(&Frame::realizeGLFWCallback, this);
	self->eventQueWindowIconify.output(&Frame::realizeGLFWCallback, this);
	self->eventQueWindowPosition.output(&Frame::realizeGLFWCallback, this);
	self->eventQueWindowRefresh.output(&Frame::realizeGLFWCallback, this);
	self->eventQueWindowSize.output(&Frame::realizeGLFWCallback, this);

	self->eventQueChar.output(onChar);
	self->eventQueDrop.output(onDrop);
	self->eventQueFramebufferSize.output(onFramebufferSize);
	self->eventQueKey.output(onKey);
	self->eventQueMouseButton.output(onMouseButton);
	self->eventQueMouseEnter.output(onMouseEnter);
	self->eventQueMousePosition.output(onMousePosition);
	self->eventQueMouseScroll.output(onMouseScroll);
	self->eventQueWindowFocus.output(onWindowFocus);
	self->eventQueWindowIconify.output(onWindowIconify);
	self->eventQueWindowPosition.output(onWindowPosition);
	self->eventQueWindowRefresh.output(onWindowRefresh);
	self->eventQueWindowSize.output(onWindowSize);
}

void Frame::distributeEvents() {
	self->eventQueChar.flush();
	self->eventQueDrop.flush();
	self->eventQueFramebufferSize.flush();
	self->eventQueKey.flush();
	self->eventQueMouseButton.flush();

	// TODO P1: serialize events
	self->eventQueMousePosition.flush();
	self->eventQueMouseScroll.flush();
	self->eventQueMouseEnter.flush(); // Enter is reordered to be the last mouse event so other events don't trigger false re-entry
	//

	self->eventQueWindowFocus.flush();
	self->eventQueWindowIconify.flush();
	self->eventQueWindowPosition.flush();
	self->eventQueWindowRefresh.flush();
	self->eventQueWindowSize.flush();
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv


//Char
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
