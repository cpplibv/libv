// Project: libv.lua, File: src/libv/lua/import.hpp

#pragma once

#include <sol/forward.hpp>
#include <sol/object.hpp>

#include <libv/utility/hash_string.hpp>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

class Importer {
public:
	using SourceLoaderFunc = std::function<std::string(std::string_view)>;

private:
	SourceLoaderFunc fileLoader;
	std::unordered_map<std::string, sol::object, libv::hash_string, std::equal_to<>> loadedModules;

private:
	std::vector<std::string> importStack;

private:
	std::vector<std::string> failureStack;
	bool failureInFlight = false;
	std::exception_ptr failure;

public:
	explicit Importer(SourceLoaderFunc fileLoader) : fileLoader(std::move(fileLoader)) {}
	void bind(sol::state& lua);
	void invalidateCache();

	[[nodiscard]] const std::vector<std::string>& currentStack() const noexcept {
		return importStack;
	}
	[[nodiscard]] const std::vector<std::string>& lastFailureStack() const noexcept {
		return failureStack;
	}

private:
	[[nodiscard]] sol::object auxImport(sol::state_view& lua, std::string_view importName);

public:
	// import (sandbox, cache and return module object)
	[[nodiscard]] sol::object import(sol::state_view& lua, std::string_view importName);
	// require (no sandbox, cache and return module object)
	// [[nodiscard]] sol::object require(sol::state_view& lua, std::string_view importName);
	// include (no sandbox, no cached and return module object)
	// [[nodiscard]] sol::object include(sol::state_view& lua, std::string_view importName);

private:
	// [[nodiscard]] std::string loadSourceFile(std::string_view includePath);
	// [[nodiscard]] sol::object require(sol::state& lua, std::string_view name);
	// [[nodiscard]] sol::object include(sol::state& lua, std::string_view name);
};

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv

// =================================================================================================
// =================================================================================================
// === Mod support and dependecy management ========================================================
//
// TODO P4: libv.lua.import: Once you import a file, your import context could change
//			(example: you import a mod, that import other mods, but the mod might not map everything the same way)
// TODO P4: libv.lua.import: Shadow dev support for mods
// TODO P4: libv.lua.import: The mod information and the Importer shall be independent
//			(but mod loading will use an importer that is configured up by the mod info)
// TODO P4: libv.lua.import: Generate stub files from uploaded mods
//
// IDEA: meta.lua file for each mod that will be loaded first in a special sandbox that will only
//      extract the dependency information
//
// --- my_mod/meta.lua:
//
//name("my_mod")
//version("v1.0")
//
//tags("...")
//image("...")
//description("...")
//
//homepage("...")
//forum("...")
//github("...")
//
//register_dependency("core.vec", "latest", "vec")
//register_dependency("core.vm4", "latest", "vm4")
//register_dependency("mod.joes_ailib", "v2.0", "ai")
//register_dependency("mod.too_many_spies", "", "ext.spy")
//
//register_dependency{
//      name = "mod.too_many_spies"
//      version = "3000"
//      alias = "spy"
//}
//
// --- my_mod/main.lua:
//
//local ai = import("ai")
//--                 ^^ Imports are based on aliases
//local spy_m = import("ext.spy.missions")
//local spy_a = import("ext.spy.agents")
//--                    ^^^^^^^ Imports are based on aliases
//local other = import("another_file_in_my_mod")
//--                    ^^^^^^^^^^^^^^^^^^^^^^ Import is also responsible for including other files in the mod
//-- After that normal lua rules apply, import and dependency management is only there to replace requires
//
//spy_a.assign_mission(ai.choose_at_random_from(spy_m.mission_list))
//
//
//
// In C++ register_dependency in meta.lua would be the include_directories flag (with additional information to allow dependency management and aliasing so scoping works)
// While imports are just basic includes with automatic header guard like caching
//
// Mod (with dependency) management and include_directories are two separate system

// =================================================================================================
// =================================================================================================
// =================================================================================================

//struct DependencyInfo {
//	enum class Type {
//		required,
//		optional,
//		//local/shadow_dev,
//	};
//
//	std::string id;
//	std::string version;
//	Type type = Type::required;
//	std::string map_to = "";
//};
//
//// -------------------------------------------------------------------------------------------------
//
//struct ModMetaInfo {
//	std::string name;
//	std::string version;
//
//	// ...
//	// ...
//	// ...
//
//	std::vector<DependencyInfo> dependencies;
//};
//
//ModMetaInfo mod_meta_executor(const std::string_view mod_meta_script) {
//	sol::state lua;
//	ModMetaInfo info;
//
//	bool name_was_set = false;
//	lua.set_function("name", [&](const std::string_view name) {
//		if (std::exchange(name_was_set, true))
//			mod_dev_visible_warning;
//		info.name = name;
//	});
//
//	bool version_was_set = false;
//	lua.set_function("version", [&](const std::string_view version) {
//		if (std::exchange(version_was_set, true))
//			mod_dev_visible_warning;
//		info.version = version;
//	});
//
//	lua.set_function("register_dependency", [](const std::string_view id, const std::string_view version, const std::string_view alias) {
//		if(id_is_already_registered_as_dependency)
//			mod_dev_visible_warning;
//		info.dependencies.emplace_back(id, version, DependencyInfo::Type::required, alias);
//	});
//
//	{
//		auto env = sol::environment(lua, sol::create, lua.globals());
//		const auto object = lua.script(script_str, env);
//		safe_script;
//		try_catch;
//	}
//
//	if (not name_was_set)
//		mod_dev_visible_or_in_lua_error;
//	if (name_was_set && info.name.size() > 255)
//		mod_dev_visible_or_in_lua_error;
//
//	if (not version_was_set)
//		mod_dev_visible_or_in_lua_error;
//	if (version_was_set && info.version_was_set.size() > 255)
//		mod_dev_visible_or_in_lua_error;
//
//	return info;
//}
