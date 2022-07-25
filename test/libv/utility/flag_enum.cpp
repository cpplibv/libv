// Project: libv.utility, File: test/libv/utility/flag_enum.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// ext
#include <fmt/ostream.h>
// pro
#include <libv/utility/flag_enum.hpp>


// -------------------------------------------------------------------------------------------------

using test_flag = libv::flag_enum<uint8_t, class TestTag>;

TEST_CASE("Constructor usage", "[flag_enum]") {
	CHECK(test_flag{libv::bit(0)}.value() == 1);
	CHECK(test_flag{libv::bit(1)}.value() == 2);
	CHECK(test_flag{libv::bit(7)}.value() == 128);

	CHECK(test_flag{1}.value() == 1);
	CHECK(test_flag{2}.value() == 2);
	CHECK(test_flag{128}.value() == 128);
}

TEST_CASE("Constructor merge flags", "[flag_enum]") {
	constexpr test_flag flag_bit_0 = libv::bit(0);
	constexpr test_flag flag_bit_1 = libv::bit(1);
	constexpr test_flag flag_bit_7 = libv::bit(7);

	constexpr test_flag flag_bit_01 = flag_bit_0 | flag_bit_1;
	constexpr test_flag flag_bit_07 = flag_bit_0 | flag_bit_7;
	constexpr test_flag flag_bit_17 = flag_bit_1 | flag_bit_7;

	CHECK(flag_bit_01.value() == 3);
	CHECK(flag_bit_07.value() == 129);
	CHECK(flag_bit_17.value() == 130);
}

TEST_CASE("static_cast", "[flag_enum]") {
	constexpr test_flag flag_0 = libv::bit(0);
	constexpr test_flag flag_1 = libv::bit(1);
	constexpr test_flag flag_7 = libv::bit(7);

	CHECK(flag_0.value() == 1);
	CHECK(flag_1.value() == 2);
	CHECK(flag_7.value() == 128);
}

TEST_CASE("NOT", "[flag_enum]") {
	constexpr test_flag flag_0{0};
	constexpr test_flag flag_1{1};
	constexpr test_flag flag_7{128};

	CHECK((~flag_0).value() == 0b11111111);
	CHECK((~flag_1).value() == 0b11111110);
	CHECK((~flag_7).value() == 0b01111111);
}

TEST_CASE("Equal", "[flag_enum]") {
	constexpr test_flag flag_0{0};
	constexpr test_flag flag_1{1};

	CHECK(flag_0 == flag_0);
	CHECK(flag_1 == flag_1);
	CHECK(flag_1 != flag_0);
	CHECK(flag_0 != flag_1);
}

TEST_CASE("AND", "[flag_enum]") {
	constexpr test_flag flag_0{0};
	constexpr test_flag flag_1{1};

	CHECK((flag_0 & flag_0).value() == 0);
	CHECK((flag_0 & flag_1).value() == 0);
	CHECK((flag_1 & flag_0).value() == 0);
	CHECK((flag_1 & flag_1).value() == 1);

	test_flag tmp_flags_0{0}; CHECK((tmp_flags_0 &= flag_0).value() == 0);
	test_flag tmp_flags_1{0}; CHECK((tmp_flags_1 &= flag_1).value() == 0);
	test_flag tmp_flags_2{1}; CHECK((tmp_flags_2 &= flag_0).value() == 0);
	test_flag tmp_flags_3{1}; CHECK((tmp_flags_3 &= flag_1).value() == 1);
}

TEST_CASE("OR", "[flag_enum]") {
	constexpr test_flag flag_0{0};
	constexpr test_flag flag_1{1};

	CHECK((flag_0 | flag_0).value() == 0);
	CHECK((flag_0 | flag_1).value() == 1);
	CHECK((flag_1 | flag_0).value() == 1);
	CHECK((flag_1 | flag_1).value() == 1);

	test_flag tmp_flags_0{0}; CHECK((tmp_flags_0 |= flag_0).value() == 0);
	test_flag tmp_flags_1{0}; CHECK((tmp_flags_1 |= flag_1).value() == 1);
	test_flag tmp_flags_2{1}; CHECK((tmp_flags_2 |= flag_0).value() == 1);
	test_flag tmp_flags_3{1}; CHECK((tmp_flags_3 |= flag_1).value() == 1);
}

