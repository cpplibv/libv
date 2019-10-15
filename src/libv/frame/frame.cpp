// File: Frame.cpp, Created on 2013. szeptember 27. 16:00, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <GLFW/glfw3.h>
// pro
#include <libv/frame/core.lpp>
#include <libv/frame/impl_frame.lpp>
#include <libv/frame/log.hpp>
#include <libv/frame/monitor.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

Frame::Frame(std::string title, libv::vec2i size) :
	self(std::make_unique<ImplFrame>(title, size)) { }

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
		bool update;
		{
			std::lock_guard lock(self->frameState_m);
			update = !self->hidden && !self->minimized && self->window;
		}
		if (update) {
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
	self->core->execute(std::bind(&Frame::cmdCoreCreate, this));
	if (self->window) {
		glfwMakeContextCurrent(self->window);
		self->context.executeAsync(std::bind(&Frame::loopInit, this));
		this->contextCreate();
	}
}

void Frame::cmdFrameRecreate() {
	assert(self->window);

	glfwMakeContextCurrent(nullptr);
	self->core->execute(std::bind(&Frame::cmdCoreRecreate, this));
	if (self->window) {
		glfwMakeContextCurrent(self->window);
		this->contextCreate();
	}
}

void Frame::cmdFrameDestroy() {
	this->contextDestroy();
	glfwMakeContextCurrent(nullptr);
	self->core->execute(std::bind(&Frame::cmdCoreDestroy, this));
}

// -------------------------------------------------------------------------------------------------

void Frame::closeDefault() {
	std::lock_guard lock(self->frameState_m);
	log_frame.trace("Close default frame {}", self->title);
	if (self->window)
		glfwSetWindowShouldClose(self->window, true);
}

void Frame::closeForce() {
	std::lock_guard lock(self->frameState_m);
	log_frame.trace("Close force frame {}", self->title);
	self->forcedClose = true;
}

void Frame::join() {
	self->context.join();
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

	std::lock_guard lock(self->frameState_m);
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

const Monitor& Frame::_getCurrentMonitor() const {
	return Monitor::getMonitorClosest(self->position + self->size / 2);
}

// -------------------------------------------------------------------------------------------------

void Frame::show() {
	self->context.executeAsync([this] {
		{
			std::lock_guard lock(self->frameState_m);
			log_frame.trace("Show frame {}", self->title);
		}
		if (!self->window) {
			cmdFrameCreate();
		}
		if (self->window) {
			self->core->execute(std::bind(glfwShowWindow, self->window));
			std::lock_guard lock(self->frameState_m);
			self->hidden = false;
		}
	});
}

void Frame::hide() {
	self->context.executeAsync([this] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Hide frame {}", self->title);
		self->hidden = true;
		if (self->window) {
			self->core->execute(std::bind(glfwHideWindow, self->window));
		}
	});
}

void Frame::maximize() {
	self->context.executeAsync([this] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Maximize frame {}", self->title);
		self->maximized = true;
		if (self->window) {
			self->core->execute(std::bind(glfwMaximizeWindow, self->window));
		}
	});
}

void Frame::minimize() {
	self->context.executeAsync([this] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Minimize frame {}", self->title);
		self->minimized = true;
		if (self->window) {
			self->core->execute(std::bind(glfwIconifyWindow, self->window));
		}
	});
}

void Frame::restore() {
	self->context.executeAsync([this] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Restore frame {}", self->title);
		self->minimized = false;
		self->maximized = false;
		if (self->window) {
			self->core->execute(std::bind(glfwRestoreWindow, self->window));
		}
	});
}

void Frame::focus() {
	// Question? Does focus wants to be a creation hint as well that is only one times used?
	self->context.executeAsync([this] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Focus frame {}", self->title);
		if (self->window)
			self->core->execute(std::bind(glfwFocusWindow, self->window));
	});
}

void Frame::requestAttention() {
	self->context.executeAsync([this] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Request Attention frame {}", self->title);
		if (self->window)
			self->core->execute(std::bind(glfwRequestWindowAttention, self->window));
	});
}

