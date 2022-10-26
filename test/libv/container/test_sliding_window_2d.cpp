// Project: libv.container, File: test/libv/container/test_sliding_window_2d.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// libv
#include <libv/math/vec.hpp>
// std
#include <sstream>
// pro
#include <libv/container/sliding_window_2d.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Test sliding_window_2d constructor and contains", "[libv.container.sliding_window_2d]") {
	libv::sliding_window_2D<int> container{0, 0, 10, 5};
	CHECK(container.size_x() == 10);
	CHECK(container.size_y() == 5);
	CHECK(container.offset_x() == 0);
	CHECK(container.offset_y() == 0);
	CHECK(container.unordered_span().size() == 10uz * 5);
	CHECK(container.unordered_span().data() == container.data());

	CHECK(not container.contains(-1, 0));
	CHECK(container.contains(0, 0));
	CHECK(container.contains(1, 0));
	CHECK(container.contains(9, 0));
	CHECK(not container.contains(10, 0));
	CHECK(not container.contains(11, 0));

	CHECK(not container.contains(0, -1));
	CHECK(container.contains(0, 0));
	CHECK(container.contains(0, 1));
	CHECK(container.contains(0, 4));
	CHECK(not container.contains(0, 5));
	CHECK(not container.contains(0, 6));

	CHECK(not container.contains(-1, -1));
	CHECK(container.contains(9, 4));
	CHECK(not container.contains(9, 5));
	CHECK(not container.contains(10, 4));
	CHECK(not container.contains(10, 5));
}

TEST_CASE("Test sliding_window_2d slided constructor and contains", "[libv.container.sliding_window_2d]") {
	libv::sliding_window_2D<int> container{-5, 2, 10, 5};
	CHECK(container.size_x() == 10);
	CHECK(container.size_y() == 5);
	CHECK(container.offset_x() == -5);
	CHECK(container.offset_y() == 2);
	CHECK(container.unordered_span().size() == 10uz * 5);
	CHECK(container.unordered_span().data() == container.data());

	CHECK(not container.contains(-6, 2));
	CHECK(container.contains(-5, 2));
	CHECK(container.contains(-4, 2));
	CHECK(container.contains(4, 2));
	CHECK(not container.contains(5, 2));
	CHECK(not container.contains(6, 2));

	CHECK(not container.contains(-5, 1));
	CHECK(container.contains(-5, 2));
	CHECK(container.contains(-5, 3));
	CHECK(container.contains(-5, 6));
	CHECK(not container.contains(-5, 7));
	CHECK(not container.contains(-5, 8));

	CHECK(not container.contains(-6, 1));
	CHECK(container.contains(4, 6));
	CHECK(not container.contains(4, 7));
	CHECK(not container.contains(5, 6));
	CHECK(not container.contains(5, 7));
}

TEST_CASE("Test sliding_window_2d index_of", "[libv.container.sliding_window_2d]") {
	{
		libv::sliding_window_2D<int> c{0, 0, 10, 5};

		CHECK(c.index_of(0, 0) == libv::vec2i{0, 0});
		CHECK(c.index_of(1, 0) == libv::vec2i{1, 0});
		CHECK(c.index_of(0, 1) == libv::vec2i{0, 1});
		CHECK(c.index_of(1, 1) == libv::vec2i{1, 1});
		c(0, 0) = 100; CHECK(c.unordered_span()[0] == 100);
		c(1, 0) = 110; CHECK(c.unordered_span()[1] == 110);
		c(0, 1) = 101; CHECK(c.unordered_span()[10] == 101);
		c(1, 1) = 111; CHECK(c.unordered_span()[11] == 111);

	} {
		libv::sliding_window_2D<int> c{-1, 0, 10, 5};
		CHECK(c.index_of(0, 0) == libv::vec2i{0, 0});
		CHECK(c.index_of(-1, 0) == libv::vec2i{9, 0});

	} {
		libv::sliding_window_2D<int> c{0, -1, 10, 5};
		CHECK(c.index_of(0, 0) == libv::vec2i{0, 0});
		CHECK(c.index_of(0, -1) == libv::vec2i{0, 4});

	} {
		libv::sliding_window_2D<int> c{-5, 2, 10, 5};

		CHECK(c.index_of(-5, 2) == libv::vec2i{5, 2});
		CHECK(c.index_of(-4, 2) == libv::vec2i{6, 2});
		CHECK(c.index_of(-5, 3) == libv::vec2i{5, 3});
		CHECK(c.index_of(-4, 3) == libv::vec2i{6, 3});
		CHECK(c.index_of(-5, 2).y * c.size_x() + c.index_of(-5, 2).x == c.index_of_raw(-5, 2));
		CHECK(c.index_of(-4, 2).y * c.size_x() + c.index_of(-4, 2).x == c.index_of_raw(-4, 2));
		CHECK(c.index_of(-5, 3).y * c.size_x() + c.index_of(-5, 3).x == c.index_of_raw(-5, 3));
		CHECK(c.index_of(-4, 3).y * c.size_x() + c.index_of(-4, 3).x == c.index_of_raw(-4, 3));
		c(-5, 2) = 100; CHECK(c.unordered_span()[c.index_of_raw(-5, 2)] == 100);
		c(-4, 2) = 110; CHECK(c.unordered_span()[c.index_of_raw(-4, 2)] == 110);
		c(-5, 3) = 101; CHECK(c.unordered_span()[c.index_of_raw(-5, 3)] == 101);
		c(-4, 3) = 111; CHECK(c.unordered_span()[c.index_of_raw(-4, 3)] == 111);
	}
}

