// File: Frame.hpp, Created on 2013. szeptember 27. 16:03, Author: Vader

#pragma once

// conf
#include <vl/ui/config.hpp>
// vl
#include <vl/memory.hpp>
#include <vl/sig/signal.hpp>
#include <vl/timer.hpp>
#include <vl/worker_thread.hpp>
// std
#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
// pro
#include <vl/ui/component/component.hpp>
#include <vl/ui/component/panel.hpp>
#include <vl/ui/events/events.hpp>
#include <vl/ui/monitor.hpp>
#include <vl/ui/render/renderer.hpp>

class GLFWwindow;
class GLFWmonitor;

namespace vl {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Frame : public Trackable, private ProtectedContainer {
	//Every time a "frame" word is used it is referring to the entire window with
	//	its VUI objects, handlers, events, states etc... A frame can be exits
	//	without a window (exp: its not showed yet). A frame temporary can have
	//	multiple window at the same time (exp: switching to undecorated (for
	//	"sharing" gl context)).
	//Every time a "window" word is used it is referring to the glfw (/ operation
	//	system) window. A window is always part of a frame.

public:
	using TypeCloseOperation = int;
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

	// Close ---------------------------------------------------------------------------------------

public:
	//TODO P5: There are more DCO:
	// Operation on current frame:
	//		DISPOSE
	//		DO_NOTHING
	//		HIDE
	//		MINIMALIZE
	// Operation on every other frame:
	//		DEFAULT
	//		DISPOSE
	//		DO_NOTHING
	//		HIDE
	//		MINIMALIZE

	/** If the frame receive a close signal (from OS) or from setting
	 * setWindowShouldClose(true) then after the current iteration close and
	 * make every other frame to execute their default close operation
	 * after their current iteration
	 * @note Does not interrupt current iteration
	 * @note Does not interrupt other frames current iterations
	 * @note May or may not produce onClose event on the other frames
	 * (depending on their default close operation)
	 * @group Default Close Operation */
	static const TypeCloseOperation ON_CLOSE_DEFAULT_EXIT = 0; // - Close, others DCO.
	/** If the frame receive a close signal (from OS) or from setting
	 * setWindowShouldClose(true) then after the current iteration close and
	 * force every frame to close after their current iteration even if the
	 * frames default close operation is not closing the frame
	 * @note Does not interrupt current iteration
	 * @note Does not interrupt other frames current iterations
	 * @group Default Close Operation */
	static const TypeCloseOperation ON_CLOSE_EXIT = 1; //         - Close every frame
	/** If the frame receive a close signal (from OS) or from setting
	 * setWindowShouldClose(true) then after the current iteration hide the
	 * frame. The frame can be restored by calling show()
	 * @note Does not interrupt current iteration
	 * @note Can be forced to close by calling close(), closing that way will
	 * produce onClose event, and also does not interrupt current iteration
	 * @group Default Close Operation */
	static const TypeCloseOperation ON_CLOSE_HIDE = 2; //         - Hide frame
	/** If the frame receive a close signal (from OS) or from setting
	 * setWindowShouldClose(true) then after the current iteration does nothing.
	 * @note Does not interrupt current iteration
	 * @note Can be forced to close by calling close(), closing that way will
	 * produce onClose event, and also does not interrupt current iteration
	 * @group Default Close Operation */
	static const TypeCloseOperation ON_CLOSE_DO_NOTHING = 3; //   - No operation
	/** If the frame receive a close signal (from OS) or from setting
	 * setWindowShouldClose(true) then after the current iteration close the
	 * frame
	 * @note Does not interrupt current iteration
	 * @group Default Close Operation */
	static const TypeCloseOperation ON_CLOSE_DISPOSE = 4; //      - Close the frame

public:
	void closeDefault();
	void closeForce();
	void join();

protected:
	bool isFrameShouldClose();

	// ---------------------------------------------------------------------------------------------

private:
	//	void cmdFrameCreate();
	/**
	 * @note If this function fail the member window will be set to nullptr
	 * @note This function may only be called by the Frame context.
	 */
	void cmdFrameRecreate();
	//	void cmdFrameDestroy();

	/**
	 * @note If this function fail the member window will be set to nullptr
	 * @note This function may only be called from the Core thread.
	 */
	void cmdCoreCreate();
	void cmdCoreRecreate();
	void cmdCoreDestroy();

	void cmdCoreUpdateDisplayMode();

protected:
	bool isRenderSkipable();
	bool isUpdateSkipable();

