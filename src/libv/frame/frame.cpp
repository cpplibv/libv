// File: Frame.cpp, Created on 2013. szeptember 27. 16:00, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <fmt/format.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/thread/executor_thread.hpp>
// pro
#include <libv/frame/log.hpp>
#include <libv/frame/impl_frame.lpp>
#include <libv/frame/monitor.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

Frame::Frame(std::string title, libv::vec2i size) :
	self(std::make_unique<ImplFrame>(title, size)) {
}

Frame::Frame(std::string title, int32_t width, int32_t height) :
	Frame(std::move(title), libv::vec2i{width, height}) { }

Frame::Frame(std::string title) :
	Frame(std::move(title), libv::vec2i{1280, 960}) { }

Frame::Frame(int32_t width, int32_t height) :
	Frame("", libv::vec2i{width, height}) { }

Frame::Frame(libv::vec2i size) :
	Frame("", size) { }

Frame::~Frame() {
	closeForce();
	join();
}

// Frame Loop --------------------------------------------------------------------------------------

void Frame::loopInit() {
	log_frame.debug("Frame entering loop");

	self->context.executeAsync(std::bind(&Frame::loop, this));
}

void Frame::loop() {
	distributeEvents();

	if (isFrameShouldClose()) {
		loopTerminate();
	} else {
		if (!isRefreshSkipable() && self->window) {
			onContextUpdate.fire(EventContextUpdate());

			glfwSwapBuffers(self->window);
		}

		self->context.executeAsync(std::bind(&Frame::loop, this));
	}
}

void Frame::loopTerminate() {
	log_frame.debug("Frame exiting loop");
	cmdFrameDestroy();
	self->context.stop();
}

void Frame::contextCreate() {
	log_frame.debug("Frame context create");
	onContextCreate.fire(EventContextCreate());
}

void Frame::contextDestroy() {
	log_frame.debug("Frame context destroy");
	onContextDestroy.fire(EventContextDestroy());
}

// -------------------------------------------------------------------------------------------------

void Frame::cmdFrameCreate() {
	self->core.exec(std::bind(&Frame::cmdCoreCreate, this));
	if (self->window) {
		glfwMakeContextCurrent(self->window);
		self->context.executeAsync(std::bind(&Frame::loopInit, this));
		this->contextCreate();
	}
}

void Frame::cmdFrameRecreate() {
	assert(self->window);

	glfwMakeContextCurrent(nullptr);
	self->core.exec(std::bind(&Frame::cmdCoreRecreate, this));
	if (self->window) {
		glfwMakeContextCurrent(self->window);
		this->contextCreate();
	}
}

void Frame::cmdFrameDestroy() {
	this->contextDestroy();
	glfwMakeContextCurrent(nullptr);
	self->core.exec(std::bind(&Frame::cmdCoreDestroy, this));
}

// -------------------------------------------------------------------------------------------------

void Frame::closeDefault() {
	log_frame.trace("Close default frame {}", self->title);
	if (self->window)
		glfwSetWindowShouldClose(self->window, true);
}

void Frame::closeForce() {
	log_frame.trace("Close force frame {}", self->title);
	self->forcedClose = true;
}

void Frame::join() {
	self->context.join();
}

bool Frame::isRefreshSkipable() {
	return self->hidden || self->minimized;
}

bool Frame::isFrameShouldClose() {
	if (self->forcedClose) { // If we are forced to close
		onCloseRequest.fire(EventCloseRequest());
		return true;
	}

	if (!self->window) // If there is still no self->window
		return false;

	bool shouldClose = glfwWindowShouldClose(self->window);
	if (shouldClose) { // Fire on close event which can change the outcome
		EventCloseRequest ecr;
		onCloseRequest.fire(ecr);
		shouldClose = !ecr.isAborted();
	}

	if (shouldClose) { // On close let DCO handle
		switch (self->defaultCloseOperation) {
		case CloseOperation::dispose:
			return true;
		case CloseOperation::do_nothing:
			glfwSetWindowShouldClose(self->window, false);
			return false;
		case CloseOperation::hide:
			glfwSetWindowShouldClose(self->window, false);
			hide();
			return false;
		case CloseOperation::minimize:
			glfwSetWindowShouldClose(self->window, false);
			minimize();
			return false;
		}
	}
	return shouldClose;
}

