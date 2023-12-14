// Project: libv.ui, File: test/libv/ui/test_layout_line.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include "test_layout_utility.hpp"
#include <libv/ui/component/panel_line.hpp>

// #include <libv/log/log.hpp>
// #include <iostream>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Layout Line: empty", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	CHECK(panel.layout1(0, 0) == approx_size(0, 0));
	panel.layout2(0, 0);
}

TEST_CASE("Layout Line: positioning with alignment", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	TestComponent comp0 = panel.add("40px, 60px", D( 20, 100));
	TestComponent comp1 = panel.add("50px, 50px", D( 60,  40));
	TestComponent comp2 = panel.add("60px, 40px", D(100,  10));

	panel.orientation(libv::ui::Orientation::right);

	SECTION("alignment: TOP_LEFT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::top);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f));
		CHECK(comp1.bounds().position == approx_pos(40.f, 250.f));
		CHECK(comp2.bounds().position == approx_pos(90.f, 260.f));
	}

	SECTION("alignment: TOP_CENTER") {
		panel.align_horizontal(libv::ui::AlignHorizontal::center);
		panel.align_vertical(libv::ui::AlignVertical::top);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(125.f, 240.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 250.f));
		CHECK(comp2.bounds().position == approx_pos(215.f, 260.f));
	}

	SECTION("alignment: TOP_RIGHT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::right);
		panel.align_vertical(libv::ui::AlignVertical::top);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(250.f, 240.f));
		CHECK(comp1.bounds().position == approx_pos(290.f, 250.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 260.f));
	}

	SECTION("alignment: TOP_JUSTIFY") {
		SECTION("Justify") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		}
		SECTION("JustifyAll") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify_all);
		}
		panel.align_vertical(libv::ui::AlignVertical::top);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 250.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 260.f));
	}

	SECTION("alignment: CENTER_LEFT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::center);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 120.f));
		CHECK(comp1.bounds().position == approx_pos(40.f, 125.f));
		CHECK(comp2.bounds().position == approx_pos(90.f, 130.f));
	}

	SECTION("alignment: CENTER_CENTER") {
		panel.align_horizontal(libv::ui::AlignHorizontal::center);
		panel.align_vertical(libv::ui::AlignVertical::center);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(125.f, 120.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 125.f));
		CHECK(comp2.bounds().position == approx_pos(215.f, 130.f));
	}

	SECTION("alignment: CENTER_RIGHT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::right);
		panel.align_vertical(libv::ui::AlignVertical::center);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(250.f, 120.f));
		CHECK(comp1.bounds().position == approx_pos(290.f, 125.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 130.f));
	}

	SECTION("alignment: CENTER_JUSTIFY") {
		SECTION("Justify") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		}
		SECTION("JustifyAll") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify_all);
		}
		panel.align_vertical(libv::ui::AlignVertical::center);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 120.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 125.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 130.f));
	}

	SECTION("alignment: BOTTOM_LEFT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::bottom);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(40.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(90.f, 0.f));
	}

	SECTION("alignment: BOTTOM_CENTER") {
		panel.align_horizontal(libv::ui::AlignHorizontal::center);
		panel.align_vertical(libv::ui::AlignVertical::bottom);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(125.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(215.f, 0.f));
	}

	SECTION("alignment: BOTTOM_RIGHT") {
		panel.align_horizontal(libv::ui::AlignHorizontal::right);
		panel.align_vertical(libv::ui::AlignVertical::bottom);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(250.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(290.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 0.f));
	}

	SECTION("alignment: BOTTOM_JUSTIFY") {
		SECTION("Justify") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		}
		SECTION("JustifyAll") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify_all);
		}
		panel.align_vertical(libv::ui::AlignVertical::bottom);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 0.f));
		CHECK(comp1.bounds().position == approx_pos(165.f, 0.f));
		CHECK(comp2.bounds().position == approx_pos(340.f, 0.f));
	}

	CHECK(comp0.bounds().size == approx_size(40.f, 60.f));
	CHECK(comp1.bounds().size == approx_size(50.f, 50.f));
	CHECK(comp2.bounds().size == approx_size(60.f, 40.f));
}

TEST_CASE("Layout Line: positioning with orientation", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::top);

	TestComponent comp0 = panel.add("40px, 60px", D( 20, 100));
	TestComponent comp1 = panel.add("50px, 50px", D( 60,  40));
	TestComponent comp2 = panel.add("60px, 40px", D(100,  10));

	SECTION("orient: Orientation::up") {
		panel.orientation(libv::ui::Orientation::up);

		CHECK(panel.layout1(0, 0) == approx_size(60, 150));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 150.f));
		CHECK(comp1.bounds().position == approx_pos(0.f, 210.f));
		CHECK(comp2.bounds().position == approx_pos(0.f, 260.f));
	}

	SECTION("orient: Orientation::right") {
		panel.orientation(libv::ui::Orientation::right);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f));
		CHECK(comp1.bounds().position == approx_pos(40.f, 250.f));
		CHECK(comp2.bounds().position == approx_pos(90.f, 260.f));
	}

	SECTION("orient: Orientation::left") {
		panel.orientation(libv::ui::Orientation::left);

		CHECK(panel.layout1(0, 0) == approx_size(150, 60));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(110.f, 240.f));
		CHECK(comp1.bounds().position == approx_pos(60.f, 250.f));
		CHECK(comp2.bounds().position == approx_pos(0.f, 260.f));
	}

	SECTION("orient: Orientation::down") {
		panel.orientation(libv::ui::Orientation::down);

		CHECK(panel.layout1(0, 0) == approx_size(60, 150));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f));
		CHECK(comp1.bounds().position == approx_pos(0.f, 190.f));
		CHECK(comp2.bounds().position == approx_pos(0.f, 150.f));
	}

	CHECK(comp0.bounds().size == approx_size(40.f, 60.f));
	CHECK(comp1.bounds().size == approx_size(50.f, 50.f));
	CHECK(comp2.bounds().size == approx_size(60.f, 40.f));
}

