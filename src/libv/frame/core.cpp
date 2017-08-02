// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/thread/semaphore.hpp>
// std
#include <atomic>
#include <memory>
#include <mutex>
// pro
#include <libv/frame/log.hpp>


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

// No plans for mapping these glfw hints:
// GLFW_ACCUM_RED_BITS,
// GLFW_ACCUM_GREEN_BITS,
// GLFW_ACCUM_BLUE_BITS,
// GLFW_ACCUM_ALPHA_BITS,
// GLFW_AUX_BUFFERS,
// GLFW_CLIENT_API,
// GLFW_DOUBLEBUFFER

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

class Core {
	// Priorities:
	// Init            0
	// Task           50
	// WaitEvent     150
	// Terminate     200
private:
	libv::ExecutorThread thread;
	std::atomic_bool stopWait{false};
	std::mutex mutex;

private:

	void init() {
		LIBV_LOG_FRAME_CORE_INFO("Initialize Core / GLFW Context");
		glfwSetErrorCallback(detail::errorCallbackGLFW);
		if (!glfwInit())
			return LIBV_LOG_FRAME_CORE_ERROR("Failed to initialize GLFW");

		glfwSetMonitorCallback(glfwMonitorCallback);

		int numMonitor; //Simulate GLFW Monitor connections to initialize Monitors
		auto monitors = glfwGetMonitors(&numMonitor);
		for (int i = 0; i < numMonitor; i++) {
			glfwMonitorCallback(monitors[i], GLFW_CONNECTED);
		}
	}

	void waitEvent() {
		glfwWaitEvents();
		if (!stopWait)
			thread.executeAsync(std::bind(&Core::waitEvent, this));
	}

	void interruptWait() {
		glfwPostEmptyEvent();
	}

	void term() {
		LIBV_LOG_FRAME_CORE_INFO("Terminate Core / GLFW Context");
		glfwSetMonitorCallback(nullptr);
		glfwTerminate();
		glfwSetErrorCallback(nullptr);
	}
public:

	void execute(std::function<void()>&& func) {
		std::lock_guard<std::mutex> lk(mutex);
		stopWait = true;
		interruptWait();
		thread.executeSync(std::move(func));
		stopWait = false;
		thread.executeAsync(std::bind(&Core::waitEvent, this));
	}

	void execute(const std::function<void()>& func) {
		std::lock_guard<std::mutex> lk(mutex);
		stopWait = true;
		interruptWait();
		thread.executeSync(func);
		stopWait = false;
		thread.executeAsync(std::bind(&Core::waitEvent, this));
	}

	Core() {
		std::lock_guard<std::mutex> lk(mutex);
		thread.executeSync(std::bind(&Core::init, this));
		thread.executeAsync(std::bind(&Core::waitEvent, this));
	}

	~Core() {
		std::lock_guard<std::mutex> lk(mutex);
		stopWait = true;
		interruptWait();
		thread.executeSync(std::bind(&Core::term, this));
	}
};

// -------------------------------------------------------------------------------------------------

std::mutex core_m;
std::weak_ptr<Core> core_wp;

void CoreProxy::exec(const std::function<void()>& func) {
	core->execute(func);
}

void CoreProxy::exec(std::function<void()>&& func) {
	core->execute(std::move(func));
}

CoreProxy::CoreProxy() {
	std::lock_guard<std::mutex> lock(core_m);
	core = core_wp.lock();
	if (!core)
		core_wp = core = std::make_shared<Core>();
}

CoreProxy::~CoreProxy() {
	std::lock_guard<std::mutex> lock(core_m);
	core.reset();
}

// -------------------------------------------------------------------------------------------------

void Frame::cmdCoreCreate() {
	LIBV_LOG_FRAME_CORE_DEBUG("Create window for frame {}", title);

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

	if (displayMode == DISPLAY_MODE_FULLSCREEN) {
		LIBV_LOG_FRAME_CORE_INFO("Switching frame {} to full screen mode", title);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		eventQueFramebufferSize.fire(EventFramebufferSize(mode->width, mode->height));
		window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), shareWindow);
	} else if (displayMode == DISPLAY_MODE_BORDERLESS) {
		LIBV_LOG_FRAME_CORE_INFO("Switching frame {} to borderless mode", title);
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
		LIBV_LOG_FRAME_CORE_ERROR("GLFW window creation failed");
		return;
	}

	if (displayMode == DISPLAY_MODE_FULLSCREEN) {
	} else if (displayMode == DISPLAY_MODE_BORDERLESS) {
		glfwSetWindowPos(window, 0, 0);
	} else if (displayMode == DISPLAY_MODE_WINDOWED) {
		glfwSetWindowPos(window, position.x, position.y);
	}

	registerEventCallbacks(this, window);

	LIBV_LOG_FRAME_CORE_DEBUG("Window creation was successful");
}

void Frame::cmdCoreRecreate() {
	LIBV_LOG_FRAME_CORE_DEBUG("Recreate window for frame {}", title);
	assert(window && "Requires a valid window");

	shareWindow = window;
	window = nullptr;

	cmdCoreCreate();
	if (!window) {
		LIBV_LOG_FRAME_CORE_ERROR("Rollback to previous window");
		window = shareWindow;
		shareWindow = nullptr;
		return;
	} else {
		LIBV_LOG_FRAME_CORE_TRACE("Destroy previous window");
		unregisterEventCallbacks(shareWindow);
		glfwDestroyWindow(shareWindow);
		shareWindow = nullptr;
	}
	if (!hidden)
		glfwShowWindow(window);
}

void Frame::cmdCoreDestroy() {
	LIBV_LOG_FRAME_CORE_DEBUG("Destroy window for frame {}", title);

	if (window) {
		unregisterEventCallbacks(window);
		glfwDestroyWindow(window);
		window = nullptr;
	}
}

void Frame::cmdCoreUpdateDisplayMode() {
	LIBV_LOG_FRAME_CORE_DEBUG("Update display mode for frame {}", title);
	LIBV_LOG_FRAME_CORE_ERROR("Not Implemented Yet"); // TODO P5: cmdCoreUpdateDisplayMode
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv