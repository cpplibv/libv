// Project: libv.ui, File: test/libv/ui/test_layout_slc.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include "test_layout_utility.hpp"
#include <libv/ui/component/layout/layout_slc.hpp>


// -------------------------------------------------------------------------------------------------

using namespace libv::ui;

// -------------------------------------------------------------------------------------------------

using approx_vec2 = libv::vec2_t<libv::approx<float, -3>>;
using approx_vec4 = libv::vec4_t<libv::approx<float, -3>>;
using vec2 = libv::vec2f;

static constexpr vec2 infpar_size = {-1, -1};

template <typename T>
[[nodiscard]] libv::vec2f testQueryPass1(libv::vec2f size, const T& plan) {
	auto copy = plan;
	return libv::ui::layoutSLCPass1(size, copy);
}

template <typename T>
void testLayoutPass2(libv::vec2f size, T& plan) {
	libv::ui::layoutSLCPass2(size, libv::ui::ContextLayout2{libv::vec3f{}, libv::vec3f{}}, plan);
}

struct TestComp {
	libv::ui::Size size;
	// libv::ui::Margin margin;
	// libv::ui::Padding padding;
	MarginExtent margin;
	PaddingExtent padding;

	libv::ui::Anchor anchor;
	libv::vec2f virtual_dynamic; // Value that simulates the result of a layout1 pass

	libv::vec2f layout_position;
	libv::vec2f layout_size;

	explicit TestComp(Size size = {}, MarginExtent margin = {}, PaddingExtent padding = {}, Anchor anchor = {}, libv::vec2f dynamic = {}) :
			size(std::move(size)),
			margin(margin),
			padding(padding),
			anchor(anchor),
			virtual_dynamic(dynamic) {}

	explicit TestComp(std::string_view size_str, MarginExtent margin = {}, PaddingExtent padding = {}, Anchor anchor = {}, libv::vec2f dynamic = {}) :
			TestComp(libv::ui::parse_size_or_throw(size_str), margin, padding, anchor, dynamic) {}

	[[nodiscard]] constexpr inline libv::vec4f bounds() const noexcept {
		return {layout_position, layout_size};
	}

	[[nodiscard]] constexpr inline libv::vec2f calculate_dynamic(libv::vec2f limits) const noexcept {
		return {
			limits.x < 0.f ? virtual_dynamic.x : std::min(limits.x, virtual_dynamic.x),
			limits.y < 0.f ? virtual_dynamic.y : std::min(limits.y, virtual_dynamic.y)
		};
	}
};

struct SLC_Test : libv::ui::SLC_Node {
	TestComp& component;

public:
	explicit constexpr LIBV_FORCE_INLINE SLC_Test(TestComp& component) : component(component) {}

public:
	[[nodiscard]] constexpr libv::vec2f calculate_ratio() {
		return ratio_sum = component.size.ratio2();
	}

	[[nodiscard]] constexpr libv::vec2f assign_pixel_size(libv::vec2f area_size) {
		// current_size += component.margin.size();
		// area_size -= component.margin.size();
		(void) area_size;
		const auto change = component.size.pixel2();
		current_size += change;
		return change;
	}
	[[nodiscard]] constexpr libv::vec2f assign_percent_size(libv::vec2f area_size) {
		const auto change = component.size.percent2() / 100.f * area_size;
		current_size += change;
		return change;
	}
	[[nodiscard]] constexpr libv::vec2f assign_dynamic_size(libv::vec2f area_size) {
		const auto dynamic = component.size.dynamic_mask2() * component.calculate_dynamic(area_size);
		const auto new_size = libv::max(current_size, dynamic);
		const auto change = new_size - current_size;
		current_size = new_size;
		return change;
	}
	[[nodiscard]] constexpr libv::vec2f assign_ratio_size(libv::vec2f area_size) {
		const auto change = component.size.ratio_mask2() * (area_size - current_size);
		current_size += change;
		return change;
	}

