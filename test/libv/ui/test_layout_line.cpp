// Project: libv.ui, File: test/libv/ui/test_layout_line.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include "test_layout_utility.hpp"
#include <libv/ui/component/panel_line.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("layout line: empty", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
	panel.layout2(0, 0);
}

TEST_CASE("layout line: positioning with alignment", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	TestComponent comp0 = panel.add("40px, 60px", D( 20, 100, 0));
	TestComponent comp1 = panel.add("50px, 50px", D( 60,  40, 0));
	TestComponent comp2 = panel.add("60px, 40px", D(100,  10, 0));

	panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

	SECTION("alignment: TOP_LEFT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::top);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(40.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(90.f, 260.f, 0.f));
	}

	SECTION("alignment: TOP_CENTER") {
		panel.align_horizontal(libv::ui::AlignHorizontal::center);
		panel.align_vertical(libv::ui::AlignVertical::top);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(125.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(215.f, 260.f, 0.f));
	}

	SECTION("alignment: TOP_RIGHT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::right);
		panel.align_vertical(libv::ui::AlignVertical::top);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(250.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(290.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 260.f, 0.f));
	}

	SECTION("alignment: TOP_JUSTIFY") {
		SECTION("Justify") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		}
		SECTION("JustifyAll") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify_all);
		}
		panel.align_vertical(libv::ui::AlignVertical::top);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 260.f, 0.f));
	}

	SECTION("alignment: CENTER_LEFT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::center);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 120.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(40.f, 125.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(90.f, 130.f, 0.f));
	}

	SECTION("alignment: CENTER_CENTER") {
		panel.align_horizontal(libv::ui::AlignHorizontal::center);
		panel.align_vertical(libv::ui::AlignVertical::center);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(125.f, 120.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 125.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(215.f, 130.f, 0.f));
	}

	SECTION("alignment: CENTER_RIGHT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::right);
		panel.align_vertical(libv::ui::AlignVertical::center);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(250.f, 120.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(290.f, 125.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 130.f, 0.f));
	}

	SECTION("alignment: CENTER_JUSTIFY") {
		SECTION("Justify") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		}
		SECTION("JustifyAll") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify_all);
		}
		panel.align_vertical(libv::ui::AlignVertical::center);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 120.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 125.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 130.f, 0.f));
	}

	SECTION("alignment: BOTTOM_LEFT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::bottom);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 0.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(40.f, 0.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(90.f, 0.f, 0.f));
	}

	SECTION("alignment: BOTTOM_CENTER") {
		panel.align_horizontal(libv::ui::AlignHorizontal::center);
		panel.align_vertical(libv::ui::AlignVertical::bottom);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(125.f, 0.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 0.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(215.f, 0.f, 0.f));
	}

	SECTION("alignment: BOTTOM_RIGHT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::right);
		panel.align_vertical(libv::ui::AlignVertical::bottom);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(250.f, 0.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(290.f, 0.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 0.f, 0.f));
	}

	SECTION("alignment: BOTTOM_JUSTIFY") {
		SECTION("Justify") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		}
		SECTION("JustifyAll") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify_all);
		}
		panel.align_vertical(libv::ui::AlignVertical::bottom);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 0.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 0.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 0.f, 0.f));
	}

	CHECK(comp0.bounds().size == approx_size(40.f, 60.f, 0.f));
	CHECK(comp1.bounds().size == approx_size(50.f, 50.f, 0.f));
	CHECK(comp2.bounds().size == approx_size(60.f, 40.f, 0.f));
}

