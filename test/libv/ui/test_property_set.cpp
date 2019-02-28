// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ui/style.hpp>
#include <libv/ui/property_set.hpp>


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
	libv::ui::PropertySet<TestPropertySet> view;

	style.set("color", libv::ui::Color{1, 0, 0, 1});

	CHECK(view.color == libv::ui::Color{0, 0, 0, 1});
	CHECK(view.color2 == libv::ui::Color{0, 0, 0, 1});

	view.set(style);

	CHECK(view.color == libv::ui::Color{1, 0, 0, 1});
	CHECK(view.color2 == libv::ui::Color{0, 0, 0, 1});
}

// -------------------------------------------------------------------------------------------------
