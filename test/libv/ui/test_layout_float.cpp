// Project: libv.ui, File: test/libv/ui/test_layout_float.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ui/component/panel_float.hpp>
#include "test_layout_utility.hpp"


// -------------------------------------------------------------------------------------------------

TEST_CASE("Layout Float: empty", "[libv.ui.layout.float]") {
	TestPanel<libv::ui::PanelFloat> panel;

	CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
	panel.layout2(0, 0);
}

TEST_CASE("Layout Float: snap_to_edge", "[libv.ui.layout.float]") {
	// Not implemented yet

	//	TestPanel<libv::ui::PanelFloat> panel;

	//	libv::ui::ContextLayout1 env1;
	//	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0));
	//
	//	panel.snap_to_edge(false);
	//	panel.snap_to_edge(true);
	//
	//	CHECK(panel.layout1(env1) == approx_size(0, 0, 0));
	//	panel.layout2(env2);
}

TEST_CASE("Layout Float: squish", "[libv.ui.layout.float]") {
	// Not implemented yet

	//	TestPanel<libv::ui::PanelFloat> panel;

	//	libv::ui::ContextLayout1 env1;
	//	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0));
	//
	//	panel.squish(false);
	//	panel.squish(true);
	//
	//	CHECK(panel.layout1(env1) == approx_size(0, 0, 0));
	//	panel.layout2(env2);
}

TEST_CASE("Layout Float: positioning with anchors", "[libv.ui.layout.float]") {
	TestPanel<libv::ui::PanelFloat> panel;

	TestComponent comp0 = panel.add("100px, 100px");

	SECTION("bottom_left") {
		comp0.anchor(libv::ui::Anchor::bottom_left);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(0, 0, 0));
	}

	SECTION("center_center") {
		comp0.anchor(libv::ui::Anchor::center_center);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(150, 100, 0));
	}

	SECTION("top_left") {
		comp0.anchor(libv::ui::Anchor::top_left);
		CHECK(panel.layout1(0, 0, 0) == approx_size(100, 100, 0));
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(0, 200, 0));
	}

	SECTION("top_center") {
		comp0.anchor(libv::ui::Anchor::top_center);
		CHECK(panel.layout1(0, 0, 0) == approx_size(100, 100, 0));
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(150, 200, 0));
	}

	SECTION("top_right") {
		comp0.anchor(libv::ui::Anchor::top_right);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(300, 200, 0));
	}

	CHECK(panel.layout1(0, 0, 0) == approx_size(100, 100, 0));
	CHECK(comp0.bounds().size == approx_size(100.f, 100.f, 0.f));
}

TEST_CASE("Layout Float: positioning with anchors, with padding", "[libv.ui.layout.float]") {
	TestPanel<libv::ui::PanelFloat> panel;
	panel.padding(5, 10, 15, 20);

	TestComponent comp0 = panel.add("100px, 100px");

	SECTION("bottom_left") {
		comp0.anchor(libv::ui::Anchor::bottom_left);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(5, 10, 0));
	}

	SECTION("center_center") {
		comp0.anchor(libv::ui::Anchor::center_center);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(145, 95, 0));
	}

	SECTION("top_left") {
		comp0.anchor(libv::ui::Anchor::top_left);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(5, 180, 0));
	}

	SECTION("top_center") {
		comp0.anchor(libv::ui::Anchor::top_center);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(145, 180, 0));
	}

	SECTION("top_right") {
		comp0.anchor(libv::ui::Anchor::top_right);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(285, 180, 0));
	}

	CHECK(panel.layout1(0, 0, 0) == approx_size(120, 130, 0));
	CHECK(comp0.bounds().size == approx_size(100.f, 100.f, 0.f));
}

