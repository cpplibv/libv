// Project: libv.lua, File: src/libv/lua/import.hpp

#pragma once

// ext
#include <sol/forward.hpp>
#include <sol/object.hpp>
// std
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

class Importer {
public:
	struct [[nodiscard]] LoadResult {
		bool success;
		std::string result; /// Error reason or include payload
	};

	using SourceLoaderFunc = std::function<LoadResult(const std::string_view)>;

	struct ImportRuleEntry {
		std::string prefix;
		SourceLoaderFunc loader;
//		std::string directory_path;
	};

private:
	std::vector<ImportRuleEntry> import_mapping;
	std::unordered_map<std::string, sol::object> loaded_modules;

public:
	void map(std::string prefix, std::string directory_path);
	void map(std::string prefix, SourceLoaderFunc loader);
	void bind(sol::state& lua);
};

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv

// =================================================================================================
// =================================================================================================
// =================================================================================================

// TODO P2: libv.lua.import: DESIGN ISSUE: Once you import a file, your import context could change (example: you import a mod, that import other mods, but the mod might not map everything the same way you do)
// TODO P4: libv.lua.import: Shadow dev support for mods

// TODO P4: libv.lua.import: Disable built-in require | OR replace it
// TODO P4: libv.lua.import: The mod information and the Importer shall be independent
//              (but mod loading will use an importer that is configured up by the mod info)
// TODO P4: libv.lua.import: Generate stub files from uploaded mods

// Use a separate Importer that can be bound to a State
//		sol::state lua;
//		libv::lua::Importer importer;
//		importer.bind(lua);
//
// Or just improve / create a State wrapper in libv::lua
//		libv::lua::State lua;
//		lua.map_import("module", folder);

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
//
// --- my_mod/main.lua:
//
//local ai = import("ai")
//--                 ^^ Imports are based on aliases
//local spy_m = import("ext.spy.missions")
//local spy_a = import("ext.spy.agents")
//--                    ^^^^^^^ Imports are based on aliases
//local other = import("another_file_in_my_mod")
//                      ^^^^^^^^^^^^^^^^^^^^^^ Import is also responsible for including other files in the mod
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
