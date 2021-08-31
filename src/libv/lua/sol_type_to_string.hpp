// Project: libv.lua, File: src/libv/lua/object_parser.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <sol/types.hpp>
// std
#include <cassert>
#include <string_view>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline std::string_view lua_type_to_string(sol::type type) noexcept {
	switch (type) {
	case sol::type::none: return "none";
	case sol::type::nil: return "nil";
	case sol::type::string: return "string";
	case sol::type::number: return "number";
	case sol::type::thread: return "thread";
	case sol::type::boolean: return "boolean";
	case sol::type::function: return "function";
	case sol::type::userdata: return "userdata";
	case sol::type::lightuserdata: return "lightuserdata";
	case sol::type::table: return "table";
	case sol::type::poly: return "poly";
	}

	assert(false && "Invalid sol::type enum value");
	return "<unknown>";
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
