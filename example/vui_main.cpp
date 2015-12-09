// File: main.cpp, Created on 2013. augusztus 16. 11:59, Author: Vader

// ext
#include <GL/glew.h>
// std
#include <iostream>
#include <string>
// pro
#include <vl/ui/log.hpp>
#include <vl/ui/frame/frame.hpp>
#include <vl/ui/component/panel.hpp>
#include <vl/ui/component/label.hpp>
#include <vl/gl/gl.hpp>

#include "vl/ui/layout/layout_flow.hpp"

using namespace vl;
using namespace vl::ui;

class TestFrame : public Frame {
	Label lbl;
	LayoutFlow lf;

	void init() {
		lbl.setText("Hello UI!");
		add(make_observer(&lbl));

		setLayout(make_observer(&lf));

		onChar.output([](const EventChar & e) {
			std::cout << "Char: " << e.utf8 << std::endl;
		});
		onKey.output([this](const EventKey & e) {
			std::cout << " Key: " << e.key << std::endl;

			if (e.key == 256)
				closeDefault();

			if (e.key == 'X') {
				setDecoration(!isDecorated());
			}
		});
	}

public:
	TestFrame(const std::string& title) : Frame(title) {
		setPosition(getCurrentMonitor()->currentVideoMode.size / 2 - getSize().xy() / 2);
		setCloseOperation(ON_CLOSE_DISPOSE);
		setOpenGLProfile(Frame::OPENGL_PROFILE_COMPAT);
		setOpenGLVersion(3, 3);
		setDecoration(false);
		init();
		show();
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
	vl::log().output(std::cout);

	TestFrame f1("TestFrame");
	f1.join();

	return 0;
}

// -------------------------------------------------------------------------------------------------