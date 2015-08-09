// File: Frame.cpp, Created on 2013. szeptember 27. 16:00, Author: Vader

// hpp
#include <vl/ui/frame/frame.hpp>
// ext
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
// vl
#include <vl/semaphore.hpp>
#include <vl/gl/vgl.hpp>
// pro
#include <vl/ui/log.hpp>
#include <vl/timer.hpp>

namespace vl {
namespace ui {

// -------------------------------------------------------------------------------------------------

std::mutex core_m;
std::unique_ptr<vl::WorkerThread> coreContext;
std::set<Frame*> frames;
std::mutex activeFrames_m;
std::set<Frame*> activeFrames;
vl::Semaphore noActiveFrame(true);

void initCore() {
	VLOG_DEBUG(vl::ui::log(), "Initialize Core / GLFW Context");
	coreContext = std::make_unique<vl::WorkerThread>("Core / GLFW");

	coreContext->executeSync([] {
		glfwSetErrorCallback(detail::errorCallbackGLFW);
		if (glfwInit()) {
			VLOG_INFO(vl::ui::log(), "Initialized Core / GLFW Context");
					glfwSetMonitorCallback(glfwMonitorCallback);

					//Simulate GLFW Monitor connections to initialize Monitors
					int numMonitor;
					auto monitors = glfwGetMonitors(&numMonitor);
			for (int i = 0; i < numMonitor; i++) {
				glfwMonitorCallback(monitors[i], GLFW_CONNECTED);
			}

		} else {
			VLOG_ERROR(vl::ui::log(), "Failed to initialize GLFW");
		}
	});
}

void terminateCore() {
	VLOG_DEBUG(vl::ui::log(), "Terminate Core / GLFW Context");

	glfwSetMonitorCallback(nullptr);
	coreContext->executeSync(glfwTerminate);
	coreContext.reset(nullptr);

	VLOG_INFO(vl::ui::log(), "Terminated Core / GLFW Context");
}

void activateFrame(Frame* frame) {
	VLOG_DEBUG(vl::ui::log(), "Activate frame [%s]", frame->getTitle());
	std::lock_guard<std::mutex> lk(activeFrames_m);
	activeFrames.insert(frame);
	noActiveFrame.reset();
}

void deactivateFrame(Frame* frame) {
	VLOG_DEBUG(vl::ui::log(), "Deactivate frame [%s]", frame->getTitle());
	std::lock_guard<std::mutex> lk(activeFrames_m);
	activeFrames.erase(frame);
	if (activeFrames.size() == 0)
		noActiveFrame.raise();
}

void registerFrame(Frame* frame) {
	std::lock_guard<std::mutex> lk(core_m);
	VLOG_DEBUG(vl::ui::log(), "Register frame [%s]", frame->getTitle());
	if (frames.size() == 0)
		initCore();
	frames.insert(frame);
}

void unregisterFrame(Frame* frame) {
	std::lock_guard<std::mutex> lk(core_m);
	VLOG_DEBUG(vl::ui::log(), "Unregister frame [%s]", frame->getTitle());
	frames.erase(frame);
	if (frames.size() == 0)
		terminateCore();
}

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

void Frame::joinAll() {
	VLOG_DEBUG(vl::ui::log(), "Joining every frame");
	noActiveFrame.wait();
}

void Frame::closeAllForce() {
	VLOG_DEBUG(vl::ui::log(), "Forced close every frame");
	std::lock_guard<std::mutex> lk(core_m);
	for (auto frame : frames)
		frame->closeForce();
}

void Frame::closeAllDefault() {
	VLOG_DEBUG(vl::ui::log(), "Default close every frame");
	std::lock_guard<std::mutex> lk(core_m);
	for (auto frame : frames)
		frame->closeDefault();
}

// -------------------------------------------------------------------------------------------------

const Frame::TypeOpenGLProfile Frame::OPENGL_PROFILE_ANY = GLFW_OPENGL_ANY_PROFILE;
const Frame::TypeOpenGLProfile Frame::OPENGL_PROFILE_COMPAT = GLFW_OPENGL_COMPAT_PROFILE;
const Frame::TypeOpenGLProfile Frame::OPENGL_PROFILE_CORE = GLFW_OPENGL_CORE_PROFILE;

const Frame::TypeDisplayMode Frame::DISPLAY_MODE_WINDOWED = 0;
const Frame::TypeDisplayMode Frame::DISPLAY_MODE_BORDERLESS = 1;
const Frame::TypeDisplayMode Frame::DISPLAY_MODE_FULLSCREEN = 2;

const Frame::TypeCloseOperation Frame::ON_CLOSE_DEFAULT_EXIT;
const Frame::TypeCloseOperation Frame::ON_CLOSE_EXIT;
const Frame::TypeCloseOperation Frame::ON_CLOSE_HIDE;
const Frame::TypeCloseOperation Frame::ON_CLOSE_DO_NOTHING;
const Frame::TypeCloseOperation Frame::ON_CLOSE_DISPOSE;

// -------------------------------------------------------------------------------------------------

void Frame::closeDefault() {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	if (window)
		glfwSetWindowShouldClose(window, true);
}

void Frame::closeForce() {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
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

void Frame::build() {
	timerBuild.reset();
	glViewport(0, 0, size.x, size.y);
	if (content) {
		content->build(renderer);
		invalidated = false;
	}
	timerBuild.time();
}

void Frame::destroy() {
	timerDestroy.reset();
	timerDestroy.time();
}

void Frame::invalidate() {
	invalidated = true;
	if (content)
		content->invalidate();
}

void Frame::render() {
	if (!isRenderSkipable() && content && window) {
		if (invalidated)
			build();

		timerRender.reset();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, size.x, size.y);
		renderer.push(glm::ortho(0, 0, size.x, size.y, -1000, 1000));
		content->render(renderer);
		timerRender.time();

		timerSwap.reset();
		glfwSwapBuffers(window);
		timerSwap.time();
	}
}

void Frame::update() {
	timerUpdate.reset();
	if (!isUpdateSkipable() && content) {
		content->update();
	}
	timerUpdate.time();
}

// -------------------------------------------------------------------------------------------------

void Frame::showImpl() {
	assert(std::this_thread::get_id() == context.get_id());
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);