	constexpr void assign_position(libv::vec2f area_position, libv::vec2f area_size, const ContextLayout2& environment) {
		(void) area_size;
		(void) environment;
		// component.layout_position = position + component.margin.LB();
		component.layout_position = area_position;
		component.layout_size = current_size;
	}
};

// -------------------------------------------------------------------------------------------------

TEST_CASE("Layout SLC: Empty", "[libv.ui.SLC]") {
	TestComp comp0;

	CHECK(comp0.layout_position == approx_vec2(0, 0));
	CHECK(comp0.layout_size == approx_vec2(0, 0));
}

TEST_CASE("Layout SLC: test", "[libv.ui.SLC]") {
	TestComp comp0("100px, 200px");
	const auto parent_size = vec2(1000, 1200);
	auto plan = SLC_Test(comp0);

	CHECK(testQueryPass1(parent_size, plan) == approx_vec2(100, 200));
	CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(100, 200));
	testLayoutPass2(parent_size, plan);
	CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 200));
}

TEST_CASE("Layout SLC: Component", "[libv.ui.SLC]") {
	const auto parent_size = vec2(1000, 1200);

	SECTION("px, px") {
		TestComp comp0("100px, 200px");
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(100, 200));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(100, 200));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 200));
	}

	SECTION("%, %") {
		TestComp comp0("10%, 20%");
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(0, 0));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(0, 0));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 240));
	}

	SECTION("r, r") {
		TestComp comp0("1r, 2r");
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(0, 0));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(0, 0));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 1000, 1200));
	}

	SECTION("d, d") {
		TestComp comp0("d, d", {}, {}, {}, {100, 200});
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(100, 200));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(100, 200));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 200));
	}
}

TEST_CASE("Layout SLC: Dynamic", "[libv.ui.SLC]") {
	const auto parent_size = vec2(100, 100);

	SECTION("px = 0") {
		TestComp comp0("D, 100px", {}, {}, {}, {15, 10});
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(15, 100));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(15, 100));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 15, 100));
	}

	SECTION("px < din") {
		TestComp comp0("10px D, 100px", {}, {}, {}, {15, 10});
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(15, 100));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(15, 100));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 15, 100));
	}

	SECTION("px = din") {
		TestComp comp0("15px D, 100px", {}, {}, {}, {15, 10});
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(15, 100));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(15, 100));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 15, 100));
	}

	SECTION("px > din") {
		TestComp comp0("20px D, 100px", {}, {}, {}, {15, 10});
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(20, 100));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(20, 100));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 20, 100));
	}

	SECTION("din > limit") {
		TestComp comp0("20px D, 100px", {}, {}, {}, {150, 100});
		auto plan = SLC_Test(comp0);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(100, 100));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(150, 100));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 100));
	}
}

