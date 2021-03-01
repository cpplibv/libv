// Project: libv, File: test/libv/algorithm/test_erase_if_unstable.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <array>
#include <vector>
// pro
#include <libv/algo/erase_if_unstable.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("erase_if_unstable every element", "[libv.algorithm.erase_if_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_if_unstable(vector, [](int){ return true; });

	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_if_unstable no element", "[libv.algorithm.erase_if_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_if_unstable(vector, [](int){ return false; });

	CHECK(ranges::equal(vector, std::array{0, 1, 2, 3, 4}));
}

TEST_CASE("erase_if_unstable empty container", "[libv.algorithm.erase_if_unstable]") {
	std::vector<int> vector = {};
	libv::erase_if_unstable(vector, [](int){ return false; });

	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_if_unstable evens", "[libv.algorithm.erase_if_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_if_unstable(vector, [](int v){ return v % 2 == 0; });

	CHECK(ranges::equal(vector, std::array{3, 1}));
}

TEST_CASE("erase_if_unstable odds", "[libv.algorithm.erase_if_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_if_unstable(vector, [](int v){ return v % 2 != 0; });

	CHECK(ranges::equal(vector, std::array{0, 4, 2}));
}

TEST_CASE("erase_if_unstable first half", "[libv.algorithm.erase_if_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4, 5};
	libv::erase_if_unstable(vector, [](int v){ return v < 3; });

	CHECK(ranges::equal(vector, std::array{5, 4, 3}));
}

TEST_CASE("erase_if_unstable second half", "[libv.algorithm.erase_if_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4, 5};
	libv::erase_if_unstable(vector, [](int v){ return v > 2; });

	CHECK(ranges::equal(vector, std::array{0, 1, 2}));
}

// -------------------------------------------------------------------------------------------------