TEST_CASE("Layout Float: positioning with anchors, with padding and margin", "[libv.ui.layout.float]") {
	TestPanel<libv::ui::PanelFloat> panel;
	panel.padding(5, 10, 15, 20);

	TestComponent comp0 = panel.add("100px, 100px");
	comp0.margin(10, 5, 15, 0);

	SECTION("bottom_left") {
		comp0.anchor(libv::ui::Anchor::bottom_left);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(15, 15, 0));
	}

	SECTION("center_center") {
		comp0.anchor(libv::ui::Anchor::center_center);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(143, 98, 0));
	}

	SECTION("top_left") {
		comp0.anchor(libv::ui::Anchor::top_left);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(15, 180, 0));
	}

	SECTION("top_center") {
		comp0.anchor(libv::ui::Anchor::top_center);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(143, 180, 0));
	}

	SECTION("top_right") {
		comp0.anchor(libv::ui::Anchor::top_right);
		panel.layout2(400, 300);
		CHECK(comp0.bounds().position == approx_pos(270, 180, 0));
	}

	CHECK(panel.layout1(0, 0, 0) == approx_size(145, 135, 0));
	CHECK(comp0.bounds().size == approx_size(100.f, 100.f, 0.f));
}

//TEST_CASE("Layout Float: positioning with anchors, with padding and margin pushes away border", "[libv.ui.layout.float]") {
//	TestPanel<libv::ui::PanelFloat> panel;
//	panel.padding(5, 10, 15, 20);
//
//	TestComponent comp0 = panel.add("100px, 100px");
//	comp0.margin(10, 5, 15, 0);
//
//	SECTION("bottom_left") {
//		comp0.anchor(libv::ui::Anchor::bottom_left);
//		panel.layout2(400, 300);
//		CHECK(comp0.bounds().position == approx_pos(5, 10, 0));
//	}
//
//	SECTION("center_center") {
//		comp0.anchor(libv::ui::Anchor::center_center);
//		panel.layout2(400, 300);
//		CHECK(comp0.bounds().position == approx_pos(150, 100, 0));
//	}
//
//	SECTION("top_left") {
//		comp0.anchor(libv::ui::Anchor::top_left);
//		panel.layout2(400, 300);
//		CHECK(comp0.bounds().position == approx_pos(5, 180, 0));
//	}
//
//	SECTION("top_center") {
//		comp0.anchor(libv::ui::Anchor::top_center);
//		panel.layout2(400, 300);
//		CHECK(comp0.bounds().position == approx_pos(150, 180, 0));
//	}
//
//	SECTION("top_right") {
//		comp0.anchor(libv::ui::Anchor::top_right);
//		panel.layout2(400, 300);
//		CHECK(comp0.bounds().position == approx_pos(285, 180, 0));
//	}
//
//	CHECK(panel.layout1(0, 0, 0) == approx_size(120, 130, 0));
//	CHECK(comp0.bounds().size == approx_size(100.f, 100.f, 0.f));
//}

