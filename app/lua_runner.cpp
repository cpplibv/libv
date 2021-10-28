// Project: libv, File: app/lua_runner.cpp

// std
#include <iostream>
// pro
#include <libv/lua/lua.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		std::cerr << "usage: " << argv[0] << " <lua_file>" << std::endl;
		return EXIT_FAILURE;
	}

	try {
		auto lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);
		lua["package"]["path"] = ";script/?.lua";
		lua.script_file(argv[1]);
		return EXIT_SUCCESS;

	} catch (const sol::error& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
