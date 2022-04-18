// Created by dbobula on 2/20/2022.

//libv
#include <libv/lua/sol_type_to_string.hpp>
#include <libv/lua/convert_color.hpp>
//space
#include <space/surface/lua_binding.hpp>
//pro
#include <iostream>
#include <fmt/format.h>


namespace surface {

[[nodiscard]] sol::table convertTable(const sol::object& object) {
	if (object.get_type() != sol::type::table)
		throw std::runtime_error(fmt::format("Expected a lua table but received a {}", libv::lua::lua_type_to_string(object.get_type())));
	return object.as<sol::table>();
}

[[nodiscard]] libv::vec4f convertColor(const sol::object& object) {
	const auto color = libv::lua::convert_color(object);
	if (!color.has_value())
		throw std::runtime_error("Color given in config couldn't be converted");
	return color.value();
}

//TODO: template container
template <typename T, typename ConvertFn>
[[nodiscard]] std::vector<T> convertArray(const sol::object& object, ConvertFn convert) {
	std::vector<T> result;
	const auto table = convertTable(object);
	const auto size = table.size();
	result.reserve(size);
	for (size_t i = 1; i <= size; ++i) {
		const auto value = table[i];
		result.emplace_back(convert(value));
	}
	return result;
}

[[nodiscard]] libv::gradientf<libv::vec4f> convertColorGradient(const sol::object& object) {
	libv::gradientf<libv::vec4f> colorGrad;
	const auto table = convertTable(object);
	for (const auto&[_, keyValue] : table) {
		if (keyValue.get_type() != sol::type::table || keyValue.as<sol::table>().size() != 2)
			throw std::runtime_error("Invalid colorGrad key-value pair");

		const auto key = keyValue.as<sol::table>()[1];
		const auto value = keyValue.as<sol::table>()[2];
		if (key.get_type() != sol::type::number)
			throw std::runtime_error("Key of color gradient has to be a number " + std::string(libv::lua::lua_type_to_string(key.get_type())));

		const auto color = convertColor(value);
		colorGrad.add(key.get<float>(), color);
	}
	return colorGrad;
}

template <typename T>
void setFractalConfig(T& node, const sol::object& object) {
	const auto table = convertTable(object);
	node.seed = table["seed"];
	node.octaves = table["octaves"];
	node.amplitude = table["amplitude"];
	node.frequency = table["frequency"];
	node.lacunarity = table["lacunarity"];
	node.persistence = table["persistence"];
//	config.noiseType = luaConfig["noiseType"];
}

// -------------------------------------------------------------------------------------------------

SurfaceLuaBinding::SurfaceLuaBinding() {
	lua.open_libraries(sol::lib::base);
	lua.open_libraries(sol::lib::table);
	lua.open_libraries(sol::lib::string);
	lua.open_libraries(sol::lib::math);
}

std::unique_ptr<Node> SurfaceLuaBinding::convertNodeTree(const sol::object& object, int depth) {
	const auto table = convertTable(object);
	std::vector<std::unique_ptr<Node>> children;
	for (const auto&[key, value] : table) {
//		std::cout << std::string(depth, '\t');
		if (value.get_type() == sol::type::table) {
//			std::cout << key.as<std::string_view>() << ":" << std::endl;
			children.emplace_back(convertNodeTree(value, depth + 1));
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
	} else if (nodeType == "cellular") {
		auto node = std::make_unique<NodeCellular>();
		node->seed = table["seed"];
		node->distanceFn = table["distanceFn"];
		node->returnType = table["returnType"];
		node->jitter = table["jitter"];
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


SurfaceObject SurfaceLuaBinding::convertSurfaceObject(const sol::object& object) {
	const auto table = convertTable(object);

	SurfaceObject result;
	result.size = table["size"];
	result.count = table["count"];
	result.color = convertColor(table["color"]);

	return result;
}

HeatMap SurfaceLuaBinding::convertHeatMap(const sol::object& object) {
	const auto table = convertTable(object);

	HeatMap result;
	result.name = table["name"];
	result.heightSensitivity = table["heightSensitivity"];
	result.colorGrad = convertColorGradient(table["colorGrad"]);
	result.rootNode = convertNodeTree(table["nodes"]);
	std::cout << "result.name: " << result.name << std::endl;

	return result;
}

std::vector<SurfaceObject> SurfaceLuaBinding::convertSurfaceObjects(const sol::object& object) {
	return convertArray<SurfaceObject>(object, convertSurfaceObject);
}

//std::vector<HeatMap> SurfaceLuaBinding::convertHeatMaps(const sol::object& object) {
//	return convertArray<HeatMap>(object, convertHeatMap);
//}

Config SurfaceLuaBinding::convertConfig(const sol::object& object) {
	const auto table = convertTable(object);

	Config result;
	result.visualization = table["visualization"];
//	result.currentHeatMap = table["currentHeatMap"];
	result.resolution = table["resolution"];
	result.numChunks = table["numChunks"];
	result.amplitude = table["amplitude"];
	result.plantDistribution = table["plantDistribution"];
	result.circleNumber = table["circleNumber"];
	result.circleSize = table["circleSize"];
	result.objects = convertSurfaceObjects(table["objects"]);

	return result;
}

Config SurfaceLuaBinding::getConfigFromLuaScript(const std::string_view script) {
	const auto env = sol::environment(lua, sol::create, lua.globals());
	lua.script(script, env);

	//set config
	sol::object luaConfig = env["config"];
	auto result = convertConfig(luaConfig);

	//set rootNode
//	{
//		sol::object luaNodes = env["nodes"];
//		auto nodes = convertNodeTree(luaNodes);
//		result.rootNode = std::move(nodes);
//	}

//	//set heatMaps
//	{
//		sol::object luaHeatMaps = env["heatMaps"];
//		auto nodes = convertHeatMaps(luaHeatMaps);
//		result.heatMaps = std::move(nodes);
//	}
	result.height = convertHeatMap(env["height"]);
	result.temperature = convertHeatMap(env["temperature"]);
	result.humidity = convertHeatMap(env["humidity"]);
	result.fertility = convertHeatMap(env["fertility"]);


	return std::move(result);
}
// -------------------------------------------------------------------------------------------------

} // namespace surface