auto wrap(int32_t start, int32_t count, int32_t size) {
	std::vector<int32_t> result;

	libv::sliding_window_2D<int> container{0, 0, 5, 5};

	container.foreach_wrap(start, count, size, [&](int32_t i, int32_t pos) {
		result.emplace_back(i);
		result.emplace_back(pos);
	});

	return result;
}

auto wrap_op(int32_t start, int32_t count, int32_t size) {
	std::vector<int32_t> result;

	libv::sliding_window_2D<int> container{0, 0, 5, 5};

	container.foreach_wrap_opposite(start, count, size, [&](int32_t i, int32_t pos) {
		result.emplace_back(i);
		result.emplace_back(pos);
	});

	return result;
}

template <typename... Is>
auto list(Is... is) {
	return std::vector<int32_t>{is...};
}

TEST_CASE("Test foreach_wrap", "[libv.container.sliding_window_2d]") {

	CHECK(wrap(0, 0, 10) == list());
	CHECK(wrap(0, 1, 10) == list(0, 10));
	CHECK(wrap(0, -1, 10) == list(9, -1));

	CHECK(wrap(0, 4, 5) == list(0, 5, 1, 6, 2, 7, 3, 8));
	CHECK(wrap(0, 5, 5) == list(0, 5, 1, 6, 2, 7, 3, 8, 4, 9));
	CHECK(wrap(0, 6, 5) == list(0, 5, 1, 6, 2, 7, 3, 8, 4, 9, 0, 10));
//	CHECK(wrap(0, 6, 5) == list(0, 10, 1, 6, 2, 7, 3, 8, 4, 9)); // This might be the preferred
//	CHECK(wrap(0, 50, 5) == list(...));
//	CHECK(wrap(15, 50, 5) == list(...));
	CHECK(wrap(0, -4, 5) == list(1, -4, 2, -3, 3, -2, 4, -1));
	CHECK(wrap(0, -5, 5) == list(0, -5, 1, -4, 2, -3, 3, -2, 4, -1));
	CHECK(wrap(0, -6, 5) == list(4, -6, 0, -5, 1, -4, 2, -3, 3, -2, 4, -1));
//	CHECK(wrap(0, -6, 5) == list(4, -1, 0, -5, 1, -4, 2, -3, 3, -2, 4, -6)); // This might be the preferred
//	CHECK(wrap(0, -6, 5) == list(4, -6, 0, -5, 1, -4, 2, -3, 3, -2)); // This might be the preferred
//	CHECK(wrap(0, -50, 5) == list(...));
//	CHECK(wrap(15, -50, 5) == list(...));

	CHECK(wrap(-20, 3, 10) == list(0, -10, /**/ 1, -9, /**/ 2, -8));
	CHECK(wrap(-19, 3, 10) == list(1,  -9, /**/ 2, -8, /**/ 3, -7));
	CHECK(wrap(-18, 3, 10) == list(2,  -8, /**/ 3, -7, /**/ 4, -6));
	CHECK(wrap(-17, 3, 10) == list(3,  -7, /**/ 4, -6, /**/ 5, -5));
	CHECK(wrap(-16, 3, 10) == list(4,  -6, /**/ 5, -5, /**/ 6, -4));
	CHECK(wrap(-15, 3, 10) == list(5,  -5, /**/ 6, -4, /**/ 7, -3));
	CHECK(wrap(-14, 3, 10) == list(6,  -4, /**/ 7, -3, /**/ 8, -2));
	CHECK(wrap(-13, 3, 10) == list(7,  -3, /**/ 8, -2, /**/ 9, -1));
	CHECK(wrap(-12, 3, 10) == list(8,  -2, /**/ 9, -1, /**/ 0,  0));
	CHECK(wrap(-11, 3, 10) == list(9,  -1, /**/ 0,  0, /**/ 1,  1));
	CHECK(wrap(-10, 3, 10) == list(0,   0, /**/ 1,  1, /**/ 2,  2));
	CHECK(wrap( -9, 3, 10) == list(1,   1, /**/ 2,  2, /**/ 3,  3));
	CHECK(wrap( -8, 3, 10) == list(2,   2, /**/ 3,  3, /**/ 4,  4));
	CHECK(wrap( -7, 3, 10) == list(3,   3, /**/ 4,  4, /**/ 5,  5));
	CHECK(wrap( -6, 3, 10) == list(4,   4, /**/ 5,  5, /**/ 6,  6));
	CHECK(wrap( -5, 3, 10) == list(5,   5, /**/ 6,  6, /**/ 7,  7));
	CHECK(wrap( -4, 3, 10) == list(6,   6, /**/ 7,  7, /**/ 8,  8));
	CHECK(wrap( -3, 3, 10) == list(7,   7, /**/ 8,  8, /**/ 9,  9));
	CHECK(wrap( -2, 3, 10) == list(8,   8, /**/ 9,  9, /**/ 0, 10));
	CHECK(wrap( -1, 3, 10) == list(9,   9, /**/ 0, 10, /**/ 1, 11));
	CHECK(wrap(  0, 3, 10) == list(0,  10, /**/ 1, 11, /**/ 2, 12));
	CHECK(wrap(  1, 3, 10) == list(1,  11, /**/ 2, 12, /**/ 3, 13));
	CHECK(wrap(  2, 3, 10) == list(2,  12, /**/ 3, 13, /**/ 4, 14));
	CHECK(wrap(  3, 3, 10) == list(3,  13, /**/ 4, 14, /**/ 5, 15));
	CHECK(wrap(  4, 3, 10) == list(4,  14, /**/ 5, 15, /**/ 6, 16));
	CHECK(wrap(  5, 3, 10) == list(5,  15, /**/ 6, 16, /**/ 7, 17));
	CHECK(wrap(  6, 3, 10) == list(6,  16, /**/ 7, 17, /**/ 8, 18));
	CHECK(wrap(  7, 3, 10) == list(7,  17, /**/ 8, 18, /**/ 9, 19));
	CHECK(wrap(  8, 3, 10) == list(8,  18, /**/ 9, 19, /**/ 0, 20));
	CHECK(wrap(  9, 3, 10) == list(9,  19, /**/ 0, 20, /**/ 1, 21));
	CHECK(wrap( 10, 3, 10) == list(0,  20, /**/ 1, 21, /**/ 2, 22));
	CHECK(wrap( 11, 3, 10) == list(1,  21, /**/ 2, 22, /**/ 3, 23));
	CHECK(wrap( 12, 3, 10) == list(2,  22, /**/ 3, 23, /**/ 4, 24));
	CHECK(wrap( 13, 3, 10) == list(3,  23, /**/ 4, 24, /**/ 5, 25));
	CHECK(wrap( 14, 3, 10) == list(4,  24, /**/ 5, 25, /**/ 6, 26));
	CHECK(wrap( 15, 3, 10) == list(5,  25, /**/ 6, 26, /**/ 7, 27));
	CHECK(wrap( 16, 3, 10) == list(6,  26, /**/ 7, 27, /**/ 8, 28));
	CHECK(wrap( 17, 3, 10) == list(7,  27, /**/ 8, 28, /**/ 9, 29));
	CHECK(wrap( 18, 3, 10) == list(8,  28, /**/ 9, 29, /**/ 0, 30));
	CHECK(wrap( 19, 3, 10) == list(9,  29, /**/ 0, 30, /**/ 1, 31));
	CHECK(wrap( 20, 3, 10) == list(0,  30, /**/ 1, 31, /**/ 2, 32));

	CHECK(wrap(-20, -3, 10) == list(7, -23, /**/ 8, -22, /**/ 9, -21));
	CHECK(wrap(-19, -3, 10) == list(8, -22, /**/ 9, -21, /**/ 0, -20));
	CHECK(wrap(-18, -3, 10) == list(9, -21, /**/ 0, -20, /**/ 1, -19));
	CHECK(wrap(-17, -3, 10) == list(0, -20, /**/ 1, -19, /**/ 2, -18));
	CHECK(wrap(-16, -3, 10) == list(1, -19, /**/ 2, -18, /**/ 3, -17));
	CHECK(wrap(-15, -3, 10) == list(2, -18, /**/ 3, -17, /**/ 4, -16));
	CHECK(wrap(-14, -3, 10) == list(3, -17, /**/ 4, -16, /**/ 5, -15));
	CHECK(wrap(-13, -3, 10) == list(4, -16, /**/ 5, -15, /**/ 6, -14));
	CHECK(wrap(-12, -3, 10) == list(5, -15, /**/ 6, -14, /**/ 7, -13));
	CHECK(wrap(-11, -3, 10) == list(6, -14, /**/ 7, -13, /**/ 8, -12));
	CHECK(wrap(-10, -3, 10) == list(7, -13, /**/ 8, -12, /**/ 9, -11));
	CHECK(wrap( -9, -3, 10) == list(8, -12, /**/ 9, -11, /**/ 0, -10));
	CHECK(wrap( -8, -3, 10) == list(9, -11, /**/ 0, -10, /**/ 1,  -9));
	CHECK(wrap( -7, -3, 10) == list(0, -10, /**/ 1,  -9, /**/ 2,  -8));
	CHECK(wrap( -6, -3, 10) == list(1,  -9, /**/ 2,  -8, /**/ 3,  -7));
	CHECK(wrap( -5, -3, 10) == list(2,  -8, /**/ 3,  -7, /**/ 4,  -6));
	CHECK(wrap( -4, -3, 10) == list(3,  -7, /**/ 4,  -6, /**/ 5,  -5));
	CHECK(wrap( -3, -3, 10) == list(4,  -6, /**/ 5,  -5, /**/ 6,  -4));
	CHECK(wrap( -2, -3, 10) == list(5,  -5, /**/ 6,  -4, /**/ 7,  -3));
	CHECK(wrap( -1, -3, 10) == list(6,  -4, /**/ 7,  -3, /**/ 8,  -2));
	CHECK(wrap(  0, -3, 10) == list(7,  -3, /**/ 8,  -2, /**/ 9,  -1));
	CHECK(wrap(  1, -3, 10) == list(8,  -2, /**/ 9,  -1, /**/ 0,   0));
	CHECK(wrap(  2, -3, 10) == list(9,  -1, /**/ 0,   0, /**/ 1,   1));
	CHECK(wrap(  3, -3, 10) == list(0,   0, /**/ 1,   1, /**/ 2,   2));
	CHECK(wrap(  4, -3, 10) == list(1,   1, /**/ 2,   2, /**/ 3,   3));
	CHECK(wrap(  5, -3, 10) == list(2,   2, /**/ 3,   3, /**/ 4,   4));
	CHECK(wrap(  6, -3, 10) == list(3,   3, /**/ 4,   4, /**/ 5,   5));
	CHECK(wrap(  7, -3, 10) == list(4,   4, /**/ 5,   5, /**/ 6,   6));
	CHECK(wrap(  8, -3, 10) == list(5,   5, /**/ 6,   6, /**/ 7,   7));
	CHECK(wrap(  9, -3, 10) == list(6,   6, /**/ 7,   7, /**/ 8,   8));
	CHECK(wrap( 10, -3, 10) == list(7,   7, /**/ 8,   8, /**/ 9,   9));
	CHECK(wrap( 11, -3, 10) == list(8,   8, /**/ 9,   9, /**/ 0,  10));
	CHECK(wrap( 12, -3, 10) == list(9,   9, /**/ 0,  10, /**/ 1,  11));
	CHECK(wrap( 13, -3, 10) == list(0,  10, /**/ 1,  11, /**/ 2,  12));
	CHECK(wrap( 14, -3, 10) == list(1,  11, /**/ 2,  12, /**/ 3,  13));
	CHECK(wrap( 15, -3, 10) == list(2,  12, /**/ 3,  13, /**/ 4,  14));
	CHECK(wrap( 16, -3, 10) == list(3,  13, /**/ 4,  14, /**/ 5,  15));
	CHECK(wrap( 17, -3, 10) == list(4,  14, /**/ 5,  15, /**/ 6,  16));
	CHECK(wrap( 18, -3, 10) == list(5,  15, /**/ 6,  16, /**/ 7,  17));
	CHECK(wrap( 19, -3, 10) == list(6,  16, /**/ 7,  17, /**/ 8,  18));
	CHECK(wrap( 20, -3, 10) == list(7,  17, /**/ 8,  18, /**/ 9,  19));

	// --- Wrap opposite

	CHECK(wrap_op(0, 0, 5) == list(0, 0, /**/ 1, 1, /**/ 2, 2, /**/ 3, 3, /**/ 4, 4));
//	CHECK(wrap_op(0, 0, 5) == list()); // Would be fine too, but listing everything is more correct
	CHECK(wrap_op(0, 1, 5) == list(1, 1, /**/ 2, 2, /**/ 3, 3, /**/ 4, 4));
	CHECK(wrap_op(0, -1, 5) == list(0, 0, /**/ 1, 1, /**/ 2, 2, /**/ 3, 3));

	CHECK(wrap_op(0, 4, 5) == list(4, 4));
	CHECK(wrap_op(0, 5, 5) == list());
	CHECK(wrap_op(0, 50, 5) == list());
	CHECK(wrap_op(15, 50, 5) == list());
	CHECK(wrap_op(0, -4, 5) == list(0, 0));
	CHECK(wrap_op(0, -5, 5) == list());
	CHECK(wrap_op(0, -6, 5) == list());
	CHECK(wrap_op(0, -50, 5) == list());
	CHECK(wrap_op(15, -50, 5) == list());

	CHECK(wrap_op(-10, 3, 5) == list(3, -7, /**/ 4, -6));
	CHECK(wrap_op( -9, 3, 5) == list(4, -6, /**/ 0, -5));
	CHECK(wrap_op( -8, 3, 5) == list(0, -5, /**/ 1, -4));
	CHECK(wrap_op( -7, 3, 5) == list(1, -4, /**/ 2, -3));
	CHECK(wrap_op( -6, 3, 5) == list(2, -3, /**/ 3, -2));
	CHECK(wrap_op( -5, 3, 5) == list(3, -2, /**/ 4, -1));
	CHECK(wrap_op( -4, 3, 5) == list(4, -1, /**/ 0,  0));
	CHECK(wrap_op( -3, 3, 5) == list(0,  0, /**/ 1,  1));
	CHECK(wrap_op( -2, 3, 5) == list(1,  1, /**/ 2,  2));
	CHECK(wrap_op( -1, 3, 5) == list(2,  2, /**/ 3,  3));
	CHECK(wrap_op(  0, 3, 5) == list(3,  3, /**/ 4,  4));
	CHECK(wrap_op(  1, 3, 5) == list(4,  4, /**/ 0,  5));
	CHECK(wrap_op(  2, 3, 5) == list(0,  5, /**/ 1,  6));
	CHECK(wrap_op(  3, 3, 5) == list(1,  6, /**/ 2,  7));
	CHECK(wrap_op(  4, 3, 5) == list(2,  7, /**/ 3,  8));
	CHECK(wrap_op(  5, 3, 5) == list(3,  8, /**/ 4,  9));
	CHECK(wrap_op(  6, 3, 5) == list(4,  9, /**/ 0, 10));
	CHECK(wrap_op(  7, 3, 5) == list(0, 10, /**/ 1, 11));
	CHECK(wrap_op(  8, 3, 5) == list(1, 11, /**/ 2, 12));
	CHECK(wrap_op(  9, 3, 5) == list(2, 12, /**/ 3, 13));
	CHECK(wrap_op( 10, 3, 5) == list(3, 13, /**/ 4, 14));

	CHECK(wrap_op(-10, -3, 5) == list(0, -10, /**/ 1, -9));
	CHECK(wrap_op( -9, -3, 5) == list(1,  -9, /**/ 2, -8));
	CHECK(wrap_op( -8, -3, 5) == list(2,  -8, /**/ 3, -7));
	CHECK(wrap_op( -7, -3, 5) == list(3,  -7, /**/ 4, -6));
	CHECK(wrap_op( -6, -3, 5) == list(4,  -6, /**/ 0, -5));
	CHECK(wrap_op( -5, -3, 5) == list(0,  -5, /**/ 1, -4));
	CHECK(wrap_op( -4, -3, 5) == list(1,  -4, /**/ 2, -3));
	CHECK(wrap_op( -3, -3, 5) == list(2,  -3, /**/ 3, -2));
	CHECK(wrap_op( -2, -3, 5) == list(3,  -2, /**/ 4, -1));
	CHECK(wrap_op( -1, -3, 5) == list(4,  -1, /**/ 0,  0));
	CHECK(wrap_op(  0, -3, 5) == list(0,   0, /**/ 1,  1));
	CHECK(wrap_op(  1, -3, 5) == list(1,   1, /**/ 2,  2));
	CHECK(wrap_op(  2, -3, 5) == list(2,   2, /**/ 3,  3));
	CHECK(wrap_op(  3, -3, 5) == list(3,   3, /**/ 4,  4));
	CHECK(wrap_op(  4, -3, 5) == list(4,   4, /**/ 0,  5));
	CHECK(wrap_op(  5, -3, 5) == list(0,   5, /**/ 1,  6));
	CHECK(wrap_op(  6, -3, 5) == list(1,   6, /**/ 2,  7));
	CHECK(wrap_op(  7, -3, 5) == list(2,   7, /**/ 3,  8));
	CHECK(wrap_op(  8, -3, 5) == list(3,   8, /**/ 4,  9));
	CHECK(wrap_op(  9, -3, 5) == list(4,   9, /**/ 0, 10));
	CHECK(wrap_op( 10, -3, 5) == list(0,  10, /**/ 1, 11));
}

