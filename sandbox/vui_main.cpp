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
	frame.onChar.output([](const EventChar& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event Char: %u %s", e.unicode, e.utf8); });
	frame.onCharMods.output([](const EventCharMods& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event CharMods: %d %u %s", e.mods, e.unicode, e.utf8); });
	frame.onCursorEnter.output([](const EventCursorEnter& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event CursorEnter: %d", e.entered); });
	frame.onCursorPos.output([](const EventCursorPos& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event CursorPos: %f %f", e.xpos, e.ypos); });
	frame.onDrop.output([](const EventDrop& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event Drop: %d ...", e.strings.size()); });
	frame.onFramebufferSize.output([](const EventFramebufferSize& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event FramebufferSize: %d %d", e.size.x, e.size.y); });
	frame.onKey.output([](const EventKey& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event Key: %d %d %d %d", e.action, e.key, e.mode, e.scancode); });
	frame.onMouseButton.output([](const EventMouseButton& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event MouseButton: %d %d %d", e.action, e.button, e.mods); });
	frame.onScroll.output([](const EventScroll& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event Scroll: %f %f", e.xoffset, e.yoffset); });
	frame.onWindowClose.output([](const EventWindowClose &) {
		LIBV_LOG_UI_EVENT_TRACE("Event WindowClose"); });
	frame.onWindowFocus.output([](const EventWindowFocus& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event WindowFocus: %d", e.focused); });
	frame.onWindowIconify.output([](const EventWindowIconify& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event WindowIconify: %d", e.iconified); });
	frame.onWindowPos.output([](const EventWindowPos& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event WindowPos: %d, %d", e.position.x, e.position.y); });
	frame.onWindowRefresh.output([](const EventWindowRefresh &) {
		LIBV_LOG_UI_EVENT_TRACE("Event WindowRefresh"); });
	frame.onWindowSize.output([](const EventWindowSize& e) {
		LIBV_LOG_UI_EVENT_TRACE("Event WindowSize: %d %d", e.size.x, e.size.y); });

	//		frame.onClose.output([](Frame* f) {
	//			LIBV_LOG_UI_EVENT_TRACE("Event onClose: %d", f); return true; });
	//		frame.onClosed.output([](Frame* f) {
	//			LIBV_LOG_UI_EVENT_TRACE("Event onClosed: %d", f); });
}

//#define checkGLEWSupport(ext) LIBV_LOG_INFO("GLEW: %-40s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")
//
//void initGLEW() {
//	if (GLenum err = glewInit() != GLEW_OK)
//		LIBV_LOG_ERROR("Failed to initialize glew: %s", (const char*) glewGetErrorString(err));
//
//	LIBV_LOG_INFO("GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
//	LIBV_LOG_INFO("GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
//	LIBV_LOG_INFO("GL Version: %s", (const char*) glGetString(GL_VERSION));
//
//	checkGLEWSupport(GL_VERSION_3_3);
//	checkGLEWSupport(GL_VERSION_4_5);
//	checkGLEWSupport(GL_ARB_draw_elements_base_vertex);
//	checkGLEWSupport(GL_ARB_gpu_shader_fp64);
//
//	checkGL();
//}
class TestFrame : public Frame {
	Label lbl0;
	Label lbl1;
	Label lbl2;
	PanelFlow lf;

	void init() {
//
// -----
//
//		lbl0.set()
//				(Property::ID, "Label0")
//				(Property::Size, percent(100), nospec(), 0)
//				(Label::Text, "Label0 with interactivity!")
//		;
//
//		lbl1.set()
//				(Property::ID, "Label1")
//				(Property::Size, percent(100), nospec(), 0)
//				(Label::Text, "Label0 with interactivity!")
//		;
//
//		lbl2.set()
//				(Property::ID, "Label2")
//				(Property::Size, percent(100), nospec(), 0)
//				(Label::Text, "Label0 with interactivity!")
//		;
//
// -----
//
//		lbl0.set(),
//				Property::ID = "Label0",
//				Property::Size = SizeType(percent(100), nospec(), 0),
//				Label::Text = "Label0 with interactivity!";
//
//		lbl1.set(),
//				Property::ID = "Label1",
//				Property::Size = SizeType(percent(100), nospec(), 0),
//				Label::Text = "Label1 with interactivity!";
//
//		lbl2.set(),
//				Property::ID = "Label2",
//				Property::Size = SizeType(percent(100), nospec(), 0),
//				Label::Text = "Label2 with interactivity!";
//
// -----
//
//		lbl0.set(Property::ID) = "Label0";
//		lbl0.set(Property::Size) = SizeType(percent(100), nospec(), 0);
//		lbl0.set(Label::Text) = "Label0 with interactivity!";
//
//		lbl1.set(Property::ID) = "Label1";
//		lbl1.set(Property::Size) = SizeType(percent(100), nospec(), 0);
//		lbl1.set(Label::Text) = "Label1 with interactivity!";
//
//		lbl2.set(Property::ID) = "Label2";
//		lbl2.set(Property::Size) = SizeType(percent(100), nospec(), 0);
//		lbl2.set(Label::Text) = "Label2 with interactivity!";
//
// -----
//
//		lbl0.set(Property::ID)("Label0");
//		lbl0.set(Property::Size)(percent(100), nospec(), 0);
//		lbl0.set(Label::Text)("Label0 with interactivity!");
//
//		lbl1.set(Property::ID)("Label1");
//		lbl1.set(Property::Size)(percent(100), nospec(), 0);
//		lbl1.set(Label::Text)("Label1 with interactivity!");
//
//		lbl2.set(Property::ID)("Label2");
//		lbl2.set(Property::Size)(percent(100), nospec(), 0);
//		lbl2.set(Label::Text)("Label2 with interactivity!");
//
// -----
//
//		lbl0.set(Property::ID) = "Label0";
//		lbl0.set(Property::Size)(percent(100), nospec(), 0);
//		lbl0.set(Label::Text) = "Label0 with interactivity!";
//
//		lbl1.set(Property::ID) = "Label1";
//		lbl1.set(Property::Size)(percent(100), nospec(), 0);
//		lbl1.set(Label::Text) = "Label1 with interactivity!";
//
//		lbl2.set(Property::ID) = "Label2";
//		lbl2.set(Property::Size)(percent(100), nospec(), 0);
//		lbl2.set(Label::Text) = "Label2 with interactivity!";
//
// -----
//
//		lbl0[Property::ID] = "Label0";
//		lbl0[Property::Size](percent(100), nospec(), 0);
//		lbl0[Label::Text] = "Label0 with interactivity!";
//
//		lbl1[Property::ID] = "Label1";
//		lbl1[Property::Size](percent(100), nospec(), 0);
//		lbl1[Label::Text] = "Label1 with interactivity!";
//
//		lbl2[Property::ID] = "Label2";
//		lbl2[Property::Size](percent(100), nospec(), 0);
//		lbl2[Label::Text] = "Label2 with interactivity!";
//
// -----

		lbl0.setComponentID("Label0");
		lbl0.setText(
				"line -1 transparent light \n"
				"line 0 transparent light [size=40]gA|\"Áreen[/size]\n"
				"line 1 transparent light [size=40]gA|\"Áreen[/size]\n"
				"line 2 transparent light [size=40]gA|\"Áreen[/size]\n"
				"line 3 transparent light [size=40]gA|\"Áreen[/size]\n"
				"line 4 transparent light [size=40]gA|\"Áreen[/size]\n"
				);
		lbl0.set()
				(Property::Size, percent(100), nospec(), 0)
				;

		lbl1.setComponentID("Label1");
		lbl1.setText("Label1 with some text on it.\n"
					"[color=#F00]re[color=#FFF4]d[/color] [color=#6F68]transparent[/color] light [size=40]green[/color] [font=res/font/Achafexp.ttf]The other[/size] font AV[/font]");
		lbl1.set()
				(Property::Size, percent(100), nospec(), 0)
				;

		lbl2.setComponentID("Label2");
		lbl2.setText("[color=#FFF4]Label2 with some text on it.[/color]");
		lbl2.set()
				(Property::Size, percent(100), nospec(), 0)
				;

		lf.setComponentID("Panel");
		lf.add(make_observer(lbl0));
		lf.add(make_observer(lbl1));
		lf.add(make_observer(lbl2));
		lf.set()
				(Property::Size, ratio(100), percent(100), 0);
		;

		lf.setAlign(ALIGN_TOP_LEFT);
		lf.setAnchor(ALIGN_TOP_LEFT);
		lf.setOrient(ORIENT_RIGHT_DOWN);

		addComponent(make_observer(lf));

		onChar.output([this](auto e) {
			lbl0.setText(lbl0.getText() + e.utf8);
		});

		onKey.output([this](const EventKey& e) {
			if (e.key == 256)
				closeDefault();
			//			if (e.key == 257 && e.action != 0)
			//				lbl0.setText(lbl0.getText() + '\n');
			//			if (e.key == 259 && e.action != 0) {
			//				auto t = lbl0.getText();
			//				if (t.size() > 0) {
			//					t.pop_back();
			//					lbl0.setText(t);
			//				}
			//			}

			if (e.key == 'X')
				setDecoration(!isDecorated());
		});
	}

public:

	TestFrame(const std::string& title) : Frame(title) {
		noisyEvents(*this);
		setSize(1200, 750);
		setPosition(POSITION_CENTER_CURRENT_MONITOR);
		setCloseOperation(ON_CLOSE_DISPOSE);
		setOpenGLProfile(OPENGL_PROFILE_COMPAT);
		setOpenGLVersion(3, 3);
		init();
		show();
	}
};

int main(int, char**) {
	std::cout << libv::log;
	//	libv::log.allow("libv.ui.component");
	//	libv::log.allow("libv.ui.event");
	//	libv::log.allow("libv.ui.frame");
	//	libv::log.allow("libv.ui.glfw");
	//	libv::log.allow("libv.ui.layout");
	//	libv::log.allow("libv.gl");
	//	libv::log.deny();

	libv::log.deny("libv.ui.event");
	libv::log.allow();

	TestFrame f1("TestFrame");
	f1.join();

	return 0;
}

// -------------------------------------------------------------------------------------------------