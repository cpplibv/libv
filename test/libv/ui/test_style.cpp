// Project: libv.ui, File: test/libv/ui/test_style.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/ui/property/color.hpp>
#include <libv/ui/style/style.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Style supports set and get property", "[libv.ui]") {
	libv::ui::Style style{"test"};

	style.set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});

	const auto* color = style.get_optional(libv::ui::StyleState::none, "color");
	const auto* color2 = style.get_optional(libv::ui::StyleState::none, "color2");

	CHECK(style.style_name == "test");
	REQUIRE(color);
	REQUIRE(std::any_cast<libv::ui::Color>(color));
	CHECK(*std::any_cast<libv::ui::Color>(color) == libv::ui::Color{1, 0, 0, 1});

	CHECK(not color2);
}

TEST_CASE("Style single inheritance", "[libv.ui]") {
	auto style_base = libv::make_intrusive2_ptr<libv::ui::Style>("base");
	libv::ui::Style style_derived{"derived"};
	style_derived.inherit(style_base);

	style_base->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});

	{
		const auto* color = style_base->get_optional(libv::ui::StyleState::none, "color");
		const auto* color2 = style_base->get_optional(libv::ui::StyleState::none, "color2");

		REQUIRE(color);
		REQUIRE(std::any_cast<libv::ui::Color>(color));
		CHECK(*std::any_cast<libv::ui::Color>(color) == libv::ui::Color{1, 0, 0, 1});
		CHECK(not color2);
	}
	{
		const auto* color = style_derived.get_optional(libv::ui::StyleState::none, "color");
		const auto* color2 = style_derived.get_optional(libv::ui::StyleState::none, "color2");

		REQUIRE(color);
		REQUIRE(std::any_cast<libv::ui::Color>(color));
		CHECK(*std::any_cast<libv::ui::Color>(color) == libv::ui::Color{1, 0, 0, 1});
		CHECK(not color2);
	}
}

TEST_CASE("Style multiple inheritance", "[libv.ui]") {
	auto style_base_a = libv::make_intrusive2_ptr<libv::ui::Style>("base-a");
	auto style_base_b = libv::make_intrusive2_ptr<libv::ui::Style>("base-b");
	auto style_derived = libv::make_intrusive2_ptr<libv::ui::Style>("derived");
	style_derived->inherit(style_base_a);
	style_derived->inherit(style_base_b);

	SECTION("style_base_a set") {
		style_base_a->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}
	SECTION("style_base_b set") {
		style_base_b->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}
	SECTION("style_derived set") {
		style_derived->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}

	const auto* color = style_derived->get_optional(libv::ui::StyleState::none, "color");
	const auto* color2 = style_derived->get_optional(libv::ui::StyleState::none, "color2");

	REQUIRE(color);
	REQUIRE(std::any_cast<libv::ui::Color>(color));
	CHECK(*std::any_cast<libv::ui::Color>(color) == libv::ui::Color{1, 0, 0, 1});
	CHECK(not color2);
}

TEST_CASE("Style transitive inheritance", "[libv.ui]") {
	auto style_base = libv::make_intrusive2_ptr<libv::ui::Style>("base");
	auto style_middle = libv::make_intrusive2_ptr<libv::ui::Style>("middle");
	auto style_derived = libv::make_intrusive2_ptr<libv::ui::Style>("derived");
	style_middle->inherit(style_base);
	style_derived->inherit(style_middle);

	SECTION("style_base set") {
		style_base->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}
	SECTION("style_middle set") {
		style_middle->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}
	SECTION("style_derived set") {
		style_derived->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}

	const auto* color = style_derived->get_optional(libv::ui::StyleState::none, "color");
	const auto* color2 = style_derived->get_optional(libv::ui::StyleState::none, "color2");

	REQUIRE(color);
	REQUIRE(std::any_cast<libv::ui::Color>(color));
	CHECK(*std::any_cast<libv::ui::Color>(color) == libv::ui::Color{1, 0, 0, 1});
	CHECK(not color2);
}

TEST_CASE("Style multiple transitive inheritance", "[libv.ui]") {
	auto style_base_a = libv::make_intrusive2_ptr<libv::ui::Style>("base-a");
	auto style_base_b = libv::make_intrusive2_ptr<libv::ui::Style>("base-b");
	auto style_middle = libv::make_intrusive2_ptr<libv::ui::Style>("middle");
	auto style_derived = libv::make_intrusive2_ptr<libv::ui::Style>("derived");
	style_middle->inherit(style_base_a);
	style_middle->inherit(style_base_b);
	style_derived->inherit(style_middle);

	SECTION("style_base set") {
		style_base_a->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}
	SECTION("style_base set") {
		style_base_b->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}
	SECTION("style_middle set") {
		style_middle->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}
	SECTION("style_derived set") {
		style_derived->set(libv::ui::StyleState::none, libv::ui::StyleState::none, "color", libv::ui::Color{1, 0, 0, 1});
	}

	const auto* color = style_derived->get_optional(libv::ui::StyleState::none, "color");
	const auto* color2 = style_derived->get_optional(libv::ui::StyleState::none, "color2");

	REQUIRE(color);
	REQUIRE(std::any_cast<libv::ui::Color>(color));
	CHECK(*std::any_cast<libv::ui::Color>(color) == libv::ui::Color{1, 0, 0, 1});
	CHECK(not color2);
}

// -------------------------------------------------------------------------------------------------
