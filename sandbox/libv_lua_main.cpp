// Project: libv.lua, File: sandbox/libv_lua_main.cpp

// libv
#include <libv/log/log.hpp>
#include <libv/math/vec.hpp>
// std
#include <iostream>
// pro
#include <libv/lua/lua.hpp>
#include <libv/lua/object_parser.hpp>


// -------------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;

	auto lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);

	int i = 0;

	lua["foo"] = [&i]{ i++; };

	libv::vec3f result = lua.script(R"(
		local v0 = vec3f(0, 1, 0)
		local v1 = vec3f(1, 1, 1)
		v0.x = v0.x + 2
		foo()
		return v0 - v1
	)");

	(void) result;
	assert(result == libv::vec3f(1, 0, -1));
	assert(i == 1);

	return EXIT_SUCCESS;
}
