// File: Frame.hpp, Created on 2013. szeptember 27. 16:03, Author: Vader

#pragma once

// cfg
#include <libv/ui/config.hpp>
// libv
#include <libv/memory.hpp>
#include <libv/sig/signal.hpp>
#include <libv/utility.hpp>
#include <libv/worker_thread.hpp>
// std
#include <atomic>
#include <array>
#include <string>
// pro
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/ui.hpp>
#include <libv/ui/events/events.hpp>
#include <libv/ui/events/inputs.hpp>
#include <libv/ui/monitor.hpp>

// TODO P3: future proxy for frame async operations: frame.show().wait();

class GLFWwindow;
class GLFWmonitor;

namespace libv {
namespace ui {

class Component;

// -------------------------------------------------------------------------------------------------

class Frame : public Trackable {
	//Every time a "frame" word is used it is referring to the entire window with
	//	its VUI objects, handlers, events, states etc... A frame can be exits
	//	without a window (exp: its not showed yet). A frame temporary can have
	//	multiple window at the same time (exp: switching to undecorated (for
	//	"sharing" gl context)).
	//Every time a "window" word is used it is referring to the glfw (/ operation
	//	system) window. A window is always part of a frame.

public:
	using TypeDisplayMode = int;
	using TypeOpenGLProfile = int;
	using TypeOpenGLRefreshRate = int;
	using TypeOpenGLSamples = int;

	// ---------------------------------------------------------------------------------------------
public:
	static void joinAll();
	static void closeAllDefault();
	static void closeAllForce();

	// ---------------------------------------------------------------------------------------------

	static const TypeDisplayMode DISPLAY_MODE_WINDOWED;
	static const TypeDisplayMode DISPLAY_MODE_BORDERLESS;
	static const TypeDisplayMode DISPLAY_MODE_FULLSCREEN;

	static const TypeOpenGLProfile OPENGL_PROFILE_ANY;
	static const TypeOpenGLProfile OPENGL_PROFILE_COMPAT;
	static const TypeOpenGLProfile OPENGL_PROFILE_CORE;

	static const TypeOpenGLRefreshRate REFRESH_RATE_DONT_CARE;

	static const TypeOpenGLSamples SAMPLES_DONT_CARE;

	enum FramePosition {
		POSITION_CENTER_CURRENT_MONITOR,
		POSITION_CENTER_PRIMARY_MONITOR
	};

	// Close ---------------------------------------------------------------------------------------

public:
	enum TypeCloseOperation {
		/** If the frame receive a close signal (from OS) or from setting
		 * setWindowShouldClose(true) then after the current iteration close the
		 * frame
		 * @note Does not interrupt current iteration
		 * @group Default Close Operation */
		ON_CLOSE_DISPOSE, //      - Close the frame
		/** If the frame receive a close signal (from OS) or from setting
		 * setWindowShouldClose(true) then after the current iteration does nothing.
		 * @note Does not interrupt current iteration
		 * @note Can be forced to close by calling close(), closing that way will
		 * produce onClose event, and also does not interrupt current iteration
		 * @group Default Close Operation */
		ON_CLOSE_DO_NOTHING, //   - No operation
		/** If the frame receive a close signal (from OS) or from setting
		 * setWindowShouldClose(true) then after the current iteration hide the
		 * frame. The frame can be restored by calling show()
		 * @note Does not interrupt current iteration
		 * @note Can be forced to close by calling close(), closing that way will
		 * produce onClose event, and also does not interrupt current iteration
		 * @group Default Close Operation */
		ON_CLOSE_HIDE, //         - Hide frame
		/** If the frame receive a close signal (from OS) or from setting
		 * setWindowShouldClose(true) then after the current iteration minimize the
		 * frame. The frame can be restored by calling restore()
		 * @note Does not interrupt current iteration
		 * @note Can be forced to close by calling close(), closing that way will
		 * produce onClose event, and also does not interrupt current iteration
		 * @group Default Close Operation */
		ON_CLOSE_MINIMIZE //      - Minimize frame
	};

public:
	void closeDefault();
	void closeForce();
	void join();

protected:
	bool isFrameShouldClose();

	// ---------------------------------------------------------------------------------------------

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

protected:
	bool isRefreshSkipable();

	// Event Callbacks and Listeners ---------------------------------------------------------------
private:
	static void registerEventCallbacks(Frame* frame, GLFWwindow* window);
	static void unregisterEventCallbacks(GLFWwindow* window);

private:
	void glfwCallback(const EventChar&);
	void glfwCallback(const EventCharMods&);
	void glfwCallback(const EventDrop&);
	void glfwCallback(const EventFramebufferSize&);
	void glfwCallback(const EventKey&);
	void glfwCallback(const EventMouseButton&);
	void glfwCallback(const EventMouseEnter&);
	void glfwCallback(const EventMousePosition&);
	void glfwCallback(const EventMouseScroll&);
	void glfwCallback(const EventWindowClose&);
	void glfwCallback(const EventWindowFocus&);
	void glfwCallback(const EventWindowIconify&);
	void glfwCallback(const EventWindowPosition&);
	void glfwCallback(const EventWindowRefresh&);
	void glfwCallback(const EventWindowSize&);

private:
	void initEvents();
	void distributeEvents();

private:
	template <typename T>
	using EventQue = CapacitivSignal<const T&>;