// -------------------------------------------------------------------------------------------------

void Frame::show() {
	self->context.executeAsync([this] {
		log_frame.trace("Show frame {}", self->title);
		if (!self->window) {
			cmdFrameCreate();
		}
		if (self->window) {
			self->core.exec(std::bind(glfwShowWindow, self->window));
			self->hidden = false;
		}
	});
}

void Frame::hide() {
	self->context.executeAsync([this] {
		log_frame.trace("Hide frame {}", self->title);
		self->hidden = true;
		if (self->window) {
			self->core.exec(std::bind(glfwHideWindow, self->window));
		}
	});
}

void Frame::maximize() {
	self->context.executeAsync([this] {
		log_frame.trace("Maximize frame {}", self->title);
		self->maximized = true;
		if (self->window) {
			self->core.exec(std::bind(glfwMaximizeWindow, self->window));
		}
	});
}

void Frame::minimize() {
	self->context.executeAsync([this] {
		log_frame.trace("Minimize frame {}", self->title);
		self->minimized = true;
		if (self->window) {
			self->core.exec(std::bind(glfwIconifyWindow, self->window));
		}
	});
}

void Frame::restore() {
	self->context.executeAsync([this] {
		log_frame.trace("Restore frame {}", self->title);
		self->minimized = false;
		self->maximized = false;
		if (self->window) {
			self->core.exec(std::bind(glfwRestoreWindow, self->window));
		}
	});
}

void Frame::focus() {
	// Question? Does focus wants to be a creation hint as well that is only one times used?
	self->context.executeAsync([this] {
		log_frame.trace("Focus frame {}", self->title);
		if (self->window)
			self->core.exec(std::bind(glfwFocusWindow, self->window));
	});
}

void Frame::requestAttention() {
	self->context.executeAsync([this] {
		log_frame.trace("Request Attention frame {}", self->title);
		if (self->window)
			self->core.exec(std::bind(glfwRequestWindowAttention, self->window));
	});
}

// -------------------------------------------------------------------------------------------------

