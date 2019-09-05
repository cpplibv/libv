// File: Frame.cpp, Created on 2013. szeptember 27. 16:00, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <fmt/format.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/math/fixed_point.hpp>
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

	initEventQueues();
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

bool Frame::isRefreshSkipable() {
	return self->hidden || self->minimized;
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
		if (self->window) {
			self->core.exec(std::bind(glfwHideWindow, self->window));
			self->hidden = true;
		}
	});
}

void Frame::restore() {
	self->context.executeAsync([this] {
		log_frame.trace("Restore frame {}", self->title);
		if (self->window) {
			self->core.exec(std::bind(glfwRestoreWindow, self->window));
			self->minimized = false;
		}
	});
}

void Frame::minimize() {
	self->context.executeAsync([this] {
		log_frame.trace("Minimize frame {}", self->title);
		if (self->window) {
			self->core.exec(std::bind(glfwIconifyWindow, self->window));
			self->minimized = true;
		}
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
			cmdFrameRecreate();
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

void Frame::setResizable(bool resizable) {
	self->context.executeAsync([this, resizable] {
		log_frame.trace("Set frame Resizable of {} to {}", self->title, resizable);
		self->resizable = resizable;
		if (self->window)
			cmdFrameRecreate();
	});
}

void Frame::setSize(int x, int y) {
	setSize(libv::vec2i(x, y));
}

void Frame::setSize(libv::vec2i newsize) {
	self->context.executeAsync([this, newsize] {
		log_frame.trace("Set frame Size of {} to {}, {}", self->title, newsize.x, newsize.y);
		self->size = newsize;
		if (self->window)
			self->core.exec(std::bind(glfwSetWindowSize, self->window, self->size.x, self->size.y));
	});
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

libv::vec2i Frame::getSize() const {
	return self->size;
}

const std::string& Frame::getTitle() const {
	return self->title;
}

bool Frame::isDecorated() const {
	return self->decorated;
}

bool Frame::isVisible() const {
	return !self->hidden && !self->minimized && self->window;
}

const Monitor& Frame::getCurrentMonitor() const {
	return Monitor::getMonitorAt(self->position + self->size / 2);
}

// -------------------------------------------------------------------------------------------------

libv::input::KeyState Frame::getKey(libv::input::Key key) {
	if (key == libv::input::Key::Unknown)
		return libv::input::KeyState::released;

	if (to_value(key) >= to_value(libv::input::Key::Last))
		return libv::input::KeyState::released;

	return self->keyStates[to_value(key)].load();
}

bool Frame::isKeyPressed(libv::input::Key key) {
	return getKey(key) == libv::input::KeyState::pressed;
}

bool Frame::isKeyReleased(libv::input::Key key) {
	return getKey(key) == libv::input::KeyState::released;
}

libv::input::KeyState Frame::getMouse(libv::input::Mouse key) {
//	if (key == Mouse::Unknown)
//		return KeyState::released;
	if (to_value(key) >= to_value(libv::input::Mouse::Last))
		return libv::input::KeyState::released;
	return self->mouseStates[to_value(key)].load();
}

bool Frame::isMousePressed(libv::input::Mouse key) {
	return getMouse(key) == libv::input::KeyState::pressed;
}

bool Frame::isMouseReleased(libv::input::Mouse key) {
	return getMouse(key) == libv::input::KeyState::released;
}

libv::vec2d Frame::getMousePosition() {
	const auto raw = self->mousePosition.load();

	// The value stored in atomic mousePosition is coded as x:s24.8 y:s24.8
	auto x = static_cast<uint32_t>((raw & 0xFFFFFFFF00000000) >> 32);
    auto y = static_cast<uint32_t>((raw & 0x00000000FFFFFFFF));

	return {convert_from_s24_8<double>(x), convert_from_s24_8<double>(y)};
}

libv::vec2d Frame::getScrollPosition() {
	const auto raw = self->scrollPosition.load();

	// The value stored in atomic wheelPosition is coded as x:s24.8 y:s24.8
	auto x = static_cast<uint32_t>((raw & 0xFFFFFFFF00000000) >> 32);
    auto y = static_cast<uint32_t>((raw & 0x00000000FFFFFFFF));

	return {convert_from_s24_8<double>(x), convert_from_s24_8<double>(y)};
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

} // namespace frame
} // namespace libv