TEST_CASE("Layout SLC: Padding", "[libv.ui.SLC]") {
	const auto parent_size = vec2(1000, 1200);

	// 1
	SECTION("px") {
		TestComp comp0("100px, 200px", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 100, 200));
	}
	SECTION("%") {
		TestComp comp0("10%, 20%", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(40, 60));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(40, 60));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 96, 228));
	}
	SECTION("r") {
		TestComp comp0("1r, 2r", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(40, 60));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(40, 60));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	SECTION("d") {
		TestComp comp0("d, d", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(90, 120));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(90, 120));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 50, 60));
	}
	// 2
	SECTION("px %") {
		TestComp comp0("100px 10%, 200px 20%", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 196, 428));
	}
	SECTION("px r") {
		TestComp comp0("100px 1r, 200px 2r", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	SECTION("px d") {
		TestComp comp0("100px d, 200px d", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 100, 200));
	}
	SECTION("px d large") {
		TestComp comp0("100px d, 200px d", {}, {}, {}, {500, 600});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(540, 660));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(540, 660));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 500, 600));
	}
	SECTION("% r") {
		TestComp comp0("10% 1r, 20% 2r", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(40, 60));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(40, 60));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	SECTION("% d") {
		TestComp comp0("10% d, 20% d", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(90, 120));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(90, 120));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 96, 228));
	}
	SECTION("% d large") {
		TestComp comp0("10% d, 20% d", {}, {}, {}, {500, 600});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(540, 660));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(540, 660));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 500, 600));
	}
	SECTION("d r") {
		TestComp comp0("1r d, 2r d", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(90, 120));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(90, 120));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	// 3
	SECTION("px % r") {
		TestComp comp0("100px 10% 1r, 200px 20% 1r", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	SECTION("px % d") {
		TestComp comp0("100px 10% d, 200px 20% d", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 196, 428));
	}
	SECTION("px % d large") {
		TestComp comp0("100px 10% d, 200px 20% d", {}, {}, {}, {500, 600});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(540, 660));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(540, 660));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 500, 600));
	}
	SECTION("px r d") {
		TestComp comp0("100px 1r d, 200px 2r d", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	SECTION("px r d large") {
		TestComp comp0("100px 1r d, 200px 2r d", {}, {}, {}, {500, 600});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(540, 660));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(540, 660));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	SECTION("% r d") {
		TestComp comp0("10% 1r d, 20% 2r d", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(90, 120));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(90, 120));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	SECTION("% r d large") {
		TestComp comp0("10% 1r d, 20% 2r d", {}, {}, {}, {500, 600});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(540, 660));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(540, 660));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	// 4
	SECTION("px % r d") {
		TestComp comp0("100px 10% 1r d, 200px 20% 2r d", {}, {}, {}, {50, 60});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
	SECTION("px % r d large") {
		TestComp comp0("100px 10% 1r d, 200px 20% 2r d", {}, {}, {}, {500, 600});
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(540, 660));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(540, 660));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 1140));
	}
}

TEST_CASE("Layout SLC: LineV", "[libv.ui.SLC]") {
	const auto parent_size = vec2(1000, 1200);

	TestComp comp0("100px, 200px");
	TestComp comp1("300px, 400px");
	TestComp comp2("500px, 600px");

	SECTION("1 component") {
		auto plan = SLC_LineStatic(5, Orientation::up, SLC_Test(comp0));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(100, 200));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(100, 200));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 200));
	}
	SECTION("2 component") {
		auto plan = SLC_LineStatic(5, Orientation::up, SLC_Test(comp0), SLC_Test(comp1));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(300, 605));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(300, 605));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 200));
		CHECK(comp1.bounds() == approx_vec4(0, 205, 300, 400));
	}
	SECTION("3 component") {
		auto plan = SLC_LineStatic(5, Orientation::up, SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(500, 1210));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(500, 1210));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 200));
		CHECK(comp1.bounds() == approx_vec4(0, 205, 300, 400));
		CHECK(comp2.bounds() == approx_vec4(0, 610, 500, 600));
	}
}

