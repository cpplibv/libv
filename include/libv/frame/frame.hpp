// File: Frame.hpp, Created on 2013. szeptember 27. 16:03, Author: Vader

#pragma once

// libv
#include <libv/input/inputs.hpp>
#include <libv/math/vec.hpp>
#include <libv/sig/signal.hpp>
// std
#include <memory>
#include <string>
// pro
#include <libv/frame/events.hpp>


// TODO P3: future proxy for frame async operations: frame.show().wait(); easy implementation:
//			add member Frame::wait()
//			member with async task return Frame&
//			Wait throws in a dummy task and waits for it.

class GLFWwindow;
class GLFWmonitor;

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

class ImplFrame;

class Frame : public Trackable {
	// "frame" - refers to the entire window with its Frame objects, handlers, events, states etc...
	//	A frame can be exits without a window (exp: its not showed yet). A frame temporary can have
	//	multiple window at the same time (exp: switching to undecorated (for "sharing" gl context)).
	// "window" - refers to the glfw (operation system) window. A window is always part of a frame.

public: // -----------------------------------------------------------------------------------------
	enum class DisplayMode : int {
		windowed = 0,
		borderless = 1,
		fullscreen = 2,
	};

	enum class OpenGLProfile : int {
		any = 0,
		core = 0x00032001,
		compat = 0x00032002,
	};

	enum class OpenGLRefreshRate : int {
		dont_care = -1,
	};

	enum class OpenGLSamples : int {
		dont_care = -1,
	};

	enum class FramePosition {
		center_current_monitor,
		center_primary_monitor,
	};

	enum class CloseOperation {
		/// Close frame
		/// If the frame receive a close signal (from OS) or from setting
		/// setWindowShouldClose(true) then after the current iteration close the frame
		/// @note Does not interrupt current iteration
		/// @group Default Close Operation
		dispose,
		/// No operation
		/// If the frame receive a close signal (from OS) or from setting
		/// setWindowShouldClose(true) then after the current iteration does nothing.
		/// @note Does not interrupt current iteration
		/// @note Can be forced to close by calling close(), closing that way will
		/// produce onClose event, and also does not interrupt current iteration
		/// @group Default Close Operation
		do_nothing,
		/// Hide frame
		/// If the frame receive a close signal (from OS) or from setting
		/// setWindowShouldClose(true) then after the current iteration hide the
		/// frame. The frame can be restored by calling show()
		/// @note Does not interrupt current iteration
		/// @note Can be forced to close by calling close(), closing that way will
		/// produce onClose event, and also does not interrupt current iteration
		/// @group Default Close Operation
		hide,
		/// Minimize frame
		/// If the frame receive a close signal (from OS) or from setting
		/// setWindowShouldClose(true) then after the current iteration minimize the
		/// frame. The frame can be restored by calling restore()
		/// @note Does not interrupt current iteration
		/// @note Can be forced to close by calling close(), closing that way will
		/// produce onClose event, and also does not interrupt current iteration
		/// @group Default Close Operation
		minimize,
	};

private:
	std::unique_ptr<ImplFrame> self;

public:
	template <typename T>
	using Event = Signal<const T&>;

	Event<EventChar> onChar;
	Event<EventDrop> onDrop;
	Event<EventFramebufferSize> onFramebufferSize;
	Event<EventKey> onKey;
	Event<EventMouseButton> onMouseButton;
	Event<EventMouseEnter> onMouseEnter;
	Event<EventMousePosition> onMousePosition;
	Event<EventMouseScroll> onMouseScroll;
	Event<EventWindowFocus> onWindowFocus;
	Event<EventWindowIconify> onWindowIconify;
	Event<EventWindowPosition> onWindowPosition;
	Event<EventWindowRefresh> onWindowRefresh;
	Event<EventWindowSize> onWindowSize;

	/// Event invoked on frame closing by frame's thread. Usable for interrupting close.
	/// This event occurs even if the default close operation does not close the frame.
	Event<EventCloseRequest> onCloseRequest;

	Event<EventContextCreate> onContextCreate;
	Event<EventContextUpdate> onContextUpdate;
	Event<EventContextDestroy> onContextDestroy;