	// Event Callbacks and Listeners ---------------------------------------------------------------
private:
	static void registerEventCallbacks(Frame* frame, GLFWwindow* window);
	static void unregisterEventCallbacks(GLFWwindow* window);

private:
	void glfwCallback(const EventChar&);
	void glfwCallback(const EventCharMods&);
	void glfwCallback(const EventCursorEnter&);
	void glfwCallback(const EventCursorPos&);
	void glfwCallback(const EventDrop&);
	void glfwCallback(const EventFramebufferSize&);
	void glfwCallback(const EventKey&);
	void glfwCallback(const EventMouseButton&);
	void glfwCallback(const EventScroll&);
	void glfwCallback(const EventWindowClose&);
	void glfwCallback(const EventWindowFocus&);
	void glfwCallback(const EventWindowIconify&);
	void glfwCallback(const EventWindowPos&);
	void glfwCallback(const EventWindowRefresh&);
	void glfwCallback(const EventWindowSize&);
	void glfwCallback(const EventMonitor&);

private:
	void initEvents();
	void distributeEvents();

private:
	CapacitivSignal<const EventChar&> eventQueChar;
	CapacitivSignal<const EventCharMods&> eventQueCharMods;
	CapacitivSignal<const EventCursorEnter&> eventQueCursorEnter;
	CapacitivSignal<const EventCursorPos&> eventQueCursorPos;
	CapacitivSignal<const EventDrop&> eventQueDrop;
	CapacitivSignal<const EventFramebufferSize&> eventQueFramebufferSize;
	CapacitivSignal<const EventKey&> eventQueKey;
	CapacitivSignal<const EventMouseButton&> eventQueMouseButton;
	CapacitivSignal<const EventScroll&> eventQueScroll;
	CapacitivSignal<const EventWindowClose&> eventQueWindowClose;
	CapacitivSignal<const EventWindowFocus&> eventQueWindowFocus;
	CapacitivSignal<const EventWindowIconify&> eventQueWindowIconify;
	CapacitivSignal<const EventWindowPos&> eventQueWindowPos;
	CapacitivSignal<const EventWindowRefresh&> eventQueWindowRefresh;
	CapacitivSignal<const EventWindowSize&> eventQueWindowSize;

public:
	Signal<const EventChar&> onChar;
	Signal<const EventCharMods&> onCharMods;
	Signal<const EventCursorEnter&> onCursorEnter;
	Signal<const EventCursorPos&> onCursorPos;
	Signal<const EventDrop&> onDrop;
	Signal<const EventFramebufferSize&> onFramebufferSize;
	Signal<const EventKey&> onKey;
	Signal<const EventMouseButton&> onMouseButton;
	Signal<const EventScroll&> onScroll;
	Signal<const EventWindowClose&> onWindowClose;
	Signal<const EventWindowFocus&> onWindowFocus;
	Signal<const EventWindowIconify&> onWindowIconify;
	Signal<const EventWindowPos&> onWindowPos;
	Signal<const EventWindowRefresh&> onWindowRefresh;
	Signal<const EventWindowSize&> onWindowSize;

	/** Event invoked on frame closing by frame's thread. Usable for interrupting close.
	 * This event occures even if the default close operation does not close the frame.
	 * If the returned value is False the closing operation will be interrupted.
	 * @return False: interrupt close; True: proceed */
	Signal<bool(Frame*) > onClose;
	/** Event invoked if the frame is closed by frame's thread.
	 * Can't be used for interrupting close.
	 * Event occures when the window is ALREADY CLOSED,
	 * but the Frame object is not deconstructed yet. */
	Signal<Frame*> onClosed;

	// ---------------------------------------------------------------------------------------------
private:
	std::atomic<bool> forcedClose{false};

private:
	vl::WorkerThread context;

	GLFWwindow* window = nullptr;
	GLFWwindow* shareWindow = nullptr;

private:
	unsigned int swapInterval = 1;
	ivec2 pos;

private:
	TypeOpenGLProfile openGLProfile = OPENGL_PROFILE_COMPAT;
	TypeOpenGLRefreshRate openGLRefreshRate = REFRESH_RATE_DONT_CARE;
	TypeOpenGLSamples openGLSamples = SAMPLES_DONT_CARE;
	int openGLVersionMajor = 3;
	int openGLVersionMinor = 3;

	bool decorated = true;
	bool hidden = true;
	bool minimalized = false;
	bool resizable = true;

	TypeCloseOperation defaultCloseOperation = ON_CLOSE_DISPOSE;
	TypeDisplayMode displayMode = DISPLAY_MODE_WINDOWED;

	std::string title;

private:
	Renderer renderer;
	//!!! I could make a layout manager only for Frame if i need to

private:
	Timer timerBuild;
	Timer timerDestroy;
	Timer timerEvent;
	Timer timerLoop;
	Timer timerOffLoop;
	Timer timerPoll;
	Timer timerRender;
	Timer timerSwap;
	Timer timerUpdate;

	// ---------------------------------------------------------------------------------------------
private:
	void hideImpl();
	void minimalizeImpl();
	void restoreImpl();
	void showImpl();

public:
	void hide();
	void hideAsync();
	void minimalize();
	void minimalizeAsync();
	void restore();
	void restoreAsync();
	void show();
	void showAsync();

private:
	void baseBuild();
	void baseDestroy();
	void baseInvalidate();
	void baseRender();
	void baseUpdate();

protected:
	using ProtectedContainer::build;
	using ProtectedContainer::destroy;
	using ProtectedContainer::invalidate;
	using ProtectedContainer::render;
	using ProtectedContainer::update;

	//public:
	//	virtual void invalidate();

public:
	using ProtectedContainer::add;
	using ProtectedContainer::remove;
	using ProtectedContainer::setLayout;

public:
	void setOpenGLProfile(TypeOpenGLProfile profile);
	void setOpenGLRefreshRate(TypeOpenGLRefreshRate rate);
	void setOpenGLSamples(TypeOpenGLSamples samples);
	void setOpenGLVersion(int major, int minor);

	void setCloseOperation(const TypeCloseOperation& operation);
	void setDecoration(bool decorated);
	void setDisplayMode(const TypeDisplayMode& mode);
	void setPosition(int x, int y);
	void setPosition(const ivec2& y);
	void setResizeable(bool resizeable);
	void setSize(int x, int y);
	void setTitle(const std::string& title);

	TypeCloseOperation getCloseOperation() const;
	TypeDisplayMode getDisplayMode() const;
	ivec3 getSize() const;
	std::string getTitle() const;
	//	unsigned int getWidth() const;
	// * * *

	bool isDecorated()const;
	bool isVisible() const;
	// * * *

	const Monitor* getCurrentMonitor() const;
private:
	void init();
	void loop();
	void term();
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

} //namespace ui
} //namespace vl