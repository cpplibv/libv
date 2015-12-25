// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include "core.hpp"
#include <libv/ui/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/semaphore.hpp>
// std
#include <atomic>
#include <memory>
// pro
#include <libv/ui/log.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::mutex frames_m;
std::set<Frame*> frames;
std::mutex activeFrames_m;
std::set<Frame*> activeFrames;
libv::Semaphore noActiveFrame(true);

// -------------------------------------------------------------------------------------------------
class Core {
	// Priorities:
	// Init            0
	// Task           50
	// WaitEvent     150
	// Terminate     200

private:
	libv::WorkerThread thread;
	std::atomic_bool stopWait{false};
	std::mutex mutex;
private:

	void init() {
		LIBV_UI_CORE_INFO("Initialize Core / GLFW Context");
		glfwSetErrorCallback(detail::errorCallbackGLFW);
		if (!glfwInit())
			return LIBV_UI_CORE_ERROR("Failed to initialize GLFW");

		glfwSetMonitorCallback(glfwMonitorCallback);

		int numMonitor; //Simulate GLFW Monitor connections to initialize Monitors
		auto monitors = glfwGetMonitors(&numMonitor);
		for (int i = 0; i < numMonitor; i++) {
			glfwMonitorCallback(monitors[i], GLFW_CONNECTED);
		}
		LIBV_UI_CORE_DEBUG("Initialized Core / GLFW Context");
	}

	void waitEvent() {
		glfwWaitEvents();
		if (!stopWait)
			thread.executeAsync(std::bind(&Core::waitEvent, this), 150);
	}

	void interruptWait() {
		glfwPostEmptyEvent();
	}

	void term() {
		LIBV_UI_CORE_INFO("Terminate Core / GLFW Context");
		glfwSetMonitorCallback(nullptr);
		glfwTerminate();
		LIBV_UI_CORE_DEBUG("Terminated Core / GLFW Context");
	}
public:

	void execute(std::function<void()>&& func) {
		std::lock_guard<std::mutex> lk(mutex);
		stopWait = true;
		interruptWait();
		thread.executeSync(std::move(func), 50);
		stopWait = false;
		thread.executeAsync(std::bind(&Core::waitEvent, this), 150);
	}

	void execute(const std::function<void()>& func) {
		std::lock_guard<std::mutex> lk(mutex);
		stopWait = true;
		interruptWait();
		thread.executeSync(func, 50);
		stopWait = false;
		thread.executeAsync(std::bind(&Core::waitEvent, this), 150);
	}

	Core() {
		std::lock_guard<std::mutex> lk(mutex);
		thread.executeSync(std::bind(&Core::init, this), 0);
		thread.executeAsync(std::bind(&Core::waitEvent, this), 150);
	}

	~Core() {
		std::lock_guard<std::mutex> lk(mutex);
		stopWait = true;
		interruptWait();
		thread.executeSync(std::bind(&Core::term, this), 200);
	}
};

static std::unique_ptr<Core> core;

// -------------------------------------------------------------------------------------------------

void coreExec(const std::function<void()>& func) {
	core->execute(func);
}

void coreExec(std::function<void()>&& func) {
	core->execute(std::move(func));
}

// -------------------------------------------------------------------------------------------------

void initCore() {
	core = std::make_unique<Core>();
}

void terminateCore() {
	core.reset(nullptr);
}

// -------------------------------------------------------------------------------------------------

void activateFrame(Frame* frame) {
	LIBV_UI_CORE_DEBUG("Activate frame [%s]", frame->getTitle());
	std::lock_guard<std::mutex> lk(activeFrames_m);
	activeFrames.insert(frame);
	noActiveFrame.reset();
}

void deactivateFrame(Frame* frame) {
	LIBV_UI_CORE_DEBUG("Deactivate frame [%s]", frame->getTitle());
	std::lock_guard<std::mutex> lk(activeFrames_m);
	activeFrames.erase(frame);
	if (activeFrames.size() == 0)
		noActiveFrame.raise();
}

void registerFrame(Frame* frame) {
	std::lock_guard<std::mutex> lk(frames_m);
	LIBV_UI_CORE_DEBUG("Register frame [%s]", frame->getTitle());
	if (frames.size() == 0)
		initCore();
	frames.insert(frame);
}

void unregisterFrame(Frame* frame) {
	std::lock_guard<std::mutex> lk(frames_m);
	LIBV_UI_CORE_DEBUG("Unregister frame [%s]", frame->getTitle());
	frames.erase(frame);
	if (frames.size() == 0)
		terminateCore();
}

// -------------------------------------------------------------------------------------------------

void Frame::joinAll() {
	LIBV_UI_CORE_DEBUG("Joining every frame");
	noActiveFrame.wait();
}

void Frame::closeAllForce() {
	LIBV_UI_CORE_DEBUG("Forced close every frame");
	std::lock_guard<std::mutex> lk(frames_m);
	for (auto frame : frames)
		frame->closeForce();
}

