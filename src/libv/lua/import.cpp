// Project: libv.lua, File: src/libv/lua/_idea_import.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/lua/import.hpp>
// libv
#include <libv/algo/slice.hpp>
// ext
//#include <fmt/format.h>
#include <sol/state.hpp>
//#include <sol/usertype.hpp>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

void Importer::map(std::string prefix, std::string directory_path) {
	const auto loader = [](const std::string_view path) {
		//		const auto script_str = libv::read_file_str_or_throw(path);
		//		try_catch;
		return LoadResult{false, "Not Implemented Yet"};
	};
	import_mapping.emplace_back(std::move(prefix), loader);
}

void Importer::map(std::string prefix, SourceLoaderFunc loader) {
	import_mapping.emplace_back(std::move(prefix), std::move(loader));
}

//		lua.set_function("import", [this, &lua](const std::string_view name, sol::table identifiers) {
//          check if identifiers is a string list
//          load name normally
//          if the loaded/returned object is a table
//              remove any element that is not in the identifiers list
//          return the loaded/returned object

void Importer::bind(sol::state& lua) {
	lua.set_function("import", [this, &lua](const std::string_view name) {
		const auto emplace_result = loaded_modules.emplace(name, sol::nil);

		auto& loaded_object = emplace_result.first->second;
		const auto new_lookup = emplace_result.second;

		if (!new_lookup)
			return loaded_object;

//		log_new_lookup;

		for (const auto& mapping : import_mapping) {
			if (!name.starts_with(mapping.prefix))
				continue;

//			const auto pure_name = libv::slice_off_view(name, mapping.prefix.size());
			const auto pure_name = libv::slice_off_view(name, mapping.prefix.size());
//			const auto path = std::filesystem::path(mapping.directory_path) / name + ".lua";
//			note_import_graph;
//
//          detect_and_abort_cycle?
//
//			const auto loader_result = mapping.loader(path);
			const auto loader_result = mapping.loader(pure_name);

//			if (!loader_result.success) {
//				generate_lua_error_on_failure;
//			}

			const auto script_str = loader_result.result;
//			try_catch;
//
//			{
				auto env = sol::environment(lua, sol::create, lua.globals());
				auto object = lua.script(script_str, env);
//				safe_script;
//				try_catch;
//				generate_lua_error_on_failure;
//			}

			loaded_object = std::move(object);
		}

//		if_no_mapping_matches_log_and_lua_error;
//
//		if (loaded_object.get_type() == sol::nil) {
//			generate_lua_error_on_import_not_found;
//		}

		return loaded_object;
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
