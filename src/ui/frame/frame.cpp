// File: Frame.cpp, Created on 2013. szeptember 27. 16:00, Author: Vader

// hpp
#include <libv/ui/frame/frame.hpp>
// ext
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/gl/gl.hpp>
// pro
#include <libv/ui/log.hpp>
#include <libv/timer.hpp>
#include "core.hpp"

//TODO P2: review size handling (in container too)

namespace libv {
namespace ui {

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

const Frame::TypeCloseOperation Frame::ON_CLOSE_DEFAULT_EXIT = 0;
const Frame::TypeCloseOperation Frame::ON_CLOSE_EXIT = 1;
const Frame::TypeCloseOperation Frame::ON_CLOSE_HIDE = 2;
const Frame::TypeCloseOperation Frame::ON_CLOSE_DO_NOTHING = 3;
const Frame::TypeCloseOperation Frame::ON_CLOSE_DISPOSE = 4;

// -------------------------------------------------------------------------------------------------

void Frame::closeDefault() {
	LIBV_UI_FRAME_TRACE("Close default frame [%s]", title);
	if (window)
		glfwSetWindowShouldClose(window, true);
}

void Frame::closeForce() {
	LIBV_UI_FRAME_TRACE("Close force frame [%s]", title);
	forcedClose = true;
}

void Frame::join() {
	context.join();
}

bool Frame::isFrameShouldClose() {
	//ON_CLOSE_DEFAULT_EXIT - Close this, default close other frames
	//ON_CLOSE_EXIT         - Close every frame
	//ON_CLOSE_HIDE         - Hide frame
	//ON_CLOSE_DO_NOTHING   - No operation
	//ON_CLOSE_DISPOSE      - Close the frame
	if (forcedClose) { //In case of forced close return true
		onClose.fire(this);
		return true;
	}

	if (!window)
		return false;

	bool shouldClose = glfwWindowShouldClose(window);
	if (shouldClose) {
		shouldClose = onClose.fire(this);
	}
	switch (defaultCloseOperation) {
	case ON_CLOSE_DEFAULT_EXIT:
		if (shouldClose) {
			closeAllDefault();
		}
		break;
	case ON_CLOSE_EXIT:
		if (shouldClose) {
			closeAllForce(); //forced
		}
		break;
	case ON_CLOSE_HIDE:
		if (shouldClose) {
			glfwSetWindowShouldClose(window, false);
			hide();
		}
		shouldClose = false;
		break;
	case ON_CLOSE_DO_NOTHING:
		if (shouldClose) {
			glfwSetWindowShouldClose(window, false);
			shouldClose = false;
		}
		break;
	case ON_CLOSE_DISPOSE:
	default: break;
	}
	return shouldClose;
}

// -------------------------------------------------------------------------------------------------

bool Frame::isRenderSkipable() {
	return hidden || minimalized;
}

bool Frame::isUpdateSkipable() {
	return false;
}

// -------------------------------------------------------------------------------------------------

void Frame::baseBuild() {
	timerBuild.reset();
	build(renderer);
	timerBuild.time();
	validate();
}

void Frame::baseDestroy() {
	timerDestroy.reset();
	destroy(renderer);
	timerDestroy.time();
}

void Frame::baseInvalidate() {
	invalidate();
}

void Frame::baseRender() {
	if (isRenderSkipable() || !window)
		return;
	if (isInvalid())
		baseBuild();

	timerRender.reset();
	render(renderer);
	timerRender.time();

	timerSwap.reset();
	glfwSwapBuffers(window);
	timerSwap.time();
}

void Frame::baseUpdate() {
	if (isUpdateSkipable())
		return;

	timerUpdate.reset();
	update();
	timerUpdate.time();
}

// -------------------------------------------------------------------------------------------------

void Frame::show() {
	context.executeAsync([this] {
		LIBV_UI_FRAME_TRACE("Show frame [%s]", title);
		if (!window) {
			coreExec(std::bind(&Frame::cmdCoreCreate, this));
			if (window) {
				glfwMakeContextCurrent(window);
						context.executeAsync(std::bind(&Frame::init, this));
			}
		}
		if (window)
				coreExec(std::bind(glfwShowWindow, window));
				hidden = false;
		});
}

void Frame::hide() {
	context.executeAsync([this] {
		LIBV_UI_FRAME_TRACE("Hide frame [%s]", title);
		if (window)
				coreExec(std::bind(glfwHideWindow, window));
				hidden = true;
		});
}

void Frame::restore() {
	context.executeAsync([this] {
		LIBV_UI_FRAME_TRACE("Restore frame [%s]", title);
		if (window)
				coreExec(std::bind(glfwRestoreWindow, window));
				minimalized = false;
		});
}

void Frame::minimalize() {
	context.executeAsync([this] {
		LIBV_UI_FRAME_TRACE("Minimalize frame [%s]", title);
		if (window)
				coreExec(std::bind(glfwIconifyWindow, window));
				minimalized = true;
		});
}

// -------------------------------------------------------------------------------------------------

void Frame::setOpenGLVersion(int major, int minor) {
	context.executeAsync([this, major, minor] {
		LIBV_UI_FRAME_TRACE("Set frame OpenGLVersion of [%s] to [%d.%d]", title, major, minor);
		this->openGLVersionMajor = major;
				this->openGLVersionMinor = minor;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLProfile(TypeOpenGLProfile profile) {
	context.executeAsync([this, profile] {
		LIBV_UI_FRAME_TRACE("Set frame OpenGLProfile of [%s] to [%d]", title, profile);
		this->openGLProfile = profile;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLSamples(TypeOpenGLSamples samples) {
	context.executeAsync([this, samples] {
		LIBV_UI_FRAME_TRACE("Set frame OpenGLSamples of [%s] to [%d]", title, samples);
		this->openGLSamples = samples;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLRefreshRate(int rate) {
	context.executeAsync([this, rate] {
		LIBV_UI_FRAME_TRACE("Set frame OpenGLRefreshRate of [%s] to [%d]", title, rate);
		this->openGLRefreshRate = rate;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setCloseOperation(const Frame::TypeCloseOperation& operation) {
	LIBV_UI_FRAME_TRACE("Set frame CloseOperation of [%s] to [%d]", title, operation);
	defaultCloseOperation = operation;
}

void Frame::setDecoration(bool decorated) {
	context.executeAsync([this, decorated] {
		LIBV_UI_FRAME_TRACE("Set frame Decoration of [%s] to [%d]", title, decorated);
		this->decorated = decorated;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setDisplayMode(const TypeDisplayMode& mode) {
	context.executeAsync([this, mode] {
		LIBV_UI_FRAME_TRACE("Set frame DisplayMode of [%s] to [%d]", title, mode);
		this->displayMode = mode;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setPosition(int x, int y) {
	setPosition(ivec2(x, y));
}

void Frame::setPosition(const ivec2& newpos) {
	context.executeAsync([this, newpos] {
		LIBV_UI_FRAME_TRACE("Set frame Position of [%s] to [%d, %d]", title, newpos.x, newpos.y);
		this->pos = newpos;
		if (window)
				coreExec(std::bind(glfwSetWindowPos, window, pos.x, pos.y));
		});
}

void Frame::setResizable(bool resizable) {
	context.executeAsync([this, resizable] {
		LIBV_UI_FRAME_TRACE("Set frame Resizable of [%s] to [%d]", title, resizable);
		this->resizable = resizable;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setSize(int x, int y) {
	setSize(ivec2(x, y));
}

void Frame::setSize(const ivec2& newsize) {
	context.executeAsync([this, newsize] {
		LIBV_UI_FRAME_TRACE("Set frame Size of [%s] to [%d, %d]", title, newsize.x, newsize.y);
		this->size = ivec3(newsize, this->size.z);
		if (window)
				coreExec(std::bind(glfwSetWindowSize, window, size.x, pos.y));
		});
}

void Frame::setTitle(const std::string& title) {
	context.executeAsync([this, title] {
		LIBV_UI_FRAME_TRACE("Set frame Title of [%s] to [%s]", this->title, title);
		this->title = title;
		if (window)
				coreExec([this, title] {
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

ivec3 Frame::getSize() const {
	return size;
}

std::string Frame::getTitle() const {
	return title;
}

bool Frame::isDecorated() const {
	return decorated;
}

bool Frame::isVisible() const {
	return !hidden && !minimalized && window;
}

const Monitor* Frame::getCurrentMonitor() const {
	return Monitor::getMonitorAt(pos + size.xy() / 2);
}

// -------------------------------------------------------------------------------------------------

void Frame::cmdFrameRecreate() {
	assert(window);

	glfwMakeContextCurrent(nullptr);
	coreExec(std::bind(&Frame::cmdCoreRecreate, this));
	if (window) {
		glfwMakeContextCurrent(window);
	}
}

void Frame::cmdFrameDestroy() {
	glfwMakeContextCurrent(nullptr);
	coreExec(std::bind(&Frame::cmdCoreDestroy, this));
}

// Frame Loop --------------------------------------------------------------------------------------

void Frame::init() {
	LIBV_UI_FRAME_DEBUG("Frame init");
	context.executeAsync(std::bind(&Frame::loop, this));
}

void Frame::loop() {
	//	LIBV_UI_FRAME_DEBUG("Frame loop");
	timerOffLoop.time();
	timerLoop.reset();

	timerEvent.reset();
	distributeEvents();
	timerEvent.time();

	if (isFrameShouldClose()) {
		term();
		return;
	}

	baseUpdate();
	baseRender();

	context.executeAsync([this] {
		loop();
	});
	timerLoop.time();
	timerOffLoop.reset();
}

void Frame::term() {
	LIBV_UI_FRAME_DEBUG("Frame terminate");
	baseDestroy();

	context.executeSync(std::bind(&Frame::cmdFrameDestroy, this));
	context.stop();

	onClosed.fire(this);
}

// -------------------------------------------------------------------------------------------------

Frame::Frame(const std::string& title, unsigned int width, unsigned int height) :
	onClose(AccumulatorLogicalAnd<bool>::get()),
	context(libv::format("Frame - %s", title)),
	title(title) {
	size = ivec3(width, height, 2000);
	registerFrame(this);
	initEvents();
}

Frame::Frame(unsigned int width, unsigned int height) :
	Frame(DEFAULT_FRAME_NAME, width, height) { }

Frame::~Frame() {
	closeForce();
	join();
	unregisterFrame(this);
}

} //namespace ui
} //namespace libv