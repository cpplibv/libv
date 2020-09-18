// Project: libv.utility, File: test/libv/utility/test_extract_bits.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/utility/extract_bits.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("extract_high_bits", "[libv.utility.extract_high_bits]") {
	CHECK(libv::extract_high_bits< uint8_t,  0, 8>(0x0123456789ABCDEFull) == 0x01);
	CHECK(libv::extract_high_bits< uint8_t,  0, 8>(0xFEDCBA9876543210ull) == 0xFE);
	CHECK(libv::extract_high_bits< uint8_t,  1, 8>(0x0123456789ABCDEFull) == 0x02);
	CHECK(libv::extract_high_bits< uint8_t,  1, 8>(0xFEDCBA9876543210ull) == 0xFD);
	CHECK(libv::extract_high_bits< uint8_t,  8, 8>(0x0123456789ABCDEFull) == 0x23);
	CHECK(libv::extract_high_bits< uint8_t,  8, 8>(0xFEDCBA9876543210ull) == 0xDC);
	CHECK(libv::extract_high_bits< uint8_t, 16, 1>(0xFEDCBA9876543210ull) == 0x01);
	CHECK(libv::extract_high_bits< uint8_t, 56, 8>(0x0123456789ABCDEFull) == 0xEF);
	CHECK(libv::extract_high_bits< uint8_t, 56, 8>(0xFEDCBA9876543210ull) == 0x10);

	CHECK(libv::extract_high_bits<uint64_t,  0, 8>(0x0123456789ABCDEFull) == 0x01);
	CHECK(libv::extract_high_bits<uint64_t,  1, 8>(0x0123456789ABCDEFull) == 0x02);
	CHECK(libv::extract_high_bits<uint64_t,  8, 8>(0x0123456789ABCDEFull) == 0x23);
	CHECK(libv::extract_high_bits<uint64_t, 16, 1>(0xFEDCBA9876543210ull) == 0x01);
	CHECK(libv::extract_high_bits<uint64_t, 56, 8>(0x0123456789ABCDEFull) == 0xEF);
}

// -------------------------------------------------------------------------------------------------
