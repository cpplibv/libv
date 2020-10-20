// Project: libv.frame, File: src/libv/frame/core.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/frame/core.lpp>
// ext
#include <GLFW/glfw3.h>
// libv
#include <libv/mt/single_instance.hpp>
#include <libv/utility/enum.hpp>
// std
#include <atomic>
#include <memory>
#include <mutex>
// pro
#include <libv/frame/frame.hpp>
#include <libv/frame/impl_frame.lpp>
#include <libv/frame/log.hpp>
#include <libv/frame/monitor.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

libv::mt::SingleInstance<Core> core;

std::shared_ptr<Core> getCoreInstance() {
	return core.get();
}

// -------------------------------------------------------------------------------------------------

Core::Core() {
	std::lock_guard lk(mutex);
	thread.executeSync(std::bind(&Core::init, this));
	thread.executeAsync(std::bind(&Core::waitEvent, this));
}

Core::~Core() {
	std::lock_guard lk(mutex);
	stopWait = true;
	interruptWait();
	thread.executeSync(std::bind(&Core::term, this));
}

void Core::init() {
	log_core.info("Initialize GLFW Context");
	glfwSetErrorCallback([] (int code, const char* msg) {
		log_glfw.error("{} - {}", code, msg);
	});

	if (!glfwInit())
		return log_core.error("Failed to initialize GLFW");

	glfwSetMonitorCallback(dispatchGLFWMonitorEvent);

	int numMonitor; //Simulate GLFW Monitor connections to initialize Monitors
	auto monitors = glfwGetMonitors(&numMonitor);
	for (int i = 0; i < numMonitor; i++) {
		dispatchGLFWMonitorEvent(monitors[i], GLFW_CONNECTED);
	}
}

void Core::waitEvent() {
	glfwWaitEvents();
	if (!stopWait)
		thread.executeAsync(std::bind(&Core::waitEvent, this));
}

void Core::interruptWait() {
	glfwPostEmptyEvent();
}

void Core::term() {
	log_core.info("Terminate GLFW Context");
	glfwSetMonitorCallback(nullptr);
	glfwTerminate();
	glfwSetErrorCallback(nullptr);
}

void Core::execute(libv::function_ref<void()> func) {
	std::lock_guard lk(mutex);
	stopWait = true;
	interruptWait();
	thread.executeSync(func);
	stopWait = false;
	thread.executeAsync(std::bind(&Core::waitEvent, this));
}

// -------------------------------------------------------------------------------------------------

void Frame::cmdCoreCreate() {
	log_core.trace("Create window for frame {}", self->title);

	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, self->openGLForwardCompat && self->openGLVersionMajor >= 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, libv::to_value(self->openGLProfile));
	glfwWindowHint(GLFW_REFRESH_RATE, libv::to_value(self->openGLRefreshRate));
	glfwWindowHint(GLFW_SAMPLES, libv::to_value(self->openGLSamples));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, self->openGLVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, self->openGLVersionMinor);
	glfwWindowHint(GLFW_DECORATED, self->decorated);
	glfwWindowHint(GLFW_MAXIMIZED, self->maximized);
	glfwWindowHint(GLFW_RESIZABLE, self->resizable);
	glfwWindowHint(GLFW_FOCUSED, self->initialFocus);
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, self->focusOnShow);
	glfwWindowHint(GLFW_FLOATING, self->alwaysOnTop);
	glfwWindowHint(GLFW_VISIBLE, false); // Always false, set after window creation

	GLFWmonitor* monitor = getCurrentMonitor().handler;

	switch (self->displayMode) {
	case DisplayMode::windowed: {
		glfwWindowHint(GLFW_DECORATED, true);
		self->window = glfwCreateWindow(self->size.x, self->size.y, self->title.c_str(), nullptr, self->shareWindow);

		if (!self->window)
			return log_core.error("GLFW window creation failed");

		glfwSetWindowPos(self->window, self->position.x, self->position.y);

		glfwSetWindowSizeLimits(self->window,
				self->sizeLimitMin.x < 0 ? GLFW_DONT_CARE : self->sizeLimitMin.x,
				self->sizeLimitMin.y < 0 ? GLFW_DONT_CARE : self->sizeLimitMin.y,
				self->sizeLimitMax.x < 0 ? GLFW_DONT_CARE : self->sizeLimitMax.x,
				self->sizeLimitMax.y < 0 ? GLFW_DONT_CARE : self->sizeLimitMax.y);

		glfwSetWindowAspectRatio(self->window,
				self->aspectRatio.x < 0 ? GLFW_DONT_CARE : self->aspectRatio.x,
				self->aspectRatio.y < 0 ? GLFW_DONT_CARE : self->aspectRatio.y);

		break;
	} case DisplayMode::borderless_maximized: {
		glfwWindowHint(GLFW_DECORATED, false);

		int workAreaX, workAreaY, workAreaWidth, workAreaHeight;
		glfwGetMonitorWorkarea(monitor, &workAreaX, &workAreaY, &workAreaWidth, &workAreaHeight);
		self->window = glfwCreateWindow(workAreaWidth, workAreaHeight, self->title.c_str(), nullptr, self->shareWindow);

		if (!self->window)
			return log_core.error("GLFW window creation failed");

		glfwSetWindowPos(self->window, workAreaX, workAreaY);

		break;
	} case DisplayMode::fullscreen: {
		self->window = glfwCreateWindow(self->size.x, self->size.y, self->title.c_str(), monitor, self->shareWindow);
		if (!self->window)
			return log_core.error("GLFW window creation failed");

		break;
	} case DisplayMode::fullscreen_windowed: {
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		self->window = glfwCreateWindow(mode->width, mode->height, self->title.c_str(), monitor, self->shareWindow);

		if (!self->window)
			return log_core.error("GLFW window creation failed");

		break;
	}}

	glfwSetInputMode(self->window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetInputMode(self->window, GLFW_CURSOR, libv::to_value(self->cursorMode));

	glfwSetWindowIcon(self->window, static_cast<int>(self->iconsGLFW.size()), self->iconsGLFW.data());

	registerEventCallbacks(this, self->window);

	log_core.debug("Window creation was successful");
}

void Frame::cmdCoreRecreate() {
	log_core.trace("Recreate window for frame {}", self->title);
	assert(self->window && "Requires a valid window");

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
	log_core.trace("Destroy window for frame {}", self->title);

	if (self->window) {
		unregisterEventCallbacks(self->window);
		glfwDestroyWindow(self->window);
		self->window = nullptr;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
