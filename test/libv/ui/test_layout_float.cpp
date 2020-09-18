// Project: libv.ui, File: test/libv/ui/test_layout_float.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ui/layout/layout_float.hpp>
#include "test_layout_utility.hpp"


// -------------------------------------------------------------------------------------------------

namespace {

struct TestChild : public BasicTestChild<libv::ui::LayoutFloat> {
	using BasicTestChild<libv::ui::LayoutFloat>::BasicTestChild;

	void size(std::string_view value) {
		components[index].ptr.core().set(components[index].property.size, libv::ui::parse_size_or_throw(value));
	}

	template <typename V>
	void anchor_parent(V&& value) {
		components[index].ptr.core().set(components[index].property.anchor_parent, std::forward<V>(value));
	}

	template <typename V>
	void anchor_target(V&& value) {
		components[index].ptr.core().set(components[index].property.anchor_target, std::forward<V>(value));
	}
};

struct TestLayout : public BasicTestLayout<libv::ui::LayoutFloat, TestChild> {
	template <typename V>
	void snap_to_edge(V&& value) {
		ignore.set(property.snap_to_edge, std::forward<V>(value));
	}

	template <typename V>
	void squish(V&& value) {
		ignore.set(property.squish, std::forward<V>(value));
	}
};

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("layout float: empty", "[libv.ui.layout.float]") {
	TestLayout layout;
	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2{libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 0), libv::ui::MouseOrder{0}};

	CHECK(layout.layout1(env1) == Size(0, 0, 0));
	layout.layout2(env2);
}

TEST_CASE("layout float: snap_to_edge", "[libv.ui.layout.float]") {
	// Not implemented yet

	//	TestLayout layout;
	//	libv::ui::ContextLayout1 env1;
	//	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0));
	//
	//	layout.snap_to_edge(false);
	//	layout.snap_to_edge(true);
	//
	//	CHECK(layout.layout1(env1) == Size(0, 0, 0));
	//	layout.layout2(env2);
}

TEST_CASE("layout float: squish", "[libv.ui.layout.float]") {
	// Not implemented yet

	//	TestLayout layout;
	//	libv::ui::ContextLayout1 env1;
	//	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0));
	//
	//	layout.squish(false);
	//	layout.squish(true);
	//
	//	CHECK(layout.layout1(env1) == Size(0, 0, 0));
	//	layout.layout2(env2);
}

