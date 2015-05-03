// File: main2.cpp, Created on 2015. március 30. 8:55, Author: Vader

#include <GL/glew.h>

#include <iostream>
#include <iomanip>

//#include "vl/ui/vui.hpp"
#include "vl/ui/log.hpp"
#include "vl/ui/frame/frame.hpp"
#include "vl/ui/component/quad.hpp"

using namespace std;
using namespace std::literals;
using namespace vl;
using namespace vl::ui;

//class VFrame : public Frame {
//public:
//	using Frame::Frame;
//
//	virtual void render() override {
//		glClearColor(0, 0, 0.3f, 1);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		std::cout << " xy " << std::endl;
//	}
//};

int main(int, char**) {
	vl::log().output(std::cout);
	
	std::cout << "привет" << std::endl;

	Frame f1("1");
	f1.setContent(std::make_shared<Quad>());
	f1.setPosition(400, 300);
	f1.show();

	f1.onChar([](const EventChar& e) {
		std::cout << e.unicode << std::endl;
	});
	
	std::this_thread::sleep_for(1000ms);
	f1.minimalize();
	std::this_thread::sleep_for(1000ms);
	f1.restore();
	std::this_thread::sleep_for(1000ms);
	f1.setDecoration(false);
	std::this_thread::sleep_for(1000ms);
	f1.setDecoration(true);
	std::this_thread::sleep_for(1000ms);
	f1.hide();
	std::this_thread::sleep_for(1000ms);
	f1.show();

	Frame::joinAll();
	return 0;
}

//	std::this_thread::sleep_for(200ms);
//	f.closeForce();
//	Frame f2("2");
//	f2.show();