	EventQue<EventChar> eventQueChar;
	EventQue<EventCharMods> eventQueCharMods;
	EventQue<EventDrop> eventQueDrop;
	EventQue<EventFramebufferSize> eventQueFramebufferSize;
	EventQue<EventKey> eventQueKey;
	EventQue<EventMouseButton> eventQueMouseButton;
	EventQue<EventMouseEnter> eventQueMouseEnter;
	EventQue<EventMousePosition> eventQueMousePosition;
	EventQue<EventMouseScroll> eventQueMouseScroll;
	EventQue<EventWindowClose> eventQueWindowClose;
	EventQue<EventWindowFocus> eventQueWindowFocus;
	EventQue<EventWindowIconify> eventQueWindowIconify;
	EventQue<EventWindowPosition> eventQueWindowPosition;
	EventQue<EventWindowRefresh> eventQueWindowRefresh;
	EventQue<EventWindowSize> eventQueWindowSize;

public:
	template <typename T>
	using Event = Signal<const T&>;

	Event<EventChar> onChar;
	Event<EventCharMods> onCharMods;
	Event<EventDrop> onDrop;
	Event<EventFramebufferSize> onFramebufferSize;
	Event<EventKey> onKey;
	Event<EventMouseButton> onMouseButton;
	Event<EventMouseEnter> onMouseEnter;
	Event<EventMousePosition> onMousePosition;
	Event<EventMouseScroll> onMouseScroll;
	Event<EventWindowClose> onWindowClose;
	Event<EventWindowFocus> onWindowFocus;
	Event<EventWindowIconify> onWindowIconify;
	Event<EventWindowPosition> onWindowPosition;
	Event<EventWindowRefresh> onWindowRefresh;
	Event<EventWindowSize> onWindowSize;

	/** Event invoked on frame closing by frame's thread. Usable for interrupting close.
	 * This event occures even if the default close operation does not close the frame.
	 * If the returned value is False the closing operation will be interrupted.
	 * @return False: interrupt close; True: proceed */
	Signal<bool(Frame*)> onClose;
	/** Event invoked if the frame is closed by frame's thread.
	 * Can't be used for interrupting close.
	 * Event occurs when the window is ALREADY CLOSED,
	 * but the Frame object is not destructed yet. */
	Signal<Frame*> onClosed;

	// ---------------------------------------------------------------------------------------------
private:
	std::atomic<bool> forcedClose{false};

private:
	libv::WorkerThread context;

	GLFWwindow* window = nullptr;
	GLFWwindow* shareWindow = nullptr;

private:
	unsigned int swapInterval = 1;
	vec2i position;
	vec2i size;

private:
	TypeOpenGLProfile openGLProfile = OPENGL_PROFILE_COMPAT;
	TypeOpenGLRefreshRate openGLRefreshRate = REFRESH_RATE_DONT_CARE;
	TypeOpenGLSamples openGLSamples = SAMPLES_DONT_CARE;
	int openGLVersionMajor = 3;
	int openGLVersionMinor = 3;

	bool decorated = true;
	bool hidden = true;
	bool minimized = false;
	bool resizable = true;

	TypeCloseOperation defaultCloseOperation = ON_CLOSE_DISPOSE;
	TypeDisplayMode displayMode = DISPLAY_MODE_WINDOWED;

	std::string title;

	std::array<std::atomic<KeyState>, to_value(Key::Last)> keyStates;
	std::array<std::atomic<KeyState>, to_value(Mouse::Last)> mouseStates;
	std::atomic<uint64_t> mousePosition{0}; // coded as x:24.8 y:24.8
	std::atomic<uint64_t> scrollPosition{0}; // coded as x:24.8 y:24.8

	// ---------------------------------------------------------------------------------------------
private:
	UI ui;

public:
	void addComponent(const observer_ptr<Component>& component);
	void addComponent(const shared_ptr<Component>& component);
	void removeComponent(const observer_ptr<Component>& component);
	void removeComponent(const shared_ptr<Component>& component);

	// ---------------------------------------------------------------------------------------------
public:
	void hide();
	void minimize();
	void restore();
	void show();

public:
	void setOpenGLProfile(TypeOpenGLProfile profile);
	void setOpenGLRefreshRate(TypeOpenGLRefreshRate rate);
	void setOpenGLSamples(TypeOpenGLSamples samples);
	void setOpenGLVersion(int major, int minor);

	void setCloseOperation(const TypeCloseOperation& operation);
	void setDecoration(bool decorated);
	void setDisplayMode(const TypeDisplayMode& mode);
	void setPosition(int x, int y);
	void setPosition(vec2i pos);
	void setPosition(FramePosition pos);
	void setResizable(bool resizable);
	void setSize(int x, int y);
	void setSize(vec2i size);
	void setTitle(const std::string& title);

	TypeCloseOperation getCloseOperation() const;
	TypeDisplayMode getDisplayMode() const;
	vec2i getSize() const;
	std::string getTitle() const;
	// * * *

	bool isDecorated()const;
	bool isVisible() const;
	// * * *

	const Monitor& getCurrentMonitor() const;

public:
	KeyState getKey(Key key);
	bool isKeyPressed(Key key);
	bool isKeyReleased(Key key);

	KeyState getMouse(Mouse key);
	bool isMousePressed(Mouse key);
	bool isMouseReleased(Mouse key);

	vec2f getMousePosition();
	vec2f getScrollPosition();

	// TODO P5: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// TODO P5: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// TODO P5: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

private:
	void loopInit();
	void loop();
	void loopTerminate();

private:
	void initContext();
	void termContext();

public:
	Frame(unsigned int width = DEFAULT_FRAME_WIDTH,
			unsigned int height = DEFAULT_FRAME_HEIGHT);
	Frame(const std::string& title,
			unsigned int width = DEFAULT_FRAME_WIDTH,
			unsigned int height = DEFAULT_FRAME_HEIGHT);
	Frame(const Frame&) = delete;
	Frame(Frame&&) = delete;
	Frame& operator=(const Frame&) = delete;
	Frame& operator=(Frame&&) = delete;
	virtual ~Frame();
};

} // namespace ui
} // namespace libv