TEST_CASE("Layout SLC: Padding + LineV", "[libv.ui.SLC]") {
	const auto parent_size = vec2(1000, 1200);

	SECTION("px only") {
		TestComp comp0("100px, 200px");
		TestComp comp1("300px, 400px");
		TestComp comp2("500px, 600px");

		SECTION("1 component") {
			auto plan = SLC_Padding({10, 20, 30, 40}, SLC_LineStatic(5, Orientation::up, SLC_Test(comp0)));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 260));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 260));
			testLayoutPass2(parent_size, plan);
			CHECK(comp0.bounds() == approx_vec4(10, 20, 100, 200));
		}
		SECTION("2 component") {
			auto plan = SLC_Padding({10, 20, 30, 40}, SLC_LineStatic(5, Orientation::up, SLC_Test(comp0), SLC_Test(comp1)));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(340, 665));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(340, 665));
			testLayoutPass2(parent_size, plan);
			CHECK(comp0.bounds() == approx_vec4(10, 20, 100, 200));
			CHECK(comp1.bounds() == approx_vec4(10, 225, 300, 400));
		}
		SECTION("3 component") {
			auto plan = SLC_Padding({10, 20, 30, 40}, SLC_LineStatic(5, Orientation::up, SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(540, 1270));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(540, 1270));
			testLayoutPass2(parent_size, plan);
			CHECK(comp0.bounds() == approx_vec4(10, 20, 100, 200));
			CHECK(comp1.bounds() == approx_vec4(10, 225, 300, 400));
			CHECK(comp2.bounds() == approx_vec4(10, 630, 500, 600));
		}
	}

	SECTION("ratio only") {
		TestComp comp0("1r, 2r");
		TestComp comp1("3r, 4r");
		TestComp comp2("5r, 6r");

		SECTION("3 component") {
			auto plan = SLC_Padding({10, 20, 30, 40}, SLC_LineStatic(5, Orientation::up, SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(40, 70));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(40, 70));
			testLayoutPass2(parent_size, plan);
			CHECK(comp0.bounds() == approx_vec4(10, 20, 960, 188.333f));
			CHECK(comp1.bounds() == approx_vec4(10, 213.333f, 960, 376.666f));
			CHECK(comp2.bounds() == approx_vec4(10, 595, 960, 565));
		}
	}
}

TEST_CASE("Layout SLC: Line Orientations", "[libv.ui.SLC]") {
	const auto parent_size = vec2(1000, 1000);
	TestComp comp0("100px, 100px");
	TestComp comp1("100px, 100px");
	TestComp comp2("100px, 100px");

	SECTION("right") {
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_LineStatic(10, Orientation::right, SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(360, 160));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(360, 160));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 100, 100));
		CHECK(comp1.bounds() == approx_vec4(120, 20, 100, 100));
		CHECK(comp2.bounds() == approx_vec4(230, 20, 100, 100));
	}
	SECTION("left") {
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_LineStatic(10, Orientation::left, SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(360, 160));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(360, 160));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(230, 20, 100, 100));
		CHECK(comp1.bounds() == approx_vec4(120, 20, 100, 100));
		CHECK(comp2.bounds() == approx_vec4(10, 20, 100, 100));
	}

	SECTION("up") {
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_LineStatic(10, Orientation::up, SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 380));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 380));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 100, 100));
		CHECK(comp1.bounds() == approx_vec4(10, 130, 100, 100));
		CHECK(comp2.bounds() == approx_vec4(10, 240, 100, 100));
	}
	SECTION("down") {
		auto plan = SLC_Padding({10, 20, 30, 40}, SLC_LineStatic(10, Orientation::down, SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(140, 380));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(140, 380));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 240, 100, 100));
		CHECK(comp1.bounds() == approx_vec4(10, 130, 100, 100));
		CHECK(comp2.bounds() == approx_vec4(10, 20, 100, 100));
	}
}

