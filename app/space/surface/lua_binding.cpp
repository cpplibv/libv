// Created by dbobula on 2/20/2022.

#include <space/surface/lua_binding.hpp>

#include <iostream>
#include <libv/lua/sol_type_to_string.hpp>


namespace surface {

template <typename T>
void setFractalConfig(T& node, const sol::table& luaConfig) {
	node.seed = luaConfig["seed"];
	node.octaves = luaConfig["octaves"];
	node.amplitude = luaConfig["amplitude"];
	node.frequency = luaConfig["frequency"];
	node.lacunarity = luaConfig["lacunarity"];
	node.persistence = luaConfig["persistence"];
//	config.noiseType = luaConfig["noiseType"];
}

std::unique_ptr<Node> SurfaceLuaBinding::getNodeTree(const sol::table& table, int depth) {
	std::vector<std::unique_ptr<Node>> children;
	for (const auto&[key, value] : table) {
		std::cout << std::string(depth, '\t');
		if (value.get_type() == sol::type::table) {
			std::cout << key.as<std::string_view>() << ":" << std::endl;
			children.emplace_back(getNodeTree(value, depth + 1));
		} else {
			std::cout << key.as<std::string_view>() << " = " << value.as<std::string_view>() << std::endl;
		}
	}

	auto solNodeType = table["nodeType"];
	if (solNodeType != sol::type::string)
		throw std::runtime_error("Valid nodeType field missing");
	std::string_view nodeType = solNodeType.get<std::string_view>();

	if (nodeType == "constant") {
		auto node = std::make_unique<NodeConstant>();
		node->value = table["value"];
		return node;
	}

	else if (nodeType == "perlin") {
		auto node = std::make_unique<NodePerlin>();
		node->seed = table["seed"];
		return node;
	}

	else if (nodeType == "simplex") {
		auto node = std::make_unique<NodeSimplex>();
		node->seed = table["seed"];
		return node;
	}

	else if (nodeType == "simplexFractal") {
		auto node = std::make_unique<NodeSimplexFractal>();
		setFractalConfig(*node, table);
		return node;
	}

	else if (nodeType == "warp") {
		if (children.size() != 1)
			throw std::runtime_error("Warp can only be applied to size of one nodes");

		auto node = std::make_unique<NodeWarp>(std::move(children[0]));
		setFractalConfig(*node, table);
		return node;

	} else if (nodeType == "add") {
		auto x = NodePlus::container();

		for (auto& child : children) {
			x.emplace_back(std::move(child));
		}
		return std::make_unique<NodePlus>(std::move(x));

	} else if (nodeType == "pow") {

		if (children.size() != 1)
			throw std::runtime_error("Warp can only be applied to size of one nodes");

		auto node = std::make_unique<NodePow>(std::move(children[0]));
		node->exponent = table["exponent"];
		return node;

	} else
		throw std::runtime_error("Unknown node type: " + table.as<std::string>());
}

SurfaceLuaBinding::SurfaceLuaBinding() {
	lua.open_libraries(sol::lib::base);
	lua.open_libraries(sol::lib::table);
	lua.open_libraries(sol::lib::string);
	lua.open_libraries(sol::lib::math);
//	lua.open_libraries(sol::lib::math);
//	std::cout << "scriptStr: " << scriptStr << std::endl;
//	int x = 0;
//	lua.set_function("beep", [&x]{ ++x; });
//		lua.set_function("size", [this](int size) { config.size = size; });

//		std::cout << "luaConfig: " << luaConfig << std::endl;
}

void SurfaceLuaBinding::setConfig(const sol::table& luaConfig) {
	config.mode = luaConfig["mode"];
	config.size = luaConfig["size"];
//	config.octaves = luaConfig["octaves"];
	config.amplitude = luaConfig["amplitude"];
//	config.frequency = luaConfig["frequency"];
//	config.lacunarity = luaConfig["lacunarity"];
//	config.persistence = luaConfig["persistence"];
//	config.seed = luaConfig["seed"];
//	config.noiseType = luaConfig["noiseType"];
}

Config SurfaceLuaBinding::getConfigFromLuaScript(std::string_view script) {
//		auto scriptStr = libv::read_file_str_or_throw(filepath);
	auto env = sol::environment(lua, sol::create, lua.globals());
	lua.script(script, env);
	sol::table luaConfig = env["result"];
	if (luaConfig.get_type() != sol::type::table) {
		//TODO: log error
//		return config;
		throw std::runtime_error(std::string("Expected lua table with name \"Config\" but received: ") + std::string(libv::lua::lua_type_to_string(luaConfig.get_type())));
	}
	auto nodes = getNodeTree(luaConfig);
//	std::cout << "luaConfig[\"mode\"] " << luaConfig["mode"].get<std::string_view>() << std::endl;
//	std::cout << "luaConfig[\"size\"] " << luaConfig["size"].get<std::string_view>() << std::endl;
//	std::cout << "luaConfig[\"octaves\"] " << luaConfig["octaves"].get<std::string_view>() << std::endl;
//	std::cout << "luaConfig[\"amplitude\"] " << luaConfig["amplitude"].get<std::string_view>() << std::endl;
//	std::cout << "luaConfig[\"frequency\"] " << luaConfig["frequency"].get<std::string_view>() << std::endl;
//	std::cout << "luaConfig[\"lacunarity\"] " << luaConfig["lacunarity"].get<std::string_view>() << std::endl;
//	std::cout << "luaConfig[\"persistence\"] " << luaConfig["persistence"].get<std::string_view>() << std::endl;
//	std::cout << "luaConfig[\"seed\"] " << luaConfig["seed"].get<std::string_view>() << std::endl;
//	std::cout << "luaConfig[\"noiseType\"] " << luaConfig["noiseType"].get<std::string_view>() << std::endl;
//		sol::type::table
	config.rootNode = std::move(nodes);
	setConfig(luaConfig);
//	config.size = 1024;
	return std::move(config);
}

//NodeTree SurfaceLuaBinding::getNodeTreeFromLuaScript(std::string_view script) {
////		auto scriptStr = libv::read_file_str_or_throw(filepath);
//	auto env = sol::environment(lua, sol::create, lua.globals());
//	lua.script(script, env);
//	sol::table luaConfig = env["NodeTree"];
//	if (luaConfig.get_type() != sol::type::table) {
//		//TODO: log error
////		return config;
//		throw std::runtime_error(std::string("Expected lua table with name \"Config\" but received: ") + std::string(libv::lua::lua_type_to_string(luaConfig.get_type())));
//	}
//	//getNodeTree(luaConfig);
//
//	setConfig(luaConfig);
//	return config;
//}

} // namespace surface