	if (!window) {
		coreContext->executeSync(std::bind(&Frame::cmdCoreCreate, this));
		if (window) {
			glfwMakeContextCurrent(window);
			context.executeAsync(std::bind(&Frame::init, this));
		}
	}
	if (window)
		coreContext->executeSync(std::bind(glfwShowWindow, window));

	hidden = false;
}

void Frame::hideImpl() {
	assert(std::this_thread::get_id() == context.get_id());
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);

	if (window)
		coreContext->executeSync(std::bind(glfwHideWindow, window));
	hidden = true;
}

void Frame::restoreImpl() {
	assert(std::this_thread::get_id() == context.get_id());
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);

	if (window)
		coreContext->executeSync(std::bind(glfwRestoreWindow, window));
	minimalized = false;
}

void Frame::minimalizeImpl() {
	assert(std::this_thread::get_id() == context.get_id());
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);

	if (window)
		coreContext->executeSync(std::bind(glfwIconifyWindow, window));
	minimalized = true;
}

// -------------------------------------------------------------------------------------------------

void Frame::show() {
	VLOG_INFO(vl::ui::log(), "Show frame [%s] synchronously", title);
	context.executeSync(std::bind(&Frame::showImpl, this));
}

void Frame::showAsync() {
	VLOG_INFO(vl::ui::log(), "Show frame [%s] asynchronously", title);
	context.executeAsync(std::bind(&Frame::showImpl, this));
}

void Frame::hide() {
	VLOG_INFO(vl::ui::log(), "Hide frame [%s] synchronously", title);
	context.executeSync(std::bind(&Frame::hideImpl, this));
}

void Frame::hideAsync() {
	VLOG_INFO(vl::ui::log(), "Hide frame [%s] asynchronously", title);
	context.executeAsync(std::bind(&Frame::hideImpl, this));
}

void Frame::minimalize() {
	VLOG_INFO(vl::ui::log(), "Minimalize frame [%s] synchronously", title);
	context.executeSync(std::bind(&Frame::minimalizeImpl, this));
}

void Frame::minimalizeAsync() {
	VLOG_INFO(vl::ui::log(), "Minimalize frame [%s] asynchronously", title);
	context.executeAsync(std::bind(&Frame::minimalizeImpl, this));
}

