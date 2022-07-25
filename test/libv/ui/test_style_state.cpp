// Project: libv.ui, File: test/libv/ui/test_style_state.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/ui/style_state.hpp>


TEST_CASE("Style State: test always_shadows", "[libv.ui.style]") {
	// hover : none 1/4
	CHECK(always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::hover,
			libv::ui::StyleState::hover, libv::ui::StyleState::hover));

	CHECK(not always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::none,
			libv::ui::StyleState::hover, libv::ui::StyleState::hover));

	CHECK(not always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::none,
			libv::ui::StyleState::hover, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::none,
			libv::ui::StyleState::hover, libv::ui::StyleState::none));

	// hover : none 2/4
	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::hover,
			libv::ui::StyleState::hover, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::none,
			libv::ui::StyleState::hover, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::none,
			libv::ui::StyleState::hover, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::none,
			libv::ui::StyleState::hover, libv::ui::StyleState::none));

	// hover : none 3/4
	CHECK(not always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::hover,
			libv::ui::StyleState::none, libv::ui::StyleState::hover));

	CHECK(not always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::none,
			libv::ui::StyleState::none, libv::ui::StyleState::hover));

	CHECK(not always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::none,
			libv::ui::StyleState::none, libv::ui::StyleState::hover));

	CHECK(not always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::none,
			libv::ui::StyleState::none, libv::ui::StyleState::none));

	// hover : none 4/4
	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::hover,
			libv::ui::StyleState::none, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::none,
			libv::ui::StyleState::none, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::none,
			libv::ui::StyleState::none, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::none,
			libv::ui::StyleState::none, libv::ui::StyleState::none));

	// misc
	CHECK(not always_shadows(
			libv::ui::StyleState::hover | libv::ui::StyleState::focus, libv::ui::StyleState::hover,
			libv::ui::StyleState::none, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::none, libv::ui::StyleState::hover,
			libv::ui::StyleState::hover | libv::ui::StyleState::focus, libv::ui::StyleState::hover));

	CHECK(always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::hover,
			libv::ui::StyleState::hover | libv::ui::StyleState::focus, libv::ui::StyleState::hover));

	CHECK(not always_shadows(
			libv::ui::StyleState::hover, libv::ui::StyleState::none,
			libv::ui::StyleState::hover | libv::ui::StyleState::focus, libv::ui::StyleState::hover));
}
