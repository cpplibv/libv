// Project: libv.range, File: test/libv/range/test_view_lines.cpp

// hpp
#include <catch/catch.hpp>
// ext
#include <fmt/format.h>
#include <range/v3/algorithm/equal.hpp>
// std
#include <array>
#include <string>
#include <string_view>
// pro
#include <libv/range/view_split_sv.hpp>

// -------------------------------------------------------------------------------------------------

template <typename R>
auto as_str(R&& range) {
	return fmt::format("[{}]", fmt::join(range, "|"));
}

#define BYPASS_SBO "012345678_012345678_012345678_012345678_012345678_012345678_012345678_012345678_"


TEST_CASE("view split_sv empty", "[libv.range.view_split_sv]") {
	const auto str = std::string("");
	const auto expected = "[]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv single line without separator", "[libv.range.view_split_sv]") {
	const auto str = std::string("line0");
	const auto expected = "[line0]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv simple", "[libv.range.view_split_sv]") {
	const auto str = std::string("line0\nline1\nline2");
	const auto expected = "[line0|line1|line2]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv simple with extra separator at end", "[libv.range.view_split_sv]") {
	const auto str = std::string("line0\nline1\nline2\n");
	const auto expected = "[line0|line1|line2|]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv empty last line", "[libv.range.view_split_sv]") {
	const auto str = std::string("line0\nline1\n\n");
	const auto expected = "[line0|line1||]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv empty first line", "[libv.range.view_split_sv]") {
	const auto str = std::string("\nline0\nline1");
	const auto expected = "[|line0|line1]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv empty first lines", "[libv.range.view_split_sv]") {
	const auto str = std::string("\n\nline0\nline1");
	const auto expected = "[||line0|line1]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv nonSBO", "[libv.range.view_split_sv]") {
	const auto str = std::string("line0\nline1" BYPASS_SBO "");
	const auto expected = "[line0|line1" BYPASS_SBO "]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv nonSBO empty last line", "[libv.range.view_split_sv]") {
	const auto str = std::string("line0\nline1" BYPASS_SBO "\n");
	const auto expected = "[line0|line1" BYPASS_SBO "|]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv empty lines", "[libv.range.view_split_sv]") {
	const auto str = std::string(BYPASS_SBO "\n\n\n");
	const auto expected = "[" BYPASS_SBO "|||]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

TEST_CASE("view split_sv scenario 0", "[libv.range.view_split_sv]") {
	const auto str = std::string(BYPASS_SBO "*/\n}") + "\n";
	const auto expected = "[" BYPASS_SBO "*/|}|]";

	CHECK(as_str(libv::view::split_sv(str, '\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv('\n')) == expected);
	CHECK(as_str(str | libv::view::split_sv) == expected);
}

// -------------------------------------------------------------------------------------------------