void Frame::restore() {
	VLOG_INFO(vl::ui::log(), "Restore frame [%s] synchronously", title);
	context.executeSync(std::bind(&Frame::restoreImpl, this));
}

void Frame::restoreAsync() {
	VLOG_INFO(vl::ui::log(), "Restore frame [%s] asynchronously", title);
	context.executeAsync(std::bind(&Frame::restoreImpl, this));
}

// -------------------------------------------------------------------------------------------------

void Frame::setOpenGLVersion(int major, int minor) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, major, minor] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->openGLVersionMajor = major;
				this->openGLVersionMinor = minor;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLProfile(TypeOpenGLProfile profile) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, profile] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->openGLProfile = profile;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLSamples(int samples) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, samples] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->openGLSamples = samples;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setOpenGLRefreshRate(int rate) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, rate] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->openGLRefreshRate = rate;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setContent(ComponentPtr content) {
	this->content = content;
}

void Frame::setCloseOperation(const Frame::TypeCloseOperation& operation) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	defaultCloseOperation = operation;
}

void Frame::setDecoration(bool decorated) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, decorated] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->decorated = decorated;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setDisplayMode(const TypeDisplayMode& mode) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, mode] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->displayMode = mode;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setPosition(int x, int y) {
	setPosition(ivec2(x, y));
}

void Frame::setPosition(const ivec2& newpos) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, newpos] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->pos = newpos;
		if (window)
				coreContext->executeSync(std::bind(glfwSetWindowPos, window, pos.x, pos.y));
		});
}

void Frame::setResizeable(bool resizable) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, resizable] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->resizable = resizable;
		if (window)
				cmdFrameRecreate();
		});
}

void Frame::setSize(int x, int y) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, x, y] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->size = ivec2(x, y);
		if (window)
				coreContext->executeSync(std::bind(glfwSetWindowSize, window, size.x, pos.y));
		});
}

void Frame::setTitle(const std::string& title) {
	VLOG_TRACE(vl::ui::log(), "%s [%s]", __PRETTY_FUNCTION__, title);
	context.executeAsync([this, title] {
		VLOG_TRACE(vl::ui::log(), __PRETTY_FUNCTION__);
		this->title = title;
		if (window)
				coreContext->executeSync(std::bind(glfwSetWindowTitle, window, title.c_str()));
		});
}

// -------------------------------------------------------------------------------------------------

//void Frame::cmdFrameCreate() {
//}

void Frame::cmdFrameRecreate() {
	assert(window);

	glfwMakeContextCurrent(nullptr);
	coreContext->executeSync(std::bind(&Frame::cmdCoreRecreate, this));
	if (window) {
		glfwMakeContextCurrent(window);
	}
}

//void Frame::cmdFrameDestroy() {
//	assert(window);
//}

// -------------------------------------------------------------------------------------------------

void Frame::cmdCoreCreate() {
	VLOG_DEBUG(vl::ui::log(), "Create window for frame [%s]", title);

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_DECORATED, decorated);
	glfwWindowHint(GLFW_VISIBLE, false);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLVersionMinor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //TODO P4: Hint GLFW_OPENGL_FORWARD_COMPAT
	glfwWindowHint(GLFW_OPENGL_PROFILE, openGLProfile);
	glfwWindowHint(GLFW_SAMPLES, openGLSamples);
	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	//glfwWindowHint(GLFW_FOCUSED, ); 
	//glfwWindowHint(GLFW_FLOATING, ); 
	//glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION);
	//TODO P4: Hint GLFW_FOCUSED
	//TODO P4: Hint GLFW_FLOATING
	//TODO P4: Hint GLFW_CONTEXT_ROBUSTNESS
	//TODO P3: Hint max refres rate, GLFW_REFRESH_RATE maximum = GLFW_DONT_CARE
	//TODO P5: context sharing
	glfwWindowHint(GLFW_REFRESH_RATE, openGLRefreshRate);

	if (displayMode == DISPLAY_MODE_FULLSCREEN) {
		VLOG_INFO(vl::ui::log(), "Switching frame [%s] to full screen mode", title);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		//		eventQueWindowSize.fire(EventWindowSize(ivec2(mode->width, mode->height)));
		onWindowSize.fire(EventWindowSize(mode->width, mode->height));
		window = glfwCreateWindow(size.x, size.y, title.c_str(), glfwGetPrimaryMonitor(), shareWindow);
	} else if (displayMode == DISPLAY_MODE_BORDERLESS) {
		VLOG_INFO(vl::ui::log(), "Switching frame [%s] to borderless mode", title);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		size = ivec2(mode->width, mode->height);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		window = glfwCreateWindow(size.x, size.y, title.c_str(), glfwGetPrimaryMonitor(), shareWindow);
	} else if (displayMode == DISPLAY_MODE_WINDOWED) {
		window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, shareWindow);
	}
	if (!window) {
		VLOG_ERROR(vl::ui::log(), "GLFW window creation failed");
		return;
	}
	invalidate();
	registerEventCallbacks(this, window);
	activateFrame(this);

	if (displayMode == DISPLAY_MODE_FULLSCREEN) {
	} else if (displayMode == DISPLAY_MODE_BORDERLESS) {
		glfwSetWindowPos(window, 0, 0);
	} else if (displayMode == DISPLAY_MODE_WINDOWED) {
		glfwSetWindowPos(window, pos.x, pos.y);
	}
}