	// ---------------------------------------------------------------------------------------------

public:
	Frame(const Frame&) = delete;
	Frame(Frame&&) = delete;
	Frame& operator=(const Frame&) = delete;
	Frame& operator=(Frame&&) = delete;

	Frame(std::string title, int32_t width, int32_t height);
	Frame(std::string title, libv::vec2i size);
	Frame(std::string title);
	Frame(int32_t width, int32_t height);
	Frame(libv::vec2i size);
	virtual ~Frame();

	// -------------------------------------------------------------------------------------------------
private:
	void cmdFrameCreate();
	/**
	 * @note If this function fail the member window will be set to nullptr
	 * @note This function may only be called by the Frame context.
	 */
	void cmdFrameRecreate();
	void cmdFrameDestroy();

	/**
	 * @note If this function fail the member window will be set to nullptr
	 * @note This function may only be called from the Core thread.
	 */
	void cmdCoreCreate();
	void cmdCoreRecreate();
	void cmdCoreDestroy();

	void cmdCoreUpdateDisplayMode();

	// Event Callbacks and Listeners ---------------------------------------------------------------
private:
	template <typename E, auto queue>
	friend class DispatchGLFWEvent;

	void realizeGLFWCallback(const EventChar&);
	void realizeGLFWCallback(const EventDrop&);
	void realizeGLFWCallback(const EventFramebufferSize&);
	void realizeGLFWCallback(const EventKey&);
	void realizeGLFWCallback(const EventMouseButton&);
	void realizeGLFWCallback(const EventMouseEnter&);
	void realizeGLFWCallback(const EventMousePosition&);
	void realizeGLFWCallback(const EventMouseScroll&);
	void realizeGLFWCallback(const EventWindowFocus&);
	void realizeGLFWCallback(const EventWindowIconify&);
	void realizeGLFWCallback(const EventWindowPosition&);
	void realizeGLFWCallback(const EventWindowRefresh&);
	void realizeGLFWCallback(const EventWindowSize&);

private:
	static void registerEventCallbacks(Frame* frame, GLFWwindow* window);
	static void unregisterEventCallbacks(GLFWwindow* window);

private:
	void initEventQueues();
	void distributeEvents();

	// ---------------------------------------------------------------------------------------------
public:
	void closeDefault();
	void closeForce();
	void join();

protected:
	bool isRefreshSkipable();
	bool isFrameShouldClose();

public:
	void hide();
	void minimize();
	void restore();
	void show();

public:
	void setOpenGLProfile(OpenGLProfile profile);
	void setOpenGLRefreshRate(OpenGLRefreshRate rate);
	void setOpenGLSamples(OpenGLSamples samples);
	void setOpenGLVersion(int major, int minor);

	void setCloseOperation(CloseOperation operation);
	void setDecoration(bool decorated);
	void setDisplayMode(DisplayMode mode);
	void setPosition(int x, int y);
	void setPosition(libv::vec2i pos);
	void setPosition(FramePosition pos);
	void setResizable(bool resizable);
	void setSize(int x, int y);
	void setSize(libv::vec2i size);
	void setTitle(std::string title);
//	void setWindowSizeLimits(int minx, int miny, int maxx, int maxy);
//	void setWindowSizeLimits(libv::vec2i min, libv::vec2i max);

public:
	CloseOperation getCloseOperation() const;
	DisplayMode getDisplayMode() const;
	libv::vec2i getSize() const;
	const std::string& getTitle() const;
	// * * *

	bool isDecorated() const;
	bool isVisible() const;
	// * * *

	const Monitor& getCurrentMonitor() const;

public:
	libv::input::KeyState getKey(libv::input::Key key);
	bool isKeyPressed(libv::input::Key key);
	bool isKeyReleased(libv::input::Key key);

	libv::input::KeyState getMouse(libv::input::Mouse key);
	bool isMousePressed(libv::input::Mouse key);
	bool isMouseReleased(libv::input::Mouse key);

	libv::vec2d getMousePosition();
	libv::vec2d getScrollPosition();

	// TODO P5: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// TODO P5: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// TODO P5: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

private:
	void loopInit();
	void loop();
	void loopTerminate();
	void contextCreate();
	void contextDestroy();
};

} // namespace frame

using Frame = frame::Frame;

} // namespace libv