TEST_CASE("Layout Line: sizing one quad", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::right);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	SECTION("ratio") {
		TestComponent comp0 = panel.add("1r, 1r", D(20, 100));

		CHECK(panel.layout1(0, 0) == approx_size(0, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().size == approx_size(400.f, 300.f));
		CHECK(comp0.bounds().position == approx_pos(0, 0));
	}

	SECTION("pixel") {
		TestComponent comp0 = panel.add("100px, 90px", D(20, 100));

		CHECK(panel.layout1(0, 0) == approx_size(100, 90));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().size == approx_size(100.f, 90.f));
		CHECK(comp0.bounds().position == approx_pos(0, 0));
	}

	SECTION("percent") {
		TestComponent comp0 = panel.add("50%, 10%", D(20, 100));

		CHECK(panel.layout1(0, 0) == approx_size(0, 0));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().size == approx_size(200.f, 30.f));
		CHECK(comp0.bounds().position == approx_pos(0, 0));
	}

	SECTION("dynamic") {
		TestComponent comp0 = panel.add("D, D", D(20, 100));

		CHECK(panel.layout1(0, 0) == approx_size(20, 100));
		panel.layout2(400, 300);

		CHECK(comp0.bounds().size == approx_size(20.f, 100.f));
		CHECK(comp0.bounds().position == approx_pos(0, 0));
	}
}

TEST_CASE("Layout Line: padding test", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::right);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	SECTION("empty") {
		panel.padding({40, 41, 42, 43});

		CHECK(panel.layout1(0, 0) == approx_size(40 + 42, 41 + 43));
	}

	SECTION("padding does not effect children layouted size") {
		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));

		CHECK(panel.layout1(0, 0) == approx_size(100, 40));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(  0, 0, 10, 10));
		CHECK(comp1.bounds() == Bounds( 10, 0, 20, 20));
		CHECK(comp2.bounds() == Bounds( 30, 0, 30, 40));
		CHECK(comp3.bounds() == Bounds( 60, 0, 40, 30));
	}

	SECTION("panel padding does effect children layouted positions") {
		panel.padding({40, 41, 42, 43});

		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));

		CHECK(panel.layout1(0, 0) == approx_size(100 + 40 + 42, 40 + 41 + 43));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(40 +  0, 41, 10, 10));
		CHECK(comp1.bounds() == Bounds(40 + 10, 41, 20, 20));
		CHECK(comp2.bounds() == Bounds(40 + 30, 41, 30, 40));
		CHECK(comp3.bounds() == Bounds(40 + 60, 41, 40, 30));
	}
}

TEST_CASE("Layout Line: margin / padding and alignment space", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::right);
	panel.align_horizontal(libv::ui::AlignHorizontal::center);
	panel.align_vertical(libv::ui::AlignVertical::center);

	panel.padding({0, 100, 0, 0});

	SECTION("padding does not effect children layouted size") {
		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));

		CHECK(panel.layout1(0, 0) == approx_size(100, 140));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(150, 195, 10, 10));
		CHECK(comp1.bounds() == Bounds(160, 190, 20, 20));
		CHECK(comp2.bounds() == Bounds(180, 180, 30, 40));
		CHECK(comp3.bounds() == Bounds(210, 185, 40, 30));
	}

	SECTION("margin does effect children layouted size alignment") {
		TestComponent comp0 = panel.add("10px, 10px", M(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", M(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", M(1, 4, 3, 2));
		TestComponent comp3 = panel.add("40px, 30px", M(2, 3, 4, 1));

		CHECK(panel.layout1(0, 0) == approx_size(120, 146));
		panel.layout2(400, 300);

		CHECK(comp0.bounds() == Bounds(141, 194, 10, 10));
		CHECK(comp1.bounds() == Bounds(158, 191, 20, 20));
		CHECK(comp2.bounds() == Bounds(181, 181, 30, 40));
		CHECK(comp3.bounds() == Bounds(216, 186, 40, 30));
	}
}

TEST_CASE("Layout Line: spacing test", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);
	panel.spacing(5);

	SECTION("empty") {
		CHECK(panel.layout1(0, 0) == approx_size(0, 0));
	}

	SECTION("4 children") {
		TestComponent comp0 = panel.add("10px, 10px", P(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", P(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", P(1, 4, 3, 2));
		TestComponent comp3 = panel.add("40px, 30px", P(2, 3, 4, 1));

		SECTION("Orientation::right") {
			panel.orientation(libv::ui::Orientation::right);

			CHECK(panel.layout1(0, 0) == approx_size(115, 40));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(  0, 0, 10, 10));
			CHECK(comp1.bounds() == Bounds( 15, 0, 20, 20));
			CHECK(comp2.bounds() == Bounds( 40, 0, 30, 40));
			CHECK(comp3.bounds() == Bounds( 75, 0, 40, 30));
		}

		SECTION("Orientation::left") {
			panel.orientation(libv::ui::Orientation::left);

			CHECK(panel.layout1(0, 0) == approx_size(115, 40));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(105, 0, 10, 10));
			CHECK(comp1.bounds() == Bounds( 80, 0, 20, 20));
			CHECK(comp2.bounds() == Bounds( 45, 0, 30, 40));
			CHECK(comp3.bounds() == Bounds(  0, 0, 40, 30));
		}

		SECTION("Orientation::down") {
			panel.orientation(libv::ui::Orientation::down);

			CHECK(panel.layout1(0, 0) == approx_size(40, 115));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0, 105, 10, 10));
			CHECK(comp1.bounds() == Bounds(0,  80, 20, 20));
			CHECK(comp2.bounds() == Bounds(0,  35, 30, 40));
			CHECK(comp3.bounds() == Bounds(0,   0, 40, 30));
		}

		SECTION("Orientation::up") {
			panel.orientation(libv::ui::Orientation::up);

			CHECK(panel.layout1(0, 0) == approx_size(40, 115));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0,  0, 10, 10));
			CHECK(comp1.bounds() == Bounds(0, 15, 20, 20));
			CHECK(comp2.bounds() == Bounds(0, 40, 30, 40));
			CHECK(comp3.bounds() == Bounds(0, 85, 40, 30));
		}
	}
}

