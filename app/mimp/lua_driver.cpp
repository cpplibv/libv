// Project: libv, File: app/mimp/lua_driver.cpp

// hpp
#include <mimp/lua_driver.hpp>
// libv
#include <libv/lua/lua.hpp>
// std
#include <regex>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4imp/importer.hpp>
#include <libv/vm4imp/log.hpp>


namespace mimp {

// -------------------------------------------------------------------------------------------------

std::string regex_replace(std::string_view subject, std::string_view pattern, std::string_view target) {
	const auto re = std::regex(std::string(pattern));
	const auto t = std::string(target);

	return std::regex_replace(std::string(subject), re, t);
}

// -------------------------------------------------------------------------------------------------

struct LuaContext {
	sol::state& lua;
	libv::vm4::Model& model;
	bool dirtyBounds = false;

	void bind() {
		using namespace libv::vm4;

		{
			auto type = lua.new_usertype<MaterialTexture>("MaterialTexture");
			// TODO P5: Bind 'type' enum values in lua
			type.set("type", &MaterialTexture::type);
			type.set("path", &MaterialTexture::path);
			type.set("name", &MaterialTexture::uvwsrc);
		} {
			auto type = lua.new_usertype<Material>("Material");
			type.set("name", &Material::name);
			type.set("shader", &Material::shader);
			type.set("textures", &Material::textures);
			// TODO P5: property (variant) access
			//		type.set("properties", &Material::properties);
		}

		auto api_table = lua.create_named_table("model");

		api_table.set_function("scale", [this](const float value) mutable {
			model.nodes[0].transformation.scale(libv::vec3f::one(value));
			dirtyBounds = true;
		});

		api_table.set_function("rotate", [this](const float x, const float y, const float z) mutable {
			model.nodes[0].transformation.rotate(libv::deg_to_rad(x), {1, 0, 0});
			model.nodes[0].transformation.rotate(libv::deg_to_rad(y), {0, 1, 0});
			model.nodes[0].transformation.rotate(libv::deg_to_rad(z), {0, 0, 1});
			dirtyBounds = true;
		});

		api_table.set_function("material", [this](const std::string_view name) mutable {
			for (auto& material : model.materials)
				if (material.name == name)
					return sol::make_object(lua, std::ref(material));

			return sol::object(sol::nil);
		});

		api_table.set_function("add_bool", [this](const std::string_view material_name, std::string property_name, bool value) mutable {
			for (auto& material : model.materials)
				if (material.name == material_name)
					material.properties[std::move(property_name)] = value;
		});

//		api_table.set_function("properties", [this](const std::string_view name) mutable {
//			for (auto& material : model.materials)
//				if (material.name == name)
//					return sol::make_object(lua, std::ref(material));
//
//			return sol::object(sol::nil);
//		});

		api_table.set_function("regex_replace_textures", [this](const std::string_view pattern, const std::string_view target) mutable {
			for (auto& material : model.materials)
				for (auto& texture : material.textures)
					texture.path = regex_replace(texture.path, pattern, target);
		});

		api_table.set_function("print_materials", [this]() {
			for (const auto& material : model.materials) {
				libv::vm4::log_vm4.fatal("name: {}", material.name);
				libv::vm4::log_vm4.fatal("    shader: {}", material.shader);
				for (const auto& [key, v] : material.properties)
					std::visit([&](const auto& value) {
						libv::vm4::log_vm4.fatal("    {} = {}", key, value);
					}, v);
				for (const auto& texture : material.textures) {
					libv::vm4::log_vm4.fatal("    texture {} = {}", to_string(texture.type), texture.path);
					libv::vm4::log_vm4.fatal("    texture uvwsrc = {}", texture.uvwsrc);
				}
			}
		});

		api_table.set_function("print_textures", [this]() {
			for (const auto& material : model.materials) {
				for (const auto& texture : material.textures) {
					libv::vm4::log_vm4.fatal("type: {} uvwsrc: {} path: {}", to_string(texture.type), texture.uvwsrc, texture.path);
				}
			}
		});

		api_table.set_function("print_model", [this]() {
			if (dirtyBounds) {
				libv::vm4::recalculateBounding(model);
				dirtyBounds = false;
			}

			libv::vm4::log_vm4.fatal("AABB_max: {}", model.AABB_max);
			libv::vm4::log_vm4.fatal("AABB_min: {}", model.AABB_min);
		});
	}

	void run(std::string_view script) {
		lua.safe_script(script, sol::script_throw_on_error);

//		if (!result.valid()) {
//			sol::error err = result;
//			return log_ui.error("Script execution failed: {}", err.what());
//		}
//
//		if (result.get_type() != sol::type::none)
//			log_ui.warn("Script return value is unused: {}:{} - {}", libv::to_underlying(result.get_type()), libv::lua::lua_type_to_string(result.get_type()), std::string(result));
//
//		log_ui.info("Script loading successful in {:7.3f}ms", timer.timef_ms().count());

		if (dirtyBounds)
			libv::vm4::recalculateBounding(model);
	}
};

// -------------------------------------------------------------------------------------------------

bool execute_script(libv::vm4::Model& model, const std::string& script) {
	try {
		auto lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);
		LuaContext lua_context{lua, model};
		lua_context.bind();
		lua_context.run(script);
		return true;

	} catch (const std::exception& e) {
		libv::vm4::log_vm4.info("Script execution failed: {}", e.what());
		return false;
	}
}


// -------------------------------------------------------------------------------------------------

} // namespace mimp
