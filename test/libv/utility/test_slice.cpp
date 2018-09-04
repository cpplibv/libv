// File: test_slice.cpp - Created on 2018.09.04. 09:37 - Author: Vader

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/utility/slice.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("slice_view simple lo / hi sub-string", "[libv.utility.slice]") {
	CHECK(libv::slice_view("0123456789", 0, 0) == "");
	CHECK(libv::slice_view("0123456789", 0, 7) == "0123456");
	CHECK(libv::slice_view("0123456789", 0, 10) == "0123456789");
	CHECK(libv::slice_view("0123456789", 1, 10) == "123456789");
	CHECK(libv::slice_view("0123456789", 1, 7) == "123456");
	CHECK(libv::slice_view("0123456789", 10, 10) == "");

	CHECK(libv::slice_view("0123456789", 0, -1) == "012345678");
	CHECK(libv::slice_view("0123456789", 0, -7) == "012");
	CHECK(libv::slice_view("0123456789", 0, -10) == "");
	CHECK(libv::slice_view("0123456789", 1, -1) == "12345678");
	CHECK(libv::slice_view("0123456789", 1, -7) == "12");
	CHECK(libv::slice_view("0123456789", 3, -3) == "3456");
	CHECK(libv::slice_view("0123456789", 5, -5) == "");

	CHECK(libv::slice_view("0123456789", -1, 10) == "9");
	CHECK(libv::slice_view("0123456789", -9, 10) == "123456789");
	CHECK(libv::slice_view("0123456789", -10, 10) == "0123456789");

	CHECK(libv::slice_view("0123456789", -9, -1) == "12345678");
	CHECK(libv::slice_view("0123456789", -9, -2) == "1234567");
	CHECK(libv::slice_view("0123456789", -5, -5) == "");
}

TEST_CASE("slice_view over-indexing lo / hi sub-string", "[libv.utility.slice]") {
	CHECK(libv::slice_view("0123456789", -20, -20) == "");
	CHECK(libv::slice_view("0123456789", 20, 20) == "");

	CHECK(libv::slice_view("0123456789", 0, -20) == "");
	CHECK(libv::slice_view("0123456789", 0, 20) == "0123456789");
	CHECK(libv::slice_view("0123456789", -20, 10) == "0123456789");
	CHECK(libv::slice_view("0123456789", 20, 10) == "");

	CHECK(libv::slice_view("0123456789", -20, 3) == "012");
	CHECK(libv::slice_view("0123456789", 3, -20) == "");

	CHECK(libv::slice_view("0123456789", 20, 3) == "");
	CHECK(libv::slice_view("0123456789", 3, 20) == "3456789");
}

TEST_CASE("slice_view simple cut sub-string", "[libv.utility.slice]") {
	CHECK(libv::slice_view("0123456789", 0) == "");
	CHECK(libv::slice_view("0123456789", 1) == "0");
	CHECK(libv::slice_view("0123456789", 2) == "01");
	CHECK(libv::slice_view("0123456789", 8) == "01234567");
	CHECK(libv::slice_view("0123456789", 9) == "012345678");
	CHECK(libv::slice_view("0123456789", 10) == "0123456789");

	CHECK(libv::slice_view("0123456789", -1) == "9");
	CHECK(libv::slice_view("0123456789", -2) == "89");
	CHECK(libv::slice_view("0123456789", -8) == "23456789");
	CHECK(libv::slice_view("0123456789", -9) == "123456789");
	CHECK(libv::slice_view("0123456789", -10) == "0123456789");
}

TEST_CASE("slice_view over-indexing cut sub-string", "[libv.utility.slice]") {
	CHECK(libv::slice_view("0123456789", -20) == "0123456789");
	CHECK(libv::slice_view("0123456789", 20) == "0123456789");
}
