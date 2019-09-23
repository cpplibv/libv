// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <array>
#include <string_view>
// pro
#include <libv/range/view_lines_string_view.hpp>

#include <iostream>
// -------------------------------------------------------------------------------------------------

TEST_CASE("view lines simple", "[libv.range.view_lines_string_view]") {
	using sv = std::string_view;

	const auto str = sv("line0\nline1\nline2\n");
	const auto expected = std::array{sv("line0\n"), sv("line1\n"), sv("line2\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines missing last newline", "[libv.range.view_lines_string_view]") {
	using sv = std::string_view;

	const auto str = sv("line0\nline1\nline2");
	const auto expected = std::array{sv("line0\n"), sv("line1\n"), sv("line2")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines empty last line", "[libv.range.view_lines_string_view]") {
	using sv = std::string_view;

	const auto str = sv("line0\nline1\n\n");
	const auto expected = std::array{sv("line0\n"), sv("line1\n"), sv("\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines empty first line", "[libv.range.view_lines_string_view]") {
	using sv = std::string_view;

	const auto str = sv("\nline1\nline2\n");
	const auto expected = std::array{sv("\n"), sv("line1\n"), sv("line2\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines empty lines", "[libv.range.view_lines_string_view]") {
	using sv = std::string_view;

	const auto str = sv("\n\n\n");
	const auto expected = std::array{sv("\n"), sv("\n"), sv("\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines empty", "[libv.range.view_lines_string_view]") {
	using sv = std::string_view;

	const auto str = sv("");
	const auto expected = std::array<sv, 0>{};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

// -------------------------------------------------------------------------------------------------
