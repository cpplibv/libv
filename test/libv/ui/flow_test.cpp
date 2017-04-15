// File: flow_test.cpp, Created on 2016. 02. 06. 9:32, Author: Vader

#include <catch.hpp>

//#include <iostream>

#include <libv/ui/container/panel_flow.hpp>

#include "test_components.hpp"

using namespace libv;
using namespace libv::ui;

// -------------------------------------------------------------------------------------------------

//TEST_CASE("Flow layout test 00") {
//	//std::cout << libv::log;
//
//	ui::Renderer gl;
//	ui::PanelFlow flow;
//	flow.setComponentID("Flow00");
//	TestQuad quad0("TestQuad00");
//	TestQuad quad1("TestQuad01");
//	TestQuad quad2("TestQuad02");
//
//	flow.add(make_observer(&quad0));
//	flow.add(make_observer(&quad1));
//	flow.add(make_observer(&quad2));
//
//	flow.set(ui::Property::Size, vec3(16, 512, 0));
//	quad0.set(ui::Property::Size, vec3(2, 64, 0));
//	quad1.set(ui::Property::Size, vec3(4, 128, 0));
//	quad2.set(ui::Property::Size, vec3(8, 256, 0));
//
//	SECTION("") {
//		flow.setAlign(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
//		flow.setAlignContent(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
//		flow.setOrient(ui::PanelFlow::ORIENT_UP_RIGHT);
//
//		flow.build(gl);
//
//		CHECK(quad0.getDisplayPosition() == vec3(0, 0, 0));
//		CHECK(quad1.getDisplayPosition() == vec3(0, 64, 0));
//		CHECK(quad2.getDisplayPosition() == vec3(0, 192, 0));
//	}
//
//	SECTION("") {
//		flow.setAlign(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
//		flow.setAlignContent(ui::PanelFlow::ALIGN_TOP_RIGHT);
//		flow.setOrient(ui::PanelFlow::ORIENT_UP_RIGHT);
//
//		flow.build(gl);
//
//		CHECK(quad0.getDisplayPosition() == vec3(8, 64, 0));
//		CHECK(quad1.getDisplayPosition() == vec3(8, 128, 0));
//		CHECK(quad2.getDisplayPosition() == vec3(8, 256, 0));
//	}
//
//	SECTION("") {
//		flow.setAlign(ui::PanelFlow::ALIGN_TOP_RIGHT);
//		flow.setAlignContent(ui::PanelFlow::ALIGN_TOP_RIGHT);
//		flow.setOrient(ui::PanelFlow::ORIENT_UP_RIGHT);
//
//		flow.build(gl);
//
//		CHECK(quad0.getDisplayPosition() == vec3(14, 64, 0));
//		CHECK(quad1.getDisplayPosition() == vec3(12, 128, 0));
//		CHECK(quad2.getDisplayPosition() == vec3(8, 256, 0));
//	}
//
//	SECTION("") {
//		flow.setAlign(ui::PanelFlow::ALIGN_CENTER_CENTER);
//		flow.setAlignContent(ui::PanelFlow::ALIGN_CENTER_CENTER);
//		flow.setOrient(ui::PanelFlow::ORIENT_DOWN_LEFT);
//
//		flow.build(gl);
//
//		CHECK(quad0.getDisplayPosition() == vec3(7, 416, 0));
//		CHECK(quad1.getDisplayPosition() == vec3(6, 288, 0));
//		CHECK(quad2.getDisplayPosition() == vec3(4, 32, 0));
//	}
//
//	CHECK(quad0.getDisplaySize() == vec3(2, 64, 0));
//	CHECK(quad1.getDisplaySize() == vec3(4, 128, 0));
//	CHECK(quad2.getDisplaySize() == vec3(8, 256, 0));
//}
//
//// -------------------------------------------------------------------------------------------------
//
//TEST_CASE("Flow layout test 01") {
//	//std::cout << libv::log;
//
//	ui::Renderer gl;
//	ui::PanelFlow flow;
//	flow.setComponentID("Flow00");
//
//	TestQuad quad0("TestQuad00");
//	TestQuad quad1("TestQuad01");
//	TestQuad quad2("TestQuad02");
//
//	flow.add(make_observer(&quad0));
//	flow.add(make_observer(&quad1));
//	flow.add(make_observer(&quad2));
//
//	flow.set(ui::Property::Size, vec3(16, 512, 0));
//	quad0.set(ui::Property::Size, vec3(2, 64, 0));
//	quad1.set(ui::Property::Size, vec3(4, 128, 0));
//	quad2.set(ui::Property::Size, vec3(8, 256, 0));
//
//	SECTION("") {
//		flow.setAlign(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
//		flow.setAlignContent(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
//		flow.setOrient(ui::PanelFlow::ORIENT_UP_RIGHT);
//
//		flow.build(gl);
//
//		CHECK(quad0.getDisplayPosition() == vec3(0, 0, 0));
//		CHECK(quad1.getDisplayPosition() == vec3(0, 64, 0));
//		CHECK(quad2.getDisplayPosition() == vec3(0, 192, 0));
//	}
//}
