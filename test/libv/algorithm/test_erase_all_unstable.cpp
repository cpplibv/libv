// Project: libv.algorithm, File: test/libv/algorithm/test_erase_all_unstable.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/algorithm/equal.hpp>
// std
#include <array>
#include <vector>
// pro
#include <libv/algo/erase_all_unstable.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("erase_all_unstable begin", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_all_unstable(vector, 0);

	CHECK(ranges::equal(vector, std::array{4, 1, 2, 3}));
}

TEST_CASE("erase_all_unstable missing", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_all_unstable(vector, 10);

	CHECK(ranges::equal(vector, std::array{0, 1, 2, 3, 4}));
}

TEST_CASE("erase_all_unstable middle", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};
	libv::erase_all_unstable(vector, 2);

	CHECK(ranges::equal(vector, std::array{0, 1, 4, 3}));
}

TEST_CASE("erase_all_unstable empty", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {};
	libv::erase_all_unstable(vector, 10);

	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_all_unstable last left", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0};
	libv::erase_all_unstable(vector, 0);

	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_all_unstable last element", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};

	libv::erase_all_unstable(vector, 4);
	CHECK(ranges::equal(vector, std::array{0, 1, 2, 3}));
}

TEST_CASE("erase_all_unstable drain 0..5", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};

	libv::erase_all_unstable(vector, 0);
	CHECK(ranges::equal(vector, std::array{4, 1, 2, 3}));

	libv::erase_all_unstable(vector, 1);
	CHECK(ranges::equal(vector, std::array{4, 3, 2}));

	libv::erase_all_unstable(vector, 2);
	CHECK(ranges::equal(vector, std::array{4, 3}));

	libv::erase_all_unstable(vector, 3);
	CHECK(ranges::equal(vector, std::array{4}));

	libv::erase_all_unstable(vector, 4);
	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_all_unstable drain first", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};

	libv::erase_all_unstable(vector, 0);
	CHECK(ranges::equal(vector, std::array{4, 1, 2, 3}));

	libv::erase_all_unstable(vector, 4);
	CHECK(ranges::equal(vector, std::array{3, 1, 2}));

	libv::erase_all_unstable(vector, 3);
	CHECK(ranges::equal(vector, std::array{2, 1}));

	libv::erase_all_unstable(vector, 2);
	CHECK(ranges::equal(vector, std::array{1}));

	libv::erase_all_unstable(vector, 1);
	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_all_unstable drain last", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 1, 2, 3, 4};

	libv::erase_all_unstable(vector, 4);
	CHECK(ranges::equal(vector, std::array{0, 1, 2, 3}));

	libv::erase_all_unstable(vector, 3);
	CHECK(ranges::equal(vector, std::array{0, 1, 2}));

	libv::erase_all_unstable(vector, 2);
	CHECK(ranges::equal(vector, std::array{0, 1}));

	libv::erase_all_unstable(vector, 1);
	CHECK(ranges::equal(vector, std::array{0}));

	libv::erase_all_unstable(vector, 0);
	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("erase_all_unstable multiple on begin and end", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 0, 1, 2, 3, 4, 0, 0};
	libv::erase_all_unstable(vector, 0);

	CHECK(ranges::equal(vector, std::array{4, 3, 1, 2}));
}

TEST_CASE("erase_all_unstable multiple on mid and mid ", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {1, 0, 0, 2, 0, 3, 0, 4};
	libv::erase_all_unstable(vector, 0);

	CHECK(ranges::equal(vector, std::array{1, 4, 3, 2}));
}

TEST_CASE("erase_all_unstable multiple middle", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 1, 2, 2, 2, 3, 4};
	libv::erase_all_unstable(vector, 2);

	CHECK(ranges::equal(vector, std::array{0, 1, 4, 3}));
}

TEST_CASE("erase_all_unstable multiple last left", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 0, 0, 1};
	libv::erase_all_unstable(vector, 0);

	CHECK(ranges::equal(vector, std::array{1}));
}

TEST_CASE("erase_all_unstable all element", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 0, 0};

	libv::erase_all_unstable(vector, 0);
	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_all_unstable multiple drain 0..5", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4};

	libv::erase_all_unstable(vector, 0);
	CHECK(ranges::equal(vector, std::array{4, 4, 1, 1, 2, 2, 3, 3}));

	libv::erase_all_unstable(vector, 1);
	CHECK(ranges::equal(vector, std::array{4, 4, 3, 3, 2, 2}));

	libv::erase_all_unstable(vector, 2);
	CHECK(ranges::equal(vector, std::array{4, 4, 3, 3}));

	libv::erase_all_unstable(vector, 3);
	CHECK(ranges::equal(vector, std::array{4, 4}));

	libv::erase_all_unstable(vector, 4);
	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_all_unstable multiple drain first", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4};

	libv::erase_all_unstable(vector, 0);
	CHECK(ranges::equal(vector, std::array{4, 4, 1, 1, 2, 2, 3, 3}));

	libv::erase_all_unstable(vector, 4);
	CHECK(ranges::equal(vector, std::array{3, 3, 1, 1, 2, 2}));

	libv::erase_all_unstable(vector, 3);
	CHECK(ranges::equal(vector, std::array{2, 2, 1, 1}));

	libv::erase_all_unstable(vector, 2);
	CHECK(ranges::equal(vector, std::array{1, 1}));

	libv::erase_all_unstable(vector, 1);
	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

TEST_CASE("erase_all_unstable multiple drain last", "[libv.algorithm.erase_all_unstable]") {
	std::vector<int> vector = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4};

	libv::erase_all_unstable(vector, 4);
	CHECK(ranges::equal(vector, std::array{0, 0, 1, 1, 2, 2, 3, 3}));

	libv::erase_all_unstable(vector, 3);
	CHECK(ranges::equal(vector, std::array{0, 0, 1, 1, 2, 2}));

	libv::erase_all_unstable(vector, 2);
	CHECK(ranges::equal(vector, std::array{0, 0, 1, 1}));

	libv::erase_all_unstable(vector, 1);
	CHECK(ranges::equal(vector, std::array{0, 0}));

	libv::erase_all_unstable(vector, 0);
	CHECK(ranges::equal(vector, std::array<int, 0>{}));
}

// -------------------------------------------------------------------------------------------------
