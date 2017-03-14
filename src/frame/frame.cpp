// File: Frame.cpp, Created on 2013. szeptember 27. 16:00, Author: Vader

// hpp
#include <libv/frame/frame.hpp>
// ext
#include <fmt/format.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/fixed.hpp>
// pro
#include <libv/frame/log.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

constexpr const char DEFAULT_FRAME_NAME[] = "";

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
	LIBV_LOG_FRAME_TRACE("Close default frame [%s]", title);
	if (window)
		glfwSetWindowShouldClose(window, true);
}

void Frame::closeForce() {
	LIBV_LOG_FRAME_TRACE("Close force frame [%s]", title);
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
		LIBV_LOG_FRAME_TRACE("Show frame [%s]", title);
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
		LIBV_LOG_FRAME_TRACE("Hide frame [%s]", title);
		if (window) {
			core.exec(std::bind(glfwHideWindow, window));
					hidden = true;
		}
	});
}

void Frame::restore() {
	context.executeAsync([this] {
		LIBV_LOG_FRAME_TRACE("Restore frame [%s]", title);
		if (window) {
			core.exec(std::bind(glfwRestoreWindow, window));
					minimized = false;
		}
	});
}

void Frame::minimize() {
	context.executeAsync([this] {
		LIBV_LOG_FRAME_TRACE("Minimize frame [%s]", title);
		if (window) {
			core.exec(std::bind(glfwIconifyWindow, window));
					minimized = true;
		}
	});
}

// -------------------------------------------------------------------------------------------------

void Frame::setOpenGLVersion(int major, int minor) {
	context.executeAsync([this, major, minor] {
		LIBV_LOG_FRAME_TRACE("Set frame OpenGLVersion of [%s] to [%d.%d]", title, major, minor);
		this->openGLVersionMajor = major;
		this->openGLVersionMinor = minor;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLProfile(TypeOpenGLProfile profile) {
	context.executeAsync([this, profile] {
		LIBV_LOG_FRAME_TRACE("Set frame OpenGLProfile of [%s] to [%d]", title, profile);
		this->openGLProfile = profile;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLSamples(TypeOpenGLSamples samples) {
	context.executeAsync([this, samples] {
		LIBV_LOG_FRAME_TRACE("Set frame OpenGLSamples of [%s] to [%d]", title, samples);
		this->openGLSamples = samples;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLRefreshRate(int rate) {
	context.executeAsync([this, rate] {
		LIBV_LOG_FRAME_TRACE("Set frame OpenGLRefreshRate of [%s] to [%d]", title, rate);
		this->openGLRefreshRate = rate;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setCloseOperation(const Frame::TypeCloseOperation& operation) {
	LIBV_LOG_FRAME_TRACE("Set frame CloseOperation of [%s] to [%d]", title, operation);
	defaultCloseOperation = operation;
}

void Frame::setDecoration(bool decorated) {
	context.executeAsync([this, decorated] {
		LIBV_LOG_FRAME_TRACE("Set frame Decoration of [%s] to [%d]", title, decorated);
		this->decorated = decorated;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setDisplayMode(const TypeDisplayMode& mode) {
	context.executeAsync([this, mode] {
		LIBV_LOG_FRAME_TRACE("Set frame DisplayMode of [%s] to [%d]", title, mode);
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
		LIBV_LOG_FRAME_TRACE("Set frame Position of [%s] to [%d, %d]", title, newpos.x, newpos.y);
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
			LIBV_LOG_FRAME_TRACE("Set frame Position of [%s] to [%d, %d] as center of current monitor", title, newpos.x, newpos.y);
			this->position = newpos;
			if (window)
					core.exec(std::bind(glfwSetWindowPos, window, position.x, position.y));
			});
		break;
	case POSITION_CENTER_PRIMARY_MONITOR:
		context.executeAsync([this] {
			auto& monitor = Monitor::getPrimaryMonitor();
			auto newpos = monitor.position + monitor.currentVideoMode.size / 2 - size / 2;
			LIBV_LOG_FRAME_TRACE("Set frame Position of [%s] to [%d, %d] as center of primary monitor", title, newpos.x, newpos.y);
			this->position = newpos;
			if (window)
					core.exec(std::bind(glfwSetWindowPos, window, position.x, position.y));
			});
		break;
	}
}

void Frame::setResizable(bool resizable) {
	context.executeAsync([this, resizable] {
		LIBV_LOG_FRAME_TRACE("Set frame Resizable of [%s] to [%d]", title, resizable);
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
		LIBV_LOG_FRAME_TRACE("Set frame Size of [%s] to [%d, %d]", title, newsize.x, newsize.y);
		size = newsize;
		if (window)
				core.exec(std::bind(glfwSetWindowSize, window, size.x, position.y));
		});
}

void Frame::setTitle(const std::string& title) {
	context.executeAsync([this, title] {
		LIBV_LOG_FRAME_TRACE("Set frame Title of [%s] to [%s]", this->title, title);
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

vec2f Frame::getMousePosition() {
	const auto raw = mousePosition.load();

	// The value stored in atomic mousePosition is coded as x:s24.8 y:s24.8
	auto x = static_cast<uint32_t>((raw & 0xFFFFFFFF00000000) >> 32);
    auto y = static_cast<uint32_t>((raw & 0x00000000FFFFFFFF));

	return vec2f(convert_from_s_24_8(x), convert_from_s_24_8(y));
}

vec2f Frame::getScrollPosition() {
	const auto raw = scrollPosition.load();

	// The value stored in atomic wheelPosition is coded as x:s24.8 y:s24.8
	auto x = static_cast<uint32_t>((raw & 0xFFFFFFFF00000000) >> 32);
    auto y = static_cast<uint32_t>((raw & 0x00000000FFFFFFFF));

	return vec2f(convert_from_s_24_8(x), convert_from_s_24_8(y));
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
	LIBV_LOG_FRAME_DEBUG("Frame entering loop");

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
	LIBV_LOG_FRAME_DEBUG("Frame exiting loop");
	cmdFrameDestroy();
	context.stop();
}

void Frame::contextInit() {
	onContextInitialization.fire(EventContextInitialization());
}

void Frame::contextTerminate() {
	onContextTerminate.fire(EventContextTerminate());
}

// -------------------------------------------------------------------------------------------------

Frame::Frame(const std::string& title, unsigned int width, unsigned int height) :
	context(fmt::sprintf("Frame - %s", title)),
	title(title) {
	size = vec2i(width, height);

	initEventQueues();
}

Frame::Frame(unsigned int width, unsigned int height) :
	Frame(DEFAULT_FRAME_NAME, width, height) { }

Frame::~Frame() {
	closeForce();
	join();
}

} // namespace frame
} // namespace libv