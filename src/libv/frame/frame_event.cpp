// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/math/fixed_point.hpp>
#include <libv/utility/overload.hpp>
// pro
#include <libv/frame/impl_frame.lpp>
#include <libv/frame/log.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

// <<< P4: multi thread safety?
std::map<GLFWwindow*, Frame*> windowHandlers;

template <typename E>
struct DispatchGLFWEvent {
	template <typename... Args>
	static inline void call(GLFWwindow* window, Args... args) {
		try {
			Frame& frame = *windowHandlers.at(window);
			frame.self->eventQueue.emplace_back(E(args...));

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}

	static inline void mouse(GLFWwindow* window, double x, double y) {
		try {
			Frame& frame = *windowHandlers.at(window);
			auto size = frame.getSize();
			frame.self->eventQueue.emplace_back(E(x, size.y - y - 1));

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}
};

void Frame::registerEventCallbacks(Frame* frame, GLFWwindow* window) {
	windowHandlers[window] = frame;

	glfwSetCharCallback           (window, DispatchGLFWEvent<EventChar           >::call);
	glfwSetDropCallback           (window, DispatchGLFWEvent<EventDrop           >::call);
	glfwSetFramebufferSizeCallback(window, DispatchGLFWEvent<EventFramebufferSize>::call);
	glfwSetKeyCallback            (window, DispatchGLFWEvent<EventKey            >::call);
	glfwSetMouseButtonCallback    (window, DispatchGLFWEvent<EventMouseButton    >::call);
	glfwSetCursorEnterCallback    (window, DispatchGLFWEvent<EventMouseEnter     >::call);
	glfwSetCursorPosCallback      (window, DispatchGLFWEvent<EventMousePosition  >::mouse);
	glfwSetScrollCallback         (window, DispatchGLFWEvent<EventMouseScroll    >::call);
	glfwSetWindowFocusCallback    (window, DispatchGLFWEvent<EventWindowFocus    >::call);
	glfwSetWindowIconifyCallback  (window, DispatchGLFWEvent<EventWindowIconify  >::call);
	glfwSetWindowPosCallback      (window, DispatchGLFWEvent<EventWindowPosition >::call);
	glfwSetWindowRefreshCallback  (window, DispatchGLFWEvent<EventWindowRefresh  >::call);
	glfwSetWindowSizeCallback     (window, DispatchGLFWEvent<EventWindowSize     >::call);
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

void Frame::distributeEvents() {
	for (const auto& event : self->eventQueue) {
		const auto visitor = libv::overload(
			[this](const EventChar& e) {
				onChar.fire(e);
			},
			[this](const EventDrop& e) {
				onDrop.fire(e);
			},
			[this](const EventFramebufferSize& e) {
				onFramebufferSize.fire(e);
			},
			[this](const EventKey& e) {
				self->keyStates[to_value(e.key)] = (e.action != libv::input::Action::release) ?
						libv::input::KeyState::pressed :
						libv::input::KeyState::released;

				onKey.fire(e);
			},
			[this](const EventMouseButton& e) {
				self->mouseStates[to_value(e.button)] = (e.action != libv::input::Action::release) ?
						libv::input::KeyState::pressed :
						libv::input::KeyState::released;

				onMouseButton.fire(e);
			},
			[this](const EventMouseEnter& e) {
				onMouseEnter.fire(e);
			},
			[this](const EventMousePosition& e) {
				self->mousePosition = (static_cast<uint64_t>
						(convert_to_s24_8(e.position.x)) << 32) | convert_to_s24_8(e.position.y);

				onMousePosition.fire(e);
			},
			[this](const EventMouseScroll& e) {
				auto old = getScrollPosition();
				self->scrollPosition =
						(static_cast<uint64_t>(convert_to_s24_8(e.offset.x + old.x)) << 32) |
						convert_to_s24_8(e.offset.y + old.y);

				onMouseScroll.fire(e);
			},
			[this](const EventWindowFocus& e) {
				onWindowFocus.fire(e);
			},
			[this](const EventWindowIconify& e) {
				self->minimized = e.iconified;

				onWindowIconify.fire(e);
			},
			[this](const EventWindowPosition& e) {
				self->position = e.position;

				onWindowPosition.fire(e);
			},
			[this](const EventWindowRefresh& e) {
				onWindowRefresh.fire(e);
			},
			[this](const EventWindowSize& e) {
				self->size = e.size;

				onWindowSize.fire(e);
			}
		);

		std::visit(visitor, event);
	}
	self->eventQueue.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
