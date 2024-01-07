// Project: libv.ctrl, File: test/libv/ctrl/test_input_id.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/ctrl/input.hpp>
#include <libv/ctrl/parse.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Test more_specialized", "[libv.ctrl]") {
	const auto test_spec = [](const auto& a, const auto& b) {
		return libv::ctrl::more_specialized(
				libv::ctrl::parse_input_id_or_throw(a),
				libv::ctrl::parse_input_id_or_throw(b)
		);
	};

	CHECK(+1 == test_spec("L Ctrl", "Ctrl"));
	CHECK( 0 == test_spec("Ctrl", "Ctrl"));
	CHECK( 0 == test_spec("L Ctrl", "L Ctrl"));
	CHECK(-1 == test_spec("Ctrl", "L Ctrl"));

	CHECK(-1 == test_spec("GP B 1", "GP 2 B 1"));
	CHECK(+1 == test_spec("GP 1 B 1", "GP B 1"));
	CHECK( 0 == test_spec("GP 1 B 1", "GP 2 B 1"));
	CHECK( 0 == test_spec("GP B 1", "GP B 1"));

	CHECK( 0 == test_spec("GP B 1", "GP B 2")); // Not alias
	CHECK( 0 == test_spec("GP B 1", "GP 2 B 2")); // Not alias
	CHECK( 0 == test_spec("GP 1 B 1", "GP 2 B 2")); // Not alias
	CHECK( 0 == test_spec("GP 1 B 1", "GP B 2")); // Not alias
}

TEST_CASE("Test alias_match", "[libv.ctrl]") {
	const auto test_alias = [](const auto& a, const auto& b) {
		return libv::ctrl::alias_match(
				libv::ctrl::parse_input_id_or_throw(a),
				libv::ctrl::parse_input_id_or_throw(b)
		);
	};

	CHECK(test_alias("Ctrl", "Ctrl"));
	CHECK(test_alias("Ctrl", "L Ctrl"));
	CHECK(test_alias("L Ctrl", "Ctrl"));
	CHECK(test_alias("L Ctrl", "L Ctrl"));

	CHECK(not test_alias("MX+", "MX-"));
	CHECK(test_alias("MX+", "MX+"));
	CHECK(test_alias("MX+", "MX"));
	CHECK(test_alias("MX-", "MX-"));
	CHECK(not test_alias("MX-", "MX+"));
	CHECK(test_alias("MX-", "MX"));
	CHECK(test_alias("MX", "MX-"));
	CHECK(test_alias("MX", "MX+"));
	CHECK(test_alias("MX", "MX"));

	CHECK(not test_alias("MX", "MY"));

	CHECK(test_alias("MS", "MS+"));
	CHECK(test_alias("MS+", "MS"));
	CHECK(test_alias("MS+", "MS+"));
	CHECK(not test_alias("MS+", "MS-"));

	CHECK(test_alias("MSY", "MSY+"));
	CHECK(test_alias("MSY+", "MSY"));
	CHECK(test_alias("MSY+", "MSY+"));
	CHECK(not test_alias("MSY+", "MSY-"));

	CHECK(test_alias("GP B 1", "GP 2 B 1"));
	CHECK(test_alias("GP 1 B 1", "GP B 1"));
	CHECK(not test_alias("GP 1 B 1", "GP 2 B 1"));

	CHECK(not test_alias("GP B 1", "GP B 2"));
}
