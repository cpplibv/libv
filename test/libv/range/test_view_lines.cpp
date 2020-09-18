// Project: libv.range, File: test/libv/range/test_view_lines.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <array>
#include <string>
#include <string_view>
// pro
#include <libv/range/view_lines_string_view.hpp>


// -------------------------------------------------------------------------------------------------

using sv = std::string_view;

#define BYPASS_SBO "012345678_012345678_012345678_012345678_012345678_012345678_012345678_012345678_"

TEST_CASE("view lines simple", "[libv.range.view_lines_string_view]") {
	const auto str = std::string("line0\nline1\nline2\n");
	const auto expected = std::array{sv("line0\n"), sv("line1\n"), sv("line2\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines missing last newline", "[libv.range.view_lines_string_view]") {
	const auto str = std::string("line0\nline1\nline2");
	const auto expected = std::array{sv("line0\n"), sv("line1\n"), sv("line2")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines empty last line", "[libv.range.view_lines_string_view]") {
	const auto str = std::string("line0\nline1\n\n");
	const auto expected = std::array{sv("line0\n"), sv("line1\n"), sv("\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines empty first line", "[libv.range.view_lines_string_view]") {
	const auto str = std::string("\nline1\nline2" BYPASS_SBO "\n");
	const auto expected = std::array{sv("\n"), sv("line1\n"), sv("line2" BYPASS_SBO "\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines empty lines", "[libv.range.view_lines_string_view]") {
	const auto str = std::string(BYPASS_SBO "\n\n\n");
	const auto expected = std::array{sv(BYPASS_SBO "\n"), sv("\n"), sv("\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("view lines empty", "[libv.range.view_lines_string_view]") {
	const auto str = std::string("");
	const auto expected = std::array<sv, 0>{};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

TEST_CASE("scenario 0", "[libv.range.view_lines_string_view]") {
	const auto str = std::string(BYPASS_SBO "*/\n}") + "\n";
	const auto expected = std::array{sv(BYPASS_SBO "*/\n"), sv("}\n")};

	CHECK(ranges::equal(libv::view::lines_string_view(str, '\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view('\n'), expected));
	CHECK(ranges::equal(str | libv::view::lines_string_view(), expected));
}

// -------------------------------------------------------------------------------------------------