TEST_CASE("Layout Line: padding and spacing over ratios", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_vertical(libv::ui::AlignVertical::top);
	panel.align_horizontal(libv::ui::AlignHorizontal::right);
	panel.orientation(libv::ui::Orientation::down);
	panel.padding({20, 10, 20, 10});
	panel.spacing(libv::ui::Spacing{6});

	TestComponent comp0 = panel.add("1r, 3r");
	TestComponent comp1 = panel.add("1r, 1r");
	TestComponent comp2 = panel.add("1r, 2r");
	TestComponent comp3 = panel.add("1r, 1r");

	CHECK(panel.layout1(0, 0) == approx_size(40, 38));
	panel.layout2(400, 388);

	CHECK(comp0.bounds() == Bounds(20, 228, 360, 150));
	CHECK(comp1.bounds() == Bounds(20, 172, 360,  50));
	CHECK(comp2.bounds() == Bounds(20,  66, 360, 100));
	CHECK(comp3.bounds() == Bounds(20,  10, 360,  50));
}

TEST_CASE("Layout Line: margin and spacing over ratios", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.padding({20, 10, 10, 10});
	panel.spacing(libv::ui::Spacing{10});

	TestComponent comp0 = panel.add("1r, 1r", M(19, 0,  9, 0));
	TestComponent comp1 = panel.add("1r, 1r", M(20, 0, 10, 0));
	TestComponent comp2 = panel.add("1r, 1r", M(21, 0, 11, 0));
	TestComponent comp3 = panel.add("1r, 1r", M(19, 0, 11, 0));
	TestComponent comp4 = panel.add("1r, 1r", M(20, 0, 10, 0));
	TestComponent comp5 = panel.add("1r, 1r", M(21, 0,  9, 0));

	SECTION("down") {
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

		panel.orientation(libv::ui::Orientation::down);

		CHECK(panel.layout1(0, 0) == approx_size(62, 70));
		panel.layout2(400, 340);

		CHECK(comp0.bounds() == Bounds(39, 285, 342, 45));
		CHECK(comp1.bounds() == Bounds(40, 230, 340, 45));
		CHECK(comp2.bounds() == Bounds(41, 175, 338, 45));
		CHECK(comp3.bounds() == Bounds(39, 120, 340, 45));
		CHECK(comp4.bounds() == Bounds(40,  65, 340, 45));
		CHECK(comp5.bounds() == Bounds(41,  10, 340, 45));
	}

	SECTION("left") {
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

		panel.orientation(libv::ui::Orientation::left);

		CHECK(panel.layout1(0, 0) == approx_size(260, 20));
		panel.layout2(400, 340);

		CHECK(comp0.bounds() == Bounds(358, 10, 23, 320));
		CHECK(comp1.bounds() == Bounds(295, 10, 24, 320));
		CHECK(comp2.bounds() == Bounds(231, 10, 23, 320));
		CHECK(comp3.bounds() == Bounds(166, 10, 23, 320));
		CHECK(comp4.bounds() == Bounds(103, 10, 24, 320));
		CHECK(comp5.bounds() == Bounds( 41, 10, 23, 320));
	}
}

