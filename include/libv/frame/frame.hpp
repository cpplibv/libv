// File: Frame.hpp, Created on 2013. szeptember 27. 16:03, Author: Vader

#pragma once

// libv
#include <libv/input/event.hpp>
#include <libv/input/input.hpp>
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
//
// TODO P4: Support glfw hint GLFW_DOUBLEBUFFER
// TODO P4: Support glfw hint GLFW_JOYSTICK_HAT_BUTTONS
//
// TODO P5: Support glfw hint GLFW_SCALE_TO_MONITOR
// TODO P5: Support glfw hint GLFW_CONTEXT_ROBUSTNESS
// TODO P5: Support glfw hint GLFW_OPENGL_DEBUG_CONTEXT
// TODO P5: Support glfw hint GLFW_AUTO_ICONIFY (can be set via glfwSetWindowAttrib)
// TODO P5: Support glfw hint GLFW_CENTER_CURSOR
//
// TODO P5: Support glfw hint GLFW_TRANSPARENT_FRAMEBUFFER
// TODO P5: Support glfw hint GLFW_SRGB_CAPABLE
// TODO P5: Support glfw hint GLFW_STEREO
// TODO P5: Support glfw hint GLFW_CONTEXT_CREATION_API
// TODO P5: Support glfw hint GLFW_CONTEXT_RELEASE_BEHAVIOR
//
// TODO P5: Context sharing
// TODO P5: Understand EventRefresh, i think i have to work with it to 'force' refresh
//			if context is frozen due to event queue 'lock' in glfw (moving window)

class GLFWwindow;

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

class ImplFrame;
class Icon;

class Frame : public Trackable {
	// "frame" - refers to the entire window with its Frame objects, handlers, events, states etc...
	//	A frame can be exits without a window (exp: its not showed yet). A frame temporary can have
	//	multiple window at the same time (exp: switching to undecorated (for "sharing" gl context)).
	// "window" - refers to the glfw (operation system) window. A window is always part of a frame.

public: // -----------------------------------------------------------------------------------------
	enum class CursorMode {
		normal = 0x00034001, /// makes the cursor visible and behaving normally.
		hidden = 0x00034002, /// makes the cursor invisible when it is over the content area of the window but does not restrict the cursor from leaving.
		disabled = 0x00034003, /// hides and grabs the cursor, providing virtual and unlimited cursor movement. This is useful for implementing for example 3D camera controls.
	};

	enum class DisplayMode : int {
		/// monitor == nullptr, decorated = true
		/// - position change possible
		/// - size change possible
		/// - refresh rate and color bit depth change not possible
		windowed,
		/// monitor == nullptr, decorated = false, size and position = monitor work area
		/// - position change not possible
		/// - size change not possible
		/// - refresh rate and color bit depth change not possible
		borderless_maximized,
		/// monitor != nullptr,
		/// - position change not possible
		/// - size change possible
		/// - refresh rate and color bit depth change possible
		fullscreen,
		/// monitor != nullptr, video mode = current
		/// - position change not possible
		/// - size change not possible
		/// - refresh rate and color bit depth change not possible
		fullscreen_windowed,
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

	Event<libv::input::EventChar> onChar;
	Event<EventContentScale> onContentScale;
	Event<libv::input::EventDrop> onDrop;
	Event<EventFocus> onFocus;
	Event<EventFramebufferSize> onFramebufferSize;
	Event<libv::input::EventKey> onKey;
	Event<EventMaximize> onMaximize;
	Event<EventMinimize> onMinimize;
	Event<libv::input::EventMouseButton> onMouseButton;
	Event<libv::input::EventMouseEnter> onMouseEnter;
	Event<libv::input::EventMousePosition> onMousePosition;
	Event<libv::input::EventMouseScroll> onMouseScroll;
	Event<EventPosition> onPosition;
	Event<EventRefresh> onRefresh;
	Event<EventSize> onSize;

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

private:
	bool isFrameShouldClose();
	const Monitor& _getCurrentMonitor() const;

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
	void setCursorMode(CursorMode cursorMode);
	void setDecoration(bool decorated);
	void setDisplayMode(DisplayMode mode);
	void setPosition(int x, int y);
	void setPosition(libv::vec2i pos);
	void setPosition(FramePosition pos);

	void setAlwaysOnTop(bool alwaysOnTop);
	void setInitialFocus(bool initialFocus);
	void setFocusOnShow(bool focusOnShow);

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

	void setIcon(std::vector<Icon> icon);
	void clearIcon();

public:
	libv::vec2f getContentScale() const;
	CloseOperation getCloseOperation() const;
	DisplayMode getDisplayMode() const;
	libv::vec2i getFramebufferSize() const;
	libv::vec2i getPosition() const;
	libv::vec2i getSize() const;
	libv::vec4i getFrameSize() const; /// x - left, y - top, z - right, w - bottom
	std::string getTitle() const;
	// ... Additional getters on demand

	bool isDecorated() const;
	bool isResizable() const;
	bool isVisible() const;
	// ... Additional getters on demand

	const Monitor& getCurrentMonitor() const;

public:
	libv::input::KeyState key(libv::input::Key key);
	bool isKeyPressed(libv::input::Key key);
	bool isKeyReleased(libv::input::Key key);

	libv::input::KeyState mouse(libv::input::MouseButton key);
	bool isMousePressed(libv::input::MouseButton key);
	bool isMouseReleased(libv::input::MouseButton key);

	libv::vec2d getMousePosition();
	libv::vec2d getScrollPosition();

public:
	std::string getClipboardString();
	void setClipboardString(const std::string& string); // Argument is std::string as it is required to be 0 terminated
};

} // namespace frame -------------------------------------------------------------------------------

using Frame = frame::Frame;

} // namespace libv --------------------------------------------------------------------------------