TEST_CASE("Layout SLC: Padding + LineH Stacking", "[libv.ui.SLC]") {
	SECTION("LineH x1") {
		const auto parent_size = vec2(2000, 1000);
		TestComp comp0("200px, 50px");
		TestComp comp1("400px, 150px");
		TestComp comp2("600px, 250px");

		auto plan = SLC_Padding({10, 20, 30, 40},
				SLC_LineStatic(0, Orientation::right,
					SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)
				));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1240, 310));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1240, 310));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 200, 50));
		CHECK(comp1.bounds() == approx_vec4(210, 20, 400, 150));
		CHECK(comp2.bounds() == approx_vec4(610, 20, 600, 250));
	}
	SECTION("LineH x2") {
		const auto parent_size = vec2(2000, 1000);
		TestComp comp0("200px, 50px");
		TestComp comp1("400px, 150px");
		TestComp comp2("600px, 250px");

		auto plan = SLC_Padding({10, 20, 30, 40},
				SLC_LineStatic(0, Orientation::right,
					SLC_LineStatic(0, Orientation::right,
						SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)
					)
				));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1240, 310));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1240, 310));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 200, 50));
		CHECK(comp1.bounds() == approx_vec4(210, 20, 400, 150));
		CHECK(comp2.bounds() == approx_vec4(610, 20, 600, 250));
	}
	SECTION("LineH x2 px and r") {
		const auto parent_size = vec2(2000, 1000);
		TestComp comp0("200px, 100px");
		TestComp comp1("1r, 100px");

		auto plan = SLC_Padding({10, 20, 30, 40},
				SLC_LineStatic(0, Orientation::right,
					SLC_Test(comp0),
					SLC_Test(comp1)
				));

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(240, 160));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(240, 160));
		testLayoutPass2(parent_size, plan);
		CHECK(comp0.bounds() == approx_vec4(10, 20, 200, 100));
		CHECK(comp1.bounds() == approx_vec4(210, 20, 1760, 100));
	}
	SECTION("LineH x3") {
		const auto parent_size = vec2(2000, 1000);
		TestComp comp0("200px, 50px");
		TestComp comp1("400px, 150px");
		TestComp comp2("600px, 250px");

		SECTION("0 0 3C") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_LineStatic(0, Orientation::right,
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)
							)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1240, 310));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1240, 310));
			testLayoutPass2(parent_size, plan);
		}
		SECTION("0 1B 2C") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_LineStatic(0, Orientation::right,
							SLC_Test(comp0),
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp1), SLC_Test(comp2)
							)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1240, 310));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1240, 310));
			testLayoutPass2(parent_size, plan);
		}
		SECTION("1B 1C 1A") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_Test(comp0),
						SLC_LineStatic(0, Orientation::right,
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp1)
							),
							SLC_Test(comp2)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1240, 310));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1240, 310));
			testLayoutPass2(parent_size, plan);
		}

		CHECK(comp0.bounds() == approx_vec4(10, 20, 200, 50));
		CHECK(comp1.bounds() == approx_vec4(210, 20, 400, 150));
		CHECK(comp2.bounds() == approx_vec4(610, 20, 600, 250));
	}

	SECTION("LineH x3 ratio") {
		const auto parent_size = vec2(2040, 1060);
		TestComp comp0("2r, 1r");
		TestComp comp1("3r, 4r");
		TestComp comp2("5r, 6r");

		SECTION("0 0 3C") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_LineStatic(0, Orientation::right,
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)
							)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(40, 60));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(40, 60));
			testLayoutPass2(parent_size, plan);
		}
		SECTION("0 1B 2C") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_LineStatic(0, Orientation::right,
							SLC_Test(comp0),
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp1), SLC_Test(comp2)
							)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(40, 60));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(40, 60));
			testLayoutPass2(parent_size, plan);
		}
		SECTION("1B 1C 1A") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_Test(comp0),
						SLC_LineStatic(0, Orientation::right,
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp1)
							),
							SLC_Test(comp2)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(40, 60));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(40, 60));
			testLayoutPass2(parent_size, plan);
		}

		CHECK(comp0.bounds() == approx_vec4(10, 20, 400, 1000));
		CHECK(comp1.bounds() == approx_vec4(410, 20, 600, 1000));
		CHECK(comp2.bounds() == approx_vec4(1010, 20, 1000, 1000));
	}

	SECTION("LineH x3 px ratio") {
		const auto parent_size = vec2(2040, 1060);
		TestComp comp0("200px 2r, 1r");
		TestComp comp1("300px 3r, 4r");
		TestComp comp2("500px 5r, 6r");

		SECTION("0 0 3C") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_LineStatic(0, Orientation::right,
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp0), SLC_Test(comp1), SLC_Test(comp2)
							)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1040, 60));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1040, 60));
			testLayoutPass2(parent_size, plan);
		}
		SECTION("0 1B 2C") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_LineStatic(0, Orientation::right,
							SLC_Test(comp0),
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp1), SLC_Test(comp2)
							)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1040, 60));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1040, 60));
			testLayoutPass2(parent_size, plan);
		}
		SECTION("1B 1C 1A") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_Test(comp0),
						SLC_LineStatic(0, Orientation::right,
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp1)
							),
							SLC_Test(comp2)
						)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1040, 60));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1040, 60));
			testLayoutPass2(parent_size, plan);
		}
		SECTION("1C 1A 1A") {
			auto plan = SLC_Padding({10, 20, 30, 40},
					SLC_LineStatic(0, Orientation::right,
						SLC_LineStatic(0, Orientation::right,
							SLC_LineStatic(0, Orientation::right,
								SLC_Test(comp0)
							),
							SLC_Test(comp1)
						),
						SLC_Test(comp2)
					));

			CHECK(testQueryPass1(parent_size, plan) == approx_vec2(1040, 60));
			CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(1040, 60));
			testLayoutPass2(parent_size, plan);
		}

		CHECK(comp0.bounds() == approx_vec4(10, 20, 400, 1000));
		CHECK(comp1.bounds() == approx_vec4(410, 20, 600, 1000));
		CHECK(comp2.bounds() == approx_vec4(1010, 20, 1000, 1000));
	}
}

