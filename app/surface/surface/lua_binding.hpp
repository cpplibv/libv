// Project: libv, File: app/surface/surface/lua_binding.hpp

#pragma once

// ext
#include <sol/sol.hpp>
// libv
#include <libv/container/flat_set_fwd.hpp>
// std
#include <memory>
// pro
#include <surface/surface/fwd.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

using Seed = uint32_t;

// -------------------------------------------------------------------------------------------------

class SurfaceLuaBinding {
private:
	sol::state lua;

public:
	SurfaceLuaBinding(const std::string& configFolder);
	[[nodiscard]] std::shared_ptr<Config> getConfigFromLuaScript(const std::string_view script, const std::vector<SpriteMappingEntry>& spriteMapping);

private:
	static Seed convertSeed(const sol::object& object, Seed seedOffset);
	static std::unique_ptr<Node> convertNodeTree(const sol::object& object, Seed seedOffset);
	static HeatMap convertHeatMap(const sol::object& object, Seed seedOffset);
	static Biome convertBiome(const sol::object& object, const std::vector<SpriteMappingEntry>& spriteMapping);
	static libv::flat_set<Biome> convertBiomes(const sol::object& object, const std::vector<SpriteMappingEntry>& spriteMapping);
	static VeggieType convertVeggieType(const sol::object& object, const std::vector<SpriteMappingEntry>& spriteMapping);
	static std::vector<VeggieType> convertVeggieTypes(const sol::object& object, const std::vector<SpriteMappingEntry>& spriteMapping);

	static std::shared_ptr<Config> convertConfig(const sol::object& object);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
