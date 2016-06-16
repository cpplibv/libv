// File: flow_test.cpp, Created on 2016. 02. 06. 9:32, Author: Vader

#include <catch.hpp>

#include <iostream>

#include <libv/ui/layout.hpp>

#include "test_components.hpp"

using namespace libv;
using namespace libv::ui;

// -------------------------------------------------------------------------------------------------

TEST_CASE("UI Layout system should handle basic layout components") {
	TestPanel panel("Panel");
	TestQuad quad0("Quad0");
	TestQuad quad1("Quad1");
	TestQuad quad2("Quad2");

	panel.set(Property::Size, vec3(16, 512, 0));
	quad0.set(Property::Size, vec3(2, 64, 0));
	quad1.set(Property::Size, vec3(4, 128, 0));
	quad2.set(Property::Size, vec3(8, 256, 0));

	panel.add(make_observer(&quad0));
	panel.add(make_observer(&quad1));
	panel.add(make_observer(&quad2));

	Layout layoutRoot(vec3(16, 512, 0));

	SECTION("") {
		panel.setAlign(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
		panel.setAlignContent(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
		panel.setOrient(ui::PanelFlow::ORIENT_RIGHT_UP);

		panel.layout(layoutRoot);
		auto& components = panel.getComponents();

		REQUIRE(components.size() == 3);
		CHECK(components[0].info.size == vec3(2, 64, 0));
		CHECK(components[1].info.size == vec3(4, 128, 0));
		CHECK(components[2].info.size == vec3(8, 256, 0));
		CHECK(components[0].info.offset == vec3(0, 0, 0));
		CHECK(components[1].info.offset == vec3(2, 0, 0));
		CHECK(components[2].info.offset == vec3(6, 0, 0));
	}
}

TEST_CASE("UI Layout system should handle dynamic layout components") {
	TestPanel panel("Panel");
	TestQuadDynamicLayout quad0("Quad0");
	TestQuadDynamicLayout quad1("Quad1");
	TestQuadDynamicLayout quad2("Quad2");

	panel.set(Property::Size, vec3(16, 512, 0));
	quad0.size = vec3(2, 64, 0);
	quad1.size = vec3(4, 128, 0);
	quad2.size = vec3(8, 256, 0);

	panel.addObserver(quad0);
	panel.addObserver(quad1);
	panel.addObserver(quad2);

	Layout layoutRoot(vec3(16, 512, 0));

	SECTION("") {
		panel.setAlign(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
		panel.setAlignContent(ui::PanelFlow::ALIGN_BOTTOM_LEFT);
		panel.setOrient(ui::PanelFlow::ORIENT_RIGHT_UP);

		panel.layout(layoutRoot);
		auto& components = panel.getComponents();

		REQUIRE(components.size() == 3);
		CHECK(components[0].info.size == vec3(2, 64, 0));
		CHECK(components[1].info.size == vec3(4, 128, 0));
		CHECK(components[2].info.size == vec3(8, 256, 0));
		CHECK(components[0].info.offset == vec3(0, 0, 0));
		CHECK(components[1].info.offset == vec3(2, 0, 0));
		CHECK(components[2].info.offset == vec3(6, 0, 0));
	}
}

TEST_CASE("UI Layout system should handle dynamic layout components2") {
	std::cout << libv::log;
	{
	TestPanel panel("Panel");
	TestQuadDynamicLayout quad0("Quad0");
	TestQuadDynamicLayout quad1("Quad1");
	TestQuadDynamicLayout quad2("Quad2");

	panel.set(Property::Size, vec3(1000, 1000, 0));
	quad0.size = vec3(800, 400, 0);
	quad1.size = vec3(300, 20, 0);
	quad2.size = vec3(300, 20, 0);

	panel.addObserver(quad0);
	panel.addObserver(quad1);
	panel.addObserver(quad2);

	Layout layoutRoot(vec3(1000, 1000, 0));

	SECTION("") {
		panel.setAlign(PanelFlow::ALIGN_TOP_LEFT);
		panel.setAlignContent(PanelFlow::ALIGN_TOP_LEFT);
		panel.setOrient(PanelFlow::ORIENT_RIGHT_DOWN);

		panel.layout(layoutRoot);
		auto& components = panel.getComponents();

		REQUIRE(components.size() == 3);
		CHECK(components[0].info.size == vec3(800, 400, 0));
		CHECK(components[1].info.size == vec3(300, 20, 0));
		CHECK(components[2].info.size == vec3(300, 20, 0));
		CHECK(components[0].info.offset == vec3(0, 600, 0));
		CHECK(components[1].info.offset == vec3(0, 580, 0));
		CHECK(components[2].info.offset == vec3(300, 580, 0));
	}
	}
	libv::log.deny();
}

//
// Root 1000px, 700px
// - Panel 1r, 1r
// - - SideL <text>, 1r
// - - - Header <text>, <text>
// - - - List 1r, <text>
// - - - - ListElement 1r, <text>
// - - - - ListElement 1r, <text>
// - - - - ListElement 1r, <text>
// - - SideR 1r, 1r
// - - - ViewPort 1r, 1r
//

TEST_CASE("UI Layout system should handle not layoutable components") {
	std::cout << libv::log;
	{
	}
	libv::log.deny();
}