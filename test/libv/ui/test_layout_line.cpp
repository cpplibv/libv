// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ui/layout/layout_line.hpp>
#include "test_layout_utility.hpp"


// -------------------------------------------------------------------------------------------------

namespace {

struct TestChild : public BasicTestChild<libv::ui::LayoutLine> {
	using BasicTestChild<libv::ui::LayoutLine>::BasicTestChild;

	void size(std::string_view value) {
		components[index].ptr->set(components[index].properties.size, libv::ui::parse_size_or_throw(value));
	}
};

struct TestLayout : public BasicTestLayout<libv::ui::LayoutLine, TestChild> {
	template <typename V>
	void alignHorizontal(V&& value) {
		ignore.set(properties.alignHorizontal, std::forward<V>(value));
	}

	template <typename V>
	void alignVertical(V&& value) {
		ignore.set(properties.alignVertical, std::forward<V>(value));
	}

	template <typename V>
	void orientation(V&& value) {
		ignore.set(properties.orientation, std::forward<V>(value));
	}
};

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("layout line: empty", "[libv.ui.layout.line]") {
	TestLayout layout;
	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2{libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 0), libv::ui::MouseOrder{0}};

	CHECK(layout.layout1(env1) == Size(0, 0, 0));
	layout.layout2(env2);
}

