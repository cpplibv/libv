// File:   bool.cpp Author: Vader Created on 2019. február 18., 4:42

// hpp
#include <libv/parse/bool.hpp>
// ext
#include <boost/spirit/home/x3.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <algorithm>
#include <stdexcept>


namespace libv {
namespace parse {

// -------------------------------------------------------------------------------------------------

bool parse_bool_or(const std::string_view str, const bool fallback) {
	const auto result = parse_bool_optional(str);
	return result ? *result : fallback;
}

bool parse_bool_or_throw(const std::string_view str) {
	const auto result = parse_bool_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat("\"", str, "\" is not a valid boolean"));
}

std::optional<bool> parse_bool_optional(const std::string_view str) {
	namespace x3 = boost::spirit::x3;

	if (std::all_of(str.begin(), str.end(), static_cast<int(*)(int)>(std::isspace)))
		return false;

	const auto value = [](bool value) {
		return [value](auto& ctx) {
			_val(ctx) = value;
		};
	};

	const auto value_from_int = [](auto& ctx) {
		if (_attr(ctx) == 0)
			_val(ctx) = false;
		else if (_attr(ctx) == 1)
			_val(ctx) = true;
		else if (_attr(ctx) == -1)
			_val(ctx) = true;
		else
			_pass(ctx) = false;
	};

	const auto bool_rule = x3::rule<class bool_rule, bool>{} =
			x3::no_case[x3::lit("true")][value(true)] |
			x3::no_case[x3::lit("false")][value(false)] |
			x3::int_[value_from_int];

	bool result = false;

	auto it = str.begin();
	auto success = x3::phrase_parse(it, str.end(), bool_rule, x3::space, result);
	success = success && it == str.end();

	return success ? std::optional<bool>{result} : std::nullopt;
}

// -------------------------------------------------------------------------------------------------

} // namespace parse
} // namespace libv
