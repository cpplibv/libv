// Project: libv.lua, File: src/libv/lua/lua.cpp

// hpp
#include <libv/lua/lua.hpp>
// ext
#include <sol/state.hpp>
#include <sol/types.hpp>
// pro
#include <libv/lua/vec.hpp>
// #include <libv/lua/vm4.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

sol::state create_state(lib libmask) {
	sol::state lua;

	open_libraries(lua, libmask);

	return lua;
}

void open_libraries(sol::state& lua, lib libmask) {
	const auto test = [&](lib test) {
		return (libmask & test) == test;
	};

	// lua standard
	if (test(lib::std_base))
		lua.open_libraries(sol::lib::base);
	if (test(lib::std_coroutine))
		lua.open_libraries(sol::lib::coroutine);
	if (test(lib::std_string))
		lua.open_libraries(sol::lib::string);
	if (test(lib::std_math))
		lua.open_libraries(sol::lib::math);
	if (test(lib::std_table))
		lua.open_libraries(sol::lib::table);
	if (test(lib::std_bit32))
		lua.open_libraries(sol::lib::bit32);
	if (test(lib::std_utf8))
		lua.open_libraries(sol::lib::utf8);
	if (test(lib::std_debug))
		lua.open_libraries(sol::lib::debug);
	if (test(lib::std_package)) {
		lua.open_libraries(sol::lib::package);
		// TODO P1: Proper setting of lua["package"]["path"] during init
		lua["package"]["path"] = ";res/script/?.lua;script/?.lua;?.lua";
	}
	if (test(lib::std_io))
		lua.open_libraries(sol::lib::io);
	if (test(lib::std_os))
		lua.open_libraries(sol::lib::os);

	// jit
	if (test(lib::jit_ffi))
		lua.open_libraries(sol::lib::ffi);
	if (test(lib::jit_jit))
		lua.open_libraries(sol::lib::jit);

	// libv
	if (test(lib::libv_vec)) {
		registerLuaVec2<libv::vec2f>(lua, "vec2f");
		registerLuaVec2<libv::vec2d>(lua, "vec2d");
		registerLuaVec3<libv::vec3f>(lua, "vec3f");
		registerLuaVec3<libv::vec3d>(lua, "vec3d");
		registerLuaVec4<libv::vec4f>(lua, "vec4f");
		registerLuaVec4<libv::vec4d>(lua, "vec4d");
	}
	// if (test(lib::libv_vm4))
	// 	registerLuaVM4(lua);
	// if (test(lib::libv_ui))
	// 	registerLuaUI(lua);
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
