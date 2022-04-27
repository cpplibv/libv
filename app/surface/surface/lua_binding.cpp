// Created by dbobula on 2/20/2022.

// hpp
#include <surface/surface/lua_binding.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/lua/convert_color.hpp>
#include <libv/lua/sol_type_to_string.hpp>
#include <libv/math/gradient.hpp>
// pro
#include <surface/surface/config.hpp>
#include <surface/surface/node.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

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
		const auto value = table.get<sol::object>(i);
		result.emplace_back(convert(value));
	}
	return result;
}

template <typename T, typename ConvertFn>
[[nodiscard]] libv::flat_set<T> convertArraySet(const sol::object& object, ConvertFn convert) {
	libv::flat_set<T> result;
	const auto table = convertTable(object);
	const auto size = table.size();
	result.reserve(size);
	for (size_t i = 1; i <= size; ++i) {
		const auto value = table.get<sol::object>(i);
		result.emplace(convert(value));
	}
	return result;
}

[[nodiscard]] libv::gradientf<libv::vec4f> convertColorGradient(const sol::object& object) {
	libv::gradientf<libv::vec4f> colorGrad;
	const auto table = convertTable(object);
	for (const auto&[_, keyValue] : table) {
		const auto keyValueTable = convertTable(keyValue);
//		if (keyValue.get_type() != sol::type::table || keyValue.as<sol::table>().size() != 2)
//			throw std::runtime_error("Invalid colorGrad key-value pair");

		const auto key = keyValueTable.get<sol::object>(1);
		const auto value = keyValueTable.get<sol::object>(2);

		if (key.get_type() != sol::type::number)
			throw std::runtime_error("Key of color gradient has to be a number " + std::string(libv::lua::lua_type_to_string(key.get_type())));

		const auto color = convertColor(value);
		colorGrad.add(key.as<float>(), color);
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
	libv::lua::open_libraries(lua, libv::lua::lualib::vec);
}

std::unique_ptr<Node> SurfaceLuaBinding::convertNodeTree(const sol::object& object, int depth) {
	const auto table = convertTable(object);

	std::vector<std::unique_ptr<Node>> children;
	for (const auto&[key, value] : table) {
		if (value.get_type() == sol::type::table)
			children.emplace_back(convertNodeTree(value, depth + 1));
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

		for (auto& child : children)
			x.emplace_back(std::move(child));

		return std::make_unique<NodePlus>(std::move(x));

	} else if (nodeType == "mul") {
		auto x = NodeMul::container();

		for (auto& child : children)
			x.emplace_back(std::move(child));

		return std::make_unique<NodeMul>(std::move(x));

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
	result.color = convertColor(table.get<sol::object>("color"));

	return result;
}

HeatMap SurfaceLuaBinding::convertHeatMap(const sol::object& object) {
	const auto table = convertTable(object);

	HeatMap result;
	result.name = table["name"];
	result.heightSensitivity = table["heightSensitivity"];
	result.colorGrad = convertColorGradient(table.get<sol::object>("colorGrad"));
	result.rootNode = convertNodeTree(table.get<sol::object>("nodes"));

	return result;
}

std::vector<SurfaceObject> SurfaceLuaBinding::convertSurfaceObjects(const sol::object& object) {
	return convertArray<SurfaceObject>(object, convertSurfaceObject);
}

VeggieType SurfaceLuaBinding::convertVeggieType(const sol::object& object) {
	const auto table = convertTable(object);

	VeggieType result;
	result.name = table["name"];
	result.size = table["size"];
	result.color = convertColor(table.get<sol::object>("color"));
	result.probability = table["probability"] != sol::type::number ? 1.f : table["probability"];
//	std::cout << "result.name: " << result.name << std::endl;
//	std::cout << "result.name: " << result.size << std::endl;
//	std::cout << "result.name: " << result.color << std::endl;
	return result;
}

std::vector<VeggieType> SurfaceLuaBinding::convertVeggieTypes(const sol::object& object) {
	return convertArray<VeggieType>(object, convertVeggieType);
}

Biome SurfaceLuaBinding::convertBiome(const sol::object& object) {
	const auto table = convertTable(object);

	Biome result;
	result.name = table["name"];
	result.coord = table["coord"];
	result.cutOff = table["cutOff"];
	result.dominance = table["dominance"] != sol::type::number ? 1.f : table["dominance"];
	result.colorGrad = convertColorGradient(table.get<sol::object>("colorGrad"));
	result.vegetation = convertVeggieTypes(table.get<sol::object>("vegetation"));

	return result;
}

libv::flat_set<Biome> SurfaceLuaBinding::convertBiomes(const sol::object& object) {
	return convertArraySet<Biome>(object, convertBiome);
}

std::shared_ptr<Config> SurfaceLuaBinding::convertConfig(const sol::object& object) {
	const auto table = convertTable(object);

	auto result = std::make_shared<Config>();
	result->visualization = table["visualization"];
//	result->currentScene = table["currentScene"];
	result->resolution = table["resolution"];
	result->numChunks = table["numChunks"];
	result->numVeggie = table["numVeggie"];
	result->amplitude = table["amplitude"];
	result->plantDistribution = table["plantDistribution"];
	result->circleNumber = table["circleNumber"];
	result->circleSize = table["circleSize"];
	result->objects = convertSurfaceObjects(table.get<sol::object>("objects"));

	return result;
}

//Biome emptyBiome() {
//	Biome result;
//	result.name = "empty";
//	result.coord = {0, 0};
////	result.cutOff = table["cutOff"];
//	result.dominance = 0.f;
//	libv::gradientf<libv::vec4f> colorGrad;
//	colorGrad.add(0.f, libv::vec4f{1, 1, 1, 1});
//	colorGrad.add(1.f, libv::vec4f{0, 0, 0, 1});
//
//	result.colorGrad = colorGrad;
////	result.vegetation = convertVeggieTypes(table["vegetation"]);
//
//	return result;
//}

//void extendBiomes(libv::flat_set<Biome> biomes) {
//	auto size = biomes.size();
//	while (size < 5) {
//		biomes.insert(emptyBiome());
//		size++;
//	}
////	return biomes;
//}

std::shared_ptr<Config> SurfaceLuaBinding::getConfigFromLuaScript(const std::string_view script) {
	const auto env = sol::environment(lua, sol::create, lua.globals());
	lua.script(script, env);

	//set config
	sol::object luaConfig = env.get<sol::object>("config");
	auto result = convertConfig(luaConfig);

	result->height = convertHeatMap(env.get<sol::object>("height"));
	result->temperature = convertHeatMap(env.get<sol::object>("temperature"));
	result->humidity = convertHeatMap(env.get<sol::object>("humidity"));
	result->fertility = convertHeatMap(env.get<sol::object>("fertility"));
	result->biomes = convertBiomes(env.get<sol::object>("biomes"));
//	extendBiomes(result.biomes);

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