void Frame::cmdCoreRecreate() {
	VLOG_DEBUG(vl::ui::log(), "Recreate window for frame [%s]", title);
	assert(window && "Requires a valid window");

	shareWindow = window;
	window = nullptr;
	cmdCoreCreate();

	if (!window) {
		VLOG_ERROR(vl::ui::log(), "Rollback to previous window");
		window = shareWindow;
		shareWindow = nullptr;
		return;
	} else {
		VLOG_TRACE(vl::ui::log(), "Destroy previous window");
		glfwDestroyWindow(shareWindow);
		unregisterEventCallbacks(shareWindow);
		shareWindow = nullptr;
	}
	if (!hidden)
		glfwShowWindow(window);
}

void Frame::cmdCoreDestroy() {
	VLOG_DEBUG(vl::ui::log(), "Destroy window for frame [%s]", title);

	if (window) {
		deactivateFrame(this);
		unregisterEventCallbacks(window);
		glfwDestroyWindow(window);
		window = nullptr;
	}
}

void Frame::cmdCoreUpdateDisplayMode() {
	VLOG_DEBUG(vl::ui::log(), "Update display mode for frame [%s]", title);

	VLOG_ERROR(vl::ui::log(), "Not Implemented Yet"); //TODO P5: cmdCoreUpdateDisplayMode
}

// -------------------------------------------------------------------------------------------------

std::map<GLFWwindow*, Frame*> windowHandlers;

template <typename E, CapacitivSignal<const E&> Frame::* que, typename... Args>
void glfwCallback(GLFWwindow* window, Args... args) {
	try {
		(windowHandlers.at(window)->*que).fire(E(args...));
	} catch (const std::out_of_range& e) {
		VLOG_ERROR(vl::ui::log(), "Unhandeled event. No event handler (frame) assigned to this window.");
	}
}

void Frame::registerEventCallbacks(Frame* frame, GLFWwindow* window) {
	windowHandlers[window] = frame;

	glfwSetCharCallback(window, ::vl::ui::glfwCallback<EventChar, &Frame::eventQueChar, unsigned int>);
	glfwSetCharModsCallback(window, ::vl::ui::glfwCallback<EventCharMods, &Frame::eventQueCharMods, unsigned int, int>);
	glfwSetCursorEnterCallback(window, ::vl::ui::glfwCallback<EventCursorEnter, &Frame::eventQueCursorEnter, int>);
	glfwSetCursorPosCallback(window, ::vl::ui::glfwCallback<EventCursorPos, &Frame::eventQueCursorPos, double, double>);
	glfwSetDropCallback(window, ::vl::ui::glfwCallback<EventDrop, &Frame::eventQueDrop, int, const char**>);
	glfwSetFramebufferSizeCallback(window, ::vl::ui::glfwCallback<EventFramebufferSize, &Frame::eventQueFramebufferSize, int, int>);
	glfwSetKeyCallback(window, ::vl::ui::glfwCallback<EventKey, &Frame::eventQueKey, int, int, int, int>);
	glfwSetMouseButtonCallback(window, ::vl::ui::glfwCallback<EventMouseButton, &Frame::eventQueMouseButton, int, int, int>);
	glfwSetScrollCallback(window, ::vl::ui::glfwCallback<EventScroll, &Frame::eventQueScroll, double, double>);
	glfwSetWindowCloseCallback(window, ::vl::ui::glfwCallback<EventWindowClose, &Frame::eventQueWindowClose>);
	glfwSetWindowFocusCallback(window, ::vl::ui::glfwCallback<EventWindowFocus, &Frame::eventQueWindowFocus, int>);
	glfwSetWindowIconifyCallback(window, ::vl::ui::glfwCallback<EventWindowIconify, &Frame::eventQueWindowIconify, int>);
	glfwSetWindowPosCallback(window, ::vl::ui::glfwCallback<EventWindowPos, &Frame::eventQueWindowPos, int, int>);
	glfwSetWindowRefreshCallback(window, ::vl::ui::glfwCallback<EventWindowRefresh, &Frame::eventQueWindowRefresh>);
	glfwSetWindowSizeCallback(window, ::vl::ui::glfwCallback<EventWindowSize, &Frame::eventQueWindowSize, int, int>);
}

