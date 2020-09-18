// Project: libv.range, File: test/libv/range/test_view_deinterleave.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// ext
#include <range/v3/view/empty.hpp>
#include <range/v3/view/indices.hpp>
// pro
#include <libv/range/view_deinterleave.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("view deinterleave", "[libv.range.view_deinterleave]") {
	auto view = ranges::view::closed_indices(0, 5) | libv::view::deinterleave(2);
	REQUIRE(view.size() == 2);

	auto row_it = view.begin();
	{
		const auto& row = *row_it;
		auto cell_it = row.begin();

		REQUIRE(row.size() == 3);
		CHECK(*cell_it == 0); cell_it++;
		CHECK(*cell_it == 2); cell_it++;
		CHECK(*cell_it == 4); cell_it++;
		CHECK(cell_it == row.end());
	}
	row_it++;
	{
		const auto& row = *row_it;
		auto cell_it = row.begin();

		REQUIRE(row.size() == 3);
		CHECK(*cell_it == 1); cell_it++;
		CHECK(*cell_it == 3); cell_it++;
		CHECK(*cell_it == 5); cell_it++;
		CHECK(cell_it == row.end());
	}
	row_it++;
	CHECK(row_it == view.end());
}

TEST_CASE("view deinterleave with partial last row", "[libv.range.view_deinterleave]") {
	auto view = ranges::view::closed_indices(0, 6) | libv::view::deinterleave(2);
	REQUIRE(view.size() == 2);

	auto row_it = view.begin();
	{
		const auto& row = *row_it;
		auto cell_it = row.begin();

		REQUIRE(row.size() == 4);
		CHECK(*cell_it == 0); cell_it++;
		CHECK(*cell_it == 2); cell_it++;
		CHECK(*cell_it == 4); cell_it++;
		CHECK(*cell_it == 6); cell_it++;
		CHECK(cell_it == row.end());
	}
	row_it++;
	{
		const auto& row = *row_it;
		auto cell_it = row.begin();

		REQUIRE(row.size() == 3);
		CHECK(*cell_it == 1); cell_it++;
		CHECK(*cell_it == 3); cell_it++;
		CHECK(*cell_it == 5); cell_it++;
		CHECK(cell_it == row.end());
	}
	row_it++;
	CHECK(row_it == view.end());
}

TEST_CASE("view deinterleave empty", "[libv.range.view_deinterleave]") {
	auto view = ranges::view::empty<int> | libv::view::deinterleave(2);
	REQUIRE(view.size() == 0);

	auto row_it = view.begin();
	CHECK(row_it == view.end());
}

TEST_CASE("view deinterleave with less than N input", "[libv.range.view_deinterleave]") {
	auto view = ranges::view::closed_indices(0, 0) | libv::view::deinterleave(2);
	REQUIRE(view.size() == 1);

	auto row_it = view.begin();
	{
		const auto& row = *row_it;
		auto cell_it = row.begin();

		REQUIRE(row.size() == 1);
		CHECK(*cell_it == 0); cell_it++;
		CHECK(cell_it == row.end());
	}
	row_it++;
	CHECK(row_it == view.end());
}

TEST_CASE("view deinterleave outer range iterator difference", "[libv.range.view_deinterleave]") {
	auto view = ranges::view::closed_indices(0, 99) | libv::view::deinterleave(10);

	auto left_it = view.begin();

	for (size_t left = 0; left < 10; ++left) {
		auto right_it = left_it;

		for (size_t right = left; right < 10; ++right) {
			CHECK(std::distance(left_it, right_it) == right - left);
			right_it++;
		}
		left_it++;
	}
}

TEST_CASE("view deinterleave inner range iterator difference", "[libv.range.view_deinterleave]") {
	auto view = ranges::view::closed_indices(0, 99) | libv::view::deinterleave(10);

	for (const auto& row : view) {
		auto left_it = row.begin();

		for (size_t left = 0; left < 10; ++left) {
			auto right_it = left_it;

			for (size_t right = left; right < 10; ++right) {
				CHECK(std::distance(left_it, right_it) == right - left);
				right_it++;
			}
			left_it++;
		}
	}
}

TEST_CASE("view deinterleave random access", "[libv.range.view_deinterleave]") {
	auto view = ranges::view::closed_indices(0, 94) | libv::view::deinterleave(10);

	REQUIRE(view.size() == 10);

	for (size_t i = 0; i < 5; ++i) {
		REQUIRE(view[i].size() == 10);
	}

	for (size_t i = 5; i < 10; ++i)
		REQUIRE(view[i].size() == 9);

	for (size_t x = 0; x < 10; ++x) {
		for (size_t y = 0; y < (x < 5 ? 10 : 9); ++y) {
			CHECK(view[x][y] == y * 10 + x);
		}
	}
}

// -------------------------------------------------------------------------------------------------
