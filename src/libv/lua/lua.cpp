// Project: libv.lua, File: src/libv/lua/lua.cpp

// hpp
#include <libv/lua/lua.hpp>
// ext
#include <sol/state.hpp>
// libv
#include <libv/utility/to_underlying.hpp>
// pro
#include <libv/lua/vec.hpp>
#include <libv/lua/vm4.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

State create_state(lualib libmask) {
	State lua;

	open_libraries(lua, libmask);

	return lua;
}

void open_libraries(sol::state& lua, lualib libmask) {
	if ((libv::to_underlying(libmask) & libv::to_underlying(lualib::base)) != 0) {
		lua.open_libraries(sol::lib::base);
		lua.open_libraries(sol::lib::coroutine);
		lua.open_libraries(sol::lib::debug);
		lua.open_libraries(sol::lib::math);
		lua.open_libraries(sol::lib::package);
		lua.open_libraries(sol::lib::string);
		lua.open_libraries(sol::lib::table);

		// TODO P1: Proper setting of lua["package"]["path"] during init
		lua["package"]["path"] = ";res/script/?.lua;script/?.lua;?.lua";
	}

	if ((libv::to_underlying(libmask) & libv::to_underlying(lualib::vec)) != 0) {
		registerLuaVec2<libv::vec2f>(lua, "vec2f");
		registerLuaVec2<libv::vec2d>(lua, "vec2d");
		registerLuaVec3<libv::vec3f>(lua, "vec3f");
		registerLuaVec3<libv::vec3d>(lua, "vec3d");
		registerLuaVec4<libv::vec4f>(lua, "vec4f");
		registerLuaVec4<libv::vec4d>(lua, "vec4d");
	}

//	if ((libv::to_underlying(libmask) & libv::to_underlying(lualib::vm4)) != 0) {
//		registerLuaVM4(lua, "vec4d");
//	}
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
