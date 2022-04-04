// Created by dbobula on 2/20/2022.

#pragma once

//ext
#include <sol/sol.hpp>

#include <space/surface/config.hpp>
#include <space/surface/node.hpp>

#include <libv/container/flat_set.hpp>


namespace surface {

class SurfaceLuaBinding {
private:
	sol::state lua;
//	Config config;

	Config convertConfig(const sol::object& object);
//	void setFractalConfig(const sol::table& luaConfig);

	static std::unique_ptr<Node> convertNodeTree(const sol::object& object, int depth = 0);
	static SurfaceObject convertSurfaceObject(const sol::object& object);
	std::vector<SurfaceObject> convertSurfaceObjects(const sol::object& object);
	static HeatMap convertHeatMap(const sol::object& object);
//	std::vector<HeatMap> convertHeatMaps(const sol::object& object);
	static Biome convertBiome(const sol::object& object);
	static libv::flat_set<Biome> convertBiomes(const sol::object& object);

public:
	SurfaceLuaBinding();
	Config getConfigFromLuaScript(const std::string_view script);
//	NodeTree getNodeTreeFromLuaScript(std::string_view script);
};

} // namespace surface

