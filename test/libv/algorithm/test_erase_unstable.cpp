// Project: libv, File: test/libv/algorithm/test_erase_unstable.cpp

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <array>
#include <vector>
// pro
#include <libv/algo/erase_unstable.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("erase_unstable iterator begin", "[libv.algorithm.erase_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_unstable(vector, vector.begin());

	CHECK(ranges::equal(vector, std::array{4, 1, 2, 3}));
}

TEST_CASE("erase_unstable iterator end", "[libv.algorithm.erase_unstable]") {
	// The iterator \c it must be valid and dereferenceable.
}

TEST_CASE("erase_unstable iterator middle", "[libv.algorithm.erase_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_unstable(vector, vector.begin() + 2);

	CHECK(ranges::equal(vector, std::array{0, 1, 4, 3}));
}

TEST_CASE("erase_unstable iterator empty", "[libv.algorithm.erase_unstable]") {
	// The iterator \c it must be valid and dereferenceable.
}

TEST_CASE("erase_unstable iterator last left", "[libv.algorithm.erase_unstable]") {
	std::vector<int> vector = {0};
	libv::erase_unstable(vector, vector.begin());

	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_unstable iterator last element", "[libv.algorithm.erase_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};

	libv::erase_unstable(vector, vector.begin() + 4);
	CHECK(ranges::equal(vector, std::array{0, 1, 2, 3}));
}

TEST_CASE("erase_unstable iterator drain begin", "[libv.algorithm.erase_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};

	libv::erase_unstable(vector, vector.begin());
	CHECK(ranges::equal(vector, std::array{4, 1, 2, 3}));

	libv::erase_unstable(vector, vector.begin());
	CHECK(ranges::equal(vector, std::array{3, 1, 2}));

	libv::erase_unstable(vector, vector.begin());
	CHECK(ranges::equal(vector, std::array{2, 1}));

	libv::erase_unstable(vector, vector.begin());
	CHECK(ranges::equal(vector, std::array{1}));

	libv::erase_unstable(vector, vector.begin());
	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

// -------------------------------------------------------------------------------------------------