TEST_CASE("layout line: positioning with orientation", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::top);

	TestComponent comp0 = panel.add("40px, 60px", D( 20, 100, 0));
	TestComponent comp1 = panel.add("50px, 50px", D( 60,  40, 0));
	TestComponent comp2 = panel.add("60px, 40px", D(100,  10, 0));

	SECTION("orient: BOTTOM_TO_TOP") {
		panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);

		CHECK(panel.layout1(0, 0, 0) == approx_size(60, 150, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 150.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(0.f, 210.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(0.f, 260.f, 0.f));
	}

	SECTION("orient: LEFT_TO_RIGHT") {
		panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(40.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(90.f, 260.f, 0.f));
	}

	SECTION("orient: RIGHT_TO_LEFT") {
		panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(110.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(60.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(0.f, 260.f, 0.f));
	}

	SECTION("orient: TOP_TO_BOTTOM") {
		panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

		CHECK(panel.layout1(0, 0, 0) == approx_size(60, 150, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(0.f, 190.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(0.f, 150.f, 0.f));
	}

	CHECK(comp0.bounds().size == approx_size(40.f, 60.f, 0.f));
	CHECK(comp1.bounds().size == approx_size(50.f, 50.f, 0.f));
	CHECK(comp2.bounds().size == approx_size(60.f, 40.f, 0.f));
}

TEST_CASE("layout line: sizing one quad", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	SECTION("ratio") {
		TestComponent comp0 = panel.add("1r, 1r", D(20, 100, 0));

		CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().size == approx_size(400.f, 300.f, 0.f));
		CHECK(comp0.bounds().position == approx_pos(0, 0, 0));
	}

	SECTION("pixel") {
		TestComponent comp0 = panel.add("100px, 90px", D(20, 100, 0));

		CHECK(panel.layout1(0, 0, 0) == approx_size(100, 90, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().size == approx_size(100.f, 90.f, 0.f));
		CHECK(comp0.bounds().position == approx_pos(0, 0, 0));
	}

	SECTION("percent") {
		TestComponent comp0 = panel.add("50%, 10%", D(20, 100, 0));

		CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().size == approx_size(200.f, 30.f, 0.f));
		CHECK(comp0.bounds().position == approx_pos(0, 0, 0));
	}

	SECTION("dynamic") {
		TestComponent comp0 = panel.add("D, D", D(20, 100, 0));

		CHECK(panel.layout1(0, 0, 0) == approx_size(20, 100, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().size == approx_size(20.f, 100.f, 0.f));
		CHECK(comp0.bounds().position == approx_pos(0, 0, 0));
	}
}

TEST_CASE("layout line: complex test 0", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::top);

	TestComponent comp0 = panel.add("  0px 20% 1r D,  10px  0% 1r D,   0px 20% 1r D", D(10, 20, 30));
	TestComponent comp1 = panel.add("150px 20% 2r  ,  75px 10% 2r D, 150px 20% 2r  ", D(11, 21, 31));
	TestComponent comp2 = panel.add("200px 10%    D,  50px 20% 3r  , 200px 10% 3r D", D(12, 22, 32));
	TestComponent comp3 = panel.add("250px  0% 4r D,   0px 10% 4r  ,   0px  0%     ", D(13, 23, 33));

	auto pass1 = panel.layout1(0, 0, 0);
	panel.layout2(0, 0, 0, 400, 300, 200);

	CHECK(pass1 == approx_size(1270.f, 106.66667f, 257.77778f));

	CHECK(comp0.bounds() == Bounds(  0, 0, 0,  28, 300, 200));
	CHECK(comp1.bounds() == Bounds( 28, 0, 0, 106, 300, 200));
	CHECK(comp2.bounds() == Bounds(134, 0, 0, 252, 300, 200));
	CHECK(comp3.bounds() == Bounds(386, 0, 0,  14, 300,   0));
}

TEST_CASE("layout line: padding test", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	SECTION("s0") {
		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", P(1, 2, 3, 4));
		TestComponent comp2 = panel.add("30px, 40px", P(1, 2, 3, 4));
		TestComponent comp3 = panel.add("40px, 30px", P(1, 2, 3, 4));

		CHECK(panel.layout1(0, 0, 0) == approx_size(40, 106.66667f, 257.77778f));
		panel.layout2(0, 0, 0, 400, 300, 200);

		CHECK(comp0.bounds() == Bounds(  0, 0, 0,  28, 300, 200));
		CHECK(comp1.bounds() == Bounds( 28, 0, 0, 106, 300, 200));
		CHECK(comp2.bounds() == Bounds(134, 0, 0, 252, 300, 200));
		CHECK(comp3.bounds() == Bounds(386, 0, 0,  14, 300,   0));
	}
}

