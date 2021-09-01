// Project: libv.ui, File: test/libv/ui/test_layout_line.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include "test_layout_utility.hpp"
#include <libv/ui/component/panel_line.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Layout Line: empty", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
	panel.layout2(0, 0);
}

TEST_CASE("Layout Line: positioning with alignment", "[libv.ui.layout.line]") {
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

TEST_CASE("Layout Line: positioning with orientation", "[libv.ui.layout.line]") {
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

TEST_CASE("Layout Line: sizing one quad", "[libv.ui.layout.line]") {
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

TEST_CASE("Layout Line: complex test 0", "[libv.ui.layout.line]") {
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

TEST_CASE("Layout Line: padding test", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	SECTION("empty") {
		panel.padding({40, 41, 42, 43});

		CHECK(panel.layout1(0, 0, 0) == approx_size(40 + 42, 41 + 43, 0));
	}

	SECTION("padding does not effect children layouted size") {
		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));

		CHECK(panel.layout1(0, 0, 0) == approx_size(100, 40, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(  0, 0, 0, 10, 10, 0));
		CHECK(comp1.bounds() == Bounds( 10, 0, 0, 20, 20, 0));
		CHECK(comp2.bounds() == Bounds( 30, 0, 0, 30, 40, 0));
		CHECK(comp3.bounds() == Bounds( 60, 0, 0, 40, 30, 0));
	}

	SECTION("panel padding does effect children layouted positions") {
		panel.padding({40, 41, 42, 43});

		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));

		CHECK(panel.layout1(0, 0, 0) == approx_size(100 + 40 + 42, 40 + 41 + 43, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(40 +  0, 41, 0, 10, 10, 0));
		CHECK(comp1.bounds() == Bounds(40 + 10, 41, 0, 20, 20, 0));
		CHECK(comp2.bounds() == Bounds(40 + 30, 41, 0, 30, 40, 0));
		CHECK(comp3.bounds() == Bounds(40 + 60, 41, 0, 40, 30, 0));
	}
}

//TEST_CASE("Layout Line: padding doesnt affects alignment space, unless it has to", "[libv.ui.layout.line]") {
//	TestPanel<libv::ui::PanelLine> panel;
//
//	panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
//	panel.align_horizontal(libv::ui::AlignHorizontal::center);
//	panel.align_vertical(libv::ui::AlignVertical::center);
//
//	panel.padding({0, 100, 0, 0});
//
//	SECTION("padding does not effect children layouted size") {
//		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
//		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
//		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
//		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));
//
//		CHECK(panel.layout1(0, 0, 0) == approx_size(100, 40, 0));
//		panel.layout2(400, 300);
//
//		CHECK(comp0.bounds() == Bounds(  0, 0, 0, 10, 10, 0));
//		CHECK(comp1.bounds() == Bounds( 10, 0, 0, 20, 20, 0));
//		CHECK(comp2.bounds() == Bounds( 30, 0, 0, 30, 40, 0));
//		CHECK(comp3.bounds() == Bounds( 60, 0, 0, 40, 30, 0));
//	}
//
//	SECTION("panel padding does effect children layouted positions") {
//		panel.padding({40, 41, 42, 43});
//
//		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
//		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
//		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
//		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));
//
//		CHECK(panel.layout1(0, 0, 0) == approx_size(100 + 40 + 42, 40 + 41 + 43, 0));
//		panel.layout2(400, 300);
//
//		CHECK(comp0.bounds() == Bounds(40 +  0, 41, 0, 10, 10, 0));
//		CHECK(comp1.bounds() == Bounds(40 + 10, 41, 0, 20, 20, 0));
//		CHECK(comp2.bounds() == Bounds(40 + 30, 41, 0, 30, 40, 0));
//		CHECK(comp3.bounds() == Bounds(40 + 60, 41, 0, 40, 30, 0));
//	}
//}

TEST_CASE("Layout Line: spacing test", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);
	panel.spacing(5);

	SECTION("empty") {
		CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
	}

	SECTION("4 children") {
		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));

		SECTION("LEFT_TO_RIGHT") {
			panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(115, 40, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(  0, 0, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds( 15, 0, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds( 40, 0, 0, 30, 40, 0));
			CHECK(comp3.bounds() == Bounds( 75, 0, 0, 40, 30, 0));
		}

		SECTION("RIGHT_TO_LEFT") {
			panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(115, 40, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(105, 0, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds( 80, 0, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds( 45, 0, 0, 30, 40, 0));
			CHECK(comp3.bounds() == Bounds(  0, 0, 0, 40, 30, 0));
		}

		SECTION("TOP_TO_BOTTOM") {
			panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

			CHECK(panel.layout1(0, 0, 0) == approx_size(40, 115, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0, 105, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(0,  80, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(0,  35, 0, 30, 40, 0));
			CHECK(comp3.bounds() == Bounds(0,   0, 0, 40, 30, 0));
		}

		SECTION("BOTTOM_TO_TOP") {
			panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);

			CHECK(panel.layout1(0, 0, 0) == approx_size(40, 115, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0,  0, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(0, 15, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(0, 40, 0, 30, 40, 0));
			CHECK(comp3.bounds() == Bounds(0, 85, 0, 40, 30, 0));
		}
	}
}

TEST_CASE("Layout Line: padding and spacing over ratios", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::right);
	panel.align_vertical(libv::ui::AlignVertical::top);
	panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
	panel.padding({20, 10, 20, 10});
	panel.spacing(libv::ui::Spacing{6});

	TestComponent comp0 = panel.add("1r, 3r");
	TestComponent comp1 = panel.add("1r, 1r");
	TestComponent comp2 = panel.add("1r, 2r");
	TestComponent comp3 = panel.add("1r, 1r");

	CHECK(panel.layout1(0, 0, 0) == approx_size(40, 38, 0));
	panel.layout2(400, 388);

	CHECK(comp0.bounds() == Bounds(20, 228, 0, 360, 150, 0));
	CHECK(comp1.bounds() == Bounds(20, 172, 0, 360,  50, 0));
	CHECK(comp2.bounds() == Bounds(20,  66, 0, 360, 100, 0));
	CHECK(comp3.bounds() == Bounds(20,  10, 0, 360,  50, 0));
}

TEST_CASE("Layout Line: margin and spacing over ratios", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	SECTION("tr") {
		panel.align_horizontal(libv::ui::AlignHorizontal::right);
		panel.align_vertical(libv::ui::AlignVertical::top);
	}
	SECTION("cc") {
		panel.align_horizontal(libv::ui::AlignHorizontal::center);
		panel.align_vertical(libv::ui::AlignVertical::center);
	}
	SECTION("bl") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::bottom);
	}

	panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
	panel.padding({20, 10, 10, 10});
	panel.spacing(libv::ui::Spacing{10});

	TestComponent comp0 = panel.add("1r, 1r", M(19, 0,  9, 0));
	TestComponent comp1 = panel.add("1r, 1r", M(20, 0, 10, 0));
	TestComponent comp2 = panel.add("1r, 1r", M(21, 0, 11, 0));
	TestComponent comp3 = panel.add("1r, 1r", M(19, 0, 11, 0));
	TestComponent comp4 = panel.add("1r, 1r", M(20, 0, 10, 0));
	TestComponent comp5 = panel.add("1r, 1r", M(21, 0,  9, 0));

	CHECK(panel.layout1(0, 0, 0) == approx_size(32, 70, 0));
	panel.layout2(400, 340);

	CHECK(comp0.bounds() == Bounds(20, 285, 0, 370, 45, 0));
	CHECK(comp1.bounds() == Bounds(20, 230, 0, 370, 45, 0));
	CHECK(comp2.bounds() == Bounds(21, 175, 0, 368, 45, 0));
	CHECK(comp3.bounds() == Bounds(20, 120, 0, 369, 45, 0));
	CHECK(comp4.bounds() == Bounds(20,  65, 0, 370, 45, 0));
	CHECK(comp5.bounds() == Bounds(21,  10, 0, 369, 45, 0));
}

TEST_CASE("Layout Line: margin and spacing test", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);
	panel.margin(5); // Will be ignored during layout

	SECTION("empty") {
		CHECK(panel.layout1(0, 0, 0) == approx_size(0, 0, 0));
	}

	SECTION("3 children") {
		TestComponent comp0 = panel.add("10px, 10px", M(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", M(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", M(1, 4, 2, 3));

		SECTION("LEFT_TO_RIGHT") {
			panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
			CHECK(panel.layout1(0, 0, 0) == approx_size(60 + 1 + 4 + 2 + 2, 40 + 4 + 3, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 2, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(15, 3, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(37, 4, 0, 30, 40, 0));
		}

		SECTION("RIGHT_TO_LEFT") {
			panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(60 + 1 + 4 + 2 + 3, 40 + 4 + 3, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(57, 2, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(35, 3, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(1, 4, 0, 30, 40, 0));
		}

		SECTION("TOP_TO_BOTTOM") {
			panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

			CHECK(panel.layout1(0, 0, 0) == approx_size(30 + 1 + 2, 70 + 2 + 4 + 4 + 3, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 69, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(4, 47, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(1, 4, 0, 30, 40, 0));
		}

		SECTION("BOTTOM_TO_TOP") {
			panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);

			CHECK(panel.layout1(0, 0, 0) == approx_size(30 + 1 + 2, 70 + 4 + 3 + 2 + 4, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 2, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(4, 16, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(1, 40, 0, 30, 40, 0));
		}
	}

	SECTION("3 children and Spacing") {
		panel.spacing(3);

		TestComponent comp0 = panel.add("10px, 10px", M(1, 2, 4, 5));
		TestComponent comp1 = panel.add("20px, 20px", M(5, 4, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", M(1, 5, 2, 4));

		SECTION("LEFT_TO_RIGHT") {
			panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(71, 49, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 2, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(16, 4, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(39, 5, 0, 30, 40, 0));
		}

		SECTION("RIGHT_TO_LEFT") {
			panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(73, 49, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(59, 2, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(36, 4, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(1, 5, 0, 30, 40, 0));
		}

		SECTION("TOP_TO_BOTTOM") {
			panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

			CHECK(panel.layout1(0, 0, 0) == approx_size(33, 87, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 72, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(5, 49, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(1, 5, 0, 30, 40, 0));
		}

		SECTION("BOTTOM_TO_TOP") {
			panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);

			CHECK(panel.layout1(0, 0, 0) == approx_size(33, 86, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 2, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(5, 17, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(1, 42, 0, 30, 40, 0));
		}
	}

	SECTION("Child pushed away from border by big margin aligned to top") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::top);
		panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

		SECTION("s0") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 50));
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 60, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 40, 0, 10, 10, 0));
		}
		SECTION("s1") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 50, 0, 0));
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 60, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 90, 0, 10, 10, 0));
		}
	}

	SECTION("Child pushed away from border by big margin aligned to center") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::center);
		panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

		SECTION("s0") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 50));
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 60, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 40, 0, 10, 10, 0));
		}
		SECTION("s1") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 50, 0, 0));
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 60, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 50, 0, 10, 10, 0));
		}
		SECTION("noop") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 30, 0, 0));
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 40, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 45, 0, 10, 10, 0));
		}
	}

	SECTION("Child pushed away from border by big margin aligned to bottom") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::bottom);
		panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

		SECTION("s0") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 50));
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 60, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 0, 0, 10, 10, 0));
		}
		SECTION("s1") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 50, 0, 0));
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 60, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 50, 0, 10, 10, 0));
		}
	}

	SECTION("Single child justify positions") {
		SECTION("s0") {
			panel.align_horizontal(libv::ui::AlignHorizontal::left);
			panel.align_vertical(libv::ui::AlignVertical::justify);
			SECTION("s0.0") {
				panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
			}
			SECTION("s0.1") {
				panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);
			}
			SECTION("s0.2") {
				panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
			}
			SECTION("s0.3") {
				panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
			}

			TestComponent comp0 = panel.add("10px, 10px");
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 10, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 90, 0, 10, 10, 0));
		}
		SECTION("s1") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
			panel.align_vertical(libv::ui::AlignVertical::top);
			SECTION("s1.0") {
				panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
			}
			SECTION("s1.1") {
				panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);
			}
			SECTION("s1.2") {
				panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
			}
			SECTION("s1.3") {
				panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
			}

			TestComponent comp0 = panel.add("10px, 10px");
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 10, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 90, 0, 10, 10, 0));
		}
		SECTION("s2") {
			panel.align_horizontal(libv::ui::AlignHorizontal::center);
			panel.align_vertical(libv::ui::AlignVertical::justify);
			SECTION("s2.0") {
				panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
			}
			SECTION("s2.1") {
				panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);
			}
			SECTION("s2.2") {
				panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
			}
			SECTION("s2.3") {
				panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
			}

			TestComponent comp0 = panel.add("10px, 10px");
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 10, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(45, 90, 0, 10, 10, 0));
		}
		SECTION("s3") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
			panel.align_vertical(libv::ui::AlignVertical::center);
			SECTION("s3.0") {
				panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
			}
			SECTION("s3.1") {
				panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);
			}
			SECTION("s3.2") {
				panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
			}
			SECTION("s3.3") {
				panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
			}

			TestComponent comp0 = panel.add("10px, 10px");
			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 10, 0));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 45, 0, 10, 10, 0));
		}
	}

