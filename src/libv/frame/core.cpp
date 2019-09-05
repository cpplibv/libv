// File: core.cpp, Created on 2015. április 12. 2:28, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/thread/executor_thread.hpp>
#include <libv/utility/enum.hpp>
// std
#include <atomic>
#include <memory>
#include <mutex>
// pro
#include <libv/frame/log.hpp>
#include <libv/frame/impl_frame.lpp>
#include <libv/frame/monitor.hpp>


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
		log_core.info("Initialize Core / GLFW Context");
		glfwSetErrorCallback([] (int code, const char* msg) {
			log_glfw.error("{} - {}", code, msg);
		});

		if (!glfwInit())
			return log_core.error("Failed to initialize GLFW");

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
		log_core.info("Terminate Core / GLFW Context");
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
	log_core.debug("Create window for frame {}", self->title);

	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	glfwWindowHint(GLFW_FOCUSED, true); // Initial focus
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, true); // Auto focus on show
	//	GLFW_CENTER_CURSOR
	//	GLFW_HOVERED
	//	GLFW_JOYSTICK_HAT_BUTTONS
	//	GLFW_LOCK_KEY_MODS
	//	GLFW_SCALE_TO_MONITOR
	//	GLFW_TRANSPARENT_FRAMEBUFFER

	glfwWindowHint(GLFW_OPENGL_PROFILE, libv::to_value(self->openGLProfile));
	glfwWindowHint(GLFW_REFRESH_RATE, libv::to_value(self->openGLRefreshRate));
	glfwWindowHint(GLFW_SAMPLES, libv::to_value(self->openGLSamples));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, self->openGLVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, self->openGLVersionMinor);
	glfwWindowHint(GLFW_DECORATED, self->decorated);
	glfwWindowHint(GLFW_MAXIMIZED, self->maximized);
	glfwWindowHint(GLFW_RESIZABLE, self->resizable);
	glfwWindowHint(GLFW_FLOATING, self->alwaysOnTop);
	glfwWindowHint(GLFW_VISIBLE, false); // Always false, set after window creation

	if (self->displayMode == DisplayMode::fullscreen) {
		log_core.info("Switching frame {} to full screen mode", self->title);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		self->eventQueue.emplace_back(EventFramebufferSize(mode->width, mode->height));
		self->window = glfwCreateWindow(mode->width, mode->height, self->title.c_str(), glfwGetPrimaryMonitor(), self->shareWindow);

	} else if (self->displayMode == DisplayMode::borderless) {
		log_core.info("Switching frame {} to borderless mode", self->title);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		self->eventQueue.emplace_back(EventFramebufferSize(mode->width, mode->height));

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		self->window = glfwCreateWindow(mode->width, mode->height, self->title.c_str(), glfwGetPrimaryMonitor(), self->shareWindow);

	} else if (self->displayMode == DisplayMode::windowed) {
		self->window = glfwCreateWindow(self->size.x, self->size.y, self->title.c_str(), nullptr, self->shareWindow);
	}

	if (!self->window) {
		log_core.error("GLFW window creation failed");
		return;
	}

	glfwSetWindowSizeLimits(self->window,
			self->sizeLimitMin.x < 0 ? GLFW_DONT_CARE : self->sizeLimitMin.x,
			self->sizeLimitMin.y < 0 ? GLFW_DONT_CARE : self->sizeLimitMin.y,
			self->sizeLimitMax.x < 0 ? GLFW_DONT_CARE : self->sizeLimitMax.x,
			self->sizeLimitMax.y < 0 ? GLFW_DONT_CARE : self->sizeLimitMax.y);

	glfwSetWindowAspectRatio(self->window,
			self->aspectRatio.x < 0 ? GLFW_DONT_CARE : self->aspectRatio.x,
			self->aspectRatio.y < 0 ? GLFW_DONT_CARE : self->aspectRatio.y);

	if (self->displayMode == DisplayMode::fullscreen) {

	} else if (self->displayMode == DisplayMode::borderless) {
		glfwSetWindowPos(self->window, 0, 0);
	} else if (self->displayMode == DisplayMode::windowed) {
		glfwSetWindowPos(self->window, self->position.x, self->position.y);
	}

	registerEventCallbacks(this, self->window);

	log_core.debug("Window creation was successful");
}

void Frame::cmdCoreRecreate() {
	log_core.debug("Recreate window for frame {}", self->title);
	assert(window && "Requires a valid window");

	self->shareWindow = self->window;
	self->window = nullptr;

	cmdCoreCreate();

	if (!self->window) {
		log_core.error("Rollback to previous window");
		self->window = self->shareWindow;
		self->shareWindow = nullptr;
		return;

	} else {
		log_core.trace("Destroy previous window");
		unregisterEventCallbacks(self->shareWindow);
		glfwDestroyWindow(self->shareWindow);
		self->shareWindow = nullptr;
	}

	if (!self->hidden)
		glfwShowWindow(self->window);
}

void Frame::cmdCoreDestroy() {
	log_core.debug("Destroy window for frame {}", self->title);

	if (self->window) {
		unregisterEventCallbacks(self->window);
		glfwDestroyWindow(self->window);
		self->window = nullptr;
	}
}

void Frame::cmdCoreUpdateDisplayMode() {
	log_core.debug("Update display mode for frame {}", self->title);
	log_core.error("Not Implemented Yet"); // TODO P5: cmdCoreUpdateDisplayMode
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
