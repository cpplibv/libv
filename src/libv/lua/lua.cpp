// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <libv/lua/lua.hpp>
// ext
#include <sol/state.hpp>
// libv
#include <libv/utility/enum.hpp>
// pro
#include <libv/lua/vec.hpp>
#include <libv/lua/vm4.hpp>
#include <libv/lua/ui.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

State create_state(lualib libmask) {
	State lua;

	if ((libv::to_value(libmask) & libv::to_value(lualib::base)) != 0) {
		lua.open_libraries(sol::lib::base);
		lua.open_libraries(sol::lib::coroutine);
		lua.open_libraries(sol::lib::debug);
		lua.open_libraries(sol::lib::math);
		lua.open_libraries(sol::lib::package);
		lua.open_libraries(sol::lib::string);
		lua.open_libraries(sol::lib::table);

		// TODO P4: lua["package"]["path"]
		lua["package"]["path"] = ";res/script/?.lua";
	}

	if ((libv::to_value(libmask) & libv::to_value(lualib::vec)) != 0) {
		registerLuaVec2<libv::vec2f>(lua, "vec2f");
		registerLuaVec2<libv::vec2d>(lua, "vec2d");
		registerLuaVec3<libv::vec3f>(lua, "vec3f");
		registerLuaVec3<libv::vec3d>(lua, "vec3d");
		registerLuaVec4<libv::vec4f>(lua, "vec4f");
		registerLuaVec4<libv::vec4d>(lua, "vec4d");
	}

//	if ((libv::to_value(libmask) & libv::to_value(lualib::vm4)) != 0) {
//		registerLuaVM4(lua, "vec4d");
//	}

	if ((libv::to_value(libmask) & libv::to_value(lualib::ui)) != 0) {
		registerUI(lua);
	}


	return lua;
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
