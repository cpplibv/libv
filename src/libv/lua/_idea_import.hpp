// Project: libv.lua, File: src/libv/lua/_idea_import.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <fmt/format.h>
#include <sol/state.hpp>
#include <sol/usertype.hpp>
// std
#include <vector>
// libv
//#include <libv/math/vec.hpp>
#include <libv/algo/slice.hpp>
#include <libv/utility/read_file.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------

// TODO P4: libv.lua.import: Disable built-in require
// TODO P4: libv.lua.import: The mod information and the Importer shall be independent (but mod loading will use an importer that is configured up by the mod info)
// TODO P4: libv.lua.import: Shadow dev support for mods
// TODO P4: libv.lua.import: Generate stub files from uploaded mods

// -------------------------------------------------------------------------------------------------

struct DependencyInfo {
	enum class Type {
		required,
		optional,
		//local/shadow_dev,
	};

	std::string id;
	std::string version;
	Type type = Type::required;
	std::string map_to = "";
};

// -------------------------------------------------------------------------------------------------

struct ModMetaInfo {
	std::string name;
	std::string version;

	// ...
	// ...
	// ...

	std::vector<DependencyInfo> dependencies;
};

ModMetaInfo mod_meta_executor(const std::string_view mod_meta_script) {
	sol::state lua;
	ModMetaInfo info;

	bool name_was_set = false;
	lua.set_function("name", [&](const std::string_view name) {
		if (std::exchange(name_was_set, true))
			mod_dev_visible_warning;
		info.name = name;
	});

	bool version_was_set = false;
	lua.set_function("version", [&](const std::string_view version) {
		if (std::exchange(version_was_set, true))
			mod_dev_visible_warning;
		info.version = version;
	});

	lua.set_function("register_dependency", [](const std::string_view id, const std::string_view version, const std::string_view alias) {
		if(id_is_already_registered_as_dependency)
			mod_dev_visible_warning;
		info.dependencies.emplace_back(id, version, DependencyInfo::Type::required, alias);
	});

	lua.script(mod_meta_script);
	safe_script_try_catch;

	if (not name_was_set)
		mod_dev_visible_or_in_lua_error;
	if (name_was_set && info.name.size() > 255)
		mod_dev_visible_or_in_lua_error;

	if (not version_was_set)
		mod_dev_visible_or_in_lua_error;
	if (version_was_set && info.version_was_set.size() > 255)
		mod_dev_visible_or_in_lua_error;

	return info;
}

// -------------------------------------------------------------------------------------------------
// =================================================================================================
// -------------------------------------------------------------------------------------------------

class Importer {
	struct ImportRuleEntry {
		std::string prefix;
		std::string directory_path;
	};

	std::vector<ImportRuleEntry> import_mapping;
	std::unordered_map<std::string, sol::object> loaded_modules;

public:
	void map(std::string prefix, std::string directory_path) {
		import_mapping.emplace_back(std::move(prefix), std::move(directory_path));
	}

	void bind(sol::state& lua) {
//		lua.set_function("import", [this, &lua](const std::string_view name, sol::table identifiers) {
//          check if identifiers is a string list
//          load name normally
//          if the loaded/returned object is a table
//              remove any element that is not in the identifiers list
//          return the loaded/returned object

		lua.set_function("import", [this, &lua](const std::string_view name) {
			const auto emplace_result = loaded_modules.emplace(name, sol::nil);

			auto& loaded_object = emplace_result.first->second;
			const auto new_lookup = emplace_result.second;

			if (new_lookup) {
				log_new_lookup;

				for (const auto& mapping : import_mapping) {
					if (name.starts_with(mapping.prefix)) {
						const auto pure_name = libv::slice_off_view(name, mapping.prefix.size());
						const auto path = std::filesystem::path(mapping.directory_path) / name + ".lua";
						note_import_graph;

						const auto script_str = libv::read_file_str_or_throw(path);
						try_catch;
						generate_lua_error_on_failure;

						not_sure_but_maybe_sandbox_question_mark;
						const auto object = lua.script(script_str);
						safe_script_try_catch;
						generate_lua_error_on_failure;

						loaded_object = object;
						break;
					}
				}
				if_no_mapping_matches_log_and_lua_error;

				if (loaded_object.get_type() == sol::nil) {
					generate_lua_error_on_import_not_found;
				}
			}

			return loaded_object;
		});
	}
};

void foo() {
	sol::state lua;
	Importer importer;
	importer.map("", current_mod_folder); // Add special case for the current mod's folder
	importer.bind(lua);

	// Or just improve / create a State wrapper in libv::lua
	libv::lua::State lua;
	lua.map_import("", current_mod_folder); // Add special case for the current mod's folder
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
