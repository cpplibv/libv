// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/utility/overload.hpp>
// pro
#include <libv/frame/events.hpp>
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

	template <typename... Args>
	static inline void frame(GLFWwindow* window, Args... args) {
		try {
			Frame& frame = *windowHandlers.at(window);
			frame.self->eventQueue.emplace_back(E(args...));

			glfwGetWindowFrameSize(window,
					&frame.self->frameSize.x,
					&frame.self->frameSize.y,
					&frame.self->frameSize.z,
					&frame.self->frameSize.w);

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}

	static inline void mouse(GLFWwindow* window, double x, double y) {
		try {
			Frame& frame = *windowHandlers.at(window);
			std::vector<Event>& queue = frame.self->eventQueue;
			auto size = frame.getSize();

			if (!queue.empty() && std::holds_alternative<E>(queue.back()))
				std::get<E>(queue.back()).position = libv::vec2d{x, size.y - y - 1};
			else
				queue.emplace_back(E(x, size.y - y - 1));

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
	glfwSetWindowMaximizeCallback (window, DispatchGLFWEvent<EventWindowMaximize >::frame);
	glfwSetWindowIconifyCallback  (window, DispatchGLFWEvent<EventWindowMinimize >::frame);
	glfwSetWindowPosCallback      (window, DispatchGLFWEvent<EventWindowPosition >::frame);
	glfwSetWindowRefreshCallback  (window, DispatchGLFWEvent<EventWindowRefresh  >::call);
	glfwSetWindowSizeCallback     (window, DispatchGLFWEvent<EventWindowSize     >::frame);
}

void Frame::unregisterEventCallbacks(GLFWwindow* window) {
	glfwSetCharCallback           (window, nullptr);
	glfwSetCursorEnterCallback    (window, nullptr);
	glfwSetCursorPosCallback      (window, nullptr);
	glfwSetDropCallback           (window, nullptr);
	glfwSetFramebufferSizeCallback(window, nullptr);
	glfwSetKeyCallback            (window, nullptr);
	glfwSetMouseButtonCallback    (window, nullptr);
	glfwSetScrollCallback         (window, nullptr);
	glfwSetWindowFocusCallback    (window, nullptr);
	glfwSetWindowMaximizeCallback (window, nullptr);
	glfwSetWindowIconifyCallback  (window, nullptr);
	glfwSetWindowPosCallback      (window, nullptr);
	glfwSetWindowRefreshCallback  (window, nullptr);
	glfwSetWindowSizeCallback     (window, nullptr);

	windowHandlers.erase(window);
}

// -------------------------------------------------------------------------------------------------

void Frame::distributeEvents() {
	for (const auto& event : self->eventQueue) {
		const auto visitor = libv::overload(
			[this](const EventChar& e) {
				onChar.fire(e);

			}, [this](const EventDrop& e) {
				onDrop.fire(e);

			}, [this](const EventFramebufferSize& e) {
				onFramebufferSize.fire(e);

			}, [this](const EventKey& e) {
				if (e.action != libv::input::Action::release) {
					self->pressedKeys.insert(e.key);
					self->pressedScancodes.insert(e.scancode);
				} else {
					self->pressedKeys.erase(e.key);
					self->pressedScancodes.erase(e.scancode);
				}

				onKey.fire(e);

			}, [this](const EventMouseButton& e) {
				if (e.action != libv::input::Action::release)
					self->pressedMouseButtons.insert(e.button);
				else
					self->pressedMouseButtons.erase(e.button);

				onMouseButton.fire(e);

			}, [this](const EventMouseEnter& e) {
				onMouseEnter.fire(e);

			}, [this](const EventMousePosition& e) {
				self->mousePosition = e.position;

				onMousePosition.fire(e);

			}, [this](const EventMouseScroll& e) {
				self->scrollPosition += e.offset;

				onMouseScroll.fire(e);

			}, [this](const EventWindowFocus& e) {
				onWindowFocus.fire(e);

			}, [this](const EventWindowMaximize& e) {
				self->maximized = e.maximized;
				self->minimized = false;

				onWindowMaximize.fire(e);

			}, [this](const EventWindowMinimize& e) {
				self->maximized = false;
				self->minimized = e.minimized;

				onWindowMinimize.fire(e);

			}, [this](const EventWindowPosition& e) {
				self->position = e.position;

				onWindowPosition.fire(e);

			}, [this](const EventWindowRefresh& e) {
				onWindowRefresh.fire(e);

			}, [this](const EventWindowSize& e) {
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
