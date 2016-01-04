// File: Frame.cpp, Created on 2013. szeptember 27. 16:00, Author: Vader

// hpp
#include <libv/ui/frame/frame.hpp>
// ext
#include <cppformat/format.h>
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
	if (forcedClose) { // If we are forced to close
		onClose.fire(this);
		return true;
	}

	if (!window) // If there is still no window
		return false;

	bool shouldClose = glfwWindowShouldClose(window);
	if (shouldClose) { // Fire on close event which can change the outcome
		shouldClose = onClose.fire(this);
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

bool Frame::isRenderSkipable() {
	return hidden || minimized;
}

bool Frame::isUpdateSkipable() {
	return false;
}

// Frame container linkage -------------------------------------------------------------------------

void Frame::frameBuild() {
	this->build(renderer);
}

void Frame::frameDestroy() {
	this->destroy(renderer);
}

void Frame::frameInvalidate() {
	this->invalidate();
}

void Frame::frameRender() {
	if (isRenderSkipable() || !window)
		return;
	if (isInvalid())
		frameBuild();

	glClearColor(0.236f, 0.311f, 0.311f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, getDisplaySize().x, getDisplaySize().y);
	this->render(renderer);
	glfwSwapBuffers(window);
}

void Frame::frameUpdate() {
	if (isUpdateSkipable())
		return;
	this->update();
}

// Frame Loop --------------------------------------------------------------------------------------

void Frame::loopInit() {
	LIBV_UI_FRAME_DEBUG("Frame init");
	context.executeAsync(std::bind(&Frame::loop, this));
}

void Frame::loop() {
	//	LIBV_UI_FRAME_DEBUG("Frame loop");
	distributeEvents();

	if (isFrameShouldClose()) {
		loopTerminate();
		return;
	}

	frameUpdate();
	frameRender();

	context.executeAsync(std::bind(&Frame::loop, this));
}

void Frame::loopTerminate() {
	LIBV_UI_FRAME_DEBUG("Frame terminate");
	frameDestroy();

	cmdFrameDestroy();
	context.stop();

	onClosed.fire(this);
}

// -------------------------------------------------------------------------------------------------

void Frame::show() {
	context.executeAsync([this] {
		LIBV_UI_FRAME_TRACE("Show frame [%s]", title);
		if (!window) {
			coreExec(std::bind(&Frame::cmdCoreCreate, this));
			if (window) {
				glfwMakeContextCurrent(window);
						context.executeAsync(std::bind(&Frame::loopInit, this));
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
				minimized = false;
		});
}

void Frame::minimize() {
	context.executeAsync([this] {
		LIBV_UI_FRAME_TRACE("Minimize frame [%s]", title);
		if (window)
				coreExec(std::bind(glfwIconifyWindow, window));
				minimized = true;
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
		setDisplaySize(ivec3(newsize, getDisplaySize().z));
		if (window)
				coreExec(std::bind(glfwSetWindowSize, window, getDisplaySize().x, pos.y));
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
	return getDisplaySize();
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

const Monitor* Frame::getCurrentMonitor() const {
	return Monitor::getMonitorAt(pos + getDisplaySize().xy() / 2);
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

// -------------------------------------------------------------------------------------------------

Frame::Frame(const std::string& title, unsigned int width, unsigned int height) :
	onClose(AccumulatorLogicalAnd<bool>::get()),
	context(fmt::sprintf("Frame - %s", title)),
	title(title) {
	setDisplaySize(ivec3(width, height, 2000));
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