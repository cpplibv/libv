// Project: libv, File: app/mimp/lua_driver.cpp

// hpp
#include <mimp/lua_driver.hpp>
// ext
#include <sol/state.hpp>
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

	void printNodes(uint32_t nodeID, int depth) {
		const auto& node = model.nodes[nodeID];

		fmt::print("{} {}: {} child, {} mesh\n", fmt::format("{:{}}", " ", depth * 4), node.name, node.childrenIDs.size(), node.meshIDs.size());
		for (const auto& meshID : node.meshIDs) {
			fmt::print("{}  > {}\n", fmt::format("{:{}}", " ", depth * 4), model.meshes[meshID].name);
		}
		for (const auto& childID : node.childrenIDs) {
			printNodes(childID, depth + 1);
		}
	}

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
			model.nodes[0].transform.scale(libv::vec3f::one(value));
			dirtyBounds = true;
		});

		api_table.set_function("rotate", [this](const float x, const float y, const float z) mutable {
			model.nodes[0].transform.rotate(libv::deg_to_rad(x), {1, 0, 0});
			model.nodes[0].transform.rotate(libv::deg_to_rad(y), {0, 1, 0});
			model.nodes[0].transform.rotate(libv::deg_to_rad(z), {0, 0, 1});
			dirtyBounds = true;
		});

		api_table.set_function("fix_XpZmYp", [this]() mutable {
			// X+ Forward
			// Z+ Right
			// Y- Up
			for (auto& node : model.nodes) {
				std::swap(node.transform[1], node.transform[2]);
			}
			for (auto& vertex : model.vertices) {
				vertex.position = xzy(vertex.position);
				vertex.normal = xzy(vertex.normal);
				vertex.tangent = xzy(vertex.tangent);
				vertex.bitangent = xzy(vertex.bitangent);
			}
			for (std::size_t i = 2; i < model.indices.size(); i += 3) {
				std::swap(model.indices[i - 1], model.indices[i]);
			}
			// for (auto& animation : model.animations) {
			// 	// ...
			// }

			dirtyBounds = true;
		});

		api_table.set_function("fix_ZpXmYp", [this]() mutable {
			// Z+ Forward
			// X- Right
			// Y+ Up
			for (auto& node : model.nodes) {
				std::swap(node.transform[0], node.transform[2]);
				std::swap(node.transform[0], node.transform[1]);
			}
			for (auto& vertex : model.vertices) {
				vertex.position = zxy(vertex.position);
				vertex.normal = zxy(vertex.normal);
				vertex.tangent = zxy(vertex.tangent);
				vertex.bitangent = zxy(vertex.bitangent);
			}
			// for (auto& animation : model.animations) {
			// 	// ...
			// }

			dirtyBounds = true;
		});
		api_table["fix_xzy"] = api_table["fix_XpZmYp"];
		api_table["fix_zxy"] = api_table["fix_ZpXmYp"];

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

		api_table.set_function("print_nodes", [this]() {
			for (std::size_t i = 0; i < model.lods.size(); ++i) {
				const auto& lod = model.lods[i];
				fmt::print("LOD: {}, near {}, far: {}\n", i, lod.rangeNear, lod.rangeFar);
				printNodes(lod.rootNodeID, 1);
			}
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
//			log_ui.warn("Script return value is unused: {}:{} - {}", libv::to_underlying(result.get_type()), libv::lua::to_string(result.get_type()), std::string(result));
//
//		log_ui.info("Script loading successful in {:7.3f}ms", timer.timef_ms().count());

		if (dirtyBounds)
			libv::vm4::recalculateBounding(model);
	}
};

// -------------------------------------------------------------------------------------------------

void execute_script(libv::vm4::Model& model, const std::string& script) {
	auto lua = libv::lua::create_state(libv::lualib::basic | libv::lualib::libv_vec);
	LuaContext lua_context{lua, model};
	lua_context.bind();
	lua_context.run(script);
}

// -------------------------------------------------------------------------------------------------

} // namespace mimp
