// Created by dbobula on 2/20/2022.

#pragma once

#include <sol/sol.hpp>
#include <space/surface/config.hpp>
#include <space/surface/node.hpp>


namespace surface {

class SurfaceLuaBinding {
private:
	sol::state lua;
	Config config;

	void setConfig(const sol::table& luaConfig);
//	void setFractalConfig(const sol::table& luaConfig);

public:
	std::unique_ptr<Node> getNodeTree(const sol::table& table, int depth = 0);
	SurfaceLuaBinding();
	Config getConfigFromLuaScript(std::string_view script);
//	NodeTree getNodeTreeFromLuaScript(std::string_view script);
};

} // namespace surface

