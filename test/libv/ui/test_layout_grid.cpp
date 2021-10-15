// Project: libv.ui, File: test/libv/ui/test_layout_grid.cpp

// hpp
#include <catch/catch.hpp>
// pro
//#include <libv/ui/layout/layout_grid.hpp>
#include "test_layout_utility.hpp"


// TODO P5: Layout separation was removed, a ui component based testing is required, re-enable these tests

//// -------------------------------------------------------------------------------------------------
//
//namespace {
//
//struct TestChild : public BasicTestChild<libv::ui::LayoutGrid> {
//	using BasicTestChild<libv::ui::LayoutGrid>::BasicTestChild;
//
//	template <typename V>
//	void anchor(V&& value) {
//		components[index].ptr.core().set(components[index].property.anchor, std::forward<V>(value));
//	}
//
//	void size(std::string_view value) {
//		components[index].ptr.core().set(components[index].property.size, libv::ui::parse_size_or_throw(value));
//	}
//};
//
//struct TestLayout : public BasicTestLayout<libv::ui::LayoutGrid, TestChild> {
//	template <typename V>
//	void anchor_content(V&& value) {
//		ignore.set(property.anchor_content, std::forward<V>(value));
//	}
//
//	template <typename V>
//	void orientation2(V&& value) {
//		ignore.set(property.orientation2, std::forward<V>(value));
//	}
//
//	template <typename V>
//	void column_count(V&& value) {
//		ignore.set(property.column_count, std::forward<V>(value));
//	}
//};
//
//} // namespace
//
//// -------------------------------------------------------------------------------------------------
//
//TEST_CASE("layout grid: empty", "[libv.ui.layout.grid]") {
//	TestLayout layout;
//	libv::ui::ContextLayout1 env1;
//	libv::ui::ContextLayout2 env2{libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 0), libv::ui::MouseOrder{0}};
//
//	CHECK(layout.layout1(env1) == libv::vec3f(0, 0, 0));
//	layout.layout2(env2);
//}
//
//TEST_CASE("layout grid: layout1 size calculation with every Orientation2s", "[libv.ui.layout.grid]") {
//	TestLayout layout;
//	auto comp0 = layout.add();
//	auto comp1 = layout.add();
//	auto comp2 = layout.add();
//	auto comp3 = layout.add();
//
//	layout.anchor_content(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp0.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp1.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp2.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp3.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//
//	libv::ui::ContextLayout1 env1;
//
//	SECTION("size dynamic 0,1;2,3") {
//		comp0.size("D, D, D"); comp0.dynamic(0, 0);
//		comp1.size("D, D, D"); comp1.dynamic(1, 1);
//		comp2.size("D, D, D"); comp2.dynamic(2, 2);
//		comp3.size("D, D, D"); comp3.dynamic(3, 3);
//		layout.column_count(2);
//
//		SECTION("Horizontal > Vertical") {
//			SECTION("Orientation2 left_down") {
//				layout.orientation2(libv::ui::Orientation2::left_down);
//			}
//			SECTION("Orientation2 left_up") {
//				layout.orientation2(libv::ui::Orientation2::left_up);
//			}
//			SECTION("Orientation2 right_down") {
//				layout.orientation2(libv::ui::Orientation2::right_down);
//			}
//			SECTION("Orientation2 right_up") {
//				layout.orientation2(libv::ui::Orientation2::right_up);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(5, 4, 0));
//		}
//
//		SECTION("Vertical > Horizontal") {
//			SECTION("Orientation2 down_left") {
//				layout.orientation2(libv::ui::Orientation2::down_left);
//			}
//			SECTION("Orientation2 down_right") {
//				layout.orientation2(libv::ui::Orientation2::down_right);
//			}
//			SECTION("Orientation2 up_left") {
//				layout.orientation2(libv::ui::Orientation2::up_left);
//			}
//			SECTION("Orientation2 up_right") {
//				layout.orientation2(libv::ui::Orientation2::up_right);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(4, 5, 0));
//		}
//	}
//
//	SECTION("size dynamic 0,1,2;3") {
//		comp0.size("D, D, D"); comp0.dynamic(0, 0);
//		comp1.size("D, D, D"); comp1.dynamic(1, 1);
//		comp2.size("D, D, D"); comp2.dynamic(2, 2);
//		comp3.size("D, D, D"); comp3.dynamic(3, 3);
//		layout.column_count(3);
//
//		SECTION("Horizontal > Vertical") {
//			SECTION("Orientation2 left_down") {
//				layout.orientation2(libv::ui::Orientation2::left_down);
//			}
//			SECTION("Orientation2 left_up") {
//				layout.orientation2(libv::ui::Orientation2::left_up);
//			}
//			SECTION("Orientation2 right_down") {
//				layout.orientation2(libv::ui::Orientation2::right_down);
//			}
//			SECTION("Orientation2 right_up") {
//				layout.orientation2(libv::ui::Orientation2::right_up);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(6, 5, 0));
//		}
//
//		SECTION("Vertical > Horizontal") {
//			SECTION("Orientation2 down_left") {
//				layout.orientation2(libv::ui::Orientation2::down_left);
//			}
//			SECTION("Orientation2 down_right") {
//				layout.orientation2(libv::ui::Orientation2::down_right);
//			}
//			SECTION("Orientation2 up_left") {
//				layout.orientation2(libv::ui::Orientation2::up_left);
//			}
//			SECTION("Orientation2 up_right") {
//				layout.orientation2(libv::ui::Orientation2::up_right);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(5, 6, 0));
//		}
//	}
//
//	SECTION("size misc 0,1;2,3") {
//		comp0.size("100px, 100px"); comp0.dynamic(0, 0);
//		comp1.size("200px,     D"); comp1.dynamic(1, 1);
//		comp2.size("    0,    1r"); comp2.dynamic(2, 2);
//		comp3.size("  50%, 100px"); comp3.dynamic(3, 3);
//		layout.column_count(2);
//
//		SECTION("Horizontal > Vertical") {
//			SECTION("Orientation2 left_down") {
//				layout.orientation2(libv::ui::Orientation2::left_down);
//			}
//			SECTION("Orientation2 left_up") {
//				layout.orientation2(libv::ui::Orientation2::left_up);
//			}
//			SECTION("Orientation2 right_down") {
//				layout.orientation2(libv::ui::Orientation2::right_down);
//			}
//			SECTION("Orientation2 right_up") {
//				layout.orientation2(libv::ui::Orientation2::right_up);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(300, 200, 0));
//		}
//
//		SECTION("Vertical > Horizontal") {
//			SECTION("Orientation2 down_left") {
//				layout.orientation2(libv::ui::Orientation2::down_left);
//			}
//			SECTION("Orientation2 down_right") {
//				layout.orientation2(libv::ui::Orientation2::down_right);
//			}
//			SECTION("Orientation2 up_left") {
//				layout.orientation2(libv::ui::Orientation2::up_left);
//			}
//			SECTION("Orientation2 up_right") {
//				layout.orientation2(libv::ui::Orientation2::up_right);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(400, 200, 0));
//		}
//	}
//
//	SECTION("size misc 0,1,2;3") {
//		comp0.size("100px, 100px"); comp0.dynamic(0, 0);
//		comp1.size("200px,     D"); comp1.dynamic(1, 1);
//		comp2.size("    0,    1r"); comp2.dynamic(2, 2);
//		comp3.size("  50%, 100px"); comp3.dynamic(3, 3);
//		layout.column_count(3);
//
//		SECTION("Horizontal > Vertical") {
//			SECTION("Orientation2 left_down") {
//				layout.orientation2(libv::ui::Orientation2::left_down);
//			}
//			SECTION("Orientation2 left_up") {
//				layout.orientation2(libv::ui::Orientation2::left_up);
//			}
//			SECTION("Orientation2 right_down") {
//				layout.orientation2(libv::ui::Orientation2::right_down);
//			}
//			SECTION("Orientation2 right_up") {
//				layout.orientation2(libv::ui::Orientation2::right_up);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(400, 200, 0));
//		}
//
//		SECTION("Vertical > Horizontal") {
//			SECTION("Orientation2 down_left") {
//				layout.orientation2(libv::ui::Orientation2::down_left);
//			}
//			SECTION("Orientation2 down_right") {
//				layout.orientation2(libv::ui::Orientation2::down_right);
//			}
//			SECTION("Orientation2 up_left") {
//				layout.orientation2(libv::ui::Orientation2::up_left);
//			}
//			SECTION("Orientation2 up_right") {
//				layout.orientation2(libv::ui::Orientation2::up_right);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(400, 101, 0));
//		}
//	}
//
//	SECTION("size empty columns 0,1,2,3") {
//		comp0.size("100px, 100px"); comp0.dynamic(0, 0);
//		comp1.size("100px, 100px"); comp1.dynamic(1, 1);
//		comp2.size("100px, 100px"); comp2.dynamic(2, 2);
//		comp3.size("100px, 100px"); comp3.dynamic(3, 3);
//		layout.column_count(100);
//
//		SECTION("Horizontal > Vertical") {
//			SECTION("Orientation2 left_down") {
//				layout.orientation2(libv::ui::Orientation2::left_down);
//			}
//			SECTION("Orientation2 left_up") {
//				layout.orientation2(libv::ui::Orientation2::left_up);
//			}
//			SECTION("Orientation2 right_down") {
//				layout.orientation2(libv::ui::Orientation2::right_down);
//			}
//			SECTION("Orientation2 right_up") {
//				layout.orientation2(libv::ui::Orientation2::right_up);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(400, 100, 0));
//		}
//
//		SECTION("Vertical > Horizontal") {
//			SECTION("Orientation2 down_left") {
//				layout.orientation2(libv::ui::Orientation2::down_left);
//			}
//			SECTION("Orientation2 down_right") {
//				layout.orientation2(libv::ui::Orientation2::down_right);
//			}
//			SECTION("Orientation2 up_left") {
//				layout.orientation2(libv::ui::Orientation2::up_left);
//			}
//			SECTION("Orientation2 up_right") {
//				layout.orientation2(libv::ui::Orientation2::up_right);
//			}
//			CHECK(layout.layout1(env1) == libv::vec3f(100, 400, 0));
//		}
//	}
//}
//
//TEST_CASE("layout grid: layout1 size calculation with multi component size", "[libv.ui.layout.grid]") {
//	TestLayout layout;
//	auto comp0 = layout.add();
//	auto comp1 = layout.add();
//	auto comp2 = layout.add();
//	auto comp3 = layout.add();
//
//	layout.anchor_content(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp0.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp1.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp2.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp3.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//
//	libv::ui::ContextLayout1 env1;
//
//	SECTION("size 4x25px 0,1,2,3") {
//		comp0.size("25px, 25px, 25px"); comp0.dynamic(25, 25, 25);
//		comp1.size("25px, 25px, 25px"); comp1.dynamic(25, 25, 25);
//		comp2.size("25px, 25px, 25px"); comp2.dynamic(25, 25, 25);
//		comp3.size("25px, 25px, 25px"); comp3.dynamic(25, 25, 25);
//		layout.column_count(4);
//
//		layout.orientation2(libv::ui::Orientation2::right_down);
//		CHECK(layout.layout1(env1) == libv::vec3f(100, 25, 25));
//
//		layout.orientation2(libv::ui::Orientation2::down_right);
//		CHECK(layout.layout1(env1) == libv::vec3f(25, 100, 25));
//	}
//
//	SECTION("size 4x25% 0,1,2,3") {
//		comp0.size("25%, 25%, 25%"); comp0.dynamic(25, 25, 25);
//		comp1.size("25%, 25%, 25%"); comp1.dynamic(25, 25, 25);
//		comp2.size("25%, 25%, 25%"); comp2.dynamic(25, 25, 25);
//		comp3.size("25%, 25%, 25%"); comp3.dynamic(25, 25, 25);
//		layout.column_count(4);
//
//		layout.orientation2(libv::ui::Orientation2::right_down);
//		CHECK(layout.layout1(env1) == libv::vec3f(0, 0, 0));
//
//		layout.orientation2(libv::ui::Orientation2::down_right);
//		CHECK(layout.layout1(env1) == libv::vec3f(0, 0, 0));
//	}
//
//	SECTION("size 4x25r 0,1,2,3") {
//		comp0.size("25r, 25r, 25r"); comp0.dynamic(25, 25, 25);
//		comp1.size("25r, 25r, 25r"); comp1.dynamic(25, 25, 25);
//		comp2.size("25r, 25r, 25r"); comp2.dynamic(25, 25, 25);
//		comp3.size("25r, 25r, 25r"); comp3.dynamic(25, 25, 25);
//		layout.column_count(4);
//
//		layout.orientation2(libv::ui::Orientation2::right_down);
//		CHECK(layout.layout1(env1) == libv::vec3f(0, 0, 0));
//
//		layout.orientation2(libv::ui::Orientation2::down_right);
//		CHECK(layout.layout1(env1) == libv::vec3f(0, 0, 0));
//	}
//
//	SECTION("size 4xD 0,1,2,3") {
//		comp0.size("D, D, D"); comp0.dynamic(25, 25, 25);
//		comp1.size("D, D, D"); comp1.dynamic(25, 25, 25);
//		comp2.size("D, D, D"); comp2.dynamic(25, 25, 25);
//		comp3.size("D, D, D"); comp3.dynamic(25, 25, 25);
//		layout.column_count(4);
//
//		layout.orientation2(libv::ui::Orientation2::right_down);
//		CHECK(layout.layout1(env1) == libv::vec3f(100, 25, 25));
//
//		layout.orientation2(libv::ui::Orientation2::down_right);
//		CHECK(layout.layout1(env1) == libv::vec3f(25, 100, 25));
//	}
//
//	SECTION("line with force expand 0,1,2,3") {
//		comp0.size("100px 20%, 0"); comp0.dynamic(0, 0);
//		comp1.size("200px 20%, 0"); comp1.dynamic(1, 1);
//		comp2.size("300px 20%, 0"); comp2.dynamic(2, 2);
//		comp3.size("400px 20%, 0"); comp3.dynamic(3, 3);
//		layout.column_count(4);
//
//		layout.orientation2(libv::ui::Orientation2::right_down);
//		CHECK(layout.layout1(env1) == libv::vec3f(5000, 0, 0));
//
//		layout.orientation2(libv::ui::Orientation2::down_right);
//		CHECK(layout.layout1(env1) == libv::vec3f(500, 0, 0));
//	}
//
//	SECTION("size line with everything 0,1,2,3") {
//		comp0.size("  0px 20% 1r D,  10px  0% 1r D"); comp0.dynamic(0, 0);
//		comp1.size("150px 20% 2r  ,  75px 10% 2r D"); comp1.dynamic(1, 0);
//		comp2.size("200px 10% 3r D,  50px 20% 3r  "); comp2.dynamic(2, 0);
//		comp3.size("250px  0% 4r D,   0px 10% 4r  "); comp3.dynamic(3, 0);
//		layout.column_count(4);
//
//		layout.orientation2(libv::ui::Orientation2::right_down);
//		CHECK(layout.layout1(env1) == libv::vec3f(1210, 83, 0));
//
//		layout.orientation2(libv::ui::Orientation2::down_right);
//		CHECK(layout.layout1(env1) == libv::vec3f(253, 225, 0));
//	}
//
//	SECTION("size with everything") {
//		comp0.size("  0px 20% 1r D,  10px  0% 1r D,   0px 20% 1r D"); comp0.dynamic(10, 20, 30);
//		comp1.size("150px 20% 2r  ,  75px 10% 2r D, 150px 20% 2r  "); comp1.dynamic(11, 21, 31);
//		comp2.size("200px 10% 3r D,  50px 20% 3r  , 200px 10% 3r D"); comp2.dynamic(12, 22, 32);
//		comp3.size("250px  0% 4r D,   0px 10% 4r  , 250px  0% 4r D"); comp3.dynamic(13, 23, 33);
//
//		SECTION("0-1-2-3-x") {
//			layout.column_count(5);
//
//			layout.orientation2(libv::ui::Orientation2::right_down);
//			CHECK(layout.layout1(env1) == libv::vec3f(1270, 107, 283));
//
//			layout.orientation2(libv::ui::Orientation2::down_right);
//			CHECK(layout.layout1(env1) == libv::vec3f(263, 294, 283));
//		}
//		SECTION("0-1-2-3") {
//			layout.column_count(4);
//
//			layout.orientation2(libv::ui::Orientation2::right_down);
//			CHECK(layout.layout1(env1) == libv::vec3f(1270, 107, 283));
//
//			layout.orientation2(libv::ui::Orientation2::down_right);
//			CHECK(layout.layout1(env1) == libv::vec3f(263, 294, 283));
//		}
//		SECTION("0-1-2  3") {
//			layout.column_count(3);
//
//			layout.orientation2(libv::ui::Orientation2::right_down);
//			CHECK(layout.layout1(env1) == libv::vec3f(893, 120, 283));
//
//			layout.orientation2(libv::ui::Orientation2::down_right);
//			CHECK(layout.layout1(env1) == libv::vec3f(528, 251, 283));
//		}
//		SECTION("0-1  2-3") {
//			layout.column_count(2);
//
//			layout.orientation2(libv::ui::Orientation2::right_down);
//			CHECK(layout.layout1(env1) == libv::vec3f(528, 209, 283));
//
//			layout.orientation2(libv::ui::Orientation2::down_right);
//			CHECK(layout.layout1(env1) == libv::vec3f(517, 209, 283));
//		}
//		SECTION("0  1  2  3") {
//			layout.column_count(1);
//
//			layout.orientation2(libv::ui::Orientation2::right_down);
//			CHECK(layout.layout1(env1) == libv::vec3f(263, 294, 283));
//
//			layout.orientation2(libv::ui::Orientation2::down_right);
//			CHECK(layout.layout1(env1) == libv::vec3f(1270, 107, 283));
//		}
//	}
//}
//
//TEST_CASE("layout grid: layout2 yields correct sizes and dimensions", "[libv.ui.layout.grid]") {
//	TestLayout layout;
//	auto comp0 = layout.add();
//	auto comp1 = layout.add();
//	auto comp2 = layout.add();
//	auto comp3 = layout.add();
//
//	layout.anchor_content(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp0.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp1.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp2.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp3.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//
//	libv::ui::ContextLayout1 env1;
//	libv::ui::ContextLayout2 env2{libv::vec3f(1000, 2000, 3000), libv::vec3f(400, 300, 200), libv::ui::MouseOrder{0}};
//
//	layout.anchor_content(libv::ui::Anchor{0, 0, 0});
//	layout.column_count(2);
//
//	SECTION("pixel") {
//		comp0.size("10px, 10px, 10px"); comp0.dynamic(10, 20, 30);
//		comp1.size("20px, 20px, 20px"); comp1.dynamic(11, 21, 31);
//		comp2.size("30px, 30px, 30px"); comp2.dynamic(12, 22, 32);
//		comp3.size("40px, 40px, 40px"); comp3.dynamic(13, 23, 33);
//
//		SECTION("right_up") {
//			layout.orientation2(libv::ui::Orientation2::right_up);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds() == Bounds(1000, 2000, 3000, 10, 10, 10));
//			CHECK(comp1.bounds() == Bounds(1030, 2000, 3000, 20, 20, 20));
//			CHECK(comp2.bounds() == Bounds(1000, 2020, 3000, 30, 30, 30));
//			CHECK(comp3.bounds() == Bounds(1030, 2020, 3000, 40, 40, 40));
//		}
//		SECTION("up_right") {
//			layout.orientation2(libv::ui::Orientation2::up_right);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds() == Bounds(1000, 2000, 3000, 10, 10, 10));
//			CHECK(comp1.bounds() == Bounds(1000, 2030, 3000, 20, 20, 20));
//			CHECK(comp2.bounds() == Bounds(1020, 2000, 3000, 30, 30, 30));
//			CHECK(comp3.bounds() == Bounds(1020, 2030, 3000, 40, 40, 40));
//		}
//	}
//	SECTION("percent") {
//		comp0.size("10%, 10%, 10%"); comp0.dynamic(10, 20, 30);
//		comp1.size("20%, 20%, 20%"); comp1.dynamic(11, 21, 31);
//		comp2.size("30%, 30%, 30%"); comp2.dynamic(12, 22, 32);
//		comp3.size("40%, 40%, 40%"); comp3.dynamic(13, 23, 33);
//
//		SECTION("right_up") {
//			layout.orientation2(libv::ui::Orientation2::right_up);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds() == Bounds(1000, 2000, 3000,  40,  30, 20));
//			CHECK(comp1.bounds() == Bounds(1120, 2000, 3000,  80,  60, 40));
//			CHECK(comp2.bounds() == Bounds(1000, 2060, 3000, 120,  90, 60));
//			CHECK(comp3.bounds() == Bounds(1120, 2060, 3000, 160, 120, 80));
//		}
//		SECTION("up_right") {
//			layout.orientation2(libv::ui::Orientation2::up_right);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds() == Bounds(1000, 2000, 3000,  40,  30, 20));
//			CHECK(comp1.bounds() == Bounds(1000, 2090, 3000,  80,  60, 40));
//			CHECK(comp2.bounds() == Bounds(1080, 2000, 3000, 120,  90, 60));
//			CHECK(comp3.bounds() == Bounds(1080, 2090, 3000, 160, 120, 80));
//		}
//	}
//	SECTION("ratio") {
//		const auto tests = [&](){
//			SECTION("right_up") {
//				layout.orientation2(libv::ui::Orientation2::right_up);
//				layout.layout1(env1);
//				layout.layout2(env2);
//
//				CHECK(comp0.bounds() == Bounds(1000, 2000, 3000,  57,  50,  50));
//				CHECK(comp1.bounds() == Bounds(1171, 2000, 3000, 115, 100, 100));
//				CHECK(comp2.bounds() == Bounds(1000, 2100, 3000, 171, 150, 150));
//				CHECK(comp3.bounds() == Bounds(1171, 2100, 3000, 229, 200, 200));
//			}
//			SECTION("up_right") {
//				layout.orientation2(libv::ui::Orientation2::up_right);
//				layout.layout1(env1);
//				layout.layout2(env2);
//
//				CHECK(comp0.bounds() == Bounds(1000, 2000, 3000,  67,  43,  50));
//				CHECK(comp1.bounds() == Bounds(1000, 2129, 3000, 133,  85, 100));
//				CHECK(comp2.bounds() == Bounds(1133, 2000, 3000, 200, 129, 150));
//				CHECK(comp3.bounds() == Bounds(1133, 2129, 3000, 267, 171, 200));
//			}
//		};
//
//		SECTION("sum less than 1") {
//			comp0.size("0.01r, 0.01r, 0.01r"); comp0.dynamic(10, 20, 30);
//			comp1.size("0.02r, 0.02r, 0.02r"); comp1.dynamic(11, 21, 31);
//			comp2.size("0.03r, 0.03r, 0.03r"); comp2.dynamic(12, 22, 32);
//			comp3.size("0.04r, 0.04r, 0.04r"); comp3.dynamic(13, 23, 33);
//
//			tests();
//		}
//		SECTION("sum more than 1") {
//			comp0.size("1r, 1r, 1r"); comp0.dynamic(10, 20, 30);
//			comp1.size("2r, 2r, 2r"); comp1.dynamic(11, 21, 31);
//			comp2.size("3r, 3r, 3r"); comp2.dynamic(12, 22, 32);
//			comp3.size("4r, 4r, 4r"); comp3.dynamic(13, 23, 33);
//
//			tests();
//		}
//	}
//	SECTION("dynamic") {
//		comp0.size("D, D, D"); comp0.dynamic(10, 20, 30);
//		comp1.size("D, D, D"); comp1.dynamic(11, 21, 31);
//		comp2.size("D, D, D"); comp2.dynamic(12, 22, 32);
//		comp3.size("D, D, D"); comp3.dynamic(13, 23, 33);
//
//		SECTION("right_up") {
//			layout.orientation2(libv::ui::Orientation2::right_up);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds() == Bounds(1000, 2000, 3000, 10, 20, 30));
//			CHECK(comp1.bounds() == Bounds(1012, 2000, 3000, 11, 21, 31));
//			CHECK(comp2.bounds() == Bounds(1000, 2021, 3000, 12, 22, 32));
//			CHECK(comp3.bounds() == Bounds(1012, 2021, 3000, 13, 23, 33));
//		}
//		SECTION("up_right") {
//			layout.orientation2(libv::ui::Orientation2::up_right);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds() == Bounds(1000, 2000, 3000, 10, 20, 30));
//			CHECK(comp1.bounds() == Bounds(1000, 2022, 3000, 11, 21, 31));
//			CHECK(comp2.bounds() == Bounds(1011, 2000, 3000, 12, 22, 32));
//			CHECK(comp3.bounds() == Bounds(1011, 2022, 3000, 13, 23, 33));
//		}
//	}
//	SECTION("complex") {
//		comp0.size("  0px 20% 1r D,  10px  0% 1r D,   0px 20% 1r D"); comp0.dynamic(10, 20, 30);
//		comp1.size("150px 20% 2r  ,  75px 10% 2r D, 150px 20% 2r  "); comp1.dynamic(11, 21, 31);
//		comp2.size("200px 10%    D,  50px 20% 3r  , 200px 10% 3r D"); comp2.dynamic(12, 22, 32);
//		comp3.size("250px  0% 4r D,   0px 10% 4r  ,   0px  0%     "); comp3.dynamic(13, 23, 33);
//
//		SECTION("right_up") {
//			layout.orientation2(libv::ui::Orientation2::right_up);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds() == Bounds(1000, 2000, 3000,  49,  43,  53));
//			CHECK(comp1.bounds() == Bounds(1252, 2000, 3000, 148, 152, 155));
//			CHECK(comp2.bounds() == Bounds(1000, 2152, 3000, 252, 148, 200));
//			CHECK(comp3.bounds() == Bounds(1252, 2152, 3000,  99,  81,   0));
//		}
//		SECTION("up_right") {
//			layout.orientation2(libv::ui::Orientation2::up_right);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds() == Bounds(1000, 2000, 3000,  49,  43,  53));
//			CHECK(comp1.bounds() == Bounds(1000, 2148, 3000, 148, 152, 155));
//			CHECK(comp2.bounds() == Bounds(1148, 2000, 3000, 252, 148, 200));
//			CHECK(comp3.bounds() == Bounds(1148, 2148, 3000,  99,  82,   0));
//		}
//	}
//}
//
//TEST_CASE("layout grid: layout2 position tests with Orientation2", "[libv.ui.layout.grid]") {
//	TestLayout layout;
//	auto comp0 = layout.add();
//	auto comp1 = layout.add();
//	auto comp2 = layout.add();
//	auto comp3 = layout.add();
//
//	layout.anchor_content(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp0.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp1.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp2.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp3.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//
//	libv::ui::ContextLayout1 env1;
//	libv::ui::ContextLayout2 env2{libv::vec3f(1000, 2000, 3000), libv::vec3f(4000, 3000, 2000), libv::ui::MouseOrder{0}};
//
//	layout.anchor_content(libv::ui::Anchor{0, 0, 0});
//	comp0.size("1px, 10px, 100px"); comp0.dynamic(0, 0, 0);
//	comp1.size("2px, 20px, 200px"); comp1.dynamic(0, 0, 0);
//	comp2.size("3px, 30px, 300px"); comp2.dynamic(0, 0, 0);
//	comp3.size("4px, 40px, 400px"); comp3.dynamic(0, 0, 0);
//
//	SECTION("column 2") {
//		layout.column_count(2);
//
//		SECTION("down_left") {
//			layout.orientation2(libv::ui::Orientation2::down_left);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1004, 2040, 3000));
//			CHECK(comp1.bounds().position == Pos(1004, 2000, 3000));
//			CHECK(comp2.bounds().position == Pos(1000, 2040, 3000));
//			CHECK(comp3.bounds().position == Pos(1000, 2000, 3000));
//		}
//		SECTION("down_right") {
//			layout.orientation2(libv::ui::Orientation2::down_right);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1000, 2040, 3000));
//			CHECK(comp1.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp2.bounds().position == Pos(1002, 2040, 3000));
//			CHECK(comp3.bounds().position == Pos(1002, 2000, 3000));
//		}
//		SECTION("left_down") {
//			layout.orientation2(libv::ui::Orientation2::left_down);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1004, 2040, 3000));
//			CHECK(comp1.bounds().position == Pos(1000, 2040, 3000));
//			CHECK(comp2.bounds().position == Pos(1004, 2000, 3000));
//			CHECK(comp3.bounds().position == Pos(1000, 2000, 3000));
//		}
//		SECTION("left_up") {
//			layout.orientation2(libv::ui::Orientation2::left_up);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1004, 2000, 3000));
//			CHECK(comp1.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp2.bounds().position == Pos(1004, 2020, 3000));
//			CHECK(comp3.bounds().position == Pos(1000, 2020, 3000));
//		}
//		SECTION("right_up") {
//			layout.orientation2(libv::ui::Orientation2::right_up);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp1.bounds().position == Pos(1003, 2000, 3000));
//			CHECK(comp2.bounds().position == Pos(1000, 2020, 3000));
//			CHECK(comp3.bounds().position == Pos(1003, 2020, 3000));
//		}
//		SECTION("right_down") {
//			layout.orientation2(libv::ui::Orientation2::right_down);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1000, 2040, 3000));
//			CHECK(comp1.bounds().position == Pos(1003, 2040, 3000));
//			CHECK(comp2.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp3.bounds().position == Pos(1003, 2000, 3000));
//		}
//		SECTION("up_left") {
//			layout.orientation2(libv::ui::Orientation2::up_left);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1004, 2000, 3000));
//			CHECK(comp1.bounds().position == Pos(1004, 2030, 3000));
//			CHECK(comp2.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp3.bounds().position == Pos(1000, 2030, 3000));
//		}
//		SECTION("up_right") {
//			layout.orientation2(libv::ui::Orientation2::up_right);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp1.bounds().position == Pos(1000, 2030, 3000));
//			CHECK(comp2.bounds().position == Pos(1002, 2000, 3000));
//			CHECK(comp3.bounds().position == Pos(1002, 2030, 3000));
//		}
//	}
//
//	SECTION("column 3") {
//		layout.column_count(3);
//
//		SECTION("down_left") {
//			layout.orientation2(libv::ui::Orientation2::down_left);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1004, 2050, 3000));
//			CHECK(comp1.bounds().position == Pos(1004, 2030, 3000));
//			CHECK(comp2.bounds().position == Pos(1004, 2000, 3000));
//			CHECK(comp3.bounds().position == Pos(1000, 2050, 3000));
//		}
//		SECTION("down_right") {
//			layout.orientation2(libv::ui::Orientation2::down_right);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1000, 2050, 3000));
//			CHECK(comp1.bounds().position == Pos(1000, 2030, 3000));
//			CHECK(comp2.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp3.bounds().position == Pos(1003, 2050, 3000));
//		}
//		SECTION("left_down") {
//			layout.orientation2(libv::ui::Orientation2::left_down);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1005, 2040, 3000));
//			CHECK(comp1.bounds().position == Pos(1003, 2040, 3000));
//			CHECK(comp2.bounds().position == Pos(1000, 2040, 3000));
//			CHECK(comp3.bounds().position == Pos(1005, 2000, 3000));
//		}
//		SECTION("left_up") {
//			layout.orientation2(libv::ui::Orientation2::left_up);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1005, 2000, 3000));
//			CHECK(comp1.bounds().position == Pos(1003, 2000, 3000));
//			CHECK(comp2.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp3.bounds().position == Pos(1005, 2030, 3000));
//		}
//		SECTION("right_up") {
//			layout.orientation2(libv::ui::Orientation2::right_up);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp1.bounds().position == Pos(1004, 2000, 3000));
//			CHECK(comp2.bounds().position == Pos(1006, 2000, 3000));
//			CHECK(comp3.bounds().position == Pos(1000, 2030, 3000));
//		}
//		SECTION("right_down") {
//			layout.orientation2(libv::ui::Orientation2::right_down);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1000, 2040, 3000));
//			CHECK(comp1.bounds().position == Pos(1004, 2040, 3000));
//			CHECK(comp2.bounds().position == Pos(1006, 2040, 3000));
//			CHECK(comp3.bounds().position == Pos(1000, 2000, 3000));
//		}
//		SECTION("up_left") {
//			layout.orientation2(libv::ui::Orientation2::up_left);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1004, 2000, 3000));
//			CHECK(comp1.bounds().position == Pos(1004, 2040, 3000));
//			CHECK(comp2.bounds().position == Pos(1004, 2060, 3000));
//			CHECK(comp3.bounds().position == Pos(1000, 2000, 3000));
//		}
//		SECTION("up_right") {
//			layout.orientation2(libv::ui::Orientation2::up_right);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(1000, 2000, 3000));
//			CHECK(comp1.bounds().position == Pos(1000, 2040, 3000));
//			CHECK(comp2.bounds().position == Pos(1000, 2060, 3000));
//			CHECK(comp3.bounds().position == Pos(1003, 2000, 3000));
//		}
//	}
//}
//
//TEST_CASE("layout grid: layout2 anchor_content test", "[libv.ui.layout.grid]") {
//	TestLayout layout;
//	auto comp0 = layout.add();
//	auto comp1 = layout.add();
//	auto comp2 = layout.add();
//	auto comp3 = layout.add();
//
//	comp0.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp1.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp2.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp3.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//
//	libv::ui::ContextLayout1 env1;
//	libv::ui::ContextLayout2 env2{libv::vec3f(100, 200, 300), libv::vec3f(100, 100, 100), libv::ui::MouseOrder{0}};
//
//	layout.orientation2(libv::ui::Orientation2::down_left);
//	comp0.size("1px, 2px, 3px"); comp0.dynamic(0, 0, 0);
//	comp1.size("1px, 2px, 3px"); comp1.dynamic(0, 0, 0);
//	comp2.size("1px, 2px, 3px"); comp2.dynamic(0, 0, 0);
//	comp3.size("1px, 2px, 3px"); comp3.dynamic(0, 0, 0);
//
//	SECTION("column 2") {
//		layout.column_count(2);
//
//		SECTION("ANCHOR_TOP_LEFT") {
//			layout.anchor_content(libv::ui::ANCHOR_TOP_LEFT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(101, 298, 300));
//			CHECK(comp1.bounds().position == Pos(101, 296, 300));
//			CHECK(comp2.bounds().position == Pos(100, 298, 300));
//			CHECK(comp3.bounds().position == Pos(100, 296, 300));
//		}
//		SECTION("ANCHOR_TOP_CENTER") {
//			layout.anchor_content(libv::ui::ANCHOR_TOP_CENTER);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(150, 298, 300));
//			CHECK(comp1.bounds().position == Pos(150, 296, 300));
//			CHECK(comp2.bounds().position == Pos(149, 298, 300));
//			CHECK(comp3.bounds().position == Pos(149, 296, 300));
//		}
//		SECTION("ANCHOR_TOP_RIGHT") {
//			layout.anchor_content(libv::ui::ANCHOR_TOP_RIGHT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(199, 298, 300));
//			CHECK(comp1.bounds().position == Pos(199, 296, 300));
//			CHECK(comp2.bounds().position == Pos(198, 298, 300));
//			CHECK(comp3.bounds().position == Pos(198, 296, 300));
//		}
//		SECTION("ANCHOR_CENTER_LEFT") {
//			layout.anchor_content(libv::ui::ANCHOR_CENTER_LEFT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(101, 250, 300));
//			CHECK(comp1.bounds().position == Pos(101, 248, 300));
//			CHECK(comp2.bounds().position == Pos(100, 250, 300));
//			CHECK(comp3.bounds().position == Pos(100, 248, 300));
//		}
//		SECTION("ANCHOR_CENTER_CENTER") {
//			layout.anchor_content(libv::ui::ANCHOR_CENTER_CENTER);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(150, 250, 300));
//			CHECK(comp1.bounds().position == Pos(150, 248, 300));
//			CHECK(comp2.bounds().position == Pos(149, 250, 300));
//			CHECK(comp3.bounds().position == Pos(149, 248, 300));
//		}
//		SECTION("ANCHOR_CENTER_RIGHT") {
//			layout.anchor_content(libv::ui::ANCHOR_CENTER_RIGHT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(199, 250, 300));
//			CHECK(comp1.bounds().position == Pos(199, 248, 300));
//			CHECK(comp2.bounds().position == Pos(198, 250, 300));
//			CHECK(comp3.bounds().position == Pos(198, 248, 300));
//		}
//		SECTION("ANCHOR_BOTTOM_LEFT") {
//			layout.anchor_content(libv::ui::ANCHOR_BOTTOM_LEFT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(101, 202, 300));
//			CHECK(comp1.bounds().position == Pos(101, 200, 300));
//			CHECK(comp2.bounds().position == Pos(100, 202, 300));
//			CHECK(comp3.bounds().position == Pos(100, 200, 300));
//		}
//		SECTION("ANCHOR_BOTTOM_CENTER") {
//			layout.anchor_content(libv::ui::ANCHOR_BOTTOM_CENTER);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(150, 202, 300));
//			CHECK(comp1.bounds().position == Pos(150, 200, 300));
//			CHECK(comp2.bounds().position == Pos(149, 202, 300));
//			CHECK(comp3.bounds().position == Pos(149, 200, 300));
//		}
//		SECTION("ANCHOR_BOTTOM_RIGHT") {
//			layout.anchor_content(libv::ui::ANCHOR_BOTTOM_RIGHT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(199, 202, 300));
//			CHECK(comp1.bounds().position == Pos(199, 200, 300));
//			CHECK(comp2.bounds().position == Pos(198, 202, 300));
//			CHECK(comp3.bounds().position == Pos(198, 200, 300));
//		}
//	}
//
//	SECTION("column 3") {
//		layout.column_count(3);
//
//		SECTION("ANCHOR_TOP_LEFT") {
//			layout.anchor_content(libv::ui::ANCHOR_TOP_LEFT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(101, 298, 300));
//			CHECK(comp1.bounds().position == Pos(101, 296, 300));
//			CHECK(comp2.bounds().position == Pos(101, 294, 300));
//			CHECK(comp3.bounds().position == Pos(100, 298, 300));
//		}
//		SECTION("ANCHOR_TOP_CENTER") {
//			layout.anchor_content(libv::ui::ANCHOR_TOP_CENTER);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(150, 298, 300));
//			CHECK(comp1.bounds().position == Pos(150, 296, 300));
//			CHECK(comp2.bounds().position == Pos(150, 294, 300));
//			CHECK(comp3.bounds().position == Pos(149, 298, 300));
//		}
//		SECTION("ANCHOR_TOP_RIGHT") {
//			layout.anchor_content(libv::ui::ANCHOR_TOP_RIGHT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(199, 298, 300));
//			CHECK(comp1.bounds().position == Pos(199, 296, 300));
//			CHECK(comp2.bounds().position == Pos(199, 294, 300));
//			CHECK(comp3.bounds().position == Pos(198, 298, 300));
//		}
//		SECTION("ANCHOR_CENTER_LEFT") {
//			layout.anchor_content(libv::ui::ANCHOR_CENTER_LEFT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(101, 251, 300));
//			CHECK(comp1.bounds().position == Pos(101, 249, 300));
//			CHECK(comp2.bounds().position == Pos(101, 247, 300));
//			CHECK(comp3.bounds().position == Pos(100, 251, 300));
//		}
//		SECTION("ANCHOR_CENTER_CENTER") {
//			layout.anchor_content(libv::ui::ANCHOR_CENTER_CENTER);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(150, 251, 300));
//			CHECK(comp1.bounds().position == Pos(150, 249, 300));
//			CHECK(comp2.bounds().position == Pos(150, 247, 300));
//			CHECK(comp3.bounds().position == Pos(149, 251, 300));
//		}
//		SECTION("ANCHOR_CENTER_RIGHT") {
//			layout.anchor_content(libv::ui::ANCHOR_CENTER_RIGHT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(199, 251, 300));
//			CHECK(comp1.bounds().position == Pos(199, 249, 300));
//			CHECK(comp2.bounds().position == Pos(199, 247, 300));
//			CHECK(comp3.bounds().position == Pos(198, 251, 300));
//		}
//		SECTION("ANCHOR_BOTTOM_LEFT") {
//			layout.anchor_content(libv::ui::ANCHOR_BOTTOM_LEFT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(101, 204, 300));
//			CHECK(comp1.bounds().position == Pos(101, 202, 300));
//			CHECK(comp2.bounds().position == Pos(101, 200, 300));
//			CHECK(comp3.bounds().position == Pos(100, 204, 300));
//		}
//		SECTION("ANCHOR_BOTTOM_CENTER") {
//			layout.anchor_content(libv::ui::ANCHOR_BOTTOM_CENTER);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(150, 204, 300));
//			CHECK(comp1.bounds().position == Pos(150, 202, 300));
//			CHECK(comp2.bounds().position == Pos(150, 200, 300));
//			CHECK(comp3.bounds().position == Pos(149, 204, 300));
//		}
//		SECTION("ANCHOR_BOTTOM_RIGHT") {
//			layout.anchor_content(libv::ui::ANCHOR_BOTTOM_RIGHT);
//			layout.layout1(env1);
//			layout.layout2(env2);
//
//			CHECK(comp0.bounds().position == Pos(199, 204, 300));
//			CHECK(comp1.bounds().position == Pos(199, 202, 300));
//			CHECK(comp2.bounds().position == Pos(199, 200, 300));
//			CHECK(comp3.bounds().position == Pos(198, 204, 300));
//		}
//	}
//}
//
//TEST_CASE("layout grid: layout2 cell anchor test", "[libv.ui.layout.grid]") {
//	TestLayout layout;
//	auto comp0 = layout.add();
//	auto comp1 = layout.add();
//	auto comp2 = layout.add();
//	auto comp3 = layout.add();
//
//	libv::ui::ContextLayout1 env1;
//	libv::ui::ContextLayout2 env2{libv::vec3f(100, 200, 300), libv::vec3f(100, 100, 100), libv::ui::MouseOrder{0}};
//
//	layout.anchor_content(libv::ui::ANCHOR_CENTER_CENTER);
//	layout.orientation2(libv::ui::Orientation2::right_down);
//	comp0.size("10px, 20px, 30px"); comp0.dynamic(0, 0, 0);
//	comp1.size("10px,  4px,  3px"); comp1.dynamic(0, 0, 0);
//	comp2.size(" 2px, 20px,  3px"); comp2.dynamic(0, 0, 0);
//	comp3.size(" 2px,  4px,  3px"); comp3.dynamic(0, 0, 0);
//	layout.column_count(2);
//
//	SECTION("Flower") {
//		comp0.anchor(libv::ui::ANCHOR_BOTTOM_RIGHT);
//		comp1.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//		comp2.anchor(libv::ui::ANCHOR_TOP_RIGHT);
//		comp3.anchor(libv::ui::ANCHOR_TOP_LEFT);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 250, 300));
//		CHECK(comp2.bounds().position == Pos(148, 230, 300));
//		CHECK(comp3.bounds().position == Pos(150, 246, 300));
//	}
//	SECTION("ANCHOR_TOP_LEFT") {
//		comp0.anchor(libv::ui::ANCHOR_TOP_LEFT);
//		comp1.anchor(libv::ui::ANCHOR_TOP_LEFT);
//		comp2.anchor(libv::ui::ANCHOR_TOP_LEFT);
//		comp3.anchor(libv::ui::ANCHOR_TOP_LEFT);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 266, 300));
//		CHECK(comp2.bounds().position == Pos(140, 230, 300));
//		CHECK(comp3.bounds().position == Pos(150, 246, 300));
//	}
//	SECTION("ANCHOR_TOP_CENTER") {
//		comp0.anchor(libv::ui::ANCHOR_TOP_CENTER);
//		comp1.anchor(libv::ui::ANCHOR_TOP_CENTER);
//		comp2.anchor(libv::ui::ANCHOR_TOP_CENTER);
//		comp3.anchor(libv::ui::ANCHOR_TOP_CENTER);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 266, 300));
//		CHECK(comp2.bounds().position == Pos(144, 230, 300));
//		CHECK(comp3.bounds().position == Pos(154, 246, 300));
//	}
//	SECTION("ANCHOR_TOP_RIGHT") {
//		comp0.anchor(libv::ui::ANCHOR_TOP_RIGHT);
//		comp1.anchor(libv::ui::ANCHOR_TOP_RIGHT);
//		comp2.anchor(libv::ui::ANCHOR_TOP_RIGHT);
//		comp3.anchor(libv::ui::ANCHOR_TOP_RIGHT);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 266, 300));
//		CHECK(comp2.bounds().position == Pos(148, 230, 300));
//		CHECK(comp3.bounds().position == Pos(158, 246, 300));
//	}
//	SECTION("ANCHOR_CENTER_LEFT") {
//		comp0.anchor(libv::ui::ANCHOR_CENTER_LEFT);
//		comp1.anchor(libv::ui::ANCHOR_CENTER_LEFT);
//		comp2.anchor(libv::ui::ANCHOR_CENTER_LEFT);
//		comp3.anchor(libv::ui::ANCHOR_CENTER_LEFT);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 258, 300));
//		CHECK(comp2.bounds().position == Pos(140, 230, 300));
//		CHECK(comp3.bounds().position == Pos(150, 238, 300));
//	}
//	SECTION("ANCHOR_CENTER_CENTER") {
//		comp0.anchor(libv::ui::ANCHOR_CENTER_CENTER);
//		comp1.anchor(libv::ui::ANCHOR_CENTER_CENTER);
//		comp2.anchor(libv::ui::ANCHOR_CENTER_CENTER);
//		comp3.anchor(libv::ui::ANCHOR_CENTER_CENTER);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 258, 300));
//		CHECK(comp2.bounds().position == Pos(144, 230, 300));
//		CHECK(comp3.bounds().position == Pos(154, 238, 300));
//	}
//	SECTION("ANCHOR_CENTER_RIGHT") {
//		comp0.anchor(libv::ui::ANCHOR_CENTER_RIGHT);
//		comp1.anchor(libv::ui::ANCHOR_CENTER_RIGHT);
//		comp2.anchor(libv::ui::ANCHOR_CENTER_RIGHT);
//		comp3.anchor(libv::ui::ANCHOR_CENTER_RIGHT);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 258, 300));
//		CHECK(comp2.bounds().position == Pos(148, 230, 300));
//		CHECK(comp3.bounds().position == Pos(158, 238, 300));
//	}
//	SECTION("ANCHOR_BOTTOM_LEFT") {
//		comp0.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//		comp1.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//		comp2.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//		comp3.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 250, 300));
//		CHECK(comp2.bounds().position == Pos(140, 230, 300));
//		CHECK(comp3.bounds().position == Pos(150, 230, 300));
//	}
//	SECTION("ANCHOR_BOTTOM_CENTER") {
//		comp0.anchor(libv::ui::ANCHOR_BOTTOM_CENTER);
//		comp1.anchor(libv::ui::ANCHOR_BOTTOM_CENTER);
//		comp2.anchor(libv::ui::ANCHOR_BOTTOM_CENTER);
//		comp3.anchor(libv::ui::ANCHOR_BOTTOM_CENTER);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 250, 300));
//		CHECK(comp2.bounds().position == Pos(144, 230, 300));
//		CHECK(comp3.bounds().position == Pos(154, 230, 300));
//	}
//	SECTION("ANCHOR_BOTTOM_RIGHT") {
//		comp0.anchor(libv::ui::ANCHOR_BOTTOM_RIGHT);
//		comp1.anchor(libv::ui::ANCHOR_BOTTOM_RIGHT);
//		comp2.anchor(libv::ui::ANCHOR_BOTTOM_RIGHT);
//		comp3.anchor(libv::ui::ANCHOR_BOTTOM_RIGHT);
//
//		layout.layout1(env1);
//		layout.layout2(env2);
//
//		CHECK(comp0.bounds().position == Pos(140, 250, 300));
//		CHECK(comp1.bounds().position == Pos(150, 250, 300));
//		CHECK(comp2.bounds().position == Pos(148, 230, 300));
//		CHECK(comp3.bounds().position == Pos(158, 230, 300));
//	}
//}
//
//TEST_CASE("layout grid: layout2 complex test 0", "[libv.ui.layout.grid]") {
//	TestLayout layout;
//	auto comp0 = layout.add();
//	auto comp1 = layout.add();
//	auto comp2 = layout.add();
//	auto comp3 = layout.add();
//
//	libv::ui::ContextLayout1 env1;
//	libv::ui::ContextLayout2 env2{libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 200), libv::ui::MouseOrder{0}};
//
//	layout.anchor_content(libv::ui::ANCHOR_CENTER_CENTER);
//	layout.orientation2(libv::ui::Orientation2::left_down);
//	layout.column_count(3);
//	comp0.anchor(libv::ui::ANCHOR_BOTTOM_RIGHT);
//	comp1.anchor(libv::ui::ANCHOR_BOTTOM_LEFT);
//	comp2.anchor(libv::ui::ANCHOR_TOP_RIGHT);
//	comp3.anchor(libv::ui::ANCHOR_TOP_LEFT);
//
//	comp0.size("  0px 20% 1r D,  10px  0% 1r D,   0px 20% 1r D"); comp0.dynamic(10, 20, 30);
//	comp1.size("150px 20% 2r  ,  75px 10% 2r D, 150px 20% 2r  "); comp1.dynamic(11, 21, 31);
//	comp2.size("200px 10%    D,  50px 20% 3r  , 200px 10% 3r D"); comp2.dynamic(12, 22, 32);
//	comp3.size("250px  0% 4r D,   0px 10% 4r  ,   0px  0%     "); comp3.dynamic(13, 23, 33);
//
//	layout.layout1(env1);
//	layout.layout2(env2);
//
//	CHECK(comp0.bounds() == Bounds(368, 121, 0,  32,  53,  53));
//	CHECK(comp1.bounds() == Bounds(252, 121, 0, 115, 172, 155));
//	CHECK(comp2.bounds() == Bounds(  0, 121, 0, 252, 179, 200));
//	CHECK(comp3.bounds() == Bounds(367,   0, 0,  33, 121,   0));
//}