TEST_CASE("Layout Line: Justify positions", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);
	panel.margin(5); // Will be ignored during layout

	SECTION("Single child justify positions") {
		SECTION("s0") {
			panel.align_horizontal(libv::ui::AlignHorizontal::left);
			panel.align_vertical(libv::ui::AlignVertical::justify);

			SECTION("s0.0") {
				panel.orientation(libv::ui::Orientation::right);
			}
			SECTION("s0.1") {
				panel.orientation(libv::ui::Orientation::left);
			}
			SECTION("s0.2") {
				panel.orientation(libv::ui::Orientation::down);
			}
			SECTION("s0.3") {
				panel.orientation(libv::ui::Orientation::up);
			}

			TestComponent comp0 = panel.add("10px, 10px");
			CHECK(panel.layout1(0, 0) == approx_size(10, 10));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 90, 10, 10));
		}

		SECTION("s1") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
			panel.align_vertical(libv::ui::AlignVertical::top);

			SECTION("s1.0") {
				panel.orientation(libv::ui::Orientation::right);
			}
			SECTION("s1.1") {
				panel.orientation(libv::ui::Orientation::left);
			}
			SECTION("s1.2") {
				panel.orientation(libv::ui::Orientation::down);
			}
			SECTION("s1.3") {
				panel.orientation(libv::ui::Orientation::up);
			}

			TestComponent comp0 = panel.add("10px, 10px");
			CHECK(panel.layout1(0, 0) == approx_size(10, 10));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 90, 10, 10));
		}

		SECTION("s2") {
			panel.align_horizontal(libv::ui::AlignHorizontal::center);
			panel.align_vertical(libv::ui::AlignVertical::justify);

			SECTION("s2.0") {
				panel.orientation(libv::ui::Orientation::right);
			}
			SECTION("s2.1") {
				panel.orientation(libv::ui::Orientation::left);
			}
			SECTION("s2.2") {
				panel.orientation(libv::ui::Orientation::down);
			}
			SECTION("s2.3") {
				panel.orientation(libv::ui::Orientation::up);
			}

			TestComponent comp0 = panel.add("10px, 10px");
			CHECK(panel.layout1(0, 0) == approx_size(10, 10));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(45, 90, 10, 10));
		}

		SECTION("s3") {
			panel.align_horizontal(libv::ui::AlignHorizontal::justify);
			panel.align_vertical(libv::ui::AlignVertical::center);
			SECTION("s3.0") {
				panel.orientation(libv::ui::Orientation::right);
			}
			SECTION("s3.1") {
				panel.orientation(libv::ui::Orientation::left);
			}
			SECTION("s3.2") {
				panel.orientation(libv::ui::Orientation::down);
			}
			SECTION("s3.3") {
				panel.orientation(libv::ui::Orientation::up);
			}

			TestComponent comp0 = panel.add("10px, 10px");
			CHECK(panel.layout1(0, 0) == approx_size(10, 10));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 45, 10, 10));
		}
	}

	SECTION("Two child justify positions") {
		SECTION("s0") {
			panel.align_horizontal(libv::ui::AlignHorizontal::left);
			panel.align_vertical(libv::ui::AlignVertical::justify);

			TestComponent comp0 = panel.add("10px, 10px");
			TestComponent comp1 = panel.add("10px, 10px");

			SECTION("s0.0") {
				panel.orientation(libv::ui::Orientation::right);
				panel.layout2(100, 100);
				CHECK(comp0.bounds() == Bounds(0, 90, 10, 10));
				CHECK(comp1.bounds() == Bounds(10, 90, 10, 10));
			}
			SECTION("s0.1") {
				panel.orientation(libv::ui::Orientation::left);
				panel.layout2(100, 100);
				CHECK(comp0.bounds() == Bounds(10, 90, 10, 10));
				CHECK(comp1.bounds() == Bounds(0, 90, 10, 10));
			}
			SECTION("s0.2") {
				panel.orientation(libv::ui::Orientation::down);
				panel.layout2(100, 100);
				CHECK(comp0.bounds() == Bounds(0, 90, 10, 10));
				CHECK(comp1.bounds() == Bounds(0, 0, 10, 10));
			}
			SECTION("s0.3") {
				panel.orientation(libv::ui::Orientation::up);
				panel.layout2(100, 100);
				CHECK(comp0.bounds() == Bounds(0, 0, 10, 10));
				CHECK(comp1.bounds() == Bounds(0, 90, 10, 10));
			}
		}
		// SECTION("s1") {
		// 	panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		// 	panel.align_vertical(libv::ui::AlignVertical::top);
		// 	SECTION("s1.0") {
		// 		panel.orientation(libv::ui::Orientation::right);
		// 	}
		// 	SECTION("s1.1") {
		// 		panel.orientation(libv::ui::Orientation::left);
		// 	}
		// 	SECTION("s1.2") {
		// 		panel.orientation(libv::ui::Orientation::down);
		// 	}
		// 	SECTION("s1.3") {
		// 		panel.orientation(libv::ui::Orientation::up);
		// 	}
		//
		// 	TestComponent comp0 = panel.add("10px, 10px");
		// 	TestComponent comp1 = panel.add("10px, 10px");
		// 	CHECK(panel.layout1(0, 0) == approx_size(10, 10));
		// 	panel.layout2(100, 100);
		// 	CHECK(comp1.bounds() == Bounds(0, 90, 10, 10));
		// }
		// SECTION("s2") {
		// 	panel.align_horizontal(libv::ui::AlignHorizontal::center);
		// 	panel.align_vertical(libv::ui::AlignVertical::justify);
		// 	SECTION("s2.0") {
		// 		panel.orientation(libv::ui::Orientation::right);
		// 	}
		// 	SECTION("s2.1") {
		// 		panel.orientation(libv::ui::Orientation::left);
		// 	}
		// 	SECTION("s2.2") {
		// 		panel.orientation(libv::ui::Orientation::down);
		// 	}
		// 	SECTION("s2.3") {
		// 		panel.orientation(libv::ui::Orientation::up);
		// 	}
		//
		// 	TestComponent comp0 = panel.add("10px, 10px");
		// 	TestComponent comp1 = panel.add("10px, 10px");
		// 	CHECK(panel.layout1(0, 0) == approx_size(10, 10));
		// 	panel.layout2(100, 100);
		// 	CHECK(comp0.bounds() == Bounds(45, 90, 10, 10));
		// 	CHECK(comp1.bounds() == Bounds(45, 90, 10, 10));
		// }
		// SECTION("s3") {
		// 	panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		// 	panel.align_vertical(libv::ui::AlignVertical::center);
		// 	SECTION("s3.0") {
		// 		panel.orientation(libv::ui::Orientation::right);
		// 	}
		// 	SECTION("s3.1") {
		// 		panel.orientation(libv::ui::Orientation::left);
		// 	}
		// 	SECTION("s3.2") {
		// 		panel.orientation(libv::ui::Orientation::down);
		// 	}
		// 	SECTION("s3.3") {
		// 		panel.orientation(libv::ui::Orientation::up);
		// 	}
		//
		// 	TestComponent comp0 = panel.add("10px, 10px");
		// 	TestComponent comp1 = panel.add("10px, 10px");
		// 	CHECK(panel.layout1(0, 0) == approx_size(10, 10));
		// 	panel.layout2(100, 100);
		// 	CHECK(comp0.bounds() == Bounds(0, 45, 10, 10));
		// 	CHECK(comp1.bounds() == Bounds(0, 45, 10, 10));
		// }
	}
}

