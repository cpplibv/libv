// File: main.cpp, Created on 2013. augusztus 16. 11:59, Author: Vader

// ext
#include <GL/glew.h>
// std
#include <iostream>
#include <string>
// pro
#include <vl/ui/log.hpp>
#include <vl/ui/frame/frame.hpp>
#include <vl/ui/component/quad.hpp>
#include <vl/ui/component/panel.hpp>
#include <vl/ui/component/label.hpp>
#include <vl/gl/gl.hpp>

using namespace vl;
using namespace vl::ui;
class TestFrame : public Frame {
	Panel pn;
	Label lbl;

	void init() {
		lbl.setText("Hello UI!");
		setContent(pn);
		pn.add(lbl);
	}

public:

	TestFrame(const std::string& title) :
		Frame(title) {
		setPosition(getCurrentMonitor()->currentVideoMode.size / 2 - getSize() / 2);
		setCloseOperation(ON_CLOSE_DISPOSE);
		init();
		show();
	}

	virtual void render() override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, getSize().x, getSize().y);
//		gl::pushVMat();
		getContent()->render(renderer);
//		gl::popVMat();

		//std::cout << " build: "   << std::chrono::duration_cast<std::chrono::nanoseconds>(lastBuildDuration).count() / 1000000.0;
		//std::cout << " destroy: " << std::chrono::duration_cast<std::chrono::nanoseconds>(lastDestroyDuration).count() / 1000000.0;
		//std::cout << " loop: "    << std::chrono::duration_cast<std::chrono::nanoseconds>(lastLoopDuration).count() / 1000000.0;
		//std::cout << " render: "  << std::chrono::duration_cast<std::chrono::nanoseconds>(lastRenderDuration).count() / 1000000.0;
		//std::cout << " swap: "    << std::chrono::duration_cast<std::chrono::nanoseconds>(lastSwapDuration).count() / 1000000.0;
		//std::cout << " update: "  << std::chrono::duration_cast<std::chrono::nanoseconds>(lastUpdateDuration).count() / 1000000.0;
		//std::cout << std::endl;
	}
};

int main(int, char**) {
	vl::log().output(std::cout);
	TestFrame f1("TestFrame");
	f1.onChar([](const EventChar & e) {
		std::cout << e.utf8 << std::endl;
	});
	f1.join();
	return 0;
}

// -------------------------------------------------------------------------------------------------

