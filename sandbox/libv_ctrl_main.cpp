// Project: libv.ctrl, File: sandbox/libv_ctrl_main.cpp

// libv
#include <libv/utility/concat.hpp>
// std
#include <iostream>
// pro
//#include <libv/ctrl/controls.hpp>
//#include <libv/ctrl/feature_register.hpp>
#include <libv/ctrl/parse.hpp>
#include <libv/ctrl/combination.hpp>



// -------------------------------------------------------------------------------------------------

std::optional<libv::ctrl::Combination> parse_combination_optional(const std::string_view str);

libv::ctrl::Combination parse_combination_or_throw(const std::string_view str) {
	auto result = parse_combination_optional(str);
	if (result)
		return std::move(*result);
	else
		throw std::invalid_argument(libv::concat("\"", str, "\" is not a valid Combination"));
}

int main() {
	auto comb_obj = parse_combination_or_throw("'+'+'+A");

	return EXIT_SUCCESS;
}