TEST_CASE("Layout Line: margin and spacing test", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);
	panel.margin(5); // Will be ignored during layout

	SECTION("empty") {
		CHECK(panel.layout1(0, 0) == approx_size(0, 0));
	}

	SECTION("3 children") {
		TestComponent comp0 = panel.add("10px, 10px", M(1, 2, 3, 4));
		TestComponent comp1 = panel.add("20px, 20px", M(4, 3, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", M(1, 4, 2, 3));

		SECTION("Orientation::right") {
			panel.orientation(libv::ui::Orientation::right);
			CHECK(panel.layout1(0, 0) == approx_size(73, 47));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 2, 10, 10));
			CHECK(comp1.bounds() == Bounds(1+3+4+10, 3, 20, 20));
			CHECK(comp2.bounds() == Bounds(1+3+4+2+1+10+20, 4, 30, 40));
		}

		SECTION("Orientation::left") {
			panel.orientation(libv::ui::Orientation::left);

			CHECK(panel.layout1(0, 0) == approx_size(73, 47));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(60, 2, 10, 10));
			CHECK(comp1.bounds() == Bounds(37, 3, 20, 20));
			CHECK(comp2.bounds() == Bounds(1, 4, 30, 40));
		}

		SECTION("Orientation::down") {
			panel.orientation(libv::ui::Orientation::down);

			CHECK(panel.layout1(0, 0) == approx_size(33, 87));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 73, 10, 10));
			CHECK(comp1.bounds() == Bounds(4, 50, 20, 20));
			CHECK(comp2.bounds() == Bounds(1, 4, 30, 40));
		}

		SECTION("Orientation::up") {
			panel.orientation(libv::ui::Orientation::up);

			CHECK(panel.layout1(0, 0) == approx_size(33, 87));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 2, 10, 10));
			CHECK(comp1.bounds() == Bounds(4, 19, 20, 20));
			CHECK(comp2.bounds() == Bounds(1, 44, 30, 40));
		}
	}

	SECTION("3 children and Spacing") {
		panel.spacing(3);

		TestComponent comp0 = panel.add("10px, 10px", M(1, 2, 4, 5));
		TestComponent comp1 = panel.add("20px, 20px", M(5, 4, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", M(1, 5, 2, 4));

		SECTION("Orientation::right") {
			panel.orientation(libv::ui::Orientation::right);

			CHECK(panel.layout1(0, 0) == approx_size(81, 49));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 2, 10, 10));
			CHECK(comp1.bounds() == Bounds(23, 4, 20, 20));
			CHECK(comp2.bounds() == Bounds(49, 5, 30, 40));
		}

		SECTION("Orientation::left") {
			panel.orientation(libv::ui::Orientation::left);

			CHECK(panel.layout1(0, 0) == approx_size(81, 49));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(67, 2, 10, 10));
			CHECK(comp1.bounds() == Bounds(41, 4, 20, 20));
			CHECK(comp2.bounds() == Bounds(1, 5, 30, 40));
		}

		SECTION("Orientation::down") {
			panel.orientation(libv::ui::Orientation::down);

			CHECK(panel.layout1(0, 0) == approx_size(33, 97));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 82, 10, 10));
			CHECK(comp1.bounds() == Bounds(5, 56, 20, 20));
			CHECK(comp2.bounds() == Bounds(1, 5, 30, 40));
		}

		SECTION("Orientation::up") {
			panel.orientation(libv::ui::Orientation::up);

			CHECK(panel.layout1(0, 0) == approx_size(33, 97));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 2, 10, 10));
			CHECK(comp1.bounds() == Bounds(5, 24, 20, 20));
			CHECK(comp2.bounds() == Bounds(1, 53, 30, 40));
		}
	}

	SECTION("Child pushed away from border by big margin aligned to top") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::top);
		panel.orientation(libv::ui::Orientation::left);

		SECTION("s0") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 50));
			CHECK(panel.layout1(0, 0) == approx_size(10, 60));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 40, 10, 10));
		}
		SECTION("s1") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 50, 0, 0));
			CHECK(panel.layout1(0, 0) == approx_size(10, 60));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 90, 10, 10));
		}
	}

	SECTION("Child pushed away from center by big margin aligned to center") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::center);
		panel.orientation(libv::ui::Orientation::left);

		SECTION("s0") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 50));
			CHECK(panel.layout1(0, 0) == approx_size(10, 60));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 20, 10, 10));
		}
		SECTION("s1") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 50, 0, 0));
			CHECK(panel.layout1(0, 0) == approx_size(10, 60));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 70, 10, 10));
		}
		SECTION("s2") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 30, 0, 0));
			CHECK(panel.layout1(0, 0) == approx_size(10, 40));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 60, 10, 10));
		}
	}

	SECTION("Child pushed away from border by big margin aligned to bottom") {
		panel.align_horizontal(libv::ui::AlignHorizontal::left);
		panel.align_vertical(libv::ui::AlignVertical::bottom);
		panel.orientation(libv::ui::Orientation::left);

		SECTION("s0") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 50));
			CHECK(panel.layout1(0, 0) == approx_size(10, 60));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 0, 10, 10));
		}
		SECTION("s1") {
			TestComponent comp0 = panel.add("10px, 10px", M(0, 50, 0, 0));
			CHECK(panel.layout1(0, 0) == approx_size(10, 60));
			panel.layout2(100, 100);
			CHECK(comp0.bounds() == Bounds(0, 50, 10, 10));
		}
	}

	SECTION("3 children and Spacing and Alignment justify with even final gap values") {
		panel.spacing(3);
		panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		panel.align_vertical(libv::ui::AlignVertical::justify);

		TestComponent comp0 = panel.add("10px, 10px", M(1, 2, 4, 5));
		TestComponent comp1 = panel.add("20px, 20px", M(5, 4, 2, 1));
		TestComponent comp2 = panel.add("30px, 40px", M(1, 5, 2, 4));

		SECTION("Orientation::right") {
			panel.orientation(libv::ui::Orientation::right);

			CHECK(panel.layout1(0, 0) == approx_size(81, 49));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(  1, 285, 10, 10));
			CHECK(comp1.bounds() == Bounds(183, 279, 20, 20));
			CHECK(comp2.bounds() == Bounds(400-30-2, 256, 30, 40));
		}

		SECTION("Orientation::left") {
			panel.orientation(libv::ui::Orientation::left);

			CHECK(panel.layout1(0, 0) == approx_size(81, 49));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(386, 285, 10, 10));
			CHECK(comp1.bounds() == Bounds(201, 279, 20, 20));
			CHECK(comp2.bounds() == Bounds(  1, 256, 30, 40));
		}

		SECTION("Orientation::down") {
			panel.orientation(libv::ui::Orientation::down);

			CHECK(panel.layout1(0, 0) == approx_size(33, 97));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1, 285, 10, 10));
			CHECK(comp1.bounds() == Bounds(5, 158, 20, 20));
			CHECK(comp2.bounds() == Bounds(1,   5, 30, 40));
		}

		SECTION("Orientation::up") {
			panel.orientation(libv::ui::Orientation::up);

			CHECK(panel.layout1(0, 0) == approx_size(33, 97));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(1,   2, 10, 10));
			CHECK(comp1.bounds() == Bounds(5, 126, 20, 20));
			CHECK(comp2.bounds() == Bounds(1, 256, 30, 40));
		}
	}

	SECTION("3/4 children margin and Alignment justify, Justify Gap sharing") {
		panel.align_horizontal(libv::ui::AlignHorizontal::justify);
		panel.align_vertical(libv::ui::AlignVertical::justify);

		SECTION("Orientation::down") {
			panel.orientation(libv::ui::Orientation::down);

			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 0));
			TestComponent comp1 = panel.add("20px, 20px", M(0, 200, 0, 0));
			TestComponent comp2 = panel.add("30px, 40px", M(0, 0, 0, 0));

			CHECK(panel.layout1(0, 0) == approx_size(30, 270));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0, 290, 10, 10));
			CHECK(comp1.bounds() == Bounds(0, 255, 20, 20));
			CHECK(comp2.bounds() == Bounds(0,   0, 30, 40));
		}

		SECTION("Orientation::up") {
			panel.orientation(libv::ui::Orientation::up);

			TestComponent comp0 = panel.add("10px, 10px", M(0, 0, 0, 0));
			TestComponent comp1 = panel.add("20px, 20px", M(0, 200, 0, 0));
			TestComponent comp2 = panel.add("30px, 40px", M(0, 0, 0, 0));

			CHECK(panel.layout1(0, 0) == approx_size(30, 270));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0,   0, 10, 10));
			CHECK(comp1.bounds() == Bounds(0, 225, 20, 20));
			CHECK(comp2.bounds() == Bounds(0, 260, 30, 40));
		}

		SECTION("Orientation::down 4") {
			panel.orientation(libv::ui::Orientation::down);

			TestComponent comp0 = panel.add("1px, 10px", M(0, 0, 0, 0));
			TestComponent comp1 = panel.add("1px, 20px", M(0, 100, 0, 0));
			TestComponent comp2 = panel.add("1px, 40px", M(0, 20, 0, 0));
			TestComponent comp3 = panel.add("1px, 80px", M(0, 0, 0, 0));

			CHECK(panel.layout1(0, 0) == approx_size(1, 270));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0, 290, 1, 10));
			CHECK(comp1.bounds() == Bounds(0, 260, 1, 20));
			CHECK(comp2.bounds() == Bounds(0, 110, 1, 40));
			CHECK(comp3.bounds() == Bounds(0,   0, 1, 80));
		}

		SECTION("Orientation::up 4") {
			panel.orientation(libv::ui::Orientation::up);

			TestComponent comp0 = panel.add("1px, 10px", M(0, 0, 0, 0));
			TestComponent comp1 = panel.add("1px, 20px", M(0, 100, 0, 0));
			TestComponent comp2 = panel.add("1px, 40px", M(0, 20, 0, 0));
			TestComponent comp3 = panel.add("1px, 80px", M(0, 0, 0, 0));

			CHECK(panel.layout1(0, 0) == approx_size(1, 270));
			panel.layout2(400, 300);

			CHECK(comp0.bounds() == Bounds(0,   0, 1, 10));
			CHECK(comp1.bounds() == Bounds(0, 120, 1, 20));
			CHECK(comp2.bounds() == Bounds(0, 170, 1, 40));
			CHECK(comp3.bounds() == Bounds(0, 220, 1, 80));
		}
	}
}