// -------------------------------------------------------------------------------------------------

void Frame::setOpenGLProfile(Frame::OpenGLProfile profile) {
	self->context.executeAsync([this, profile] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame OpenGLProfile of {} to {}", self->title, libv::to_value(profile));
		self->openGLProfile = profile;
		if (self->window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLRefreshRate(Frame::OpenGLRefreshRate rate) {
	self->context.executeAsync([this, rate] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame OpenGLRefreshRate of {} to {}", self->title, libv::to_value(rate));
		self->openGLRefreshRate = rate;
		if (self->window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLSamples(Frame::OpenGLSamples samples) {
	self->context.executeAsync([this, samples] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame OpenGLSamples of {} to {}", self->title, libv::to_value(samples));
		self->openGLSamples = samples;
		if (self->window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLVersion(int major, int minor) {
	self->context.executeAsync([this, major, minor] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame OpenGLVersion of {} to {}.{}", self->title, major, minor);
		self->openGLVersionMajor = major;
		self->openGLVersionMinor = minor;
		if (self->window)
			cmdFrameRecreate();
	});
}

// ---

void Frame::setCloseOperation(Frame::CloseOperation operation) {
	self->context.executeAsync([this, operation] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame CloseOperation of {} to {}", self->title, libv::to_value(operation));
		self->defaultCloseOperation = operation;
	});
}

void Frame::setCursorMode(Frame::CursorMode cursorMode) {
	self->context.executeAsync([this, cursorMode] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame CursorMode of {} to {}", self->title, libv::to_value(cursorMode));
		self->cursorMode = cursorMode;
		if (self->window)
			self->core->execute(std::bind(glfwSetInputMode, self->window, GLFW_CURSOR, libv::to_value(cursorMode)));
	});
}

void Frame::setDecoration(bool decorated) {
	self->context.executeAsync([this, decorated] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame Decoration of {} to {}", self->title, decorated);
		self->decorated = decorated;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowAttrib, self->window, GLFW_DECORATED, decorated));
	});
}

void Frame::setDisplayMode(Frame::DisplayMode displayMode) {
	self->context.executeAsync([this, displayMode] {
		{
			std::lock_guard lock(self->frameState_m);
			log_frame.trace("Set frame DisplayMode of {} to {}", self->title, libv::to_value(displayMode));
			self->displayMode = displayMode;
			if (!self->window)
				return;
		}

		self->core->execute([this] {
			std::lock_guard lock(self->frameState_m);
			const Monitor& monitor = _getCurrentMonitor();

			switch (self->displayMode) {
			case DisplayMode::windowed: {
				glfwSetWindowMonitor(self->window, nullptr, self->position.x, self->position.y, self->size.x, self->size.y, GLFW_DONT_CARE);
				glfwSetWindowAttrib(self->window, GLFW_DECORATED, self->decorated);

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
				glfwSetWindowAttrib(self->window, GLFW_DECORATED, false);
				int workAreaX, workAreaY, workAreaWidth, workAreaHeight;
				glfwGetMonitorWorkarea(monitor.handler, &workAreaX, &workAreaY, &workAreaWidth, &workAreaHeight);
				glfwSetWindowMonitor(self->window, nullptr, workAreaX, workAreaY, workAreaWidth, workAreaHeight, GLFW_DONT_CARE);

				break;
			} case DisplayMode::fullscreen: {
				glfwSetWindowMonitor(self->window, monitor.handler, 0, 0, self->size.x, self->size.y, libv::to_value(self->openGLRefreshRate));

				break;
			} case DisplayMode::fullscreen_windowed: {
				const GLFWvidmode* mode = glfwGetVideoMode(monitor.handler);
				glfwSetWindowMonitor(self->window, monitor.handler, 0, 0, mode->width, mode->height, mode->refreshRate);

				break;
			}}
		});

	});
}

void Frame::setPosition(int x, int y) {
	setPosition(libv::vec2i(x, y));
}

void Frame::setPosition(libv::vec2i newpos) {
	self->context.executeAsync([this, newpos] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame Position of {} to {}, {}", self->title, newpos.x, newpos.y);
		self->position = newpos;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowPos, self->window, self->position.x, self->position.y));
	});
}

