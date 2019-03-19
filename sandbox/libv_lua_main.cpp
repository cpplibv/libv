// File: lua.hpp Author: Vader Created on 2016. november 14., 23:44

// libv
#include <libv/log/log.hpp>
#include <libv/math/vec.hpp>
// std
#include <iostream>
// pro
#include <libv/lua/lua.hpp>


// -------------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger;

	auto lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);

	libv::vec3f result = lua.script(R"(
		local v0 = vec3f(0, 1, 0)
		local v1 = vec3f(1, 1, 1)
		v0.x = v0.x + 2
		return v0 - v1
	)");

	assert(result == libv::vec3f(1, 0, -1));

	return EXIT_SUCCESS;
}
