// Project: libv.parse, File: test/libv/ui/parse/test_parse_size.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// std
#include <sstream>
// pro
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/property/size.hpp>


// -------------------------------------------------------------------------------------------------

namespace {

std::string parse_to_string(const std::string& size) {
	const auto result = libv::ui::parse_size_optional(size);
	std::ostringstream ss;
	if (result)
		ss << *result;
	else
		ss << "invalid";
	return ss.str();
}

} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("Parse Size: 0, 1 and >3 dimension single quantity definitions are invalid", "[libv.ui]") {
	CHECK(parse_to_string("                   ") == "invalid");
	CHECK(parse_to_string("100px              ") == "invalid");
	CHECK(parse_to_string("100%               ") == "invalid");
	CHECK(parse_to_string("100r               ") == "invalid");
	CHECK(parse_to_string("d                  ") == "invalid");
	CHECK(parse_to_string("á") == "invalid");
	CHECK(parse_to_string("100px, 200px, 300px, 400px") == "invalid");
	CHECK(parse_to_string("100% , 200% , 300% , 400% ") == "invalid");
	CHECK(parse_to_string("100r , 200r , 300r , 400r ") == "invalid");
	CHECK(parse_to_string("d    , d    , d    , d    ") == "invalid");
}

TEST_CASE("Parse Size: 0, 1 and >3 dimension no quantity definitions are invalid", "[libv.ui]") {
	CHECK(parse_to_string("                   ") == "invalid");
	CHECK(parse_to_string(",                  ") == "invalid");
	CHECK(parse_to_string(",,,                ") == "invalid");
}

TEST_CASE("Parse Size: 2 and 3 dimension single quantity are valid", "[libv.ui]") {
	CHECK(parse_to_string("100px, 200px       ") == "100px 200px 1r");
	CHECK(parse_to_string("100% , 200%        ") == "100% 200% 1r");
	CHECK(parse_to_string("100r , 200r        ") == "100r 200r 1r");
	CHECK(parse_to_string("d    , d           ") == "D D 1r");
	CHECK(parse_to_string("100px, 200px, 300px") == "100px 200px 300px");
	CHECK(parse_to_string("100 px, 200 px, 300 px") == "100px 200px 300px");
	CHECK(parse_to_string("100% , 200% , 300% ") == "100% 200% 300%");
	CHECK(parse_to_string("100r , 200r , 300r ") == "100r 200r 300r");
	CHECK(parse_to_string("d    , d    , d    ") == "D D D");
	CHECK(parse_to_string("dynamic, d    , d    ") == "D D D");
}

TEST_CASE("Parse Size: 2 and 3 dimension multi are valid", "[libv.ui]") {
	CHECK(parse_to_string("100px d, 1r       ") == "100pxD 1r 1r");
	CHECK(parse_to_string("100px d, 1r       ") == "100pxD 1r 1r");
	CHECK(parse_to_string("100px D, 1r       ") == "100pxD 1r 1r");
	CHECK(parse_to_string("100pxdynamic , 1r ") == "100pxD 1r 1r");
	CHECK(parse_to_string("100pxDynamic , 1r ") == "100pxD 1r 1r");
	CHECK(parse_to_string("100px dynamic , 1r") == "100pxD 1r 1r");
	CHECK(parse_to_string("100px Dynamic , 1r") == "100pxD 1r 1r");
	CHECK(parse_to_string("100pxd , 1r       ") == "100pxD 1r 1r");
	CHECK(parse_to_string("100pxD , 1r       ") == "100pxD 1r 1r");

	CHECK(parse_to_string("100pxD , 1rD      ") == "100pxD 1rD 1r");
	CHECK(parse_to_string("100px D, 1r D 10% ") == "100pxD 10%1rD 1r");
	CHECK(parse_to_string("100px D 1r, 1r D 10% ") == "100px1rD 10%1rD 1r");
}

TEST_CASE("Parse Size: 2 and 3 dimension 0 values are valid", "[libv.ui]") {
	CHECK(parse_to_string("0, 0") == "  1r");
	CHECK(parse_to_string("0px, 0r") == "  1r");
	CHECK(parse_to_string("0px, 0r 0% 0px") == "  1r");
	CHECK(parse_to_string("0, 0, 0") == "  ");
	CHECK(parse_to_string("0px, 0r, 0%") == "  ");
}

TEST_CASE("Parse Size: 2 and 3 dimension repeating or no quantities are invalid", "[libv.ui]") {
	CHECK(parse_to_string("1px, 1%, 15 300 px") == "invalid");
	CHECK(parse_to_string(",                 ") == "invalid");
	CHECK(parse_to_string(",,                ") == "invalid");
	CHECK(parse_to_string(" x, x, x          ") == "invalid");
	CHECK(parse_to_string(" , ,              ") == "invalid");
	CHECK(parse_to_string(" , 10             ") == "invalid");
	CHECK(parse_to_string(" , 10, 10         ") == "invalid");
	CHECK(parse_to_string("D D          , 1r ") == "invalid");
	CHECK(parse_to_string("100px D 100px, 1r ") == "invalid");
	CHECK(parse_to_string("100%  1r 100%, 1r ") == "invalid");
	CHECK(parse_to_string("1r 1r        , 1r ") == "invalid");
}
