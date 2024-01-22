// Project: libv, File: app/surface/surface/lua_binding.cpp

// hpp
#include <surface/surface/lua_binding.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/container/flat_set.hpp>
#include <libv/lua/convert_color.hpp>
#include <libv/lua/lua.hpp>
#include <libv/lua/to_string.hpp>
#include <libv/math/gradient.hpp>
// pro
#include <surface/surface/config.hpp>
#include <surface/surface/node.hpp>
#include <surface/surface/sprite_mapping.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] sol::table convertTable(const sol::object& object) {
	if (object.get_type() != sol::type::table)
		throw std::runtime_error(fmt::format("Expected a lua table but received a {}", libv::lua::to_string(object.get_type())));
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

[[nodiscard]] inline std::optional<libv::vec3f> convertVec3fOptional(const sol::object& object) {
	if (object.get_type() == sol::type::userdata) {
		if (object.is<libv::vec3f>())
			return object.as<libv::vec3f>();
		else
			return std::nullopt;

	} else if (object.get_type() == sol::type::table) {
		auto table = object.as<sol::table>();
		{
			auto n1 = table.get<sol::object>(1);
			auto n2 = table.get<sol::object>(2);
			auto n3 = table.get<sol::object>(3);
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number)
				return libv::vec3f(n1.as<float>(), n2.as<float>(), n3.as<float>());
		}
		{
			auto n1 = table.get<sol::object>("r");
			auto n2 = table.get<sol::object>("g");
			auto n3 = table.get<sol::object>("b");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number)
				return libv::vec3f(n1.as<float>(), n2.as<float>(), n3.as<float>());
		}
		{
			auto n1 = table.get<sol::object>("x");
			auto n2 = table.get<sol::object>("y");
			auto n3 = table.get<sol::object>("z");
			if (n1.get_type() == sol::type::number && n2.get_type() == sol::type::number && n3.get_type() == sol::type::number)
				return libv::vec3f(n1.as<float>(), n2.as<float>(), n3.as<float>());
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
template <typename T, typename ConvertFn, typename... Args>
[[nodiscard]] std::vector<T> convertArray(const sol::object& object, ConvertFn convert, Args&&... args) {
	std::vector<T> result;
	const auto table = convertTable(object);
	const auto size = table.size();
	result.reserve(size);
	for (size_t i = 1; i <= size; ++i) {
		const auto value = table.get<sol::object>(i);
		result.emplace_back(convert(value, args...));
	}
	return result;
}

template <typename T, typename ConvertFn, typename... Args>
[[nodiscard]] libv::flat_set<T> convertArraySet(const sol::object& object, ConvertFn convert, Args&&... args) {
	libv::flat_set<T> result;
	const auto table = convertTable(object);
	const auto size = table.size();
	result.reserve(size);
	for (size_t i = 1; i <= size; ++i) {
		const auto value = table.get<sol::object>(i);
		result.emplace(convert(value, args...));
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
			throw std::runtime_error("Key of color gradient has to be a number " + std::string(libv::lua::to_string(key.get_type())));

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

SurfaceLuaBinding::SurfaceLuaBinding(const std::string& configFolder) {
	libv::lua::open_libraries(lua, libv::lualib::basic | libv::lualib::std_package | libv::lualib::libv_vec);

	lua["package"]["path"] = lua["package"]["path"].get<std::string>() + ";" + configFolder + "?.lua";
}

Seed SurfaceLuaBinding::convertSeed(const sol::object& object, Seed seedOffset) {
	if (object.get_type() != sol::type::number)
		throw std::runtime_error(fmt::format("Seed has to be a number, instead it was {}", libv::lua::to_string(object.get_type())));

	return object.as<Seed>() + seedOffset;
}

std::unique_ptr<Node> SurfaceLuaBinding::convertNodeTree(const sol::object& object, Seed seedOffset) {
	const auto table = convertTable(object);

	std::vector<std::unique_ptr<Node>> children;
	for (const auto& [key, value] : table) {
		if (value.get_type() == sol::type::table)
			if (value.as<sol::table>().get<sol::object>("nodeType").get_type() == sol::type::string)
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

HeatMap SurfaceLuaBinding::convertHeatMap(const sol::object& object, Seed seedOffset) {
	const auto table = convertTable(object);

	HeatMap result;
	result.name = table["name"].get<std::string>();
	result.heightSensitivity = table["heightSensitivity"].get_or<float, float>(1.f);
//	result.colorGrad = convertColorGradient(table.get<sol::object>("colorGrad"));
	result.rootNode = convertNodeTree(table.get<sol::object>("nodes"), seedOffset);

	return result;
}

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

VeggieType SurfaceLuaBinding::convertVeggieType(const sol::object& object, const std::vector<SpriteMappingEntry>& spriteMapping) {
	const auto table = convertTable(object);

	VeggieType result;
	result.name = table["name"].get<std::string>();

	const auto model = table.get<std::string_view>("model");
	const auto mapping = libv::linear_find_optional(spriteMapping, model, &SpriteMappingEntry::name);
	result.modelID = mapping ? mapping->id : 0;

	result.probability = table["probability"] != sol::type::number ? 1.f : table["probability"];
	result.scale = table["scale"] == sol::type::nil ? Range<float>{0.5f, 2.0f} : convertRange<float>(table.get<sol::object>("scale"));
	result.hue = table["hue"] == sol::type::nil ? Shift<float>{0.f, 1.f} : convertShift<float>(table.get<sol::object>("hue"));
	result.saturation = table["saturation"] == sol::type::nil ? Shift<float>{0.0f, 0.1f} : convertShift<float>(table.get<sol::object>("saturation"));
	result.value = table["value"] == sol::type::nil ? Shift<float>{0.0f, 0.1f} : convertShift<float>(table.get<sol::object>("value"));
	return result;
}

std::vector<VeggieType> SurfaceLuaBinding::convertVeggieTypes(const sol::object& object, const std::vector<SpriteMappingEntry>& spriteMapping) {
	return convertArray<VeggieType>(object, convertVeggieType, spriteMapping);
}

Biome SurfaceLuaBinding::convertBiome(const sol::object& object, const std::vector<SpriteMappingEntry>& spriteMapping) {
	const auto table = convertTable(object);

	Biome result;
	result.name = table["name"].get<std::string>();;
	result.coord = table["coord"];
	result.handover = table["handover"] == sol::type::nil ? Limit<float>{0.35f, 0.25f} : convertLimit<float>(table.get<sol::object>("handover"));
	result.takeover = table["takeover"] == sol::type::nil ? Limit<float>{0.45f, 0.6f} : convertLimit<float>(table.get<sol::object>("takeover"));
	result.dominance = table["dominance"] != sol::type::number ? 1.f : table["dominance"];
	result.colorGrad = convertColorGradient(table.get<sol::object>("colorGrad"));
	result.vegetation = convertVeggieTypes(table.get<sol::object>("vegetation"), spriteMapping);

	return result;
}

libv::flat_set<Biome> SurfaceLuaBinding::convertBiomes(const sol::object& object, const std::vector<SpriteMappingEntry>& spriteMapping) {
	return convertArraySet<Biome>(object, convertBiome, spriteMapping);
}

std::shared_ptr<Config> SurfaceLuaBinding::convertConfig(const sol::object& object) {
	const auto table = convertTable(object);

	auto result = std::make_shared<Config>();
	result->globalSeed = table["seed"]; // Global seed is optional
	result->resolution = table["resolution"];
//	result->numChunks = table["numChunks"];
	result->numVeggie = table["numVeggie"];
//	result->plantDistribution = table["plantDistribution"];

	const auto skyboxTextureObj = table.get<sol::object>("skyboxTexture");
	result->skyboxTexture = skyboxTextureObj.get_type() == sol::type::string ?
			skyboxTextureObj.as<std::string_view>() :
			"cube_fluffball.dds";

	const auto fogColorOpt = libv::lua::convert_color(table.get<sol::object>("fogColor"));
	result->fogColor = fogColorOpt ? *fogColorOpt : libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f};

	result->fogIntensity = table["fogIntensity"].get_or(0.05f);

	const auto sunColorOpt = libv::lua::convert_color(table.get<sol::object>("sunColor"));
	result->sunColor = sunColorOpt ? xyz(*sunColorOpt) : libv::vec3f{1.0f, 1.0f, 1.0f};

	const auto sunDirectionOpt = convertVec3fOptional(table.get<sol::object>("sunDirection"));
	result->sunDirection = sunDirectionOpt ? sunDirectionOpt->normalize_copy() : libv::vec3f{0.784464f, 0.196116f, 0.588348f};

	return result;
}

std::shared_ptr<Config> SurfaceLuaBinding::getConfigFromLuaScript(const std::string_view script, const std::vector<SpriteMappingEntry>& spriteMapping) {
	const auto env = sol::environment(lua, sol::create, lua.globals());
	lua.script(script, env);

	//set config
	sol::object luaConfig = env.get<sol::object>("config");
	auto result = convertConfig(luaConfig);

	result->height = convertHeatMap(env.get<sol::object>("height"), result->globalSeed);
	result->temperature = convertHeatMap(env.get<sol::object>("temperature"), result->globalSeed);
	result->humidity = convertHeatMap(env.get<sol::object>("humidity"), result->globalSeed);
	result->fertility = convertHeatMap(env.get<sol::object>("fertility"), result->globalSeed);
	result->biomes = convertBiomes(env.get<sol::object>("biomes"), spriteMapping);
//	extendBiomes(result.biomes);

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
