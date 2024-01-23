// Project: libv.lua, File: src/libv/lua/to_string.cpp

#include <libv/lua/to_string.hpp>

#include <sol/object.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

std::string to_string(sol::object object) {
	switch (object.get_type()) {
	case sol::type::none: return "none";
	case sol::type::nil: return "nil";
	case sol::type::string: return object.as<std::string>();
	case sol::type::number: return std::to_string(object.as<double>());
	case sol::type::thread: return "<thread>";
	case sol::type::boolean: return object.as<bool>() ? "true" : "false";
	case sol::type::function: return "<function>";
	case sol::type::userdata: return "<userdata>"; // TODO P5: attempt a to string call to userdata
	case sol::type::lightuserdata: return "<lightuserdata>";
	case sol::type::table: return "<table>"; // TODO P3: Implement a dump function in C++ for tables (with tacking seen tables)
	case sol::type::poly: return "<poly>";
	}

	assert(false && "Invalid sol::type enum value");
	return "<unknown>";
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
