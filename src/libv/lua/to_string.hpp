// Project: libv.lua, File: src/libv/lua/to_string.hpp

#pragma once

#include <sol/forward.hpp>
#include <sol/types.hpp>

#include <cassert>
#include <string>
#include <string_view>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline std::string_view to_string(sol::type type) noexcept {
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

[[nodiscard]] std::string to_string(sol::object object);

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