TEST_CASE("Layout Line: Incorrect height limit during layout 1 query", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);
	panel.orientation(libv::ui::Orientation::right);
	panel.size(libv::ui::parse_size_or_throw("1r, D"));

	TestComponent comp0 = panel.add("40px, D", D{20, 20});
	TestComponent comp1 = panel.add("40px, D", D{20, 20});
	TestComponent comp2 = panel.add("40px, D", D{20, 20});

	CHECK(panel.layout1(1600, -1) == approx_size(120, 20));
	CHECK(comp0.providedLimit() == libv::vec2f(40, -1));
	CHECK(comp1.providedLimit() == libv::vec2f(40, -1));
	CHECK(comp2.providedLimit() == libv::vec2f(40, -1));

	panel.layout2(1600, 20);

	CHECK(comp0.bounds() == Bounds( 0, 0, 40, 20));
	CHECK(comp1.bounds() == Bounds(40, 0, 40, 20));
	CHECK(comp2.bounds() == Bounds(80, 0, 40, 20));
}

TEST_CASE("Layout Line: complex test 0", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::right);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::top);

	TestComponent comp0 = panel.add("  0px 20% 1r D,  10px  0% 1r D", D(10, 20));
	TestComponent comp1 = panel.add("150px 20% 2r  ,  75px 10% 2r D", D(11, 21));
	TestComponent comp2 = panel.add("200px 10%    D,  50px 20% 3r  ", D(12, 22));
	TestComponent comp3 = panel.add("250px  0% 4r D,   0px 10% 4r  ", D(13, 23));

	auto pass1 = panel.layout1(1000, 600);
	CHECK(pass1 == approx_size(850.f, 120.f));
	CHECK(comp0.providedLimit() == approx_size(350, 600));
	CHECK(comp1.providedLimit() == approx_size(350, 600));
	CHECK(comp2.providedLimit() == approx_size(350, 600));
	CHECK(comp3.providedLimit() == approx_size(400, 600));

	panel.layout2(0, 0, 400, 300);
	CHECK(comp0.bounds() == Bounds(  0, 0,  80, 300));
	CHECK(comp1.bounds() == Bounds( 80, 0, 150, 300));
	CHECK(comp2.bounds() == Bounds(230, 0, 200, 300));
	CHECK(comp3.bounds() == Bounds(430, 0, 250, 300));
}

