// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/utility/overload.hpp>
// std
#include <mutex>
// pro
#include <libv/frame/events.hpp>
#include <libv/frame/impl_frame.lpp>
#include <libv/frame/log.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

std::mutex windowHandlers_m;
std::map<GLFWwindow*, Frame*> windowHandlers;

template <typename E>
struct DispatchGLFWEvent {
	static inline void eventDrop(GLFWwindow* window, int count, const char** data) {
		try {
			std::vector<std::string> payload;
			payload.reserve(count);
			for (int i = 0; i < count; ++i)
				payload.emplace_back(data[i]);

			std::lock_guard lock_handler(windowHandlers_m);
			Frame& frame = *windowHandlers.at(window);
			std::lock_guard lock_queue(frame.self->eventQueue_m);
			frame.self->eventQueue.emplace_back(E{std::move(payload)});

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}

	static inline void eventKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
		try {
			std::lock_guard lock_handler(windowHandlers_m);
			Frame& frame = *windowHandlers.at(window);
			std::lock_guard lock_queue(frame.self->eventQueue_m);
			frame.self->eventQueue.emplace_back(E{libv::input::Key{key}, libv::input::Scancode{scancode}, libv::input::Action{action}, libv::input::KeyModifier{mods}});

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}

	static inline void eventMouseButton(GLFWwindow* window, int button, int action, int mods) {
		try {
			std::lock_guard lock_handler(windowHandlers_m);
			Frame& frame = *windowHandlers.at(window);
			std::lock_guard lock_queue(frame.self->eventQueue_m);
			frame.self->eventQueue.emplace_back(E{libv::input::MouseButton{button}, libv::input::Action{action}, libv::input::KeyModifier{mods}});

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}

	template <typename... Args>
	static inline void call(GLFWwindow* window, Args... args) {
		try {
			std::lock_guard lock_handler(windowHandlers_m);
			Frame& frame = *windowHandlers.at(window);
			std::lock_guard lock_queue(frame.self->eventQueue_m);
			frame.self->eventQueue.emplace_back(E(args...));

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}

	template <typename... Args>
	static inline void frame(GLFWwindow* window, Args... args) {
		try {
			std::lock_guard lock_handler(windowHandlers_m);
			Frame& frame = *windowHandlers.at(window);
			std::lock_guard lock_queue(frame.self->eventQueue_m);
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
			std::lock_guard lock_handler(windowHandlers_m);
			Frame& frame = *windowHandlers.at(window);
			std::vector<Event>& queue = frame.self->eventQueue;
			auto size = frame.getSize();

			std::lock_guard lock_queue(frame.self->eventQueue_m);
			if (!queue.empty() && std::holds_alternative<E>(queue.back()))
				std::get<E>(queue.back()).position = libv::vec2d{x, size.y - y - 1};
			else
				queue.emplace_back(E(x, size.y - y - 1));

		} catch (const std::out_of_range& e) {
			log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");
		}
	}

	// Workaround: 2019.09.07 Win7: Windowed full-screen focus loss sets the window size and framebuffer size to zero, and does not restore it on focus gain.
	//		As a zero area window makes no sense it safe to discard these events
	static inline void sizeFB(GLFWwindow* window, int x, int y) {
		if (x == 0 && y == 0)
			return log_event.warn("Discarded zero size FrameBufferSize event");

		call(window, x, y);
	}

	// Workaround: 2019.09.07 Win7: Windowed full-screen focus loss sets the window size and framebuffer size to zero, and does not restore it on focus gain.
	//		As a zero area window makes no sense it safe to discard these events
	static inline void sizeWS(GLFWwindow* window, int x, int y) {
		if (x == 0 && y == 0)
			return log_event.warn("Discarded zero size WindowSize event");

		frame(window, x, y);
	}

	// Workaround: 2019.09.07 Win7: Windowed full-screen focus loss sets the window position to (-32000, -32000), and does not restore it on focus gain.
	//		As such position values would make no sense it safe to discard these events
	static inline void pos(GLFWwindow* window, int x, int y) {
		if (x == -32000 && y == -32000)
			return log_event.warn("Discarded invalid WindowPosition event");

		frame(window, x, y);
	}
};

void Frame::registerEventCallbacks(Frame* frame, GLFWwindow* window) {
	{
		std::lock_guard lock(windowHandlers_m);
		windowHandlers[window] = frame;
	}

	glfwSetCharCallback              (window, DispatchGLFWEvent<libv::input::EventChar         >::call);
	glfwSetWindowContentScaleCallback(window, DispatchGLFWEvent<EventContentScale              >::call);
	glfwSetDropCallback              (window, DispatchGLFWEvent<libv::input::EventDrop         >::eventDrop);
	glfwSetWindowFocusCallback       (window, DispatchGLFWEvent<EventFocus                     >::call);
	glfwSetFramebufferSizeCallback   (window, DispatchGLFWEvent<EventFramebufferSize           >::sizeFB);
	glfwSetKeyCallback               (window, DispatchGLFWEvent<libv::input::EventKey          >::eventKey);
	glfwSetWindowMaximizeCallback    (window, DispatchGLFWEvent<EventMaximize                  >::frame);
	glfwSetWindowIconifyCallback     (window, DispatchGLFWEvent<EventMinimize                  >::frame);
	glfwSetMouseButtonCallback       (window, DispatchGLFWEvent<libv::input::EventMouseButton  >::eventMouseButton);
	glfwSetCursorEnterCallback       (window, DispatchGLFWEvent<libv::input::EventMouseEnter   >::call);
	glfwSetCursorPosCallback         (window, DispatchGLFWEvent<libv::input::EventMousePosition>::mouse);
	glfwSetScrollCallback            (window, DispatchGLFWEvent<libv::input::EventMouseScroll  >::call);
	glfwSetWindowPosCallback         (window, DispatchGLFWEvent<EventPosition                  >::pos);
	glfwSetWindowRefreshCallback     (window, DispatchGLFWEvent<EventRefresh                   >::call);
	glfwSetWindowSizeCallback        (window, DispatchGLFWEvent<EventSize                      >::sizeWS);
}

void Frame::unregisterEventCallbacks(GLFWwindow* window) {
	glfwSetCharCallback              (window, nullptr);
	glfwSetWindowContentScaleCallback(window, nullptr);
	glfwSetDropCallback              (window, nullptr);
	glfwSetWindowFocusCallback       (window, nullptr);
	glfwSetFramebufferSizeCallback   (window, nullptr);
	glfwSetKeyCallback               (window, nullptr);
	glfwSetWindowMaximizeCallback    (window, nullptr);
	glfwSetMouseButtonCallback       (window, nullptr);
	glfwSetCursorEnterCallback       (window, nullptr);
	glfwSetCursorPosCallback         (window, nullptr);
	glfwSetScrollCallback            (window, nullptr);
	glfwSetWindowIconifyCallback     (window, nullptr);
	glfwSetWindowPosCallback         (window, nullptr);
	glfwSetWindowRefreshCallback     (window, nullptr);
	glfwSetWindowSizeCallback        (window, nullptr);

	{
		std::lock_guard lock(windowHandlers_m);
		windowHandlers.erase(window);
	}
}

// -------------------------------------------------------------------------------------------------

void Frame::distributeEvents() {
	std::lock_guard lock(self->eventQueue_m);
	for (const auto& event : self->eventQueue) {
		const auto visitor = libv::overload(
			[this](const libv::input::EventChar& e) {
				onChar.fire(e);

			}, [this](const EventContentScale& e) {
				{
					std::lock_guard lock(self->frameState_m);
					self->contentScale = e.scale;
				}
				onContentScale.fire(e);

			}, [this](const libv::input::EventDrop& e) {
				onDrop.fire(e);

			}, [this](const EventFocus& e) {
				onFocus.fire(e);

			}, [this](const EventFramebufferSize& e) {
				{
					std::lock_guard lock(self->frameState_m);
					self->framebufferSize = e.size;
				}

				onFramebufferSize.fire(e);

			}, [this](const libv::input::EventKey& e) {
				{
					std::lock_guard lock(self->frameState_m);
					if (e.action != libv::input::Action::release) {
						self->pressedKeys.insert(e.key);
						self->pressedScancodes.insert(e.scancode);
					} else {
						self->pressedKeys.erase(e.key);
						self->pressedScancodes.erase(e.scancode);
					}
				}

				onKey.fire(e);

			}, [this](const EventMaximize& e) {
				{
					std::lock_guard lock(self->frameState_m);
					self->maximized = e.maximized;
					self->minimized = false;
				}

				onMaximize.fire(e);

			}, [this](const EventMinimize& e) {
				{
					std::lock_guard lock(self->frameState_m);
					self->maximized = false;
					self->minimized = e.minimized;
				}

				onMinimize.fire(e);

			}, [this](const libv::input::EventMouseButton& e) {
				{
					std::lock_guard lock(self->frameState_m);
					if (e.action != libv::input::Action::release)
						self->pressedMouseButtons.insert(e.button);
					else
						self->pressedMouseButtons.erase(e.button);
				}

				onMouseButton.fire(e);

			}, [this](const libv::input::EventMouseEnter& e) {
				onMouseEnter.fire(e);

			}, [this](const libv::input::EventMousePosition& e) {
				{
					std::lock_guard lock(self->frameState_m);
					self->mousePosition = e.position;
				}

				onMousePosition.fire(e);

			}, [this](const libv::input::EventMouseScroll& e) {
				{
					std::lock_guard lock(self->frameState_m);
					self->scrollPosition += e.offset;
				}

				onMouseScroll.fire(e);

			}, [this](const EventPosition& e) {
				{
					std::lock_guard lock(self->frameState_m);
					self->position = e.position;
				}

				onPosition.fire(e);

			}, [this](const EventRefresh& e) {
				onRefresh.fire(e);

			}, [this](const EventSize& e) {
				{
					std::lock_guard lock(self->frameState_m);
					self->size = e.size;
				}

				onSize.fire(e);
			}
		);

		std::visit(visitor, event);
	}
	self->eventQueue.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
