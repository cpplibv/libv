// File: main.cpp, Created on 2013. augusztus 16. 11:59, Author: Vader

// ext
#include <GL/glew.h>
// std
#include <iostream>
#include <string>
// pro
#include <libv/gl/gl.hpp>
#include <libv/gl/log.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel.hpp>
#include <libv/ui/frame/frame.hpp>
#include <libv/ui/layout/layout_flow.hpp>
#include <libv/ui/log.hpp>

using namespace libv;
using namespace libv::ui;

void noisyEvents(Frame& frame) {
	//	frame.onChar.output([](const EventChar & e) {
	//		LIBV_UI_EVENT_TRACE("Event Char: %u %s", e.unicode, e.utf8); });
	//	frame.onCharMods.output([](const EventCharMods & e) {
	//		LIBV_UI_EVENT_TRACE("Event CharMods: %d %u %s", e.mods, e.unicode, e.utf8); });
	//	frame.onCursorEnter.output([](const EventCursorEnter & e) {
	//		LIBV_UI_EVENT_TRACE("Event CursorEnter: %d", e.entered); });
	//	frame.onCursorPos.output([](const EventCursorPos & e) {
	//		LIBV_UI_EVENT_TRACE("Event CursorPos: %f %f", e.xpos, e.ypos); });
	//	frame.onDrop.output([](const EventDrop & e) {
	//		LIBV_UI_EVENT_TRACE("Event Drop: %d ...", e.strings.size()); });
	//	frame.onFramebufferSize.output([](const EventFramebufferSize & e) {
	//		LIBV_UI_EVENT_TRACE("Event FramebufferSize: %d %d", e.size.x, e.size.y); });
	//	frame.onKey.output([](const EventKey & e) {
	//		LIBV_UI_EVENT_TRACE("Event Key: %d %d %d %d", e.action, e.key, e.mode, e.scancode); });
	//	frame.onMouseButton.output([](const EventMouseButton & e) {
	//		LIBV_UI_EVENT_TRACE("Event MouseButton: %d %d %d", e.action, e.button, e.mods); });
	//	frame.onScroll.output([](const EventScroll & e) {
	//		LIBV_UI_EVENT_TRACE("Event Scroll: %f %f", e.xoffset, e.yoffset); });
	//	frame.onWindowClose.output([](const EventWindowClose &) {
	//		LIBV_UI_EVENT_TRACE("Event WindowClose"); });
	//	frame.onWindowFocus.output([](const EventWindowFocus & e) {
	//		LIBV_UI_EVENT_TRACE("Event WindowFocus: %d", e.focused); });
	//	frame.onWindowIconify.output([](const EventWindowIconify & e) {
	//		LIBV_UI_EVENT_TRACE("Event WindowIconify: %d", e.iconified); });
	//	frame.onWindowPos.output([](const EventWindowPos & e) {
	//		LIBV_UI_EVENT_TRACE("Event WindowPos: %d, %d", e.position.x, e.position.y); });
	//	frame.onWindowRefresh.output([](const EventWindowRefresh &) {
	//		LIBV_UI_EVENT_TRACE("Event WindowRefresh"); });
	//	frame.onWindowSize.output([](const EventWindowSize & e) {
	//		LIBV_UI_EVENT_TRACE("Event WindowSize: %d %d", e.size.x, e.size.y); });

	//	frame.onClose.output([](Frame* f) {
	//		LIBV_UI_EVENT_TRACE("Event onClose: %d", f); return true; });
	//	frame.onClosed.output([](Frame* f) {
	//		LIBV_UI_EVENT_TRACE("Event onClosed: %d", f); });
}

#define checkGLEWSupport(ext) LIBV_LIBV_INFO("GLEW: %-40s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")

void initGLEW() {
	if (GLenum err = glewInit() != GLEW_OK)
		LIBV_LIBV_ERROR("Failed to initialize glew: %s", (const char*) glewGetErrorString(err));

	LIBV_LIBV_INFO("GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
	LIBV_LIBV_INFO("GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
	LIBV_LIBV_INFO("GL Version: %s", (const char*) glGetString(GL_VERSION));

	checkGLEWSupport(GL_VERSION_3_3);
	checkGLEWSupport(GL_VERSION_4_5);
	checkGLEWSupport(GL_ARB_draw_elements_base_vertex);
	checkGLEWSupport(GL_ARB_gpu_shader_fp64);

	checkGL();
}

void initGL() {
	glEnable(GL_DEPTH_TEST); //Depth
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha Type
	glEnable(GL_BLEND); //Alpha
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW); //Counter clockwise polys only

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, true ? GL_FILL : GL_LINE);
	checkGL();
}
class TestFrame : public Frame {
	Label lbl;
	LayoutFlow lf;

	void init() {
		lbl.setText("Hello UI!");
		add(make_observer(&lbl));

		setLayout(make_observer(&lf));

		onKey.output([this](const EventKey & e) {
			if (e.key == 256)
				closeDefault();

			if (e.key == 'X') {
				setDecoration(!isDecorated());
			}
		});
	}

public:

	TestFrame(const std::string& title) : Frame(title) {
		noisyEvents(*this);
		setPosition(getCurrentMonitor()->currentVideoMode.size / 2 - getSize().xy() / 2);
		setCloseOperation(ON_CLOSE_DISPOSE);
		setOpenGLProfile(Frame::OPENGL_PROFILE_COMPAT);
		setOpenGLVersion(3, 3);
		setSize(512, 512);
		setDecoration(false);
		init();
		show();
	}

	virtual void build(Renderer& renderer) override {
		initGLEW();
		initGL();
		//		std::this_thread::sleep_for(std::chrono::seconds(2));
		Frame::build(renderer);
	}

	//	virtual void render() override {
	//		glClearColor(0.236f, 0.311f, 0.311f, 0.f);
	//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////		glViewport(0, 0, getSize().x, getSize().y);
	//
	//		getContent()->render(renderer);
	//		//std::cout << " build  : " << std::chrono::duration_cast<std::chrono::nanoseconds>(lastBuildDuration).count() / 1000000.0;
	//		//std::cout << " destroy: " << std::chrono::duration_cast<std::chrono::nanoseconds>(lastDestroyDuration).count() / 1000000.0;
	//		//std::cout << " loop   : " << std::chrono::duration_cast<std::chrono::nanoseconds>(lastLoopDuration).count() / 1000000.0;
	//		//std::cout << " render : " << std::chrono::duration_cast<std::chrono::nanoseconds>(lastRenderDuration).count() / 1000000.0;
	//		//std::cout << " swap   : " << std::chrono::duration_cast<std::chrono::nanoseconds>(lastSwapDuration).count() / 1000000.0;
	//		//std::cout << " update : " << std::chrono::duration_cast<std::chrono::nanoseconds>(lastUpdateDuration).count() / 1000000.0;
	//		//std::cout << std::endl;
	//	}
};

int main(int, char**) {
	std::cout << libv::log;

	TestFrame f1("TestFrame");
	f1.join();

	return 0;
}

// -------------------------------------------------------------------------------------------------