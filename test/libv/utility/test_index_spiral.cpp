// Project: libv.utility, File: test/libv/utility/test_extract_bits.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/utility/index_spiral.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Test index_spiral", "[libv.utility.index_spiral]") {
	CHECK(libv::index_spiral( 0) == libv::vec2i{ 0,  0});
	CHECK(libv::index_spiral( 1) == libv::vec2i{ 1,  0});
	CHECK(libv::index_spiral( 2) == libv::vec2i{ 1,  1});
	CHECK(libv::index_spiral( 3) == libv::vec2i{ 0,  1});
	CHECK(libv::index_spiral( 4) == libv::vec2i{-1,  1});
	CHECK(libv::index_spiral( 5) == libv::vec2i{-1,  0});
	CHECK(libv::index_spiral( 6) == libv::vec2i{-1, -1});
	CHECK(libv::index_spiral( 7) == libv::vec2i{ 0, -1});
	CHECK(libv::index_spiral( 8) == libv::vec2i{ 1, -1});
	CHECK(libv::index_spiral( 9) == libv::vec2i{ 2, -1});
	CHECK(libv::index_spiral(10) == libv::vec2i{ 2,  0});
	CHECK(libv::index_spiral(11) == libv::vec2i{ 2,  1});
	CHECK(libv::index_spiral(12) == libv::vec2i{ 2,  2});
	CHECK(libv::index_spiral(13) == libv::vec2i{ 1,  2});
	CHECK(libv::index_spiral(14) == libv::vec2i{ 0,  2});
	CHECK(libv::index_spiral(15) == libv::vec2i{-1,  2});
	CHECK(libv::index_spiral(16) == libv::vec2i{-2,  2});
	CHECK(libv::index_spiral(17) == libv::vec2i{-2,  1});
	CHECK(libv::index_spiral(18) == libv::vec2i{-2,  0});
	CHECK(libv::index_spiral(19) == libv::vec2i{-2, -1});
	CHECK(libv::index_spiral(20) == libv::vec2i{-2, -2});
	CHECK(libv::index_spiral(21) == libv::vec2i{-1, -2});
	CHECK(libv::index_spiral(22) == libv::vec2i{ 0, -2});
	CHECK(libv::index_spiral(23) == libv::vec2i{ 1, -2});
	CHECK(libv::index_spiral(24) == libv::vec2i{ 2, -2});
	CHECK(libv::index_spiral(25) == libv::vec2i{ 3, -2});
	CHECK(libv::index_spiral(26) == libv::vec2i{ 3, -1});
	CHECK(libv::index_spiral(27) == libv::vec2i{ 3,  0});
	CHECK(libv::index_spiral(28) == libv::vec2i{ 3,  1});
	CHECK(libv::index_spiral(29) == libv::vec2i{ 3,  2});
	CHECK(libv::index_spiral(30) == libv::vec2i{ 3,  3});
	CHECK(libv::index_spiral(31) == libv::vec2i{ 2,  3});
	CHECK(libv::index_spiral(32) == libv::vec2i{ 1,  3});
	CHECK(libv::index_spiral(33) == libv::vec2i{ 0,  3});
	CHECK(libv::index_spiral(34) == libv::vec2i{-1,  3});
	CHECK(libv::index_spiral(35) == libv::vec2i{-2,  3});
	CHECK(libv::index_spiral(36) == libv::vec2i{-3,  3});
	CHECK(libv::index_spiral(37) == libv::vec2i{-3,  2});
	CHECK(libv::index_spiral(38) == libv::vec2i{-3,  1});
	CHECK(libv::index_spiral(39) == libv::vec2i{-3,  0});
	CHECK(libv::index_spiral(40) == libv::vec2i{-3, -1});
	CHECK(libv::index_spiral(41) == libv::vec2i{-3, -2});
	CHECK(libv::index_spiral(42) == libv::vec2i{-3, -3});
	CHECK(libv::index_spiral(43) == libv::vec2i{-2, -3});
	CHECK(libv::index_spiral(44) == libv::vec2i{-1, -3});
	CHECK(libv::index_spiral(45) == libv::vec2i{ 0, -3});
	CHECK(libv::index_spiral(46) == libv::vec2i{ 1, -3});
	CHECK(libv::index_spiral(47) == libv::vec2i{ 2, -3});
	CHECK(libv::index_spiral(48) == libv::vec2i{ 3, -3});
	CHECK(libv::index_spiral(49) == libv::vec2i{ 4, -3});
}

// -------------------------------------------------------------------------------------------------

auto list_from_spiral_loop(int32_t ring) {
	std::vector<libv::vec2i> result;

	libv::index_spiral_loop(ring, [&](libv::vec2i index) {
		result.emplace_back(index);
	});

	return result;
}

auto list_from_index_spiral(int32_t count) {
	std::vector<libv::vec2i> result;

	for (int32_t i = 0; i < count; ++i)
		result.emplace_back(libv::index_spiral(i));

	return result;
}

template <typename... Is>
auto list(Is... is) {
	return std::vector<libv::vec2i>{is...};
}


TEST_CASE("Test index_spiral_loop", "[libv.utility.index_spiral]") {
	using vec2 = libv::vec2i;

	CHECK(list_from_spiral_loop(0) == list());
	CHECK(list_from_spiral_loop(1) == list(vec2(0, 0)));
	CHECK(list_from_spiral_loop(2) == list(
			vec2(0, 0),
			vec2(1, 0),
			vec2(1, 1),
			vec2(0, 1),
			vec2(-1, 1),
			vec2(-1, 0),
			vec2(-1, -1),
			vec2(0, -1),
			vec2(1, -1)
	));

	CHECK(list_from_spiral_loop(0) == list_from_index_spiral(0));
	CHECK(list_from_spiral_loop(1) == list_from_index_spiral(1));
	CHECK(list_from_spiral_loop(2) == list_from_index_spiral(9));
	CHECK(list_from_spiral_loop(3) == list_from_index_spiral(25));
	CHECK(list_from_spiral_loop(4) == list_from_index_spiral(7 * 7));
	CHECK(list_from_spiral_loop(5) == list_from_index_spiral(9 * 9));
}

// -------------------------------------------------------------------------------------------------