//	SECTION("Two child justify positions") {
//		SECTION("s0") {
//			panel.align_horizontal(libv::ui::AlignHorizontal::left);
//			panel.align_vertical(libv::ui::AlignVertical::justify);
////			SECTION("s0.0") {
////				panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
////			}
////			SECTION("s0.1") {
////				panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);
////			}
////			SECTION("s0.2") {
////				panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
////			}
////			SECTION("s0.3") {
////				panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
////			}
//
//			TestComponent comp0 = panel.add("10px, 10px");
//			TestComponent comp1 = panel.add("10px, 10px");
//			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 10, 0));
//			panel.layout2(100, 100);
//			CHECK(comp0.bounds() == Bounds(0, 90, 0, 10, 10, 0));
//			CHECK(comp1.bounds() == Bounds(0, 90, 0, 10, 10, 0));
//		}
////		SECTION("s1") {
////			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
////			panel.align_vertical(libv::ui::AlignVertical::top);
////			SECTION("s1.0") {
////				panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
////			}
////			SECTION("s1.1") {
////				panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);
////			}
////			SECTION("s1.2") {
////				panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
////			}
////			SECTION("s1.3") {
////				panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
////			}
////
////			TestComponent comp0 = panel.add("10px, 10px");
////			TestComponent comp1 = panel.add("10px, 10px");
////			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 10, 0));
////			panel.layout2(100, 100);
////			CHECK(comp1.bounds() == Bounds(0, 90, 0, 10, 10, 0));
////		}
////		SECTION("s2") {
////			panel.align_horizontal(libv::ui::AlignHorizontal::center);
////			panel.align_vertical(libv::ui::AlignVertical::justify);
////			SECTION("s2.0") {
////				panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
////			}
////			SECTION("s2.1") {
////				panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);
////			}
////			SECTION("s2.2") {
////				panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
////			}
////			SECTION("s2.3") {
////				panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
////			}
////
////			TestComponent comp0 = panel.add("10px, 10px");
////			TestComponent comp1 = panel.add("10px, 10px");
////			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 10, 0));
////			panel.layout2(100, 100);
////			CHECK(comp0.bounds() == Bounds(45, 90, 0, 10, 10, 0));
////			CHECK(comp1.bounds() == Bounds(45, 90, 0, 10, 10, 0));
////		}
////		SECTION("s3") {
////			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
////			panel.align_vertical(libv::ui::AlignVertical::center);
////			SECTION("s3.0") {
////				panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
////			}
////			SECTION("s3.1") {
////				panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);
////			}
////			SECTION("s3.2") {
////				panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
////			}
////			SECTION("s3.3") {
////				panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);
////			}
////
////			TestComponent comp0 = panel.add("10px, 10px");
////			TestComponent comp1 = panel.add("10px, 10px");
////			CHECK(panel.layout1(0, 0, 0) == approx_size(10, 10, 0));
////			panel.layout2(100, 100);
////			CHECK(comp0.bounds() == Bounds(0, 45, 0, 10, 10, 0));
////			CHECK(comp1.bounds() == Bounds(0, 45, 0, 10, 10, 0));
////		}
//	}

	SECTION("3 children and Spacing and Alignment center") {
		panel.align_horizontal(libv::ui::AlignHorizontal::center);
		panel.align_vertical(libv::ui::AlignVertical::center);
		panel.spacing(3);

		TestComponent comp0 = panel.add("10px, 10px", M(1, 2, 4, 5));
		TestComponent comp1 = panel.add("20px, 20px", M(5, 4, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", M(1, 5, 2, 4));

		SECTION("LEFT_TO_RIGHT") {
			panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(71, 49, 0));
			panel.layout2(400, 300);

			const auto cx = std::round((400.f - 71.f) * 0.5f);
			const auto cy = [](float sy) { return std::round((300.f - sy) * 0.5f); };
			CHECK(comp0.bounds() == Bounds(cx +  1, cy(10), 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(cx + 16, cy(20), 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(cx + 39, cy(40), 0, 30, 40, 0));
		}

		SECTION("RIGHT_TO_LEFT") {
			panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(73, 49, 0));
			panel.layout2(400, 300);

			const auto cx = std::round((400.f - 73.f) * 0.5f);
			const auto cy = [](float sy) { return std::round((300.f - sy) * 0.5f); };
			CHECK(comp0.bounds() == Bounds(cx + 59, cy(10), 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(cx + 36, cy(20), 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(cx +  1, cy(40), 0, 30, 40, 0));
		}
	}

	SECTION("3 children and Spacing and Alignment justify with even final gap values") {
		panel.spacing(3);
		panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		panel.align_vertical(libv::ui::AlignVertical::justify);

		TestComponent comp0 = panel.add("10px, 10px", M(1, 2, 4, 5));
		TestComponent comp1 = panel.add("20px, 20px", M(5, 4, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", M(1, 5, 2, 4));

		SECTION("LEFT_TO_RIGHT") {
			panel.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(71, 49, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(  1, 285, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(180, 279, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(400-30-2, 256, 0, 30, 40, 0));
		}

		SECTION("RIGHT_TO_LEFT") {
			panel.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

			CHECK(panel.layout1(0, 0, 0) == approx_size(73, 49, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(386, 285, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(199, 279, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(  1, 256, 0, 30, 40, 0));
		}

		SECTION("TOP_TO_BOTTOM") {
			panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

			CHECK(panel.layout1(0, 0, 0) == approx_size(33, 87, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 285, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(5, 155, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(1,   5, 0, 30, 40, 0));
		}

		SECTION("BOTTOM_TO_TOP") {
			panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);

			CHECK(panel.layout1(0, 0, 0) == approx_size(33, 86, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1,   2, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(5, 124, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(1, 256, 0, 30, 40, 0));
		}
	}

	SECTION("3/4 children margin and Alignment justify, Justify Gap asymmetric sharing due to margin") {
		panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		panel.align_vertical(libv::ui::AlignVertical::justify);

		SECTION("TOP_TO_BOTTOM everything to one gap") {
			panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 0));
			TestComponent comp1 = panel.add("20px, 20px", M(0, 200, 0, 0));
			TestComponent comp2 = panel.add("30px, 40px", M(0, 0, 0, 0));

			CHECK(panel.layout1(0, 0, 0) == approx_size(30, 270, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0, 290, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(0, 240, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(0,   0, 0, 30, 40, 0));
		}

		SECTION("BOTTOM_TO_TOP everything to one gap") {
			panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);

			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 0));
			TestComponent comp1 = panel.add("20px, 20px", M(0, 200, 0, 0));
			TestComponent comp2 = panel.add("30px, 40px", M(0, 0, 0, 0));

			CHECK(panel.layout1(0, 0, 0) == approx_size(30, 270, 0));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0,   0, 0, 10, 10, 0));
			CHECK(comp1.bounds() == Bounds(0, 210, 0, 20, 20, 0));
			CHECK(comp2.bounds() == Bounds(0, 260, 0, 30, 40, 0));
		}

		SECTION("TOP_TO_BOTTOM split between multiple, but not all") {
			panel.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

			TestComponent comp0 = panel.add("1px, 10px", M(0, 0, 0, 0));
			TestComponent comp1 = panel.add("1px, 20px", M(0, 100, 0, 0));
			TestComponent comp2 = panel.add("1px, 40px", M(0, 20, 0, 0));
			TestComponent comp3 = panel.add("1px, 80px", M(0, 0, 0, 0));

			CHECK(panel.layout1(0, 0, 0) == approx_size(1, 270, 0));
			panel.layout2(400, 300);

			// 0 |--10--| 0 |--20--| 100 |--40--| 20 |--80--| 0 | Unused
			//            ^           ^            ^              30
			//          +20           ^            ^              10
			//          +25           ^           +5               0
			// 0 |--10--| 25 |--20--| 100 |--40--| 25 |--80--| 0
			CHECK(comp0.bounds() == Bounds(0, 290, 0, 1, 10, 0));
			CHECK(comp1.bounds() == Bounds(0, 245, 0, 1, 20, 0));
			CHECK(comp2.bounds() == Bounds(0, 105, 0, 1, 40, 0));
			CHECK(comp3.bounds() == Bounds(0,   0, 0, 1, 80, 0));
		}

		SECTION("BOTTOM_TO_TOP split between multiple, but not all") {
			panel.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);

			TestComponent comp0 = panel.add("1px, 10px", M(0, 0, 0, 0));
			TestComponent comp1 = panel.add("1px, 20px", M(0, 100, 0, 0));
			TestComponent comp2 = panel.add("1px, 40px", M(0, 20, 0, 0));
			TestComponent comp3 = panel.add("1px, 80px", M(0, 0, 0, 0));

			CHECK(panel.layout1(0, 0, 0) == approx_size(1, 270, 0));
			panel.layout2(400, 300);

			// 0 |--80--| 0 |--40--| 20 |--20--| 100 |--10--| 0 | Unused
			//            ^           ^            ^              30
			//          +20           ^            ^              10
			//          +25          +5            ^               0
			// 0 |--80--| 25 |--40--| 25 |--20--| 100 |--10--| 0
			CHECK(comp0.bounds() == Bounds(0,   0, 0, 1, 10, 0));
			CHECK(comp1.bounds() == Bounds(0, 110, 0, 1, 20, 0));
			CHECK(comp2.bounds() == Bounds(0, 155, 0, 1, 40, 0));
			CHECK(comp3.bounds() == Bounds(0, 220, 0, 1, 80, 0));
		}
	}
}
