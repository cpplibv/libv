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

[[nodiscard]] inline std::optional<libv::vec2f> convertVec2fOptional(const sol::object& object) {
	if (object.get_type() == sol::type::userdata) {
		if (object.is<libv::vec2f>())
			return object.as<libv::vec2f>();
		else
			return std::nullopt;

	} else if (object.get_type() == sol::type::table) {
		auto table = object.as<sol::table>();
		{
			auto n1 = table.get<sol::object>(1);
			auto n2 = table.get<sol::object>(2);
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number)
				return libv::vec2f(n1.as<float>(), n2.as<float>());
		}
		{
			auto n1 = table.get<sol::object>("r");
			auto n2 = table.get<sol::object>("g");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number)
				return libv::vec2f(n1.as<float>(), n2.as<float>());
		}
		{
			auto n1 = table.get<sol::object>("x");
			auto n2 = table.get<sol::object>("y");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number)
				return libv::vec2f(n1.as<float>(), n2.as<float>());
		}
		{
			auto n1 = table.get<sol::object>("u");
			auto n2 = table.get<sol::object>("v");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number)
				return libv::vec2f(n1.as<float>(), n2.as<float>());
		}

		return std::nullopt;

	} else {
		return std::nullopt;
	}
}

[[nodiscard]] inline libv::vec2f convertVec2f(const sol::object& object) {
	auto result = convertVec2fOptional(object);

	if (!result)
		throw std::runtime_error("Vec2f given in config couldn't be converted");

	return *result;
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
	lua.open_libraries(sol::lib::package);
	libv::lua::open_libraries(lua, libv::lua::lualib::vec);

}

Seed SurfaceLuaBinding::convertSeed(const sol::object& object, Seed seedOffset) {
	if (object.get_type() != sol::type::number)
		throw std::runtime_error(fmt::format("Seed has to be a number, instead it was {}", libv::lua::lua_type_to_string(object.get_type())));

	return object.as<Seed>() + seedOffset;
}

