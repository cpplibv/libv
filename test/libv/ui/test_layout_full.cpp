// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ui/layout/layout_full.hpp>
#include "test_layout_utility.hpp"


// -------------------------------------------------------------------------------------------------

namespace {

struct TestChild : public BasicTestChild<libv::ui::LayoutFull> {
	using BasicTestChild<libv::ui::LayoutFull>::BasicTestChild;

	void size(std::string_view value) {
		components[index].ptr->set(components[index].properties.size, libv::ui::parse_size_or_throw(value));
	}
};

struct TestLayout : public BasicTestLayout<libv::ui::LayoutFull, TestChild> {
};

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("layout full: empty", "[libv.ui.layout.full]") {
	TestLayout layout;
	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2{libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 0), libv::ui::MouseOrder{0}};

	CHECK(layout.layout1(env1) == Size(0, 0, 0));
	layout.layout2(env2);
}

TEST_CASE("layout full: simple test 0", "[libv.ui.layout.full]") {
	TestLayout layout;
	auto comp0 = layout.add();
	auto comp1 = layout.add();
	auto comp2 = layout.add();

	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0), libv::ui::MouseOrder{0});

	SECTION("ratio") {
		comp0.size("1r, 1r"); comp0.dynamic( 20, 100);
		comp1.size("2r, 1r"); comp1.dynamic( 30,  90);
		comp2.size("3r, 1r"); comp2.dynamic( 40,  80);

		CHECK(layout.layout1(env1) == Size(0, 0, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(400.f, 300.f, 0.f));
		CHECK(comp1.bounds().size == Size(400.f, 300.f, 0.f));
		CHECK(comp2.bounds().size == Size(400.f, 300.f, 0.f));
	}

	SECTION("pixel") {
		comp0.size("1px, 30px"); comp0.dynamic( 20, 100);
		comp1.size("2px, 30px"); comp1.dynamic( 30,  90);
		comp2.size("3px, 30px"); comp2.dynamic( 40,  80);

		CHECK(layout.layout1(env1) == Size(3, 30, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(400.f, 300.f, 0.f));
		CHECK(comp1.bounds().size == Size(400.f, 300.f, 0.f));
		CHECK(comp2.bounds().size == Size(400.f, 300.f, 0.f));
	}

	SECTION("percent") {
		comp0.size("1%, 30%"); comp0.dynamic( 20, 100);
		comp1.size("2%, 30%"); comp1.dynamic( 30,  90);
		comp2.size("3%, 30%"); comp2.dynamic( 40,  80);

		CHECK(layout.layout1(env1) == Size(0, 0, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(400.f, 300.f, 0.f));
		CHECK(comp1.bounds().size == Size(400.f, 300.f, 0.f));
		CHECK(comp2.bounds().size == Size(400.f, 300.f, 0.f));
	}

	SECTION("dynamic") {
		comp0.size("D, D"); comp0.dynamic( 20, 100);
		comp1.size("D, D"); comp1.dynamic( 30,  90);
		comp2.size("D, D"); comp2.dynamic( 40,  80);

		CHECK(layout.layout1(env1) == Size(40, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(400.f, 300.f, 0.f));
		CHECK(comp1.bounds().size == Size(400.f, 300.f, 0.f));
		CHECK(comp2.bounds().size == Size(400.f, 300.f, 0.f));
	}

	CHECK(comp0.bounds().position == Pos(0, 0, 0));
}

TEST_CASE("layout full: complex test 0", "[libv.ui.layout.full]") {
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

	CHECK(layout.layout1(env1) == Size(263.f, 106.66667f, 257.77778f));
	layout.layout2(env2);

	CHECK(comp0.bounds() == Bounds(0, 0, 0, 400, 300, 200));
	CHECK(comp1.bounds() == Bounds(0, 0, 0, 400, 300, 200));
	CHECK(comp2.bounds() == Bounds(0, 0, 0, 400, 300, 200));
	CHECK(comp3.bounds() == Bounds(0, 0, 0, 400, 300, 200));
}