TEST_CASE("Layout Line: secondary dimension can have 'fake' margin zones", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::right);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	// D is only used in sizes to check providedLimit, does not effects output
	TestComponent comp0 = panel.add("100px, 50% D", M(10, 10, 10, 10));
	TestComponent comp1 = panel.add("100px, 50% D", M(20, 20, 20, 20));
	TestComponent comp2 = panel.add("100px, 50% D", M(30, 30, 30, 30));

	SECTION("Without padding") {
		auto pass1 = panel.layout1(1000, 600);
		CHECK(pass1 == approx_size(420.f, 330.f));
		CHECK(comp0.providedLimit() == approx_size(100, 580));
		CHECK(comp1.providedLimit() == approx_size(100, 560));
		CHECK(comp2.providedLimit() == approx_size(100, 540));

		panel.layout2(0, 0, 400, 300);
		CHECK(comp0.bounds() == Bounds( 10, 10, 100, 120));
		CHECK(comp1.bounds() == Bounds(140, 20, 100, 120));
		CHECK(comp2.bounds() == Bounds(290, 30, 100, 120));
	}
	SECTION("With padding") {
		panel.padding(5, 15, 25, 35);

		auto pass1 = panel.layout1(1000, 600);
		CHECK(pass1 == approx_size(450.f, 355.f));
		CHECK(comp0.providedLimit() == approx_size(100, 530));
		CHECK(comp1.providedLimit() == approx_size(100, 510));
		CHECK(comp2.providedLimit() == approx_size(100, 490));

		panel.layout2(0, 0, 400, 300);
		CHECK(comp0.bounds() == Bounds( 15, 25, 100, 95));
		CHECK(comp1.bounds() == Bounds(145, 35, 100, 95));
		CHECK(comp2.bounds() == Bounds(295, 45, 100, 95));
	}
}

TEST_CASE("Layout Line: ratio fills secondary dimension even in the 'fake' margin zones", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::right);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	// D is only used in sizes to check providedLimit, does not effects output
	TestComponent comp0 = panel.add("100px, 50% 1r D", M(10, 10, 10, 10));
	TestComponent comp1 = panel.add("100px, 50% 1r D", M(20, 20, 20, 20));
	TestComponent comp2 = panel.add("100px, 50% 1r D", M(30, 30, 30, 30));

	SECTION("Without padding") {
		auto pass1 = panel.layout1(1000, 600);
		CHECK(pass1 == approx_size(420.f, 330.f));
		CHECK(comp0.providedLimit() == approx_size(100, 580));
		CHECK(comp1.providedLimit() == approx_size(100, 560));
		CHECK(comp2.providedLimit() == approx_size(100, 540));

		panel.layout2(0, 0, 400, 300);
		CHECK(comp0.bounds() == Bounds( 10, 10, 100, 280));
		CHECK(comp1.bounds() == Bounds(140, 20, 100, 260));
		CHECK(comp2.bounds() == Bounds(290, 30, 100, 240));
	}
	SECTION("With padding") {
		panel.padding(5, 15, 25, 35);

		auto pass1 = panel.layout1(1000, 600);
		CHECK(pass1 == approx_size(450.f, 355.f));
		CHECK(comp0.providedLimit() == approx_size(100, 530));
		CHECK(comp1.providedLimit() == approx_size(100, 510));
		CHECK(comp2.providedLimit() == approx_size(100, 490));

		panel.layout2(0, 0, 400, 300);
		CHECK(comp0.bounds() == Bounds( 15, 25, 100, 230));
		CHECK(comp1.bounds() == Bounds(145, 35, 100, 210));
		CHECK(comp2.bounds() == Bounds(295, 45, 100, 190));
	}
}