void Frame::setOpenGLProfile(Frame::OpenGLProfile profile) {
	self->context.executeAsync([this, profile] {
		log_frame.trace("Set frame OpenGLProfile of {} to {}", self->title, libv::to_value(profile));
		self->openGLProfile = profile;
		if (self->window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLRefreshRate(Frame::OpenGLRefreshRate rate) {
	self->context.executeAsync([this, rate] {
		log_frame.trace("Set frame OpenGLRefreshRate of {} to {}", self->title, libv::to_value(rate));
		self->openGLRefreshRate = rate;
		if (self->window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLSamples(Frame::OpenGLSamples samples) {
	self->context.executeAsync([this, samples] {
		log_frame.trace("Set frame OpenGLSamples of {} to {}", self->title, libv::to_value(samples));
		self->openGLSamples = samples;
		if (self->window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLVersion(int major, int minor) {
	self->context.executeAsync([this, major, minor] {
		log_frame.trace("Set frame OpenGLVersion of {} to {}.{}", self->title, major, minor);
		self->openGLVersionMajor = major;
		self->openGLVersionMinor = minor;
		if (self->window)
			cmdFrameRecreate();
	});
}

// ---

void Frame::setCloseOperation(Frame::CloseOperation operation) {
	log_frame.trace("Set frame CloseOperation of {} to {}", self->title, libv::to_value(operation));
	self->defaultCloseOperation = operation;
}

void Frame::setDecoration(bool decorated) {
	self->context.executeAsync([this, decorated] {
		log_frame.trace("Set frame Decoration of {} to {}", self->title, decorated);
		self->decorated = decorated;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowAttrib, self->window, GLFW_DECORATED, decorated));
	});
}

void Frame::setDisplayMode(Frame::DisplayMode mode) {
	self->context.executeAsync([this, mode] {
		log_frame.trace("Set frame DisplayMode of {} to {}", self->title, libv::to_value(mode));
		self->displayMode = mode;
		if (self->window)
			cmdFrameRecreate();
	});
}

void Frame::setPosition(int x, int y) {
	setPosition(libv::vec2i(x, y));
}

void Frame::setPosition(libv::vec2i newpos) {
	self->context.executeAsync([this, newpos] {
		log_frame.trace("Set frame Position of {} to {}, {}", self->title, newpos.x, newpos.y);
		self->position = newpos;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowPos, self->window, self->position.x, self->position.y));
	});
}

void Frame::setPosition(FramePosition pos) {
	switch (pos) {
	case FramePosition::center_current_monitor:
		self->context.executeAsync([this] {
			auto& monitor = getCurrentMonitor();
			auto newpos = monitor.position + monitor.currentVideoMode.size / 2 - self->size / 2;
			log_frame.trace("Set frame Position of {} to {}, {} as center of current monitor", self->title, newpos.x, newpos.y);
			self->position = newpos;
			if (self->window)
				self->core.exec(std::bind(glfwSetWindowPos, self->window, self->position.x, self->position.y));
		});
		break;
	case FramePosition::center_primary_monitor:
		self->context.executeAsync([this] {
			auto& monitor = Monitor::getPrimaryMonitor();
			auto newpos = monitor.position + monitor.currentVideoMode.size / 2 - self->size / 2;
			log_frame.trace("Set frame Position of {} to {}, {} as center of primary monitor", self->title, newpos.x, newpos.y);
			self->position = newpos;
			if (self->window)
				self->core.exec(std::bind(glfwSetWindowPos, self->window, self->position.x, self->position.y));
		});
		break;
	}
}

void Frame::setAspectRatio(libv::vec2i fraction) {
	self->context.executeAsync([this, fraction] {
		log_frame.trace("Set frame AspectRatio of {} to {}/{}", self->title, fraction.x, fraction.y);
		self->aspectRatio = fraction;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowAspectRatio, self->window,
					self->aspectRatio.x < 0 ? GLFW_DONT_CARE : self->aspectRatio.x,
					self->aspectRatio.y < 0 ? GLFW_DONT_CARE : self->aspectRatio.y));
	});
}

void Frame::setAlwaysOnTop(bool alwaysOnTop) {
	self->context.executeAsync([this, alwaysOnTop] {
		log_frame.trace("Set frame AlwaysOnTop of {} to {}", self->title, alwaysOnTop);
		self->alwaysOnTop = alwaysOnTop;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowAttrib, self->window, GLFW_FLOATING, alwaysOnTop));
	});
}

void Frame::setInitialFocus(bool initialFocus) {
	self->context.executeAsync([this, initialFocus] {
		log_frame.trace("Set frame initialFocus of {} to {}", self->title, initialFocus);
		self->initialFocus = initialFocus;
	});
}

void Frame::setFocusOnShow(bool focusOnShow) {
	self->context.executeAsync([this, focusOnShow] {
		log_frame.trace("Set frame FocusOnShow of {} to {}", self->title, focusOnShow);
		self->focusOnShow = focusOnShow;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowAttrib, self->window, GLFW_FOCUS_ON_SHOW, focusOnShow));
	});
}

void Frame::setAspectRatio(int numer, int denom) {
	setAspectRatio(libv::vec2i{numer, denom});
}

void Frame::setResizable(bool resizable) {
	self->context.executeAsync([this, resizable] {
		log_frame.trace("Set frame Resizable of {} to {}", self->title, resizable);
		self->resizable = resizable;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowAttrib, self->window, GLFW_RESIZABLE, resizable));
	});
}

void Frame::setSize(int x, int y) {
	setSize(libv::vec2i(x, y));
}

void Frame::setSize(libv::vec2i newsize) {
	self->context.executeAsync([this, newsize] {
		log_frame.trace("Set frame size of {} to {}", self->title, newsize);
		self->size = newsize;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowSize, self->window, self->size.x, self->size.y));
	});
}

