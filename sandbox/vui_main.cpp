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
#include <libv/ui/container/panel_flow.hpp>
#include <libv/ui/frame/frame.hpp>
#include <libv/ui/layout.hpp>
#include <libv/ui/log.hpp>

using namespace libv;
using namespace libv::ui;

void noisyEvents(Frame& frame) {
	(void) frame;
	frame.onChar.output([](const EventChar & e) {
		LIBV_UI_EVENT_TRACE("Event Char: %u %s", e.unicode, e.utf8); });
	frame.onCharMods.output([](const EventCharMods & e) {
		LIBV_UI_EVENT_TRACE("Event CharMods: %d %u %s", e.mods, e.unicode, e.utf8); });
	frame.onCursorEnter.output([](const EventCursorEnter & e) {
		LIBV_UI_EVENT_TRACE("Event CursorEnter: %d", e.entered); });
	frame.onCursorPos.output([](const EventCursorPos & e) {
		LIBV_UI_EVENT_TRACE("Event CursorPos: %f %f", e.xpos, e.ypos); });
	frame.onDrop.output([](const EventDrop & e) {
		LIBV_UI_EVENT_TRACE("Event Drop: %d ...", e.strings.size()); });
	frame.onFramebufferSize.output([](const EventFramebufferSize & e) {
		LIBV_UI_EVENT_TRACE("Event FramebufferSize: %d %d", e.size.x, e.size.y); });
	frame.onKey.output([](const EventKey & e) {
		LIBV_UI_EVENT_TRACE("Event Key: %d %d %d %d", e.action, e.key, e.mode, e.scancode); });
	frame.onMouseButton.output([](const EventMouseButton & e) {
		LIBV_UI_EVENT_TRACE("Event MouseButton: %d %d %d", e.action, e.button, e.mods); });
	frame.onScroll.output([](const EventScroll & e) {
		LIBV_UI_EVENT_TRACE("Event Scroll: %f %f", e.xoffset, e.yoffset); });
	frame.onWindowClose.output([](const EventWindowClose &) {
		LIBV_UI_EVENT_TRACE("Event WindowClose"); });
	frame.onWindowFocus.output([](const EventWindowFocus & e) {
		LIBV_UI_EVENT_TRACE("Event WindowFocus: %d", e.focused); });
	frame.onWindowIconify.output([](const EventWindowIconify & e) {
		LIBV_UI_EVENT_TRACE("Event WindowIconify: %d", e.iconified); });
	frame.onWindowPos.output([](const EventWindowPos & e) {
		LIBV_UI_EVENT_TRACE("Event WindowPos: %d, %d", e.position.x, e.position.y); });
	frame.onWindowRefresh.output([](const EventWindowRefresh &) {
		LIBV_UI_EVENT_TRACE("Event WindowRefresh"); });
	frame.onWindowSize.output([](const EventWindowSize & e) {
		LIBV_UI_EVENT_TRACE("Event WindowSize: %d %d", e.size.x, e.size.y); });

	//		frame.onClose.output([](Frame* f) {
	//			LIBV_UI_EVENT_TRACE("Event onClose: %d", f); return true; });
	//		frame.onClosed.output([](Frame* f) {
	//			LIBV_UI_EVENT_TRACE("Event onClosed: %d", f); });
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
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); //Counter clockwise polys only

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, true ? GL_FILL : GL_LINE);
	checkGL();
}

class TestFrame : public Frame {
	Label lbl0;
	Label lbl1;
	Label lbl2;
	PanelFlow lf;

	void init() {
		lbl0.setComponentID("Label0");
		lbl0.setText("Label0 with interactivity!");
		lbl0.set()
//			(Property::Size, vec3(512, 512, 0))
		;

		lbl1.setComponentID("Label1");
		lbl1.setText("Label1 with some text on it.");
//		lbl1.set()
//			(Property::Layout, Property::LayoutType::Block)
//		;

		lbl2.setComponentID("Label2");
		lbl2.setText("Label2 with some text on it.");
//		lbl2.set()
//			(Property::Layout, Property::LayoutType::Block)
//		;

		lf.setComponentID("Panel");
		lf.add(make_observer(&lbl0));
		lf.add(make_observer(&lbl1));
		lf.add(make_observer(&lbl2));
		lf.set()
			(Property::Size, vec3(1024, 1024, 0));
		;
		lf.setAlign(PanelFlow::ALIGN_TOP_LEFT);
		lf.setAlignContent(PanelFlow::ALIGN_TOP_LEFT);
		lf.setOrient(PanelFlow::ORIENT_RIGHT_DOWN);

//		lf.setAlign(PanelFlow::ALIGN_BOTTOM_CENTER);
//		lf.setAlignContent(PanelFlow::ALIGN_TOP_CENTER);
//		lf.setOrient(PanelFlow::ORIENT_RIGHT_DOWN);

		addComponent(make_observer(&lf));

		onChar.output([this](auto e) {
			lbl0.setText(lbl0.getText() + e.utf8);
		});

		onKey.output([this](const EventKey & e) {
			if (e.key == 256)
				closeDefault();
			if (e.key == 257 && e.action != 0)
				lbl0.setText(lbl0.getText() + '\n');
			if (e.key == 'A') {
				Layout layoutRoot(vec3(1024, 1024, 0));
				lf.layout(layoutRoot);
				lf.set();
			}
			if (e.key == 259 && e.action != 0) {
				auto t = lbl0.getText();
				if (t.size() > 0) {
					t.pop_back();
					lbl0.setText(t);
				}
			}

			if (e.key == 'X') {
				setDecoration(!isDecorated());
			}
		});
	}

public:

	TestFrame(const std::string& title) : Frame(title) {
		noisyEvents(*this);
		setPosition(getCurrentMonitor()->currentVideoMode.size / 2 - getSize() / 2);
		setCloseOperation(ON_CLOSE_DISPOSE);
		setOpenGLProfile(Frame::OPENGL_PROFILE_COMPAT);
		setOpenGLVersion(3, 3);
		setSize(1024, 1024);
		init();
		show();
	}

private:
	virtual void initContext() override {
		LIBV_UI_FRAME_DEBUG("Initialize context");
		initGLEW();
		initGL();
	}

	virtual void termContext() override {
		LIBV_UI_FRAME_DEBUG("Terminate context");
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
	//	libv::log.allow("libv.ui.component");
	////	libv::log.allow("libv.ui.event");
	//	libv::log.allow("libv.ui.frame");
	//	libv::log.allow("libv.ui.glfw");
	//	libv::log.allow("libv.ui.layout");
	//	libv::log.allow("libv.gl");
	//	libv::log.allow_above(libv::Info);
	//	libv::log.deny();

	libv::log.deny("libv.ui.event");

	TestFrame f1("TestFrame");
	f1.join();

	return 0;
}

// -------------------------------------------------------------------------------------------------