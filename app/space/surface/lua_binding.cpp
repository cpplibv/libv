// Created by dbobula on 2/20/2022.

#include <space/surface/lua_binding.hpp>

#include <iostream>
#include <libv/lua/sol_type_to_string.hpp>


namespace surface {
void SurfaceLuaBinding::traverseTable(sol::table table, int depth) {
//	int depth = 0;
	for (const auto&[key, value] : table) {
		std::cout << std::string(depth, '\t');
		if (value.get_type() == sol::type::table) {
			std::cout << key.as<std::string_view>() << ":" << std::endl;
			traverseTable(value, depth + 1);
		} else {
			std::cout << key.as<std::string_view>() << " = " << value.as<std::string_view>() << std::endl;
		}
	}
}

SurfaceLuaBinding::SurfaceLuaBinding() {
	lua.open_libraries(sol::lib::base);
//	std::cout << "scriptStr: " << scriptStr << std::endl;
//	int x = 0;
//	lua.set_function("beep", [&x]{ ++x; });
//		lua.set_function("size", [this](int size) { config.size = size; });

//		std::cout << "luaConfig: " << luaConfig << std::endl;
}

void SurfaceLuaBinding::setConfig(const sol::table& luaConfig) {
	config.mode = luaConfig["mode"];
	config.size = luaConfig["size"];
	config.octaves = luaConfig["octaves"];
	config.amplitude = luaConfig["amplitude"];
	config.frequency = luaConfig["frequency"];
	config.lacunarity = luaConfig["lacunarity"];
	config.persistence = luaConfig["persistence"];
	config.seed = luaConfig["seed"];
	config.noiseType = luaConfig["noiseType"];
}

Config SurfaceLuaBinding::getConfigFromLuaScript(std::string_view script) {
//		auto scriptStr = libv::read_file_str_or_throw(filepath);
	auto env = sol::environment(lua, sol::create, lua.globals());
	lua.script(script, env);
	sol::table luaConfig = env["Config"];
	if (luaConfig.get_type() != sol::type::table) {
		//TODO: log error
//		return config;
		throw std::runtime_error(std::string("Expected lua table with name \"Config\" but received: ") + std::string(libv::lua::lua_type_to_string(luaConfig.get_type())));
	}
	//traverseTable(luaConfig);
	std::cout << "luaConfig[\"mode\"] " << luaConfig["mode"].get<std::string_view>() << std::endl;
	std::cout << "luaConfig[\"size\"] " << luaConfig["size"].get<std::string_view>() << std::endl;
	std::cout << "luaConfig[\"octaves\"] " << luaConfig["octaves"].get<std::string_view>() << std::endl;
	std::cout << "luaConfig[\"amplitude\"] " << luaConfig["amplitude"].get<std::string_view>() << std::endl;
	std::cout << "luaConfig[\"frequency\"] " << luaConfig["frequency"].get<std::string_view>() << std::endl;
	std::cout << "luaConfig[\"lacunarity\"] " << luaConfig["lacunarity"].get<std::string_view>() << std::endl;
	std::cout << "luaConfig[\"persistence\"] " << luaConfig["persistence"].get<std::string_view>() << std::endl;
	std::cout << "luaConfig[\"seed\"] " << luaConfig["seed"].get<std::string_view>() << std::endl;
	std::cout << "luaConfig[\"noiseType\"] " << luaConfig["noiseType"].get<std::string_view>() << std::endl;
//		sol::type::table
	setConfig(luaConfig);
//	config.size = 1024;
	return config;
}

} // namespace surface