void Frame::setSizeLimit(libv::vec2i min, libv::vec2i max) {
	self->context.executeAsync([this, min, max] {
		log_frame.trace("Set frame size limit of {} to {}, {}", self->title, min, max);
		self->sizeLimitMax = max;
		self->sizeLimitMin = min;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowSizeLimits, self->window,
					self->sizeLimitMin.x < 0 ? GLFW_DONT_CARE : self->sizeLimitMin.x,
					self->sizeLimitMin.y < 0 ? GLFW_DONT_CARE : self->sizeLimitMin.y,
					self->sizeLimitMax.x < 0 ? GLFW_DONT_CARE : self->sizeLimitMax.x,
					self->sizeLimitMax.y < 0 ? GLFW_DONT_CARE : self->sizeLimitMax.y));
	});
}

void Frame::setSizeLimit(int minx, int miny, int maxx, int maxy) {
	setSizeLimit(libv::vec2i{minx, miny}, libv::vec2i{maxx, maxy});
}

void Frame::setSizeLimit(int minx, int miny, libv::vec2i max) {
	setSizeLimit(libv::vec2i{minx, miny}, max);
}

void Frame::setSizeLimit(libv::vec2i min, int maxx, int maxy) {
	setSizeLimit(min, libv::vec2i{maxx, maxy});
}

void Frame::setSizeLimitMin(int minx, int miny) {
	setSizeLimit(libv::vec2i{minx, miny}, self->sizeLimitMax);
}

void Frame::setSizeLimitMin(libv::vec2i min) {
	setSizeLimit(min, self->sizeLimitMax);
}

void Frame::setSizeLimitMax(int maxx, int maxy) {
	setSizeLimit(self->sizeLimitMin, libv::vec2i{maxx, maxy});
}

void Frame::setSizeLimitMax(libv::vec2i max) {
	setSizeLimit(self->sizeLimitMin, max);
}

void Frame::setTitle(std::string title) {
	self->context.executeAsync([this, title = std::move(title)] {
		log_frame.trace("Set frame Title of {} to {}", self->title, title);
		self->title = std::move(title);
		if (self->window)
			self->core.exec([this] {
				glfwSetWindowTitle(self->window, self->title.c_str());
			});
	});
}

// Getters -----------------------------------------------------------------------------------------

Frame::CloseOperation Frame::getCloseOperation() const {
	return self->defaultCloseOperation;
}

Frame::DisplayMode Frame::getDisplayMode() const {
	return self->displayMode;
}

libv::vec2i Frame::getPosition() const {
	return self->position;
}

libv::vec2i Frame::getSize() const {
	return self->size;
}

libv::vec4i Frame::getFrameSize() const {
	return self->frameSize;
}

const std::string& Frame::getTitle() const {
	return self->title;
}

bool Frame::isDecorated() const {
	return self->decorated;
}

bool Frame::isResizable() const {
	return self->resizable;
}

bool Frame::isVisible() const {
	return !self->hidden && !self->minimized && self->window;
}

const Monitor& Frame::getCurrentMonitor() const {
	return Monitor::getMonitorAt(self->position + self->size / 2);
}

// -------------------------------------------------------------------------------------------------

libv::input::KeyState Frame::getKey(libv::input::Key key) {
	return self->pressedKeys.contains(key) ? libv::input::KeyState::pressed : libv::input::KeyState::released;
}

bool Frame::isKeyPressed(libv::input::Key key) {
	return self->pressedKeys.contains(key);
}

bool Frame::isKeyReleased(libv::input::Key key) {
	return not self->pressedKeys.contains(key);
}

libv::input::KeyState Frame::getMouse(libv::input::Mouse key) {
	return self->pressedMouseButtons.contains(key) ? libv::input::KeyState::pressed : libv::input::KeyState::released;
}

bool Frame::isMousePressed(libv::input::Mouse key) {
	return self->pressedMouseButtons.contains(key);
}

bool Frame::isMouseReleased(libv::input::Mouse key) {
	return not self->pressedMouseButtons.contains(key);
}

libv::vec2d Frame::getMousePosition() {
	return self->mousePosition;
}

libv::vec2d Frame::getScrollPosition() {
	return self->scrollPosition;
}

// -------------------------------------------------------------------------------------------------

std::string Frame::getClipboardString() {
	const char* text = glfwGetClipboardString(nullptr);
	if (text != nullptr)
		return std::string{text};
	else
		return std::string{""};
}

void Frame::setClipboardString(const std::string& string) {
	glfwSetClipboardString(nullptr, string.c_str());
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