TEST_CASE("layout float: positioning with anchors", "[iris.ui]") {
	TestLayout layout;
	auto comp0 = layout.add();

	libv::ui::ContextLayout1 env1;
	libv::ui::ContextLayout2 env2(libv::vec3f(0, 0, 0), libv::vec3f(400, 300, 0), libv::ui::MouseOrder{0});

	comp0.size("100px, 100px"); comp0.dynamic(0, 0);

	SECTION("11") {
		comp0.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
		comp0.anchor_target(libv::ui::ANCHOR_BOTTOM_LEFT);

		CHECK(layout.layout1(env1) == Size(100, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0, 0, 0));
	}

	SECTION("19") {
		comp0.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
		comp0.anchor_target(libv::ui::ANCHOR_TOP_RIGHT);

		CHECK(layout.layout1(env1) == Size(100, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(-100, -100, 0));
	}

	SECTION("55") {
		comp0.anchor_parent(libv::ui::ANCHOR_CENTER_CENTER);
		comp0.anchor_target(libv::ui::ANCHOR_CENTER_CENTER);

		CHECK(layout.layout1(env1) == Size(100, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(150, 100, 0));
	}

	SECTION("59") {
		comp0.anchor_parent(libv::ui::ANCHOR_CENTER_CENTER);
		comp0.anchor_target(libv::ui::ANCHOR_TOP_RIGHT);

		CHECK(layout.layout1(env1) == Size(100, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(100, 50, 0));
	}

	SECTION("63") {
		comp0.anchor_parent(libv::ui::ANCHOR_CENTER_RIGHT);
		comp0.anchor_target(libv::ui::ANCHOR_BOTTOM_RIGHT);

		CHECK(layout.layout1(env1) == Size(100, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(300, 150, 0));
	}

	SECTION("77") {
		comp0.anchor_parent(libv::ui::ANCHOR_TOP_LEFT);
		comp0.anchor_target(libv::ui::ANCHOR_TOP_LEFT);

		CHECK(layout.layout1(env1) == Size(100, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(0, 200, 0));
	}

	SECTION("99") {
		comp0.anchor_parent(libv::ui::ANCHOR_TOP_RIGHT);
		comp0.anchor_target(libv::ui::ANCHOR_TOP_RIGHT);

		CHECK(layout.layout1(env1) == Size(100, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().position == Pos(300, 200, 0));
	}

	CHECK(comp0.bounds().size == Size(100.f, 100.f, 0.f));
}

TEST_CASE("layout float: simple test 0", "[libv.ui.layout.float]") {
	TestLayout layout;
	auto comp0 = layout.add();
	auto comp1 = layout.add();
	auto comp2 = layout.add();

	comp0.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
	comp0.anchor_target(libv::ui::ANCHOR_BOTTOM_LEFT);

	comp1.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
	comp1.anchor_target(libv::ui::ANCHOR_BOTTOM_LEFT);

	comp2.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
	comp2.anchor_target(libv::ui::ANCHOR_BOTTOM_LEFT);

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

		CHECK(comp0.bounds().size == Size(1.f, 30.f, 0.f));
		CHECK(comp1.bounds().size == Size(2.f, 30.f, 0.f));
		CHECK(comp2.bounds().size == Size(3.f, 30.f, 0.f));
	}

	SECTION("percent") {
		comp0.size("1%, 30%"); comp0.dynamic( 20, 100);
		comp1.size("2%, 30%"); comp1.dynamic( 30,  90);
		comp2.size("3%, 30%"); comp2.dynamic( 40,  80);

		CHECK(layout.layout1(env1) == Size(0, 0, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size( 4.f, 90.f, 0.f));
		CHECK(comp1.bounds().size == Size( 8.f, 90.f, 0.f));
		CHECK(comp2.bounds().size == Size(12.f, 90.f, 0.f));
	}

	SECTION("dynamic") {
		comp0.size("D, D"); comp0.dynamic( 20, 100);
		comp1.size("D, D"); comp1.dynamic( 30,  90);
		comp2.size("D, D"); comp2.dynamic( 40,  80);

		CHECK(layout.layout1(env1) == Size(40, 100, 0));
		layout.layout2(env2);

		CHECK(comp0.bounds().size == Size(20.f, 100.f, 0.f));
		CHECK(comp1.bounds().size == Size(30.f,  90.f, 0.f));
		CHECK(comp2.bounds().size == Size(40.f,  80.f, 0.f));
	}

	CHECK(comp0.bounds().position == Pos(0, 0, 0));
}

TEST_CASE("layout float: complex test 0", "[libv.ui.layout.float]") {
	TestLayout layout;
	auto comp0 = layout.add();
	auto comp1 = layout.add();
	auto comp2 = layout.add();
	auto comp3 = layout.add();

	comp0.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
	comp0.anchor_target(libv::ui::ANCHOR_BOTTOM_LEFT);

	comp1.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
	comp1.anchor_target(libv::ui::ANCHOR_BOTTOM_LEFT);

	comp2.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
	comp2.anchor_target(libv::ui::ANCHOR_BOTTOM_LEFT);

	comp3.anchor_parent(libv::ui::ANCHOR_BOTTOM_LEFT);
	comp3.anchor_target(libv::ui::ANCHOR_BOTTOM_LEFT);

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
	CHECK(comp2.bounds() == Bounds(0, 0, 0, 252, 300, 200));
	CHECK(comp3.bounds() == Bounds(0, 0, 0, 400, 300,   0));
}