TEST_CASE("Layout SLC: Composition: 3 Box", "[libv.ui.SLC]") {
	SECTION("px, px, px") {
		const auto parent_size = vec2(1200, 800);
		TestComp comp0("100px, 100px");
		TestComp comp1("100px, 100px");
		TestComp comp2("200px, 100px");

		auto plan =
				SLC_Padding({0, 0, 0, 0},
					SLC_LineStatic(0, Orientation::up,
						SLC_LineStatic(0, Orientation::right,
							SLC_Test(comp0),
							SLC_Test(comp1)
						),
						SLC_Test(comp2)
					)
				);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(200, 200));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(200, 200));
		testLayoutPass2(parent_size, plan);

		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 100));
		CHECK(comp1.bounds() == approx_vec4(100, 0, 100, 100));
		CHECK(comp2.bounds() == approx_vec4(0, 100, 200, 100));
	}

	SECTION("px, px, 1r") {
		const auto parent_size = vec2(1200, 800);
		TestComp comp0("100px, 100px");
		TestComp comp1("100px, 100px");
		TestComp comp2("1r, 100px");

		auto plan =
				SLC_Padding({0, 0, 0, 0},
					SLC_LineStatic(0, Orientation::up,
						SLC_LineStatic(0, Orientation::right,
							SLC_Test(comp0),
							SLC_Test(comp1)
						),
						SLC_Test(comp2)
					)
				);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(200, 200));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(200, 200));
		testLayoutPass2(parent_size, plan);

		CHECK(comp0.bounds() == approx_vec4(0, 0, 100, 100));
		CHECK(comp1.bounds() == approx_vec4(100, 0, 100, 100));
		CHECK(comp2.bounds() == approx_vec4(0, 100, 1200, 100));
	}

	SECTION("1r, 1r, 1r") {
		const auto parent_size = vec2(100, 100);
		TestComp comp0("1r, 1r");
		TestComp comp1("1r, 1r");
		TestComp comp2("1r, 1r");

		auto plan =
				SLC_Padding({0, 0, 0, 0},
					SLC_LineStatic(0, Orientation::up,
						SLC_LineStatic(0, Orientation::right,
							SLC_Test(comp0),
							SLC_Test(comp1)
						),
						SLC_Test(comp2)
					)
				);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(0, 0));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(0, 0));
		testLayoutPass2(parent_size, plan);

		CHECK(comp0.bounds() == approx_vec4(0, 0, 50, 50));
		CHECK(comp1.bounds() == approx_vec4(50, 0, 50, 50));
		CHECK(comp2.bounds() == approx_vec4(0, 50, 100, 50));
	}
}