std::unique_ptr<Node> SurfaceLuaBinding::convertNodeTree(const sol::object& object, Seed seedOffset) {
	const auto table = convertTable(object);

	std::vector<std::unique_ptr<Node>> children;
	for (const auto&[key, value] : table) {
		if (value.get_type() == sol::type::table)
			children.emplace_back(convertNodeTree(value, seedOffset));
	}

	auto solNodeType = table["nodeType"];
	if (solNodeType != sol::type::string)
		throw std::runtime_error("Valid nodeType field missing");
	std::string_view nodeType = solNodeType.get<std::string_view>();

	if (nodeType == "constant") {
		auto node = std::make_unique<NodeConstant>();
		node->value = table["value"];
		return node;

	} else if (nodeType == "value") {
		auto node = std::make_unique<NodeValue>();
		node->seed = convertSeed(table.get<sol::object>("seed"), seedOffset);
		return node;

	} else if (nodeType == "perlin") {
		auto node = std::make_unique<NodePerlin>();
		node->seed = convertSeed(table.get<sol::object>("seed"), seedOffset);
		return node;

	} else if (nodeType == "simplex") {
		auto node = std::make_unique<NodeSimplex>();
		node->seed = convertSeed(table.get<sol::object>("seed"), seedOffset);
		return node;

	} else if (nodeType == "cellular") {
		auto node = std::make_unique<NodeCellular>();
		node->seed = convertSeed(table.get<sol::object>("seed"), seedOffset);
		node->distanceFn = table["distanceFn"];
		node->returnType = table["returnType"];
		node->jitter = table["jitter"];
		return node;

	} else if (nodeType == "simplexFractal") {
		auto node = std::make_unique<NodeSimplexFractal>();
		node->seed = convertSeed(table.get<sol::object>("seed"), seedOffset);
		setFractalConfig(*node, table);
		return node;

	} else if (nodeType == "warp") {
		if (children.size() != 1)
			throw std::runtime_error("Warp can only be applied to size of one nodes");

		auto node = std::make_unique<NodeWarp>(std::move(children[0]));
		node->seed = convertSeed(table.get<sol::object>("seed"), seedOffset);
		setFractalConfig(*node, table);
		return node;

	} else if (nodeType == "coord" || nodeType == "coordinate") {
		auto node = std::make_unique<NodeCoordinate>();
		node->weights = convertVec2f(table.get<sol::object>("weights"));
		return node;

	} else if (nodeType == "fractal") {
		if (children.size() != 1)
			throw std::runtime_error("Fractal can only be applied to size of one nodes");

		auto node = std::make_unique<NodeFractal>(std::move(children[0]));
		node->seed = convertSeed(table.get<sol::object>("seed"), seedOffset);
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

	} else if (nodeType == "mix") {
		if (children.size() != 3)
			throw std::runtime_error("Mix can only be applied to size of three nodes");

		return std::make_unique<NodeMix>(std::move(children[0]), std::move(children[1]), std::move(children[2]));

	} else if (nodeType == "clamp") {
		if (children.size() != 3)
			throw std::runtime_error("Clamp can only be applied to size of three nodes");

		return std::make_unique<NodeClamp>(std::move(children[0]), std::move(children[1]), std::move(children[2]));

	} else if (nodeType == "saturate") {
		if (children.size() != 1)
			throw std::runtime_error("Saturate can only be applied to size of one nodes");

		return std::make_unique<NodeSaturate>(std::move(children[0]));

	} else if (nodeType == "min") {
		if (children.size() != 2)
			throw std::runtime_error("Min can only be applied to size of two nodes");

		return std::make_unique<NodeMin>(std::move(children[0]), std::move(children[1]));

	} else if (nodeType == "max") {
		if (children.size() != 2)
			throw std::runtime_error("Max can only be applied to size of two nodes");

		return std::make_unique<NodeMax>(std::move(children[0]), std::move(children[1]));

	} else if (nodeType == "smoothstep") {
		if (children.size() != 3)
			throw std::runtime_error("Smoothstep can only be applied to size of three nodes");

		return std::make_unique<NodeSmoothstep>(std::move(children[0]), std::move(children[1]), std::move(children[2]));
	} else if (nodeType == "sin") {
		if (children.size() != 1)
			throw std::runtime_error("Sin can only be applied to size of one nodes");

		return std::make_unique<NodeSin>(std::move(children[0]));

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

HeatMap SurfaceLuaBinding::convertHeatMap(const sol::object& object, Seed seedOffset) {
	const auto table = convertTable(object);

	HeatMap result;
	result.name = table["name"];
	result.heightSensitivity = table["heightSensitivity"];
	result.colorGrad = convertColorGradient(table.get<sol::object>("colorGrad"));
	result.rootNode = convertNodeTree(table.get<sol::object>("nodes"), seedOffset);

	return result;
}

//std::vector<SurfaceObject> SurfaceLuaBinding::convertSurfaceObjects(const sol::object& object) {
//	return convertArray<SurfaceObject>(object, convertSurfaceObject);
//}

template <typename T>
Range<T> convertRange(const sol::object& object) {
	const auto table = convertTable(object);

	Range<T> result;
	result.min = table["min"];
	result.max = table["max"];
	return result;
}

template <typename T>
Shift<T> convertShift(const sol::object& object) {
	const auto table = convertTable(object);

	Shift<T> result;
	result.offset = table["offset"];
	result.radius = table["radius"];
	return result;
}

template <typename T>
Limit<T> convertLimit(const sol::object& object) {
	const auto table = convertTable(object);

	Limit<T> result;
	result.soft = table["soft"];
	result.hard = table["hard"];
	return result;
}

VeggieType SurfaceLuaBinding::convertVeggieType(const sol::object& object) {
	const auto table = convertTable(object);

	VeggieType result;
	result.name = table["name"];
	result.probability = table["probability"] != sol::type::number ? 1.f : table["probability"];
	result.path = table["path"];
	result.scale = table["scale"] == sol::type::nil ? Range<float>{0.5f, 2.0f} : convertRange<float>(table.get<sol::object>("scale"));
	result.hue = table["hue"] == sol::type::nil ? Shift<float>{0.f, 1.f} : convertShift<float>(table.get<sol::object>("hue"));
	result.saturation = table["saturation"] == sol::type::nil ? Shift<float>{0.0f, 0.1f} : convertShift<float>(table.get<sol::object>("saturation"));
	result.value = table["value"] == sol::type::nil ? Shift<float>{0.0f, 0.1f} : convertShift<float>(table.get<sol::object>("value"));
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
	result.handover = table["handover"] == sol::type::nil ? Limit<float>{0.35f, 0.25f} : convertLimit<float>(table.get<sol::object>("handover"));
	result.takeover = table["takeover"] == sol::type::nil ? Limit<float>{0.45f, 0.6f} : convertLimit<float>(table.get<sol::object>("takeover"));
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
	result->globalSeed = table["seed"]; // Global seed is optional
//	result->visualization = table["visualization"];
//	result->currentScene = table["currentScene"];
	result->resolution = table["resolution"];
	result->numChunks = table["numChunks"];
	result->numVeggie = table["numVeggie"];
	result->amplitude = table["amplitude"];
//	result->plantDistribution = table["plantDistribution"];
//	result->circleNumber = table["circleNumber"];
//	result->circleSize = table["circleSize"];

	result->fogIntensity = table["fogIntensity"].get_or(0.05f);
	const auto fogColorOpt = libv::lua::convert_color(table.get<sol::object>("fogColor"));
	result->fogColor = fogColorOpt ? *fogColorOpt : libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f};
//	result->objects = convertSurfaceObjects(table.get<sol::object>("objects"));

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

	result->height = convertHeatMap(env.get<sol::object>("height"), result->globalSeed);
	result->temperature = convertHeatMap(env.get<sol::object>("temperature"), result->globalSeed);
	result->humidity = convertHeatMap(env.get<sol::object>("humidity"), result->globalSeed);
	result->fertility = convertHeatMap(env.get<sol::object>("fertility"), result->globalSeed);
	result->biomes = convertBiomes(env.get<sol::object>("biomes"));
//	extendBiomes(result.biomes);

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
