// File: Frame.cpp, Created on 2013. szeptember 27. 16:00, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <fmt/format.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/math/fixed_point.hpp>
// pro
#include <libv/frame/log.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

constexpr char DEFAULT_FRAME_NAME[] = "";

// -------------------------------------------------------------------------------------------------

const Frame::TypeDisplayMode Frame::DISPLAY_MODE_WINDOWED = 0;
const Frame::TypeDisplayMode Frame::DISPLAY_MODE_BORDERLESS = 1;
const Frame::TypeDisplayMode Frame::DISPLAY_MODE_FULLSCREEN = 2;

const Frame::TypeOpenGLProfile Frame::OPENGL_PROFILE_ANY = GLFW_OPENGL_ANY_PROFILE;
const Frame::TypeOpenGLProfile Frame::OPENGL_PROFILE_COMPAT = GLFW_OPENGL_COMPAT_PROFILE;
const Frame::TypeOpenGLProfile Frame::OPENGL_PROFILE_CORE = GLFW_OPENGL_CORE_PROFILE;

const Frame::TypeOpenGLRefreshRate Frame::REFRESH_RATE_DONT_CARE = GLFW_DONT_CARE;

const Frame::TypeOpenGLSamples Frame::SAMPLES_DONT_CARE = GLFW_DONT_CARE;

// -------------------------------------------------------------------------------------------------

void Frame::closeDefault() {
	log_frame.trace("Close default frame {}", title);
	if (window)
		glfwSetWindowShouldClose(window, true);
}

void Frame::closeForce() {
	log_frame.trace("Close force frame {}", title);
	forcedClose = true;
}

void Frame::join() {
	context.join();
}

bool Frame::isFrameShouldClose() {
	if (forcedClose) { // If we are forced to close
		onCloseRequest.fire(EventCloseRequest());
		return true;
	}

	if (!window) // If there is still no window
		return false;

	bool shouldClose = glfwWindowShouldClose(window);
	if (shouldClose) { // Fire on close event which can change the outcome
		EventCloseRequest ecr;
		onCloseRequest.fire(ecr);
		shouldClose = !ecr.isAborted();
	}

	if (shouldClose) { // On close let DCO handle
		switch (defaultCloseOperation) {
		case ON_CLOSE_DISPOSE:
			return true;
		case ON_CLOSE_DO_NOTHING:
			glfwSetWindowShouldClose(window, false);
			return false;
		case ON_CLOSE_HIDE:
			glfwSetWindowShouldClose(window, false);
			hide();
			return false;
		case ON_CLOSE_MINIMIZE:
			glfwSetWindowShouldClose(window, false);
			minimize();
			return false;
		}
	}
	return shouldClose;
}

// -------------------------------------------------------------------------------------------------

bool Frame::isRefreshSkipable() {
	return hidden || minimized;
}

// -------------------------------------------------------------------------------------------------

void Frame::show() {
	context.executeAsync([this] {
		log_frame.trace("Show frame {}", title);
		if (!window) {
			cmdFrameCreate();
		}
		if (window) {
			core.exec(std::bind(glfwShowWindow, window));
			hidden = false;
		}
	});
}

void Frame::hide() {
	context.executeAsync([this] {
		log_frame.trace("Hide frame {}", title);
		if (window) {
			core.exec(std::bind(glfwHideWindow, window));
			hidden = true;
		}
	});
}

void Frame::restore() {
	context.executeAsync([this] {
		log_frame.trace("Restore frame {}", title);
		if (window) {
			core.exec(std::bind(glfwRestoreWindow, window));
			minimized = false;
		}
	});
}

void Frame::minimize() {
	context.executeAsync([this] {
		log_frame.trace("Minimize frame {}", title);
		if (window) {
			core.exec(std::bind(glfwIconifyWindow, window));
			minimized = true;
		}
	});
}

// -------------------------------------------------------------------------------------------------