TEST_CASE("XOR", "[flag_enum]") {
	constexpr test_flag flag_0{0};
	constexpr test_flag flag_1{1};

	CHECK((flag_0 ^ flag_0).value() == 0);
	CHECK((flag_0 ^ flag_1).value() == 1);
	CHECK((flag_1 ^ flag_0).value() == 1);
	CHECK((flag_1 ^ flag_1).value() == 0);

	test_flag tmp_flags_0{0}; CHECK((tmp_flags_0 ^= flag_0).value() == 0);
	test_flag tmp_flags_1{0}; CHECK((tmp_flags_1 ^= flag_1).value() == 1);
	test_flag tmp_flags_2{1}; CHECK((tmp_flags_2 ^= flag_0).value() == 1);
	test_flag tmp_flags_3{1}; CHECK((tmp_flags_3 ^= flag_1).value() == 0);
}

TEST_CASE("Match mask", "[flag_enum]") {
	constexpr test_flag flag_bit_0 = libv::bit(0);
	constexpr test_flag flag_bit_1 = libv::bit(1);
	constexpr test_flag flag_bit_7 = libv::bit(7);

	constexpr test_flag flag_bit_01 = flag_bit_0 | flag_bit_1;
	constexpr test_flag flag_bit_07 = flag_bit_0 | flag_bit_7;
	constexpr test_flag flag_bit_17 = flag_bit_1 | flag_bit_7;

	constexpr test_flag flag_bit_017 = flag_bit_0 | flag_bit_1 | flag_bit_7;

	CHECK(flag_bit_01.match_mask(flag_bit_0));
	CHECK(flag_bit_01.match_mask(flag_bit_1));
	CHECK(not flag_bit_01.match_mask(flag_bit_7));

	CHECK(flag_bit_01.match_mask(flag_bit_01));
	CHECK(not flag_bit_01.match_mask(flag_bit_07));

	CHECK(flag_bit_017.match_mask(flag_bit_01));
	CHECK(flag_bit_017.match_mask(flag_bit_07));
	CHECK(flag_bit_017.match_mask(flag_bit_17));

	CHECK(not flag_bit_17.match_mask(flag_bit_017));
}

TEST_CASE("OStream", "[flag_enum]") {
	constexpr test_flag flag_1{1};
	constexpr test_flag flag_2{2};
	constexpr test_flag flag_128{128};

	CHECK(fmt::format("{}", flag_1) == "1");
	CHECK(fmt::format("{}", flag_2) == "2");
	CHECK(fmt::format("{}", flag_128) == "128");
}

TEST_CASE("Mix operations with bit", "[flag_enum]") {
	constexpr test_flag flag{0b11110000};

	CHECK((flag | libv::bit(2)).value() == 0b11110100);
	CHECK((flag | libv::bit(5)).value() == 0b11110000);
	CHECK((flag ^ libv::bit(2)).value() == 0b11110100);
	CHECK((flag ^ libv::bit(5)).value() == 0b11010000);
	CHECK((flag & libv::bit(2)).value() == 0b00000000);
	CHECK((flag & libv::bit(5)).value() == 0b00100000);

	CHECK((libv::bit(2) | flag).value() == 0b11110100);
	CHECK((libv::bit(5) | flag).value() == 0b11110000);
	CHECK((libv::bit(2) ^ flag).value() == 0b11110100);
	CHECK((libv::bit(5) ^ flag).value() == 0b11010000);
	CHECK((libv::bit(2) & flag).value() == 0b00000000);
	CHECK((libv::bit(5) & flag).value() == 0b00100000);

	auto tmp_flag_0 = flag; CHECK((tmp_flag_0 |= libv::bit(2)).value() == 0b11110100);
	auto tmp_flag_1 = flag; CHECK((tmp_flag_1 |= libv::bit(5)).value() == 0b11110000);
	auto tmp_flag_2 = flag; CHECK((tmp_flag_2 ^= libv::bit(2)).value() == 0b11110100);
	auto tmp_flag_3 = flag; CHECK((tmp_flag_3 ^= libv::bit(5)).value() == 0b11010000);
	auto tmp_flag_4 = flag; CHECK((tmp_flag_4 &= libv::bit(2)).value() == 0b00000000);
	auto tmp_flag_5 = flag; CHECK((tmp_flag_5 &= libv::bit(5)).value() == 0b00100000);
}

TEST_CASE("reset", "[flag_enum]") {
	test_flag flag_1{1};
	test_flag flag_2{2};
	auto flag_3 = flag_1 | flag_2;

	CHECK(flag_3.value() == 0b00000011);
	flag_3.reset(flag_1);
	CHECK(flag_3.value() == 0b00000010);
	flag_3.set(flag_1);
	CHECK(flag_3.value() == 0b00000011);
	flag_3.reset(flag_2);
	CHECK(flag_3.value() == 0b00000001);
	flag_3.reset(flag_1);
	CHECK(flag_3.value() == 0b00000000);
}

// -------------------------------------------------------------------------------------------------