TEST_CASE("Layout SLC: Composition: Scroll Pane", "[libv.ui.SLC]") {
	const auto parent_size = vec2(1200, 800);
	TestComp area("1r, 1r");
	TestComp hbar("1r, 10px");
	TestComp vbar("10px, 1r");

	SECTION("both") {
		auto plan =
				SLC_Padding({0, 0, 0, 0},
					SLC_LineStatic(0, Orientation::right,
						SLC_LineStatic(0, Orientation::down,
							SLC_Test(area),
							SLC_Test(hbar)
						),
						SLC_Test(vbar)
					)
				);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(10, 10));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(10, 10));
		testLayoutPass2(parent_size, plan);
		CHECK(area.bounds() == approx_vec4(0, 10, 1190, 790));
		CHECK(hbar.bounds() == approx_vec4(0, 0, 1190, 10));
		CHECK(vbar.bounds() == approx_vec4(1190, 0, 10, 800));
	}

	SECTION("vert") {
		auto plan =
				SLC_Padding({0, 0, 0, 0},
					SLC_LineStatic(0, Orientation::right,
						SLC_Test(area),
						SLC_Test(vbar)
					)
				);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(10, 0));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(10, 0));
		testLayoutPass2(parent_size, plan);
		CHECK(area.bounds() == approx_vec4(0, 0, 1190, 800));
		CHECK(vbar.bounds() == approx_vec4(1190, 0, 10, 800));
	}

	SECTION("hori") {
		auto plan =
				SLC_Padding({0, 0, 0, 0},
					SLC_LineStatic(0, Orientation::down,
						SLC_Test(area),
						SLC_Test(hbar)
					)
				);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(0, 10));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(0, 10));
		testLayoutPass2(parent_size, plan);
		CHECK(area.bounds() == approx_vec4(0, 10, 1200, 790));
		CHECK(hbar.bounds() == approx_vec4(0, 0, 1200, 10));
	}

	SECTION("none") {
		auto plan =
				SLC_Padding({0, 0, 0, 0},
					SLC_Test(area)
				);

		CHECK(testQueryPass1(parent_size, plan) == approx_vec2(0, 0));
		CHECK(testQueryPass1(infpar_size, plan) == approx_vec2(0, 0));
		testLayoutPass2(parent_size, plan);
		CHECK(area.bounds() == approx_vec4(0, 0, 1200, 800));
	}
}

// TEST_CASE("Layout SLC: positioning with alignment", "[libv.ui.SLC]") {
// 	TestPanel<libv::ui::PanelLine> panel;
//
// 	TestComponent comp0 = panel.add("40px, 60px", D( 20, 100, 0));
// 	TestComponent comp1 = panel.add("50px, 50px", D( 60,  40, 0));
// 	TestComponent comp2 = panel.add("60px, 40px", D(100,  10, 0));
//
// 	panel.orientation(libv::ui::Orientation::right);
//
// 	SECTION("alignment: TOP_LEFT") {
// 		panel.align_horizontal(libv::ui::AlignHorizontal::left);
// 		panel.align_vertical(libv::ui::AlignVertical::top);
//
// 		CHECK(panel.layout1(0, 0, 0) == approx_size(150, 60, 0));
// 		panel.layout2(400, 300);
//
// 		CHECK(comp0.bounds().position == approx_pos(0.f, 240.f, 0.f));
// 		CHECK(comp1.bounds().position == approx_pos(40.f, 250.f, 0.f));
// 		CHECK(comp2.bounds().position == approx_pos(90.f, 260.f, 0.f));
// 	}
//
// 	CHECK(comp0.bounds().size == approx_size(40.f, 60.f, 0.f));
// 	CHECK(comp1.bounds().size == approx_size(50.f, 50.f, 0.f));
// 	CHECK(comp2.bounds().size == approx_size(60.f, 40.f, 0.f));
// }