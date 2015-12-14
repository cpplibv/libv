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
//TODO P3: provide async and sync version of setters that require context change like setDecoration

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
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	if (window)
		glfwSetWindowShouldClose(window, true);
}

void Frame::closeForce() {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
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

void Frame::showImpl() {
	assert(std::this_thread::get_id() == context.getID());
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);

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
}

void Frame::hideImpl() {
	assert(std::this_thread::get_id() == context.getID());
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);

	if (window)
		coreExec(std::bind(glfwHideWindow, window));
	hidden = true;
}

void Frame::restoreImpl() {
	assert(std::this_thread::get_id() == context.getID());
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);

	if (window)
		coreExec(std::bind(glfwRestoreWindow, window));
	minimalized = false;
}

void Frame::minimalizeImpl() {
	assert(std::this_thread::get_id() == context.getID());
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);

	if (window)
		coreExec(std::bind(glfwIconifyWindow, window));
	minimalized = true;
}

// -------------------------------------------------------------------------------------------------

void Frame::show() {
	VLOG_INFO(libv::ui::log(), "Show frame [%s] synchronously", title);
	context.executeSync(std::bind(&Frame::showImpl, this));
}

void Frame::showAsync() {
	VLOG_INFO(libv::ui::log(), "Show frame [%s] asynchronously", title);
	context.executeAsync(std::bind(&Frame::showImpl, this));
}

void Frame::hide() {
	VLOG_INFO(libv::ui::log(), "Hide frame [%s] synchronously", title);
	context.executeSync(std::bind(&Frame::hideImpl, this));
}

void Frame::hideAsync() {
	VLOG_INFO(libv::ui::log(), "Hide frame [%s] asynchronously", title);
	context.executeAsync(std::bind(&Frame::hideImpl, this));
}

void Frame::minimalize() {
	VLOG_INFO(libv::ui::log(), "Minimalize frame [%s] synchronously", title);
	context.executeSync(std::bind(&Frame::minimalizeImpl, this));
}

void Frame::minimalizeAsync() {
	VLOG_INFO(libv::ui::log(), "Minimalize frame [%s] asynchronously", title);
	context.executeAsync(std::bind(&Frame::minimalizeImpl, this));
}

void Frame::restore() {
	VLOG_INFO(libv::ui::log(), "Restore frame [%s] synchronously", title);
	context.executeSync(std::bind(&Frame::restoreImpl, this));
}

void Frame::restoreAsync() {
	VLOG_INFO(libv::ui::log(), "Restore frame [%s] asynchronously", title);
	context.executeAsync(std::bind(&Frame::restoreImpl, this));
}

// -------------------------------------------------------------------------------------------------

void Frame::setOpenGLVersion(int major, int minor) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, major, minor] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->openGLVersionMajor = major;
				this->openGLVersionMinor = minor;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLProfile(TypeOpenGLProfile profile) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, profile] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->openGLProfile = profile;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLSamples(TypeOpenGLSamples samples) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, samples] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->openGLSamples = samples;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLRefreshRate(int rate) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, rate] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->openGLRefreshRate = rate;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setCloseOperation(const Frame::TypeCloseOperation& operation) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	defaultCloseOperation = operation;
}

void Frame::setDecoration(bool decorated) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, decorated] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->decorated = decorated;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setDisplayMode(const TypeDisplayMode& mode) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, mode] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->displayMode = mode;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setPosition(int x, int y) {
	setPosition(ivec2(x, y));
}

void Frame::setPosition(const ivec2& newpos) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, newpos] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->pos = newpos;
		if (window)
				coreExec(std::bind(glfwSetWindowPos, window, pos.x, pos.y));
		});
}

void Frame::setResizeable(bool resizable) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, resizable] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->resizable = resizable;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setSize(int x, int y) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, x, y] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
		this->size = ivec3(x, y, this->size.z);
		if (window)
				coreExec(std::bind(glfwSetWindowSize, window, size.x, pos.y));
		});
}

void Frame::setTitle(const std::string& title) {
	VLOG_TRACE(libv::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, title] {
		VLOG_TRACE(libv::ui::log(), __PRETTY_FUNCTION__);
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
	VLOG_DEBUG(libv::ui::log(), "Frame init");
	context.executeAsync(std::bind(&Frame::loop, this));
}

void Frame::loop() {
//	VLOG_DEBUG(libv::ui::log(), "Frame loop");
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
	VLOG_DEBUG(libv::ui::log(), "Frame terminate ");
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