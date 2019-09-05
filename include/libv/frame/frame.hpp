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


// TODO P2: future proxy for frame async operations: frame.show().wait(); easy implementation:
//			add member Frame::wait()
//			member with async task return Frame&
//			Wait throws in a dummy task and waits for it.
// TODO P3: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
// TODO P3: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
// TODO P3: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
// TODO P4: Map glfw hint GLFW_FLOATING
// TODO P4: Map glfw hint GLFW_FOCUSED
// TODO P4: Support glfw hint GLFW_OPENGL_DEBUG_CONTEXT
// TODO P4: Support glfw hint GLFW_AUTO_ICONIFY
// TODO P4: Support glfw hint GLFW_CONTEXT_ROBUSTNESS
// TODO P4: Support glfw hint GLFW_CONTEXT_RELEASE_BEHAVIOR
// TODO P4: Support glfw hint GLFW_OPENGL_FORWARD_COMPAT
// TODO P5: Support glfw hint GLFW_STEREO
// TODO P5: Support glfw hint GLFW_SRGB_CAPABLE
// TODO P4: Disable deprecated openGL functionality by enable GLFW_OPENGL_FORWARD_COMPAT
// TODO P4: Learn about GLFW_CONTEXT_ROBUSTNESS
// TODO P4: Learn about GLFW_CONTEXT_RELEASE_BEHAVIOR
// TODO P5: Context sharing
// TODO P5: Most of the event should get a observer_ref<Frame> as member, or even better: Frame&!
// TODO P5: Remove? EventWindowRefresh and EventWindowClose as they are handled by frame
//			EventWindowRefresh is not that simple, i think i have to work with it to 'force' refresh
//			if context is frozen due to event queue 'lock' in glfw (moving window)

// No plans for mapping these glfw hints:
// GLFW_ACCUM_RED_BITS,
// GLFW_ACCUM_GREEN_BITS,
// GLFW_ACCUM_BLUE_BITS,
// GLFW_ACCUM_ALPHA_BITS,
// GLFW_AUX_BUFFERS,
// GLFW_CLIENT_API,
// GLFW_DOUBLEBUFFER

class GLFWwindow;

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
	Event<EventWindowMaximize> onWindowMaximize;
	Event<EventWindowMinimize> onWindowMinimize;
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

private: // --------------------------------------------------------------------------------------------
	void loopInit();
	void loop();
	void loopTerminate();
	void contextCreate();
	void contextDestroy();

private: // --------------------------------------------------------------------------------------------
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

private: // --------------------------------------------------------------------------------------------
	template <typename E>
	friend class DispatchGLFWEvent;

	static void registerEventCallbacks(Frame* frame, GLFWwindow* window);
	static void unregisterEventCallbacks(GLFWwindow* window);

	void distributeEvents();

public: // ---------------------------------------------------------------------------------------------
	void closeDefault();
	void closeForce();
	void join();

protected:
	bool isRefreshSkipable();
	bool isFrameShouldClose();

public:
	void show();
	void hide();
	void maximize();
	void minimize();
	void restore();
	/// The window should already be visible and not minimized.
	void focus();
	void requestAttention();

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

	/// Negative values interpreted as no aspect ratio constraint
	void setAspectRatio(int numer, int denom);
	/// Negative values interpreted as no aspect ratio constraint
	void setAspectRatio(libv::vec2i fraction);

	void setResizable(bool resizable);
	void setSize(int x, int y);
	void setSize(libv::vec2i size);

	/// Negative values interpreted as no limit
	void setSizeLimit(int minx, int miny, int maxx, int maxy);
	/// Negative values interpreted as no limit
	void setSizeLimit(int minx, int miny, libv::vec2i max);
	/// Negative values interpreted as no limit
	void setSizeLimit(libv::vec2i min, int maxx, int maxy);
	/// Negative values interpreted as no limit
	void setSizeLimit(libv::vec2i min, libv::vec2i max);
	/// Negative values interpreted as no limit
	void setSizeLimitMin(int minx, int miny);
	/// Negative values interpreted as no limit
	void setSizeLimitMin(libv::vec2i min);
	/// Negative values interpreted as no limit
	void setSizeLimitMax(int maxx, int maxy);
	/// Negative values interpreted as no limit
	void setSizeLimitMax(libv::vec2i max);


	void setTitle(std::string title);
//	void setWindowSizeLimits(int minx, int miny, int maxx, int maxy);
//	void setWindowSizeLimits(libv::vec2i min, libv::vec2i max);

public:
	CloseOperation getCloseOperation() const;
	DisplayMode getDisplayMode() const;
	libv::vec2i getPosition() const;
	libv::vec2i getSize() const;
	libv::vec4i getFrameSize() const; /// x - left, y - top, z - right, w - bottom
	const std::string& getTitle() const;
	// ... Additional getters on demand

	bool isDecorated() const;
	bool isResizable() const;
	bool isVisible() const;
	// ... Additional getters on demand

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

public:
	std::string getClipboardString();
	void setClipboardString(const std::string& string);
};

} // namespace frame -------------------------------------------------------------------------------

using Frame = frame::Frame;

} // namespace libv --------------------------------------------------------------------------------
