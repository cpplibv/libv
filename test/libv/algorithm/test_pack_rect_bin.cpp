// Project: libv.algorithm, File: test/libv/algorithm/test_pack_rect_bin.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
#include <libv/algorithm/pack_rect_bin.hpp>
// libv
#include <libv/utility/approx.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("pack_rect_bin simple API test", "[libv.utility.pack]") {
	libv::pack_rect_bin pack(100, 100);

	CHECK(pack.used_area() == 0);

	const auto* rect_a = pack.insert(10, 15);

	REQUIRE(rect_a != nullptr);
	CHECK(pack.used_area() == 10 * 15);
	CHECK(rect_a->position.x == 0);
	CHECK(rect_a->position.y == 0);
	CHECK(rect_a->size.x == 10);
	CHECK(rect_a->size.y == 15);

	const auto* rect_b = pack.insert(20, 25);

	REQUIRE(rect_b != nullptr);
	CHECK(pack.used_area() == 10 * 15 + 20 * 25);
	CHECK(rect_b->position.x == 10);
	CHECK(rect_b->position.y == 0);
	CHECK(rect_b->size.x == 20);
	CHECK(rect_b->size.y == 25);

	const auto* rect_c = pack.insert(100, 100);
	CHECK(rect_c == nullptr);
	CHECK(pack.used_area() == 10 * 15 + 20 * 25);

	CHECK(pack.occupancy() == libv::approx(0.065f));
}