TEST_CASE("Layout Line: min sizing mode", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.orientation(libv::ui::Orientation::right);
	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	SECTION("x") {
		TestComponent comp0 = panel.add("min 100px 100%, 100px");

		SECTION("s0") {
			CHECK(panel.layout1(50, 600) == approx_size(50.f, 100.f));
			CHECK(panel.layout1(100, 600) == approx_size(100.f, 100.f));
			CHECK(panel.layout1(1000, 600) == approx_size(100.f, 100.f));
		}
		SECTION("s1") {
			panel.layout2(0, 0, 50, 300);
			CHECK(comp0.bounds() == Bounds(0, 0, 50, 100));
		}
		SECTION("s2") {
			panel.layout2(0, 0, 100, 300);
			CHECK(comp0.bounds() == Bounds(0, 0, 100, 100));
		}
		SECTION("s3") {
			panel.layout2(0, 0, 1000, 300);
			CHECK(comp0.bounds() == Bounds(0, 0, 100, 100));
		}
	}
	SECTION("xy") {
		TestComponent comp0 = panel.add("min 100px 100%, min 200px 100%");

		SECTION("s0") {
			CHECK(panel.layout1(50, 100) == approx_size(50.f, 100.f));
			CHECK(panel.layout1(100, 200) == approx_size(100.f, 200.f));
			CHECK(panel.layout1(1000, 1000) == approx_size(100.f, 200.f));
		}
		SECTION("s1") {
			panel.layout2(0, 0, 50, 100);
			CHECK(comp0.bounds() == Bounds(0, 0, 50, 100));
		}
		SECTION("s2") {
			panel.layout2(0, 0, 100, 200);
			CHECK(comp0.bounds() == Bounds(0, 0, 100, 200));
		}
		SECTION("s3") {
			panel.layout2(0, 0, 1000, 300);
			CHECK(comp0.bounds() == Bounds(0, 0, 100, 200));
		}
	}
}

TEST_CASE("Layout Line: dyanmic can use space marked for ratio distribution", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	TestComponent comp0 = panel.add("1r, D", D(100, 200));
	TestComponent comp1 = panel.add("1r, D", D(200, 300));

	SECTION("without competing ratios") {
		panel.orientation(libv::ui::Orientation::up);

		CHECK(panel.layout1(1000, 600) == approx_size(0.f, 500.f));
		CHECK(comp0.providedLimit() == approx_size(1000, 600));
		CHECK(comp1.providedLimit() == approx_size(1000, 400));

		panel.layout2(0, 0, 1000, 600);
		CHECK(comp0.bounds() == Bounds(0,   0, 1000, 200));
		CHECK(comp1.bounds() == Bounds(0, 200, 1000, 300));
	}
	SECTION("with competing ratios") {
		panel.orientation(libv::ui::Orientation::right);

		CHECK(panel.layout1(1000, 600) == approx_size(0.f, 300.f));
		CHECK(comp0.providedLimit() == approx_size(1000, 600));
		CHECK(comp1.providedLimit() == approx_size(1000, 600));

		panel.layout2(0, 0, 1000, 600);
		CHECK(comp0.providedLimit() == approx_size(500, 600));
		CHECK(comp1.providedLimit() == approx_size(500, 600));
		CHECK(comp0.bounds() == Bounds(  0, 0, 500, 200));
		CHECK(comp1.bounds() == Bounds(500, 0, 500, 300));
	}
}

TEST_CASE("Layout Line: Not supported dyanmic and ratio mixed on primary", "[libv.ui.layout.line]") {
	TestPanel<libv::ui::PanelLine> panel;

	panel.align_horizontal(libv::ui::AlignHorizontal::left);
	panel.align_vertical(libv::ui::AlignVertical::bottom);

	TestComponent comp0 = panel.add("1r, D", D(100, 200));
	TestComponent comp1 = panel.add("1rD, D", D(200, 300));

	SECTION("without competing ratios") {
		panel.orientation(libv::ui::Orientation::up);

		CHECK(panel.layout1(1000, 600) == approx_size(200.f, 500.f));
		CHECK(comp0.providedLimit() == approx_size(1000, 600));
		CHECK(comp1.providedLimit() == approx_size(1000, 400));

		panel.layout2(0, 0, 1000, 600);
		CHECK(comp0.bounds() == Bounds(0,   0, 1000, 200));
		CHECK(comp1.bounds() == Bounds(0, 200, 1000, 300));
	}
	SECTION("with competing ratios") {
		panel.orientation(libv::ui::Orientation::right);

		CHECK(panel.layout1(1000, 600) == approx_size(200.f, 300.f));
		// CHECK(comp0.providedLimit() == approx_size(900, 600)); // Currently incorrect: 1000, 600
		CHECK(comp1.providedLimit() == approx_size(1000, 600));

		panel.layout2(0, 0, 1000, 600);
		// CHECK(comp0.providedLimit() == approx_size(400, 600)); // Currently incorrect: 500, 600
		// CHECK(comp1.providedLimit() == approx_size(1000, 600)); // Currently incorrect: 500, 600
		CHECK(comp0.bounds() == Bounds(  0, 0, 400, 200));
		CHECK(comp1.bounds() == Bounds(400, 0, 600, 300));
	}
}