//#define GLFW_PRESS 1
//#define GLFW_KEY_ESCAPE 1
//
//class MyFrame : public Frame,
//public ListenerChar, public ListenerCursorEnter, public ListenerCursorPos,
//public ListenerFramebufferSize, public ListenerKey, public ListenerMouseButton,
//public ListenerScroll, public ListenerWindowClose, public ListenerWindowFocus,
//public ListenerWindowIconify, public ListenerWindowPos,
//public ListenerWindowRefresh, public ListenerWindowSize, public ListenerMonitor {
//public:
//	MyFrame() {
//		//addListenerFramebufferSize(this);
//		//addListenerWindowPos(this);
//		addListenerChar(this);
//		addListenerWindowSize(this);
//		addListenerKey(this);
//		setTitle(std::string("ON_CLOSE_DISPOSE"));
//		setCloseOperation(Frame::ON_CLOSE_DISPOSE);
//
//		setOpenGLVersion(4, 0);
//		setOpenGLProfile(Frame::OPENGL_PROFILE_COMPAT);
//
//		setSize(900, 600);
//		setPosition(150, 150);
//		//setDisplayMode(VFrame::DISPLAY_MODE_FULLSCREEN);
//	}
//
//protected:
//	virtual void handleEvent(const EventChar& e) {
//		std::cout << " char: " << e.codepoint;
//	}
//	virtual void handleEvent(const EventKey & e) {
//		if (e.action == GLFW_PRESS) {
//			std::cout << " key: " << e.key << " scancode: " << e.scancode << std::endl;
//			return;
//		}
//
//		if (e.key == GLFW_KEY_ESCAPE)
//			Frame::closeAll();
//
//		if (e.key == 'X')
//			setDecoration(!isDecorated());
//
//		if (e.key == 'M') {
//			quad0->setProperty(Property::Size, ivec2(quad0->getSize() + vec2(1.0f, 1.0f)));
//			quad1->setProperty(Property::Size, ivec2(quad1->getSize() + vec2(1.0f, 1.0f)));
//			quad2->setProperty(Property::Size, ivec2(quad2->getSize() + vec2(1.0f, 1.0f)));
//			quad3->setProperty(Property::Size, ivec2(quad3->getSize() + vec2(1.0f, 1.0f)));
//			std::cout << "quad3->setProperty(Property::Size,quad3->getSize() + vec2(1.0f, 1.0f));" << std::endl;
//		}
//		if (e.key == 'N') {
//			quad0->setProperty(Property::Size, ivec2(quad0->getSize() + vec2(-1.0f, -1.0f)));
//			quad1->setProperty(Property::Size, ivec2(quad1->getSize() + vec2(-1.0f, -1.0f)));
//			quad2->setProperty(Property::Size, ivec2(quad2->getSize() + vec2(-1.0f, -1.0f)));
//			quad3->setProperty(Property::Size, ivec2(quad3->getSize() + vec2(-1.0f, -1.0f)));
//			std::cout << "quad3->setProperty(Property::Size,quad3->getSize() + vec2(-1.0f, -1.0f));" << std::endl;
//		}
//		if (e.key == 'C') {
//			std::cout << "quad0 inc" << std::endl;
//			quad0->setProperty(Property::Size, ivec2(quad0->getSize() + vec2(+1.0f, +1.0f)));
//		}
//		if (e.key == 'V') {
//			std::cout << "quad0 inc" << std::endl;
//			quad0->setProperty(Property::Size, ivec2(quad0->getSize() + vec2(-1.0f, -1.0f)));
//		}
//		if (e.key == 'Z')
//			std::cout << lm1->layoutSize((Container*) f1->getContent()) << std::endl;
//
//		switch (e.key) {
//			case '0': lm1->setOrientation(LayoutFlow::ORIENTATION_DOWN_LEFT);
//				break;
//			case '1': lm1->setOrientation(LayoutFlow::ORIENTATION_DOWN_RIGHT);
//				break;
//			case '2': lm1->setOrientation(LayoutFlow::ORIENTATION_LEFT_DOWN);
//				break;
//			case '3': lm1->setOrientation(LayoutFlow::ORIENTATION_LEFT_UP);
//				break;
//			case '4': lm1->setOrientation(LayoutFlow::ORIENTATION_RIGHT_DOWN);
//				break;
//			case '5': lm1->setOrientation(LayoutFlow::ORIENTATION_RIGHT_UP);
//				break;
//			case '6': lm1->setOrientation(LayoutFlow::ORIENTATION_UP_LEFT);
//				break;
//			case '7': lm1->setOrientation(LayoutFlow::ORIENTATION_UP_RIGHT);
//				break;
//		}
//		switch (e.key) {
//			case 'Q': lm1->setAlign(LayoutFlow::ALIGNMENT_BOTTOM_LEFT);
//				break;
//			case 'W': lm1->setAlign(LayoutFlow::ALIGNMENT_BOTTOM_CENTER);
//				break;
//			case 'E': lm1->setAlign(LayoutFlow::ALIGNMENT_BOTTOM_RIGHT);
//				break;
//			case 'R': lm1->setAlign(LayoutFlow::ALIGNMENT_CENTER_LEFT);
//				break;
//			case 'T': lm1->setAlign(LayoutFlow::ALIGNMENT_CENTER_CENTER);
//				break;
//			case 'Y': lm1->setAlign(LayoutFlow::ALIGNMENT_CENTER_RIGHT);
//				break;
//			case 'U': lm1->setAlign(LayoutFlow::ALIGNMENT_TOP_LEFT);
//				break;
//			case 'I': lm1->setAlign(LayoutFlow::ALIGNMENT_TOP_CENTER);
//				break;
//			case 'O': lm1->setAlign(LayoutFlow::ALIGNMENT_TOP_RIGHT);
//				break;
//		}
//		switch (e.key) {
//			case 'A': lm1->setContentAlign(LayoutFlow::ALIGNMENT_BOTTOM_LEFT);
//				break;
//			case 'S': lm1->setContentAlign(LayoutFlow::ALIGNMENT_BOTTOM_CENTER);
//				break;
//			case 'D': lm1->setContentAlign(LayoutFlow::ALIGNMENT_BOTTOM_RIGHT);
//				break;
//			case 'F': lm1->setContentAlign(LayoutFlow::ALIGNMENT_CENTER_LEFT);
//				break;
//			case 'G': lm1->setContentAlign(LayoutFlow::ALIGNMENT_CENTER_CENTER);
//				break;
//			case 'H': lm1->setContentAlign(LayoutFlow::ALIGNMENT_CENTER_RIGHT);
//				break;
//			case 'J': lm1->setContentAlign(LayoutFlow::ALIGNMENT_TOP_LEFT);
//				break;
//			case 'K': lm1->setContentAlign(LayoutFlow::ALIGNMENT_TOP_CENTER);
//				break;
//			case 'L': lm1->setContentAlign(LayoutFlow::ALIGNMENT_TOP_RIGHT);
//				break;
//		}
//		panel->doLayout();
//	}
//	virtual void handleEvent(const EventFramebufferSize & e) {
//		std::cout << "Buffer: " << e.width << "; " << e.height << std::endl;
//	}
//	virtual void handleEvent(const EventWindowSize & e) {
//		std::cout << "Size: " << e.size.x << "; " << e.size.y << std::endl;
//	}
//	virtual void handleEvent(const EventWindowPos & e) {
//		std::cout << "Pos: " << e.position.x << "; " << e.position.y << std::endl;
//	}
//
//};
//
//Font2D font;
//struct VPanel : public Container {
//	virtual void render() {
//		//		ivec2 size = getSize();
//		//		ivec2 pos = getPosition();
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		checkGL();
//		glPushMatrix();
//		checkGL();
//		glMatrixMode(GL_MODELVIEW);
//		checkGL();
//		renderComponents();
//		ivec2 size = ivec2(400, 400);
//		ivec2 pos = ivec2(10, 10);
//		glLoadIdentity();
//		checkGL();
//		glViewport(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
//		checkGL();
//		gluOrtho2D(0, size.x, 0, size.y);
//		checkGL();
//		glDisable(GL_DEPTH_TEST);
//		checkGL();
//
//		//		glEnable(GL_TEXTURE_2D);
//		//		glBindTexture(GL_TEXTURE_2D, font.texture);
//		//		glColor4f(1.f, 1.f, 1.f, 0.9f);
//		//		glBegin(GL_QUADS);
//		//		glTexCoord2f(0.0f, 1.0f);
//		//		glVertex2i(0, 0);
//		//		glTexCoord2f(1.0f, 1.0f);
//		//		glVertex2i(256, 0);
//		//		glTexCoord2f(1.0f, 0.0f);
//		//		glVertex2i(256, 256);
//		//		glTexCoord2f(0.0f, 0.0f);
//		//		glVertex2i(0, 256);
//		//		glEnd();
//		//		glBindTexture(GL_TEXTURE_2D, 0);
//		//		glDisable(GL_TEXTURE_2D);
//
//		font.begin();
//		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//		font.align(FONT2D_ALIGN_LEFT, FONT2D_ALIGN_BOTTOM);
//		//		font.print(50, 50, "Hello World!");
//		font.print(10, 10, "Everything not saved will be lost.");
//		font.end();
//		checkGL();
//
//		glPopMatrix();
//		glEnable(GL_DEPTH_TEST);
//		checkGL();
//	}
//};
//void initGL() {
//	debugLog(VUI_SEVERITY_TRACE, __PRETTY_FUNCTION__);
//	glewExperimental = GL_TRUE;
//	glewInit();
//	checkGL();
//
//	glEnable(GL_BLEND);
//	checkGL();
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	checkGL();
//	glEnable(GL_DEPTH_TEST);
//	checkGL();
//}
//int main(int, char**) {
//	vuiAddLogCallback(cb);
//	debugLog(VUI_SEVERITY_TRACE, " ======== MAIN THREAD ======== ");
//	debugLog(VUI_SEVERITY_TRACE, __PRETTY_FUNCTION__);
//
//	f1 = new MyFrame();
//
//	lm1 = new LayoutFlow();
//
//	quad0 = new Quad();
//	quad1 = new Quad();
//	quad2 = new Quad();
//	quad3 = new Quad();
//
//	panel = new VPanel();
//	panel->setProperty(Property::Size, ivec2(250, 250));
//
//	quad0->setProperty(Property::Size, ivec2(70, 70));
//	quad1->setProperty(Property::Size, ivec2(80, 80));
//	quad2->setProperty(Property::Size, ivec2(60, 60));
//	quad3->setProperty(Property::Size, ivec2(40, 40));
//
//	panel->setLayout(lm1);
//
//	panel->add(quad0);
//	panel->add(quad1);
//	panel->add(quad2);
//	panel->add(quad3);
//
//	panel->doLayout();
//	f1->setContent(std::shared_ptr<Component>(panel));
//
//	f1->show();
//	f1->invokeQue.invokeSync([] {
//		checkGL();
//		initGL();
//		checkGL();
//		font.load("Default.ttf", 10);
//		checkGL();
//
//		if (glGetString(GL_VENDOR))
//			std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
//		if (glGetString(GL_RENDERER))
//			std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
//		if (glGetString(GL_VERSION))
//			std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
//		if (glGetString(GL_SHADING_LANGUAGE_VERSION))
//			std::cout << "Shader: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
//		//if (glGetString(GL_EXTENSIONS))
//		//	std::cout << "Extensions: " << glGetString(GL_EXTENSIONS) << std::endl;
//			checkGL();
//	});
//
//	vuiTerminate();
//	return 0;
//}
//
//
//
//
//
//
//
////	std::this_thread::sleep_for(std::chrono::milliseconds(2000));