TEST_CASE("layout line: positioning with alignment", "[libv.ui.layout.line]") {
	TestLayout layout;
	auto comp0 = layout.add();
	auto comp1 = layout.add();
	auto comp2 = layout.add();

	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0), libv::ui::MouseOrder{0});

	comp0.size("40px, 60px"); comp0.dynamic( 20, 100);
	comp1.size("50px, 50px"); comp1.dynamic( 60,  40);
	comp2.size("60px, 40px"); comp2.dynamic(100,  10);

	layout.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

	SECTION("alignment: TOP_LEFT") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Left);
		layout.alignVertical(libv::ui::AlignVertical::Top);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == Pos(40.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == Pos(90.f, 260.f, 0.f));
	}

	SECTION("alignment: TOP_CENTER") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Center);
		layout.alignVertical(libv::ui::AlignVertical::Top);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(125.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == Pos(165.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == Pos(215.f, 260.f, 0.f));
	}

	SECTION("alignment: TOP_RIGHT") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Right);
		layout.alignVertical(libv::ui::AlignVertical::Top);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(250.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == Pos(290.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == Pos(340.f, 260.f, 0.f));
	}

	SECTION("alignment: TOP_JUSTIFY") {
		SECTION("Justify") {
			layout.alignHorizontal(libv::ui::AlignHorizontal::Justify);
		}
		SECTION("JustifyAll") {
			layout.alignHorizontal(libv::ui::AlignHorizontal::JustifyAll);
		}
		layout.alignVertical(libv::ui::AlignVertical::Top);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == Pos(165.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == Pos(340.f, 260.f, 0.f));
	}

	SECTION("alignment: CENTER_LEFT") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Left);
		layout.alignVertical(libv::ui::AlignVertical::Center);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 120.f, 0.f));
		CHECK(comp1.bounds().position == Pos(40.f, 125.f, 0.f));
		CHECK(comp2.bounds().position == Pos(90.f, 130.f, 0.f));
	}

	SECTION("alignment: CENTER_CENTER") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Center);
		layout.alignVertical(libv::ui::AlignVertical::Center);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(125.f, 120.f, 0.f));
		CHECK(comp1.bounds().position == Pos(165.f, 125.f, 0.f));
		CHECK(comp2.bounds().position == Pos(215.f, 130.f, 0.f));
	}

	SECTION("alignment: CENTER_RIGHT") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Right);
		layout.alignVertical(libv::ui::AlignVertical::Center);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(250.f, 120.f, 0.f));
		CHECK(comp1.bounds().position == Pos(290.f, 125.f, 0.f));
		CHECK(comp2.bounds().position == Pos(340.f, 130.f, 0.f));
	}

	SECTION("alignment: CENTER_JUSTIFY") {
		SECTION("Justify") {
			layout.alignHorizontal(libv::ui::AlignHorizontal::Justify);
		}
		SECTION("JustifyAll") {
			layout.alignHorizontal(libv::ui::AlignHorizontal::JustifyAll);
		}
		layout.alignVertical(libv::ui::AlignVertical::Center);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 120.f, 0.f));
		CHECK(comp1.bounds().position == Pos(165.f, 125.f, 0.f));
		CHECK(comp2.bounds().position == Pos(340.f, 130.f, 0.f));
	}

	SECTION("alignment: BOTTOM_LEFT") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Left);
		layout.alignVertical(libv::ui::AlignVertical::Bottom);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 0.f, 0.f));
		CHECK(comp1.bounds().position == Pos(40.f, 0.f, 0.f));
		CHECK(comp2.bounds().position == Pos(90.f, 0.f, 0.f));
	}

	SECTION("alignment: BOTTOM_CENTER") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Center);
		layout.alignVertical(libv::ui::AlignVertical::Bottom);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(125.f, 0.f, 0.f));
		CHECK(comp1.bounds().position == Pos(165.f, 0.f, 0.f));
		CHECK(comp2.bounds().position == Pos(215.f, 0.f, 0.f));
	}

	SECTION("alignment: BOTTOM_RIGHT") {
		layout.alignHorizontal(libv::ui::AlignHorizontal::Right);
		layout.alignVertical(libv::ui::AlignVertical::Bottom);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(250.f, 0.f, 0.f));
		CHECK(comp1.bounds().position == Pos(290.f, 0.f, 0.f));
		CHECK(comp2.bounds().position == Pos(340.f, 0.f, 0.f));
	}

	SECTION("alignment: BOTTOM_JUSTIFY") {
		SECTION("Justify") {
			layout.alignHorizontal(libv::ui::AlignHorizontal::Justify);
		}
		SECTION("JustifyAll") {
			layout.alignHorizontal(libv::ui::AlignHorizontal::JustifyAll);
		}
		layout.alignVertical(libv::ui::AlignVertical::Bottom);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 0.f, 0.f));
		CHECK(comp1.bounds().position == Pos(165.f, 0.f, 0.f));
		CHECK(comp2.bounds().position == Pos(340.f, 0.f, 0.f));
	}

	CHECK(comp0.bounds().size == Size(40.f, 60.f, 0.f));
	CHECK(comp1.bounds().size == Size(50.f, 50.f, 0.f));
	CHECK(comp2.bounds().size == Size(60.f, 40.f, 0.f));
}