void Frame::setOpenGLVersion(int major, int minor) {
	context.executeAsync([this, major, minor] {
		log_frame.trace("Set frame OpenGLVersion of {} to {}.{}", title, major, minor);
		this->openGLVersionMajor = major;
		this->openGLVersionMinor = minor;
		if (window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLProfile(TypeOpenGLProfile profile) {
	context.executeAsync([this, profile] {
		log_frame.trace("Set frame OpenGLProfile of {} to {}", title, profile);
		this->openGLProfile = profile;
		if (window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLSamples(TypeOpenGLSamples samples) {
	context.executeAsync([this, samples] {
		log_frame.trace("Set frame OpenGLSamples of {} to {}", title, samples);
		this->openGLSamples = samples;
		if (window)
			cmdFrameRecreate();
	});
}

void Frame::setOpenGLRefreshRate(int rate) {
	context.executeAsync([this, rate] {
		log_frame.trace("Set frame OpenGLRefreshRate of {} to {}", title, rate);
		this->openGLRefreshRate = rate;
		if (window)
			cmdFrameRecreate();
	});
}

void Frame::setCloseOperation(const Frame::TypeCloseOperation& operation) {
	log_frame.trace("Set frame CloseOperation of {} to {}", title, operation);
	defaultCloseOperation = operation;
}

void Frame::setDecoration(bool decorated) {
	context.executeAsync([this, decorated] {
		log_frame.trace("Set frame Decoration of {} to {}", title, decorated);
		this->decorated = decorated;
		if (window)
			cmdFrameRecreate();
	});
}

void Frame::setDisplayMode(const TypeDisplayMode& mode) {
	context.executeAsync([this, mode] {
		log_frame.trace("Set frame DisplayMode of {} to {}", title, mode);
		this->displayMode = mode;
		if (window)
			cmdFrameRecreate();
	});
}

void Frame::setPosition(int x, int y) {
	setPosition(vec2i(x, y));
}

void Frame::setPosition(vec2i newpos) {
	context.executeAsync([this, newpos] {
		log_frame.trace("Set frame Position of {} to {}, {}", title, newpos.x, newpos.y);
		this->position = newpos;
		if (window)
			core.exec(std::bind(glfwSetWindowPos, window, position.x, position.y));
	});
}

void Frame::setPosition(FramePosition pos) {
	switch (pos) {
	case POSITION_CENTER_CURRENT_MONITOR:
		context.executeAsync([this] {
			auto& monitor = getCurrentMonitor();
			auto newpos = monitor.position + monitor.currentVideoMode.size / 2 - size / 2;
			log_frame.trace("Set frame Position of {} to {}, {} as center of current monitor", title, newpos.x, newpos.y);
			this->position = newpos;
			if (window)
				core.exec(std::bind(glfwSetWindowPos, window, position.x, position.y));
		});
		break;
	case POSITION_CENTER_PRIMARY_MONITOR:
		context.executeAsync([this] {
			auto& monitor = Monitor::getPrimaryMonitor();
			auto newpos = monitor.position + monitor.currentVideoMode.size / 2 - size / 2;
			log_frame.trace("Set frame Position of {} to {}, {} as center of primary monitor", title, newpos.x, newpos.y);
			this->position = newpos;
			if (window)
				core.exec(std::bind(glfwSetWindowPos, window, position.x, position.y));
		});
		break;
	}
}

void Frame::setResizable(bool resizable) {
	context.executeAsync([this, resizable] {
		log_frame.trace("Set frame Resizable of {} to {}", title, resizable);
		this->resizable = resizable;
		if (window)
			cmdFrameRecreate();
	});
}

void Frame::setSize(int x, int y) {
	setSize(vec2i(x, y));
}

void Frame::setSize(vec2i newsize) {
	context.executeAsync([this, newsize] {
		log_frame.trace("Set frame Size of {} to {}, {}", title, newsize.x, newsize.y);
		size = newsize;
		if (window)
			core.exec(std::bind(glfwSetWindowSize, window, size.x, position.y));
	});
}

void Frame::setTitle(const std::string& title) {
	context.executeAsync([this, title] {
		log_frame.trace("Set frame Title of {} to {}", this->title, title);
		this->title = title;
		if (window)
			core.exec([this, title] {
				glfwSetWindowTitle(window, title.c_str());
			});
	});
}

// Getters -----------------------------------------------------------------------------------------

Frame::TypeCloseOperation Frame::getCloseOperation() const {
	return defaultCloseOperation;
}

Frame::TypeDisplayMode Frame::getDisplayMode() const {
	return displayMode;
}

vec2i Frame::getSize() const {
	return size;
}

std::string Frame::getTitle() const {
	return title;
}

bool Frame::isDecorated() const {
	return decorated;
}

bool Frame::isVisible() const {
	return !hidden && !minimized && window;
}

const Monitor& Frame::getCurrentMonitor() const {
	return Monitor::getMonitorAt(position + size / 2);
}

// -------------------------------------------------------------------------------------------------

KeyState Frame::getKey(Key key) {
	if (key == Key::Unknown)
		return KeyState::released;
	if (to_value(key) >= to_value(Key::Last))
		return KeyState::released;
	return keyStates[to_value(key)].load();
}

bool Frame::isKeyPressed(Key key) {
	return getKey(key) == KeyState::pressed;
}

bool Frame::isKeyReleased(Key key) {
	return getKey(key) == KeyState::released;
}

KeyState Frame::getMouse(Mouse key) {
//	if (key == Mouse::Unknown)
//		return KeyState::released;
	if (to_value(key) >= to_value(Mouse::Last))
		return KeyState::released;
	return mouseStates[to_value(key)].load();
}

bool Frame::isMousePressed(Mouse key) {
	return getMouse(key) == KeyState::pressed;
}

bool Frame::isMouseReleased(Mouse key) {
	return getMouse(key) == KeyState::released;
}

vec2d Frame::getMousePosition() {
	const auto raw = mousePosition.load();

	// The value stored in atomic mousePosition is coded as x:s24.8 y:s24.8
	auto x = static_cast<uint32_t>((raw & 0xFFFFFFFF00000000) >> 32);
    auto y = static_cast<uint32_t>((raw & 0x00000000FFFFFFFF));

	return {convert_from_s_24_8<double>(x), convert_from_s_24_8<double>(y)};
}

vec2d Frame::getScrollPosition() {
	const auto raw = scrollPosition.load();

	// The value stored in atomic wheelPosition is coded as x:s24.8 y:s24.8
	auto x = static_cast<uint32_t>((raw & 0xFFFFFFFF00000000) >> 32);
    auto y = static_cast<uint32_t>((raw & 0x00000000FFFFFFFF));

	return {convert_from_s_24_8<double>(x), convert_from_s_24_8<double>(y)};
}

// -------------------------------------------------------------------------------------------------

void Frame::cmdFrameCreate() {
	core.exec(std::bind(&Frame::cmdCoreCreate, this));
	if (window) {
		glfwMakeContextCurrent(window);
		context.executeAsync(std::bind(&Frame::loopInit, this));
		contextInit();
	}
}

void Frame::cmdFrameRecreate() {
	assert(window);

	glfwMakeContextCurrent(nullptr);
	core.exec(std::bind(&Frame::cmdCoreRecreate, this));
	if (window) {
		glfwMakeContextCurrent(window);
		contextInit();
	}
}

void Frame::cmdFrameDestroy() {
	contextTerminate();
	glfwMakeContextCurrent(nullptr);
	core.exec(std::bind(&Frame::cmdCoreDestroy, this));
}

// Frame Loop --------------------------------------------------------------------------------------

void Frame::loopInit() {
	log_frame.debug("Frame entering loop");

	context.executeAsync(std::bind(&Frame::loop, this));
}

void Frame::loop() {
	distributeEvents();

	if (isFrameShouldClose()) {
		loopTerminate();
	} else {
		if (!isRefreshSkipable() && window) {
			onContextRefresh.fire(EventContextRefresh());

			glfwSwapBuffers(window);
		}

		context.executeAsync(std::bind(&Frame::loop, this));
	}
}

void Frame::loopTerminate() {
	log_frame.debug("Frame exiting loop");
	cmdFrameDestroy();
	context.stop();
}

void Frame::contextInit() {
	log_frame.debug("Frame context initializing");
	onContextInitialization.fire(EventContextInitialization());
}

void Frame::contextTerminate() {
	log_frame.debug("Frame context terminating");
	onContextTerminate.fire(EventContextTerminate());
}

// -------------------------------------------------------------------------------------------------

Frame::Frame(const std::string& title, int32_t width, int32_t height) :
	context(fmt::format("Thread of {}", title)),
	title(title) {
	size = vec2i(width, height);

	initEventQueues();
}

Frame::Frame(int32_t width, int32_t height) :
	Frame(DEFAULT_FRAME_NAME, width, height) { }

Frame::~Frame() {
	closeForce();
	join();
}

} // namespace frame
} // namespace libv
