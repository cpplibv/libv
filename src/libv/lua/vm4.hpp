// Project: libv.lua, File: src/libv/lua/vm4.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
//#include <fmt/format.h>
#include <sol/state.hpp>
//#include <sol/usertype.hpp>
// libv
//#include <libv/math/vec.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

void registerLuaVM4(sol::state& lua) {
	(void) lua;

//	using V = typename T::value_type;
//
//	auto type = lua.create_simple_usertype<T>(
//			sol::call_constructor, sol::constructors<sol::types<V, V>>{}
//	);
//
//	type.set("x", &T::x);
//	type.set("y", &T::y);
//
//	type.set(sol::meta_function::to_string, [](const T& vec) {
//			return fmt::format("{} {}", vec.x, vec.y);
//	});
//
//	registerLuaVecCommon<V, T>(type);
//	lua.set_usertype(name, type);
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