void Frame::unregisterEventCallbacks(GLFWwindow * window) {
	glfwSetCharCallback(window, nullptr);
	glfwSetCursorEnterCallback(window, nullptr);
	glfwSetCursorPosCallback(window, nullptr);
	glfwSetFramebufferSizeCallback(window, nullptr);
	glfwSetKeyCallback(window, nullptr);
	glfwSetMouseButtonCallback(window, nullptr);
	glfwSetScrollCallback(window, nullptr);
	glfwSetWindowCloseCallback(window, nullptr);
	glfwSetWindowFocusCallback(window, nullptr);
	glfwSetWindowIconifyCallback(window, nullptr);
	glfwSetWindowPosCallback(window, nullptr);
	glfwSetWindowRefreshCallback(window, nullptr);
	glfwSetWindowSizeCallback(window, nullptr);

	windowHandlers.erase(window);
	//sub for every monitor
	//glfwSetMonitorCallback(nullptr);
}

// -------------------------------------------------------------------------------------------------

void Frame::glfwCallback(const EventChar&) { }

void Frame::glfwCallback(const EventCharMods&) { }

void Frame::glfwCallback(const EventCursorEnter&) { }

void Frame::glfwCallback(const EventCursorPos&) { }

void Frame::glfwCallback(const EventDrop&) { }

void Frame::glfwCallback(const EventFramebufferSize&) {
	//	invalidate();
}

void Frame::glfwCallback(const EventKey&) { }

void Frame::glfwCallback(const EventMouseButton&) { }

void Frame::glfwCallback(const EventScroll&) { }

void Frame::glfwCallback(const EventWindowClose&) { }

void Frame::glfwCallback(const EventWindowFocus&) { }

void Frame::glfwCallback(const EventWindowIconify&) { }

void Frame::glfwCallback(const EventWindowPos& e) {
	pos = e.position;
}

void Frame::glfwCallback(const EventWindowRefresh&) { }

void Frame::glfwCallback(const EventWindowSize& e) {
	size = e.size; //TODO P4: Whenever we set change size we have to change content Property::Size too
	if (content)
		content->set(Property::Size, size);
}

// -------------------------------------------------------------------------------------------------