void Frame::setPosition(FramePosition pos) {
	self->context.executeAsync([this, pos] {
		std::lock_guard lock(self->frameState_m);
		auto& monitor = pos == FramePosition::center_current_monitor ? _getCurrentMonitor() : Monitor::getPrimaryMonitor();

		auto newpos = monitor.position + monitor.currentVideoMode.size / 2 - self->size / 2;
		log_frame.trace("Set frame Position of {} to {}, {} as center of current monitor", self->title, newpos.x, newpos.y);
		self->position = newpos;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowPos, self->window, self->position.x, self->position.y));
	});
}

void Frame::setAspectRatio(libv::vec2i fraction) {
	self->context.executeAsync([this, fraction] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame AspectRatio of {} to {}/{}", self->title, fraction.x, fraction.y);
		self->aspectRatio = fraction;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowAspectRatio, self->window,
					self->aspectRatio.x < 0 ? GLFW_DONT_CARE : self->aspectRatio.x,
					self->aspectRatio.y < 0 ? GLFW_DONT_CARE : self->aspectRatio.y));
	});
}

void Frame::setAlwaysOnTop(bool alwaysOnTop) {
	self->context.executeAsync([this, alwaysOnTop] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame AlwaysOnTop of {} to {}", self->title, alwaysOnTop);
		self->alwaysOnTop = alwaysOnTop;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowAttrib, self->window, GLFW_FLOATING, alwaysOnTop));
	});
}

void Frame::setInitialFocus(bool initialFocus) {
	self->context.executeAsync([this, initialFocus] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame initialFocus of {} to {}", self->title, initialFocus);
		self->initialFocus = initialFocus;
	});
}

void Frame::setFocusOnShow(bool focusOnShow) {
	self->context.executeAsync([this, focusOnShow] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame FocusOnShow of {} to {}", self->title, focusOnShow);
		self->focusOnShow = focusOnShow;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowAttrib, self->window, GLFW_FOCUS_ON_SHOW, focusOnShow));
	});
}

void Frame::setAspectRatio(int numer, int denom) {
	setAspectRatio(libv::vec2i{numer, denom});
}

void Frame::setResizable(bool resizable) {
	self->context.executeAsync([this, resizable] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame Resizable of {} to {}", self->title, resizable);
		self->resizable = resizable;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowAttrib, self->window, GLFW_RESIZABLE, resizable));
	});
}

void Frame::setSize(int x, int y) {
	setSize(libv::vec2i(x, y));
}

void Frame::setSize(libv::vec2i newsize) {
	self->context.executeAsync([this, newsize] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame size of {} to {}", self->title, newsize);
		self->size = newsize;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowSize, self->window, self->size.x, self->size.y));
	});
}

void Frame::setSizeLimit(libv::vec2i min, libv::vec2i max) {
	self->context.executeAsync([this, min, max] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame size limit of {} to {}, {}", self->title, min, max);
		self->sizeLimitMax = max;
		self->sizeLimitMin = min;
		if (self->window)
			self->core->execute(std::bind(glfwSetWindowSizeLimits, self->window,
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
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame Title of {} to {}", self->title, title);
		self->title = std::move(title);
		if (self->window)
			self->core->execute([this] {
				glfwSetWindowTitle(self->window, self->title.c_str());
			});
	});
}

void Frame::setIcon(std::vector<Icon> icons) {
	self->context.executeAsync([this, icons = std::move(icons)] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame Icons of {} with {} icon", self->title, icons.size());
		self->icons = std::move(icons);
		self->iconsGLFW.resize(self->icons.size());

		for (size_t i = 0; i < self->icons.size(); ++i) {
			self->iconsGLFW[i].width = self->icons[i].size.x;
			self->iconsGLFW[i].height = self->icons[i].size.y;
			self->iconsGLFW[i].pixels = self->icons[i].pixels[0].ptr();
		}
		if (self->window)
			self->core->execute([this] {
				glfwSetWindowIcon(self->window, static_cast<int>(self->iconsGLFW.size()), self->iconsGLFW.data());
			});
	});
}