TEST_CASE("layout line: positioning with orientation", "[libv.ui.layout.line]") {
	TestLayout layout;
	auto comp0 = layout.add();
	auto comp1 = layout.add();
	auto comp2 = layout.add();

	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0), libv::ui::MouseOrder{0});

	comp0.size("40px, 60px"); comp0.dynamic( 20, 100);
	comp1.size("50px, 50px"); comp1.dynamic( 60,  40);
	comp2.size("60px, 40px"); comp2.dynamic(100,  10);

	layout.alignHorizontal(libv::ui::AlignHorizontal::Left);
	layout.alignVertical(libv::ui::AlignVertical::Top);

	SECTION("orient: BOTTOM_TO_TOP") {
		layout.orientation(libv::ui::Orientation::BOTTOM_TO_TOP);

		CHECK(layout.layout1(env1) == Size(60, 150, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 150.f, 0.f));
		CHECK(comp1.bounds().position == Pos(0.f, 210.f, 0.f));
		CHECK(comp2.bounds().position == Pos(0.f, 260.f, 0.f));
	}

	SECTION("orient: LEFT_TO_RIGHT") {
		layout.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == Pos(40.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == Pos(90.f, 260.f, 0.f));
	}

	SECTION("orient: RIGHT_TO_LEFT") {
		layout.orientation(libv::ui::Orientation::RIGHT_TO_LEFT);

		CHECK(layout.layout1(env1) == Size(150, 60, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(110.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == Pos(60.f, 250.f, 0.f));
		CHECK(comp2.bounds().position == Pos(0.f, 260.f, 0.f));
	}

	SECTION("orient: TOP_TO_BOTTOM") {
		layout.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

		CHECK(layout.layout1(env1) == Size(60, 150, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0.f, 240.f, 0.f));
		CHECK(comp1.bounds().position == Pos(0.f, 190.f, 0.f));
		CHECK(comp2.bounds().position == Pos(0.f, 150.f, 0.f));
	}

	CHECK(comp0.bounds().size == Size(40.f, 60.f, 0.f));
	CHECK(comp1.bounds().size == Size(50.f, 50.f, 0.f));
	CHECK(comp2.bounds().size == Size(60.f, 40.f, 0.f));
}

TEST_CASE("layout line: sizing one quad", "[libv.ui.layout.line]") {
	TestLayout layout;
	auto comp0 = layout.add();

	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0), libv::ui::MouseOrder{0});

	layout.alignHorizontal(libv::ui::AlignHorizontal::Left);
	layout.alignVertical(libv::ui::AlignVertical::Bottom);
	layout.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);

	SECTION("ratio") {
		comp0.size("1r, 1r"); comp0.dynamic( 20, 100);

		CHECK(layout.layout1(env1) == Size(0, 0, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(400.f, 300.f, 0.f));
	}

	SECTION("pixel") {
		comp0.size("100px, 90px"); comp0.dynamic( 20, 100);

		CHECK(layout.layout1(env1) == Size(100, 90, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(100.f, 90.f, 0.f));
	}

	SECTION("percent") {
		comp0.size("50%, 10%"); comp0.dynamic( 20, 100);

		CHECK(layout.layout1(env1) == Size(0, 0, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(200.f, 30.f, 0.f));
	}

	SECTION("dynamic") {
		comp0.size("D, D"); comp0.dynamic( 20, 100);

		CHECK(layout.layout1(env1) == Size(20, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(20.f, 100.f, 0.f));
	}

	CHECK(comp0.bounds().position == Pos(0, 0, 0));
}

TEST_CASE("layout line: complex test 0", "[libv.ui.layout.line]") {
	TestLayout layout;
	auto comp0 = layout.add();
	auto comp1 = layout.add();
	auto comp2 = layout.add();
	auto comp3 = layout.add();

	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 200), libv::ui::MouseOrder{0});

	comp0.size("  0px 20% 1r D,  10px  0% 1r D,   0px 20% 1r D"); comp0.dynamic(10, 20, 30);
	comp1.size("150px 20% 2r  ,  75px 10% 2r D, 150px 20% 2r  "); comp1.dynamic(11, 21, 31);
	comp2.size("200px 10%    D,  50px 20% 3r  , 200px 10% 3r D"); comp2.dynamic(12, 22, 32);
	comp3.size("250px  0% 4r D,   0px 10% 4r  ,   0px  0%     "); comp3.dynamic(13, 23, 33);

	layout.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
	layout.alignHorizontal(libv::ui::AlignHorizontal::Left);
	layout.alignVertical(libv::ui::AlignVertical::Top);

	CHECK(layout.layout1(env1) == Size(1270.f, 106.66667f, 257.77778f));
	layout.layout2(env2);

	CHECK(comp0.bounds() == Bounds(  0, 0, 0,  28, 300, 200));
	CHECK(comp1.bounds() == Bounds( 28, 0, 0, 106, 300, 200));
	CHECK(comp2.bounds() == Bounds(134, 0, 0, 252, 300, 200));
	CHECK(comp3.bounds() == Bounds(386, 0, 0,  14, 300,   0));
}
