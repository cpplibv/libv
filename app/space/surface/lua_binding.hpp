// Created by dbobula on 2/20/2022.

#pragma once

#include <sol/sol.hpp>
#include <space/surface/config.hpp>

namespace surface {

class SurfaceLuaBinding {
private:
	sol::state lua;
	Config config;

	void traverseTable(sol::table table, int depth = 0);
	void setConfig(const sol::table& luaConfig);

		public:
	SurfaceLuaBinding();
	Config getConfigFromLuaScript(std::string_view script);
};

} // namespace surface