void Frame::initEvents() {
	eventQueChar.output(onChar);
	eventQueCharMods.output(onCharMods);
	eventQueCursorEnter.output(onCursorEnter);
	eventQueCursorPos.output(onCursorPos);
	eventQueDrop.output(onDrop);
	eventQueFramebufferSize.output(onFramebufferSize);
	eventQueKey.output(onKey);
	eventQueMouseButton.output(onMouseButton);
	eventQueScroll.output(onScroll);
	eventQueWindowClose.output(onWindowClose);
	eventQueWindowFocus.output(onWindowFocus);
	eventQueWindowIconify.output(onWindowIconify);
	eventQueWindowPos.output(onWindowPos);
	eventQueWindowRefresh.output(onWindowRefresh);
	eventQueWindowSize.output(onWindowSize);

	eventQueChar.output(&Frame::glfwCallback, this);
	eventQueCharMods.output(&Frame::glfwCallback, this);
	eventQueCursorEnter.output(&Frame::glfwCallback, this);
	eventQueCursorPos.output(&Frame::glfwCallback, this);
	eventQueDrop.output(&Frame::glfwCallback, this);
	eventQueFramebufferSize.output(&Frame::glfwCallback, this);
	eventQueKey.output(&Frame::glfwCallback, this);
	eventQueMouseButton.output(&Frame::glfwCallback, this);
	eventQueScroll.output(&Frame::glfwCallback, this);
	eventQueWindowClose.output(&Frame::glfwCallback, this);
	eventQueWindowFocus.output(&Frame::glfwCallback, this);
	eventQueWindowIconify.output(&Frame::glfwCallback, this);
	eventQueWindowPos.output(&Frame::glfwCallback, this);
	eventQueWindowRefresh.output(&Frame::glfwCallback, this);
	eventQueWindowSize.output(&Frame::glfwCallback, this);
}

void Frame::distributeEvents() {
	eventQueChar.flush();
	eventQueCharMods.flush();
	eventQueCursorEnter.flush();
	eventQueCursorPos.flush();
	eventQueDrop.flush();
	eventQueFramebufferSize.flush();
	eventQueKey.flush();
	eventQueMouseButton.flush();
	eventQueScroll.flush();
	eventQueWindowClose.flush();
	eventQueWindowFocus.flush();
	eventQueWindowIconify.flush();
	eventQueWindowPos.flush();
	eventQueWindowRefresh.flush();
	eventQueWindowSize.flush();
}

// Getters -----------------------------------------------------------------------------------------

ComponentPtr Frame::getContent() {
	return content;
}

Frame::TypeCloseOperation Frame::getCloseOperation() const {
	return defaultCloseOperation;
}

Frame::TypeDisplayMode Frame::getDisplayMode() const {
	return displayMode;
}

ivec2 Frame::getSize() const {
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
	return Monitor::getMonitorAt(pos + size / 2);
}

// Frame Loop ----------------------------------------------------------

void Frame::init() {
	context.executeAsync(std::bind(&Frame::loop, this));
}

void Frame::loop() {
	timerOffLoop.time();
	timerLoop.reset();

	timerPoll.reset();
	coreContext->executeSync(glfwPollEvents);
	timerPoll.time();
	timerEvent.reset();
	distributeEvents();
	timerEvent.time();

	if (isFrameShouldClose()) {
		term();
		return;
	}

	update();
	render();

	context.executeAsync(std::bind(&Frame::loop, this));
	timerLoop.time();
	timerOffLoop.reset();
}

void Frame::term() {
	VLOG_DEBUG(vl::ui::log(), "Frame terminate ");
	destroy();

	coreContext->executeAsync(std::bind(&Frame::cmdCoreDestroy, this));
	context.terminate();

	onClosed.fire(this);
}

// -------------------------------------------------------------------------------------------------

Frame::Frame(const std::string& title, unsigned int width, unsigned int height) :
	onClose(AccumulatorLogicalAnd<bool>::get()),
	forcedClose(false),
	context(vl::format("Frame - %s", title)),
	window(nullptr),
	size(width, height),
	openGLRefreshRate(GLFW_DONT_CARE),
	decorated(true),
	hidden(true),
	minimalized(false),
	resizable(true),
	invalidated(true),
	title(title),
	content(nullptr) {
	registerFrame(this);
	initEvents();
}

Frame::Frame(unsigned int width, unsigned int height) : Frame("", width, height) { }

Frame::~Frame() {
	closeForce();
	join();
	unregisterFrame(this);
}

} //namespace ui
} //namespace vl

//Char           
//CharMods       
//CursorEnter    
//CursorPos      
//Drop           
//FramebufferSize
//Key            
//MouseButton    
//Scroll         
//WindowClose    
//WindowFocus    
//WindowIconify  
//WindowPos      
//WindowRefresh  
//WindowSize     
//Monitor        

//Char
//CharMods
//CursorEnter
//CursorPos
//Drop
//FramebufferSize
//Key
//MouseButton
//Scroll
//WindowClose
//WindowFocus
//WindowIconify
//WindowPos
//WindowRefresh
//WindowSize
//Monitor