void Frame::closeAllDefault() {
	LIBV_UI_CORE_DEBUG("Default close every frame");
	std::lock_guard<std::mutex> lk(frames_m);
	for (auto frame : frames)
		frame->closeDefault();
}

// =================================================================================================

void Frame::cmdCoreCreate() {
	LIBV_UI_CORE_DEBUG("Create window for frame [%s]", title);

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLVersionMinor);
	glfwWindowHint(GLFW_DECORATED, decorated);
	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	glfwWindowHint(GLFW_FLOATING, false); // Always on top
	glfwWindowHint(GLFW_FOCUSED, true); // Initial focus
	glfwWindowHint(GLFW_OPENGL_PROFILE, openGLProfile);
	glfwWindowHint(GLFW_REFRESH_RATE, openGLRefreshRate);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_SAMPLES, openGLSamples);
	glfwWindowHint(GLFW_VISIBLE, false); // Always false, set after window creation

	//TODO P4: Map glfw hint GLFW_FLOATING
	//TODO P4: Map glfw hint GLFW_FOCUSED
	//TODO P4: Support glfw hint GLFW_OPENGL_DEBUG_CONTEXT
	//TODO P4: Support glfw hint GLFW_AUTO_ICONIFY
	//TODO P4: Support glfw hint GLFW_CONTEXT_ROBUSTNESS
	//TODO P4: Support glfw hint GLFW_CONTEXT_RELEASE_BEHAVIOR
	//TODO P4: Support glfw hint GLFW_OPENGL_FORWARD_COMPAT
	//TODO P5: Support glfw hint GLFW_STEREO
	//TODO P5: Support glfw hint GLFW_SRGB_CAPABLE

	//TODO P4: Disable deprecated openGL functionality by enable GLFW_OPENGL_FORWARD_COMPAT
	//TODO P4: Learn about GLFW_CONTEXT_ROBUSTNESS
	//TODO P4: Learn about GLFW_CONTEXT_RELEASE_BEHAVIOR
	//TODO P5: Context sharing

	// No plans for mapping these glfw hints:
	//GLFW_ACCUM_RED_BITS, GLFW_ACCUM_GREEN_BITS, GLFW_ACCUM_BLUE_BITS, GLFW_ACCUM_ALPHA_BITS,
	//GLFW_AUX_BUFFERS, GLFW_CLIENT_API, GLFW_DOUBLEBUFFER

	if (displayMode == DISPLAY_MODE_FULLSCREEN) {
		LIBV_UI_CORE_INFO("Switching frame [%s] to full screen mode", title);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		eventQueFramebufferSize.fire(EventFramebufferSize(mode->width, mode->height));
		window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), shareWindow);
	} else if (displayMode == DISPLAY_MODE_BORDERLESS) {
		LIBV_UI_CORE_INFO("Switching frame [%s] to borderless mode", title);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		eventQueFramebufferSize.fire(EventFramebufferSize(mode->width, mode->height));

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), shareWindow);
	} else if (displayMode == DISPLAY_MODE_WINDOWED) {
		window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, shareWindow);
	}
	if (!window) {
		LIBV_UI_CORE_ERROR("GLFW window creation failed");
		return;
	}

	if (displayMode == DISPLAY_MODE_FULLSCREEN) {
	} else if (displayMode == DISPLAY_MODE_BORDERLESS) {
		glfwSetWindowPos(window, 0, 0);
	} else if (displayMode == DISPLAY_MODE_WINDOWED) {
		glfwSetWindowPos(window, pos.x, pos.y);
	}

	registerEventCallbacks(this, window);
	activateFrame(this);
	baseInvalidate();

	LIBV_UI_CORE_DEBUG("Window creation was successful");
}

void Frame::cmdCoreRecreate() {
	LIBV_UI_CORE_DEBUG("Recreate window for frame [%s]", title);
	assert(window && "Requires a valid window");

	shareWindow = window;
	window = nullptr;

	cmdCoreCreate();
	if (!window) {
		LIBV_UI_CORE_ERROR("Rollback to previous window");
		window = shareWindow;
		shareWindow = nullptr;
		return;
	} else {
		LIBV_UI_CORE_TRACE("Destroy previous window");
		glfwDestroyWindow(shareWindow);
		unregisterEventCallbacks(shareWindow);
		shareWindow = nullptr;
	}
	if (!hidden)
		glfwShowWindow(window);
}

void Frame::cmdCoreDestroy() {
	LIBV_UI_CORE_DEBUG("Destroy window for frame [%s]", title);

	if (window) {
		deactivateFrame(this);
		unregisterEventCallbacks(window);
		glfwDestroyWindow(window);
		window = nullptr;
	}
}

void Frame::cmdCoreUpdateDisplayMode() {
	LIBV_UI_CORE_DEBUG("Update display mode for frame [%s]", title);
	LIBV_UI_CORE_ERROR("Not Implemented Yet"); //TODO P5: cmdCoreUpdateDisplayMode
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv