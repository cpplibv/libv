// Project: libv.ui, File: test/libv/ui/test_property_set.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/ui/property/color.hpp>
#include <libv/ui/style.hpp>


// -------------------------------------------------------------------------------------------------

namespace {
struct TestPropertySet {
	libv::ui::Color color;
	libv::ui::Color color2;

	template <typename Archive>
	constexpr void properties(Archive& ar) {
		ar(color, "color", "Font color description", libv::ui::Color{0, 0, 0, 1});
		ar(color2, "color2", "Font color2 description", libv::ui::Color{0, 0, 0, 1});
	}
};
} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("PropertySet should fetch Style properties", "[libv.ui]") {
	libv::ui::Style style{"test"};
//	libv::ui::PropertySet<TestPropertySet> view;
//
//	style.set("color", libv::ui::Color{1, 0, 0, 1});

//	CHECK(view.color == libv::ui::Color{0, 0, 0, 1});
//	CHECK(view.color2 == libv::ui::Color{0, 0, 0, 1});
//
//	view.set(style);
//
//	CHECK(view.color == libv::ui::Color{1, 0, 0, 1});
//	CHECK(view.color2 == libv::ui::Color{0, 0, 0, 1});
}

// -------------------------------------------------------------------------------------------------
