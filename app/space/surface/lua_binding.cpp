// Created by dbobula on 2/20/2022.

#include <space/surface/lua_binding.hpp>

#include <iostream>
#include <libv/lua/sol_type_to_string.hpp>
#include <libv/lua/convert_color.hpp>


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

libv::vec4f convertColor(const sol::object& solColor) {
	const auto color = libv::lua::convert_color(solColor);
	if (!color.has_value())
		throw std::runtime_error("Color given in config couldn't be converted");
	return color.value();
}

SurfaceObject convertSurfaceObject(const sol::table& solSurfaceObject) {
	SurfaceObject result;
	result.size = solSurfaceObject["size"];
	result.count = solSurfaceObject["count"];
	result.color = convertColor(solSurfaceObject["color"]);
	return result;
}

std::vector<SurfaceObject> convertSurfaceObjects(const sol::table& solSurfaceObjects) {
	std::vector<SurfaceObject> result;
	result.reserve(solSurfaceObjects.size());
	for (const auto &[key, value] : solSurfaceObjects) {
		if (value.get_type() != sol::type::table)
			throw std::runtime_error("Lua type has to be table");
		result.emplace_back(convertSurfaceObject(value.as<sol::table>()));
	}
	return result;
}

void SurfaceLuaBinding::setConfig(const sol::table& luaConfig) {
	config.colorGrad = convertColorGradient(luaConfig["colorGrad"]);
	config.mode = luaConfig["mode"];
	config.visualization = luaConfig["visualization"];
	config.resolution = luaConfig["resolution"];
	config.numChunks = luaConfig["numChunks"];
	config.amplitude = luaConfig["amplitude"];
	config.plantDistribution = luaConfig["plantDistribution"];
	config.circleNumber = luaConfig["circleNumber"];
	config.circleSize = luaConfig["circleSize"];
	config.objects = convertSurfaceObjects(luaConfig["objects"]);
}

libv::gradientf<libv::vec4f> SurfaceLuaBinding::convertColorGradient(const sol::table& table) {
	libv::gradientf<libv::vec4f> colorGrad;
	for (const auto&[_, keyValue] : table) {
		if (keyValue.get_type() != sol::type::table || keyValue.as<sol::table>().size() != 2)
			throw std::runtime_error("Invalid colorGrad key-value pair");

//		colorGrad.add(12.4f, libv::lua::convert_color(std::string{"magenta"}));
//if(miniTable. != sol::type::table)

		const auto key = keyValue.as<sol::table>()[1];
		const auto value = keyValue.as<sol::table>()[2];
		if (key.get_type() != sol::type::number)
			throw std::runtime_error("Key of color gradient has to be a number " + std::string(libv::lua::lua_type_to_string(key.get_type())));
		const auto color = convertColor(value);

//		std::cout << key.get<std::string_view>() << " = " << value.get<std::string_view>() << std::endl;

		colorGrad.add(key.get<float>(), color);
	}
	return colorGrad;
}

std::unique_ptr<Node> SurfaceLuaBinding::getNodeTree(const sol::table& table, int depth) {
	std::vector<std::unique_ptr<Node>> children;
	for (const auto&[key, value] : table) {
//		std::cout << std::string(depth, '\t');
		if (value.get_type() == sol::type::table) {
//			std::cout << key.as<std::string_view>() << ":" << std::endl;
			children.emplace_back(getNodeTree(value, depth + 1));
		} else {
//			std::cout << key.as<std::string_view>() << " = " << value.as<std::string_view>() << std::endl;
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
	} else if (nodeType == "perlin") {
		auto node = std::make_unique<NodePerlin>();
		node->seed = table["seed"];
		return node;
	} else if (nodeType == "simplex") {
		auto node = std::make_unique<NodeSimplex>();
		node->seed = table["seed"];
		return node;
	} else if (nodeType == "simplexFractal") {
		auto node = std::make_unique<NodeSimplexFractal>();
		setFractalConfig(*node, table);
		return node;
	} else if (nodeType == "warp") {
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

Config SurfaceLuaBinding::getConfigFromLuaScript(std::string_view script) {
	auto env = sol::environment(lua, sol::create, lua.globals());
	lua.script(script, env);
	sol::table luaNodes = env["nodes"];
	sol::table luaConfig = env["config"];
	if (luaNodes.get_type() != sol::type::table)
		throw std::runtime_error(std::string("Expected lua table with name \"Config\" but received: ") + std::string(libv::lua::lua_type_to_string(luaNodes.get_type())));
	auto nodes = getNodeTree(luaNodes);
	config.rootNode = std::move(nodes);
	setConfig(luaConfig);
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