TEST_CASE("Layout Float: simple size tests 0", "[libv.ui.layout.float]") {
	TestPanel<libv::ui::PanelFloat> panel;

	SECTION("ratio") {
		TestComponent comp0 = panel.add("1r, 1r", D(20, 100, 0));
		TestComponent comp1 = panel.add("2r, 1r", D(30,  90, 0));
		TestComponent comp2 = panel.add("3r, 1r", D(40,  80, 0));
		comp0.anchor(libv::ui::Anchor::bottom_left);
		comp1.anchor(libv::ui::Anchor::bottom_left);
		comp2.anchor(libv::ui::Anchor::bottom_left);

		CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(0, 0, 0, 400.f, 300.f, 0.f));
		CHECK(comp1.bounds() == Bounds(0, 0, 0, 400.f, 300.f, 0.f));
		CHECK(comp2.bounds() == Bounds(0, 0, 0, 400.f, 300.f, 0.f));
	}

	SECTION("pixel") {
		TestComponent comp0 = panel.add("1px, 30px", D(20, 100, 0));
		TestComponent comp1 = panel.add("2px, 30px", D(30,  90, 0));
		TestComponent comp2 = panel.add("3px, 30px", D(40,  80, 0));
		comp0.anchor(libv::ui::Anchor::bottom_left);
		comp1.anchor(libv::ui::Anchor::bottom_left);
		comp2.anchor(libv::ui::Anchor::bottom_left);

		CHECK(panel.layout1(0, 0, 0) == approx_size(3, 30, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(0, 0, 0, 1.f, 30.f, 0.f));
		CHECK(comp1.bounds() == Bounds(0, 0, 0, 2.f, 30.f, 0.f));
		CHECK(comp2.bounds() == Bounds(0, 0, 0, 3.f, 30.f, 0.f));
	}

	SECTION("percent") {
		TestComponent comp0 = panel.add("1%, 30%", D(20, 100, 0));
		TestComponent comp1 = panel.add("2%, 30%", D(30,  90, 0));
		TestComponent comp2 = panel.add("3%, 30%", D(40,  80, 0));
		comp0.anchor(libv::ui::Anchor::bottom_left);
		comp1.anchor(libv::ui::Anchor::bottom_left);
		comp2.anchor(libv::ui::Anchor::bottom_left);

		CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(0, 0, 0,  4.f, 90.f, 0.f));
		CHECK(comp1.bounds() == Bounds(0, 0, 0,  8.f, 90.f, 0.f));
		CHECK(comp2.bounds() == Bounds(0, 0, 0, 12.f, 90.f, 0.f));
	}

	SECTION("dynamic") {
		TestComponent comp0 = panel.add("D, D", D(20, 100, 0));
		TestComponent comp1 = panel.add("D, D", D(30,  90, 0));
		TestComponent comp2 = panel.add("D, D", D(40,  80, 0));
		comp0.anchor(libv::ui::Anchor::bottom_left);
		comp1.anchor(libv::ui::Anchor::bottom_left);
		comp2.anchor(libv::ui::Anchor::bottom_left);

		CHECK(panel.layout1(0, 0, 0) == approx_size(40, 100, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(0, 0, 0, 20.f, 100.f, 0.f));
		CHECK(comp1.bounds() == Bounds(0, 0, 0, 30.f,  90.f, 0.f));
		CHECK(comp2.bounds() == Bounds(0, 0, 0, 40.f,  80.f, 0.f));
	}
}

TEST_CASE("Layout Float: complex size test 0", "[libv.ui.layout.float]") {
	TestPanel<libv::ui::PanelFloat> panel;

	TestComponent comp0 = panel.add("  0px 20% 1r D,  10px  0% 1r D,   0px 20% 1r D", D(10, 20, 30));
	TestComponent comp1 = panel.add("150px 20% 2r  ,  75px 10% 2r D, 150px 20% 2r  ", D(11, 21, 31));
	TestComponent comp2 = panel.add("200px 10%    D,  50px 20% 3r  , 200px 10% 3r D", D(12, 22, 32));
	TestComponent comp3 = panel.add("250px  0% 4r D,   0px 10% 4r  ,   0px  0%     ", D(13, 23, 33));
	comp0.anchor(libv::ui::Anchor::bottom_left);
	comp1.anchor(libv::ui::Anchor::bottom_left);
	comp2.anchor(libv::ui::Anchor::bottom_left);
	comp3.anchor(libv::ui::Anchor::bottom_left);

	CHECK(panel.layout1(0, 0, 0) == approx_size(263.f, 106.66667f, 257.77778f));
	panel.layout2(400, 300, 200);

	CHECK(comp0.bounds() == Bounds(0, 0, 0, 400, 300, 200));
	CHECK(comp1.bounds() == Bounds(0, 0, 0, 400, 300, 200));
	CHECK(comp2.bounds() == Bounds(0, 0, 0, 252, 300, 200));
	CHECK(comp3.bounds() == Bounds(0, 0, 0, 400, 300,   0));
}
