// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <array>
// pro
#include <libv/range/view_setbits.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("view setbits", "[libv.range.view_setbits]") {
	auto view = libv::view::setbits(1 | 2 | 4 | 16);
	auto test = std::array{0, 1, 2, 4};

	CHECK(ranges::equal(view, test));
}

TEST_CASE("view setbits 2", "[libv.range.view_setbits]") {
	auto view = libv::view::setbits(2 | 4 | 16);
	auto test = std::array{1, 2, 4};

	CHECK(ranges::equal(view, test));
}

TEST_CASE("view setbits empty", "[libv.range.view_setbits]") {
	auto view = libv::view::setbits(0);
	auto test = std::array<int, 0>{};

	CHECK(ranges::equal(view, test));
}

TEST_CASE("view setbits max", "[libv.range.view_setbits]") {
	auto view = libv::view::setbits(1 << 31);
	auto test = std::array{31};

	CHECK(ranges::equal(view, test));
}

// -------------------------------------------------------------------------------------------------