TEST_CASE("Test sliding_window_2d slide", "[libv.container.sliding_window_2d]") {

	auto init = []() {
		libv::sliding_window_2D<libv::vec3i> container{0, 0, 5, 5};

		for (int y = container.size_y() - 1; y >= 0; --y)
			for (int x = 0; x < container.size_x(); ++x)
				container.storage_at(x, y) = libv::vec3i(0, x, y);

		return container;
	};
	auto print = [](const auto& container) {
		std::ostringstream os;
		for (int y = container.size_y() - 1; y >= 0; --y) {
			for (int x = 0; x < container.size_x(); ++x) {
				os << container.storage_at(x, y).x << (container.storage_offset() == libv::vec2i(x, y) ? ")" : " ");
			}
			os << "        ";
			for (int x = 0; x < container.size_x(); ++x) {
				os << container.storage_at(x, y).y << ",";
				os << container.storage_at(x, y).z << "  ";
			}
			os << '\n';
		}
		return std::move(os).str();
	};
	auto slide = [](auto& container, int32_t x, int32_t y, int32_t id) {
		container.slide(x, y, [&](libv::vec2i position, libv::vec3i cell) {
			(void) cell;
			return libv::vec3i{id, position};
		});
	};
	auto slide_gen = [](auto& container, int32_t x, int32_t y) {
		container.slide(x, y, [&](libv::vec2i position, libv::vec3i cell) {
			return libv::vec3i{cell.x + 1, position};
		});
	};

	SECTION("Slide up") {
		auto container = init();
		slide(container, 0, 1, 1);
		CHECK(print(container) ==
				"0 0 0 0 0         0,4  1,4  2,4  3,4  4,4  \n"
				"0 0 0 0 0         0,3  1,3  2,3  3,3  4,3  \n"
				"0 0 0 0 0         0,2  1,2  2,2  3,2  4,2  \n"
				"0)0 0 0 0         0,1  1,1  2,1  3,1  4,1  \n"
				"1 1 1 1 1         0,5  1,5  2,5  3,5  4,5  \n"
		);
		slide(container, 0, 1, 2);
		slide(container, 0, 1, 3);
		slide(container, 0, 1, 4);
		slide(container, 0, 1, 5);
		CHECK(print(container) ==
				"5 5 5 5 5         0,9  1,9  2,9  3,9  4,9  \n"
				"4 4 4 4 4         0,8  1,8  2,8  3,8  4,8  \n"
				"3 3 3 3 3         0,7  1,7  2,7  3,7  4,7  \n"
				"2 2 2 2 2         0,6  1,6  2,6  3,6  4,6  \n"
				"1)1 1 1 1         0,5  1,5  2,5  3,5  4,5  \n"
		);
		slide(container, 0, 1, 6);
		slide(container, 0, 1, 7);
		CHECK(print(container) ==
				"5 5 5 5 5         0,9  1,9  2,9  3,9  4,9  \n"
				"4 4 4 4 4         0,8  1,8  2,8  3,8  4,8  \n"
				"3)3 3 3 3         0,7  1,7  2,7  3,7  4,7  \n"
				"7 7 7 7 7         0,11  1,11  2,11  3,11  4,11  \n"
				"6 6 6 6 6         0,10  1,10  2,10  3,10  4,10  \n"
		);
		slide(container, 0, 4, 8);
		CHECK(print(container) ==
				"8 8 8 8 8         0,14  1,14  2,14  3,14  4,14  \n"
				"8 8 8 8 8         0,13  1,13  2,13  3,13  4,13  \n"
				"8 8 8 8 8         0,12  1,12  2,12  3,12  4,12  \n"
				"7)7 7 7 7         0,11  1,11  2,11  3,11  4,11  \n"
				"8 8 8 8 8         0,15  1,15  2,15  3,15  4,15  \n"
		);
	}

	SECTION("Slide down") {
		auto container = init();
		slide(container, 0, -1, 1);
		CHECK(print(container) ==
				"1)1 1 1 1         0,-1  1,-1  2,-1  3,-1  4,-1  \n"
				"0 0 0 0 0         0,3  1,3  2,3  3,3  4,3  \n"
				"0 0 0 0 0         0,2  1,2  2,2  3,2  4,2  \n"
				"0 0 0 0 0         0,1  1,1  2,1  3,1  4,1  \n"
				"0 0 0 0 0         0,0  1,0  2,0  3,0  4,0  \n"
		);
		slide(container, 0, -1, 2);
		slide(container, 0, -1, 3);
		slide(container, 0, -1, 4);
		slide(container, 0, -1, 5);
		CHECK(print(container) ==
				"1 1 1 1 1         0,-1  1,-1  2,-1  3,-1  4,-1  \n"
				"2 2 2 2 2         0,-2  1,-2  2,-2  3,-2  4,-2  \n"
				"3 3 3 3 3         0,-3  1,-3  2,-3  3,-3  4,-3  \n"
				"4 4 4 4 4         0,-4  1,-4  2,-4  3,-4  4,-4  \n"
				"5)5 5 5 5         0,-5  1,-5  2,-5  3,-5  4,-5  \n"
		);
		slide(container, 0, -1, 6);
		slide(container, 0, -1, 7);
		CHECK(print(container) ==
				"6 6 6 6 6         0,-6  1,-6  2,-6  3,-6  4,-6  \n"
				"7)7 7 7 7         0,-7  1,-7  2,-7  3,-7  4,-7  \n"
				"3 3 3 3 3         0,-3  1,-3  2,-3  3,-3  4,-3  \n"
				"4 4 4 4 4         0,-4  1,-4  2,-4  3,-4  4,-4  \n"
				"5 5 5 5 5         0,-5  1,-5  2,-5  3,-5  4,-5  \n"
		);
		slide(container, 0, -4, 8);
		CHECK(print(container) ==
				"8)8 8 8 8         0,-11  1,-11  2,-11  3,-11  4,-11  \n"
				"7 7 7 7 7         0,-7  1,-7  2,-7  3,-7  4,-7  \n"
				"8 8 8 8 8         0,-8  1,-8  2,-8  3,-8  4,-8  \n"
				"8 8 8 8 8         0,-9  1,-9  2,-9  3,-9  4,-9  \n"
				"8 8 8 8 8         0,-10  1,-10  2,-10  3,-10  4,-10  \n"
		);
	}

	SECTION("Slide right") {
		auto container = init();
		slide(container, 1, 0, 1);
		CHECK(print(container) ==
				"1 0 0 0 0         5,4  1,4  2,4  3,4  4,4  \n"
				"1 0 0 0 0         5,3  1,3  2,3  3,3  4,3  \n"
				"1 0 0 0 0         5,2  1,2  2,2  3,2  4,2  \n"
				"1 0 0 0 0         5,1  1,1  2,1  3,1  4,1  \n"
				"1 0)0 0 0         5,0  1,0  2,0  3,0  4,0  \n"
		);
		slide(container, 1, 0, 2);
		slide(container, 1, 0, 3);
		slide(container, 1, 0, 4);
		slide(container, 1, 0, 5);
		CHECK(print(container) ==
				"1 2 3 4 5         5,4  6,4  7,4  8,4  9,4  \n"
				"1 2 3 4 5         5,3  6,3  7,3  8,3  9,3  \n"
				"1 2 3 4 5         5,2  6,2  7,2  8,2  9,2  \n"
				"1 2 3 4 5         5,1  6,1  7,1  8,1  9,1  \n"
				"1)2 3 4 5         5,0  6,0  7,0  8,0  9,0  \n"
		);
		slide(container, 1, 0, 6);
		slide(container, 1, 0, 7);
		CHECK(print(container) ==
				"6 7 3 4 5         10,4  11,4  7,4  8,4  9,4  \n"
				"6 7 3 4 5         10,3  11,3  7,3  8,3  9,3  \n"
				"6 7 3 4 5         10,2  11,2  7,2  8,2  9,2  \n"
				"6 7 3 4 5         10,1  11,1  7,1  8,1  9,1  \n"
				"6 7 3)4 5         10,0  11,0  7,0  8,0  9,0  \n"
		);
		slide(container, 4, 0, 8);
		CHECK(print(container) ==
				"8 7 8 8 8         15,4  11,4  12,4  13,4  14,4  \n"
				"8 7 8 8 8         15,3  11,3  12,3  13,3  14,3  \n"
				"8 7 8 8 8         15,2  11,2  12,2  13,2  14,2  \n"
				"8 7 8 8 8         15,1  11,1  12,1  13,1  14,1  \n"
				"8 7)8 8 8         15,0  11,0  12,0  13,0  14,0  \n"
		);
	}

	SECTION("Slide left") {
		auto container = init();
		slide(container, -1, 0, 1);
		CHECK(print(container) ==
				"0 0 0 0 1         0,4  1,4  2,4  3,4  -1,4  \n"
				"0 0 0 0 1         0,3  1,3  2,3  3,3  -1,3  \n"
				"0 0 0 0 1         0,2  1,2  2,2  3,2  -1,2  \n"
				"0 0 0 0 1         0,1  1,1  2,1  3,1  -1,1  \n"
				"0 0 0 0 1)        0,0  1,0  2,0  3,0  -1,0  \n"
		);
		slide(container, -1, 0, 2);
		slide(container, -1, 0, 3);
		slide(container, -1, 0, 4);
		slide(container, -1, 0, 5);
		CHECK(print(container) ==
				"5 4 3 2 1         -5,4  -4,4  -3,4  -2,4  -1,4  \n"
				"5 4 3 2 1         -5,3  -4,3  -3,3  -2,3  -1,3  \n"
				"5 4 3 2 1         -5,2  -4,2  -3,2  -2,2  -1,2  \n"
				"5 4 3 2 1         -5,1  -4,1  -3,1  -2,1  -1,1  \n"
				"5)4 3 2 1         -5,0  -4,0  -3,0  -2,0  -1,0  \n"
		);
		slide(container, -1, 0, 6);
		slide(container, -1, 0, 7);
		CHECK(print(container) ==
				"5 4 3 7 6         -5,4  -4,4  -3,4  -7,4  -6,4  \n"
				"5 4 3 7 6         -5,3  -4,3  -3,3  -7,3  -6,3  \n"
				"5 4 3 7 6         -5,2  -4,2  -3,2  -7,2  -6,2  \n"
				"5 4 3 7 6         -5,1  -4,1  -3,1  -7,1  -6,1  \n"
				"5 4 3 7)6         -5,0  -4,0  -3,0  -7,0  -6,0  \n"
		);
		slide(container, -4, 0, 8);
		CHECK(print(container) ==
				"8 8 8 7 8         -10,4  -9,4  -8,4  -7,4  -11,4  \n"
				"8 8 8 7 8         -10,3  -9,3  -8,3  -7,3  -11,3  \n"
				"8 8 8 7 8         -10,2  -9,2  -8,2  -7,2  -11,2  \n"
				"8 8 8 7 8         -10,1  -9,1  -8,1  -7,1  -11,1  \n"
				"8 8 8 7 8)        -10,0  -9,0  -8,0  -7,0  -11,0  \n"
		);
	}

	SECTION("Slide here there back to place") {
		auto container = init();
		slide(container, 0, 2, 1);
		slide(container, 0, -2, 2);
		CHECK(print(container) ==
				"0 0 0 0 0         0,4  1,4  2,4  3,4  4,4  \n"
				"0 0 0 0 0         0,3  1,3  2,3  3,3  4,3  \n"
				"0 0 0 0 0         0,2  1,2  2,2  3,2  4,2  \n"
				"2 2 2 2 2         0,1  1,1  2,1  3,1  4,1  \n"
				"2)2 2 2 2         0,0  1,0  2,0  3,0  4,0  \n"
		);
	}

	SECTION("Slide here there back to place 2") {
		auto container = init();
		slide(container, 2, 0, 1);
		slide(container, -2, 0, 2);
		CHECK(print(container) ==
				"2 2 0 0 0         0,4  1,4  2,4  3,4  4,4  \n"
				"2 2 0 0 0         0,3  1,3  2,3  3,3  4,3  \n"
				"2 2 0 0 0         0,2  1,2  2,2  3,2  4,2  \n"
				"2 2 0 0 0         0,1  1,1  2,1  3,1  4,1  \n"
				"2)2 0 0 0         0,0  1,0  2,0  3,0  4,0  \n"
		);
	}

	SECTION("Slide here there back to place 3") {
		auto container = init();
		slide(container, 0, 1, 1);
		slide(container, 0, -2, 2);
		slide(container, 0, 1, 3);
		CHECK(print(container) ==
				"3 3 3 3 3         0,4  1,4  2,4  3,4  4,4  \n"
				"0 0 0 0 0         0,3  1,3  2,3  3,3  4,3  \n"
				"0 0 0 0 0         0,2  1,2  2,2  3,2  4,2  \n"
				"0 0 0 0 0         0,1  1,1  2,1  3,1  4,1  \n"
				"2)2 2 2 2         0,0  1,0  2,0  3,0  4,0  \n"
		);
		slide(container, 1, 0, 4);
		slide(container, -2, 0, 5);
		slide(container, 1, 0, 6);
		CHECK(print(container) ==
				"5 3 3 3 6         0,4  1,4  2,4  3,4  4,4  \n"
				"5 0 0 0 6         0,3  1,3  2,3  3,3  4,3  \n"
				"5 0 0 0 6         0,2  1,2  2,2  3,2  4,2  \n"
				"5 0 0 0 6         0,1  1,1  2,1  3,1  4,1  \n"
				"5)2 2 2 6         0,0  1,0  2,0  3,0  4,0  \n"
		);
	}

	SECTION("Slide multiple direction") {
		auto container = init();
		slide_gen(container, 2, 2);
		CHECK(print(container) ==
				"1 1 0 0 0         5,4  6,4  2,4  3,4  4,4  \n"
				"1 1 0 0 0         5,3  6,3  2,3  3,3  4,3  \n"
				"1 1 0)0 0         5,2  6,2  2,2  3,2  4,2  \n"
				"1 1 1 1 1         5,6  6,6  2,6  3,6  4,6  \n"
				"1 1 1 1 1         5,5  6,5  2,5  3,5  4,5  \n"
		);
		slide_gen(container, -2, -2);
		CHECK(print(container) ==
				"2 2 0 0 0         0,4  1,4  2,4  3,4  4,4  \n"
				"2 2 0 0 0         0,3  1,3  2,3  3,3  4,3  \n"
				"2 2 0 0 0         0,2  1,2  2,2  3,2  4,2  \n"
				"2 2 2 2 2         0,1  1,1  2,1  3,1  4,1  \n"
				"2)2 2 2 2         0,0  1,0  2,0  3,0  4,0  \n"
		);
		slide_gen(container, 2, 3);
		CHECK(print(container) ==
				"3 3 0 0 0         5,4  6,4  2,4  3,4  4,4  \n"
				"3 3 0)0 0         5,3  6,3  2,3  3,3  4,3  \n"
				"3 3 1 1 1         5,7  6,7  2,7  3,7  4,7  \n"
				"3 3 3 3 3         5,6  6,6  2,6  3,6  4,6  \n"
				"3 3 3 3 3         5,5  6,5  2,5  3,5  4,5  \n"
		);
		slide_gen(container, 0, -3);
		CHECK(print(container) ==
				"3 3 0 0 0         5,4  6,4  2,4  3,4  4,4  \n"
				"3 3 0 0 0         5,3  6,3  2,3  3,3  4,3  \n"
				"4 4 2 2 2         5,2  6,2  2,2  3,2  4,2  \n"
				"4 4 4 4 4         5,1  6,1  2,1  3,1  4,1  \n"
				"4 4 4)4 4         5,0  6,0  2,0  3,0  4,0  \n"
		);
		slide_gen(container, -2, -2);
		CHECK(print(container) ==
				"4 4 1 1 1         0,-1  1,-1  2,-1  3,-1  4,-1  \n"
				"4)4 1 1 1         0,-2  1,-2  2,-2  3,-2  4,-2  \n"
				"5 5 2 2 2         0,2  1,2  2,2  3,2  4,2  \n"
				"5 5 4 4 4         0,1  1,1  2,1  3,1  4,1  \n"
				"5 5 4 4 4         0,0  1,0  2,0  3,0  4,0  \n"
		);
		slide_gen(container, 0, -3);
		CHECK(print(container) ==
				"4 4 1 1 1         0,-1  1,-1  2,-1  3,-1  4,-1  \n"
				"4 4 1 1 1         0,-2  1,-2  2,-2  3,-2  4,-2  \n"
				"6 6 3 3 3         0,-3  1,-3  2,-3  3,-3  4,-3  \n"
				"6 6 5 5 5         0,-4  1,-4  2,-4  3,-4  4,-4  \n"
				"6)6 5 5 5         0,-5  1,-5  2,-5  3,-5  4,-5  \n"
		);
		slide_gen(container, 1, 2);
		CHECK(print(container) ==
				"5 4 1 1 1         5,-1  1,-1  2,-1  3,-1  4,-1  \n"
				"5 4 1 1 1         5,-2  1,-2  2,-2  3,-2  4,-2  \n"
				"7 6)3 3 3         5,-3  1,-3  2,-3  3,-3  4,-3  \n"
				"7 7 6 6 6         5,1  1,1  2,1  3,1  4,1  \n"
				"7 7 6 6 6         5,0  1,0  2,0  3,0  4,0  \n"
		);
	}

	SECTION("Slide multiple pages") {
		auto container = init();
		slide_gen(container, 0, 100);
		CHECK(print(container) ==
				"1 1 1 1 1         0,104  1,104  2,104  3,104  4,104  \n"
				"1 1 1 1 1         0,103  1,103  2,103  3,103  4,103  \n"
				"1 1 1 1 1         0,102  1,102  2,102  3,102  4,102  \n"
				"1 1 1 1 1         0,101  1,101  2,101  3,101  4,101  \n"
				"1)1 1 1 1         0,100  1,100  2,100  3,100  4,100  \n"
		);
		slide_gen(container, 0, -200);
		CHECK(print(container) ==
				"2 2 2 2 2         0,-96  1,-96  2,-96  3,-96  4,-96  \n"
				"2 2 2 2 2         0,-97  1,-97  2,-97  3,-97  4,-97  \n"
				"2 2 2 2 2         0,-98  1,-98  2,-98  3,-98  4,-98  \n"
				"2 2 2 2 2         0,-99  1,-99  2,-99  3,-99  4,-99  \n"
				"2)2 2 2 2         0,-100  1,-100  2,-100  3,-100  4,-100  \n"
		);
		slide_gen(container, 100, 0);
		CHECK(print(container) ==
				"3 3 3 3 3         100,-96  101,-96  102,-96  103,-96  104,-96  \n"
				"3 3 3 3 3         100,-97  101,-97  102,-97  103,-97  104,-97  \n"
				"3 3 3 3 3         100,-98  101,-98  102,-98  103,-98  104,-98  \n"
				"3 3 3 3 3         100,-99  101,-99  102,-99  103,-99  104,-99  \n"
				"3)3 3 3 3         100,-100  101,-100  102,-100  103,-100  104,-100  \n"
		);
		slide_gen(container, -200, 0);
		CHECK(print(container) ==
				"4 4 4 4 4         -100,-96  -99,-96  -98,-96  -97,-96  -96,-96  \n"
				"4 4 4 4 4         -100,-97  -99,-97  -98,-97  -97,-97  -96,-97  \n"
				"4 4 4 4 4         -100,-98  -99,-98  -98,-98  -97,-98  -96,-98  \n"
				"4 4 4 4 4         -100,-99  -99,-99  -98,-99  -97,-99  -96,-99  \n"
				"4)4 4 4 4         -100,-100  -99,-100  -98,-100  -97,-100  -96,-100  \n"
		);
		slide_gen(container, 100, 300);
		CHECK(print(container) ==
				"5 5 5 5 5         0,204  1,204  2,204  3,204  4,204  \n"
				"5 5 5 5 5         0,203  1,203  2,203  3,203  4,203  \n"
				"5 5 5 5 5         0,202  1,202  2,202  3,202  4,202  \n"
				"5 5 5 5 5         0,201  1,201  2,201  3,201  4,201  \n"
				"5)5 5 5 5         0,200  1,200  2,200  3,200  4,200  \n"
		);
		slide_gen(container, -100, -200);
		CHECK(print(container) ==
				"6 6 6 6 6         -100,4  -99,4  -98,4  -97,4  -96,4  \n"
				"6 6 6 6 6         -100,3  -99,3  -98,3  -97,3  -96,3  \n"
				"6 6 6 6 6         -100,2  -99,2  -98,2  -97,2  -96,2  \n"
				"6 6 6 6 6         -100,1  -99,1  -98,1  -97,1  -96,1  \n"
				"6)6 6 6 6         -100,0  -99,0  -98,0  -97,0  -96,0  \n"
		);
		slide_gen(container, 100, 0);
		CHECK(print(container) ==
				"7 7 7 7 7         0,4  1,4  2,4  3,4  4,4  \n"
				"7 7 7 7 7         0,3  1,3  2,3  3,3  4,3  \n"
				"7 7 7 7 7         0,2  1,2  2,2  3,2  4,2  \n"
				"7 7 7 7 7         0,1  1,1  2,1  3,1  4,1  \n"
				"7)7 7 7 7         0,0  1,0  2,0  3,0  4,0  \n"
		);
	}
}
