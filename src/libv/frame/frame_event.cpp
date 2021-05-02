// Project: libv.frame, File: src/libv/frame/frame_event.cpp, Author: Császár Mátyás [Vader]

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

template <typename E>
struct DispatchGLFWEvent {
	static inline void eventDrop(GLFWwindow* window, int count, const char** data) {
		std::vector<std::string> payload;
		payload.reserve(count);
		for (int i = 0; i < count; ++i)
			payload.emplace_back(data[i]);

		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(window));
		if (frame == nullptr)
			return log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");

		std::lock_guard lock_queue(frame->self->eventQueue_m);
		frame->self->eventQueue.emplace_back(E{std::move(payload)});
	}

	static inline void eventKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
		(void) mods; // TODO P1: Store mods and create events from caps/num/scroll lock changes | Also check if any other has mods too

		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(window));
		if (frame == nullptr)
			return log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");

		std::lock_guard lock_queue(frame->self->eventQueue_m);
		frame->self->eventQueue.emplace_back(E{libv::input::Keycode{key}, libv::input::Scancode{scancode}, libv::input::Action{action}});
	}

	static inline void eventMouseButton(GLFWwindow* window, int button, int action, int mods) {
		(void) mods; // TODO P1: Store mods and create events from caps/num/scroll lock changes | Also check if any other has mods too

		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(window));
		if (frame == nullptr)
			return log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");

		std::lock_guard lock_queue(frame->self->eventQueue_m);
		frame->self->eventQueue.emplace_back(E{libv::input::MouseButton{button}, libv::input::Action{action}});
	}

	template <typename... Args>
	static inline void call(GLFWwindow* window, Args... args) {
		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(window));
		if (frame == nullptr)
			return log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");

		std::lock_guard lock_queue(frame->self->eventQueue_m);
		frame->self->eventQueue.emplace_back(E(args...));
	}

	template <typename... Args>
	static inline void frame(GLFWwindow* window, Args... args) {
		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(window));
		if (frame == nullptr)
			return log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");

		std::lock_guard lock_queue(frame->self->eventQueue_m);
		frame->self->eventQueue.emplace_back(E(args...));

		glfwGetWindowFrameSize(window,
				&frame->self->frameSize.x,
				&frame->self->frameSize.y,
				&frame->self->frameSize.z,
				&frame->self->frameSize.w);
	}

	static inline void mouse(GLFWwindow* window, double x, double y) {
		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(window));
		if (frame == nullptr)
			return log_event.error("Unhandled event. No event handler (frame) assigned to this GLFW window.");

		std::vector<Event>& queue = frame->self->eventQueue;
		auto size = frame->getSize();

		std::lock_guard lock_queue(frame->self->eventQueue_m);
		if (!queue.empty() && std::holds_alternative<E>(queue.back()))
			std::get<E>(queue.back()).position = libv::vec2d{x, size.y - y - 1};
		else
			queue.emplace_back(E(x, size.y - y - 1));
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
	glfwSetWindowUserPointer(window, frame);

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
	// TODO P2: libv.frame: Joystick/Gamepad support: Watch or pool events
	// TODO P2: libv.frame: Joystick/Gamepad support: Connect-Disconnect events
	// TODO P2: libv.frame: Joystick/Gamepad support: Upon disconnect release every button
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

	glfwSetWindowUserPointer(window, nullptr);
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
						self->pressedKeys.insert(e.keycode);
						self->pressedScancodes.insert(e.scancode);
					} else {
						self->pressedKeys.erase(e.keycode);
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

			}, [this](const libv::input::EventGamepadAnalog& e) {
				// TODO P2: libv.frame: Joystick/Gamepad support: Store states
				onGamepadAnalog.fire(e);

			}, [this](const libv::input::EventGamepadButton& e) {
				// TODO P2: libv.frame: Joystick/Gamepad support: Store states
				onGamepadButton.fire(e);

			}, [this](const libv::input::EventJoystickAnalog& e) {
				// TODO P2: libv.frame: Joystick/Gamepad support: Store states
				onJoystickAnalog.fire(e);

			}, [this](const libv::input::EventJoystickButton& e) {
				// TODO P2: libv.frame: Joystick/Gamepad support: Store states
				onJoystickButton.fire(e);

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