void Frame::clearIcon() {
	self->context.executeAsync([this] {
		std::lock_guard lock(self->frameState_m);
		log_frame.trace("Set frame clear Icons of {}", self->title);
		self->icons.clear();
		self->iconsGLFW.clear();
		if (self->window)
			self->core->execute([this] {
				glfwSetWindowIcon(self->window, 0, nullptr);
			});
	});
}

// Getters -----------------------------------------------------------------------------------------

Frame::CloseOperation Frame::getCloseOperation() const {
	std::lock_guard lock(self->frameState_m);
	return self->defaultCloseOperation;
}

Frame::DisplayMode Frame::getDisplayMode() const {
	std::lock_guard lock(self->frameState_m);
	return self->displayMode;
}

libv::vec2i Frame::getPosition() const {
	std::lock_guard lock(self->frameState_m);
	return self->position;
}

libv::vec2i Frame::getSize() const {
	std::lock_guard lock(self->frameState_m);
	return self->size;
}

libv::vec4i Frame::getFrameSize() const {
	std::lock_guard lock(self->frameState_m);
	return self->frameSize;
}

libv::vec2f Frame::getContentScale() const {
	std::lock_guard lock(self->frameState_m);
	return self->contentScale;
}

std::string Frame::getTitle() const {
	std::lock_guard lock(self->frameState_m);
	return self->title;
}

bool Frame::isDecorated() const {
	std::lock_guard lock(self->frameState_m);
	return self->decorated;
}

bool Frame::isResizable() const {
	std::lock_guard lock(self->frameState_m);
	return self->resizable;
}

bool Frame::isVisible() const {
	std::lock_guard lock(self->frameState_m);
	return !self->hidden && !self->minimized && self->window;
}

const Monitor& Frame::getCurrentMonitor() const {
	std::lock_guard lock(self->frameState_m);
	return _getCurrentMonitor();
}

// -------------------------------------------------------------------------------------------------

libv::input::KeyState Frame::key(libv::input::Key key) {
	std::lock_guard lock(self->frameState_m);
	return self->pressedKeys.contains(key) ? libv::input::KeyState::pressed : libv::input::KeyState::released;
}

bool Frame::isKeyPressed(libv::input::Key key) {
	std::lock_guard lock(self->frameState_m);
	return self->pressedKeys.contains(key);
}

bool Frame::isKeyReleased(libv::input::Key key) {
	std::lock_guard lock(self->frameState_m);
	return not self->pressedKeys.contains(key);
}

libv::input::KeyState Frame::mouse(libv::input::Mouse key) {
	std::lock_guard lock(self->frameState_m);
	return self->pressedMouseButtons.contains(key) ? libv::input::KeyState::pressed : libv::input::KeyState::released;
}

bool Frame::isMousePressed(libv::input::Mouse key) {
	std::lock_guard lock(self->frameState_m);
	return self->pressedMouseButtons.contains(key);
}

bool Frame::isMouseReleased(libv::input::Mouse key) {
	std::lock_guard lock(self->frameState_m);
	return not self->pressedMouseButtons.contains(key);
}

libv::vec2d Frame::getMousePosition() {
	std::lock_guard lock(self->frameState_m);
	return self->mousePosition;
}

libv::vec2d Frame::getScrollPosition() {
	std::lock_guard lock(self->frameState_m);
	return self->scrollPosition;
}

// -------------------------------------------------------------------------------------------------

std::string Frame::getClipboardString() {
	std::string result;
	self->core->execute([&result] {
		const char* text = glfwGetClipboardString(nullptr);
		if (text != nullptr)
			result = text;
	});
	return result;
}

void Frame::setClipboardString(const std::string& string) {
	self->core->execute([&string] {
		glfwSetClipboardString(nullptr, string.c_str());
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
