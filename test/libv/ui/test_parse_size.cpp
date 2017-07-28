// File: test_parse_size.cpp - Created on 2017.10.22. 06:23 - Author: Vader

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/utility/approxing.hpp>
// std
#include <sstream>
// pro
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/size.hpp>


// -------------------------------------------------------------------------------------------------

namespace {
	std::string parse_to_string(const std::string& size) {
		const auto result = libv::ui::parse_size(size);
		std::ostringstream ss;
		if (result)
			ss << *result;
		else
			ss << "invalid";
		return ss.str();
	}
} // namespace

// -------------------------------------------------------------------------------------------------

TEST_CASE("parse size: positioning with alignment", "[iris.ui]") {
	CHECK(parse_to_string("                  ") == "invalid");
//	CHECK(parse_to_string("100px, 100px      ") == "100px 100px 0");
}
