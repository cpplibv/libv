// Created by dbobula on 4/23/2022.

// hpp
#include <surface/view/render/renderer_sprite.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/rev/materials/material_sprite_baker.hpp>
#include <libv/rev/model.hpp>
#include <libv/rev/resource_manager.hpp>
#include <libv/utility/min_max.hpp>
//#include <libv/glr/procedural/cube.hpp>

//#include <surface/view/render/renderer.hpp>
// pro
#include <surface/view/render/baker_sprite_atlas.hpp>
#include <surface/log.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererSprite::RendererSprite(libv::rev::ResourceManager& loader) :
		shader(loader.shader, "builtin/sprite.vs", "builtin/sprite.gs", "builtin/sprite.fs") {}

void RendererSprite::updateMesh() {
	dirty = false;
	active = !entries_type.empty();
//	active = !entries.empty();

	mesh.clear();

	if (!active)
		return;

//		std::cout << libv::glr::layout_to_string<UniformLayoutSpriteDefinitions>("SpriteDefinitions") << std::endl;
//		std::cout << libv::glr::layout_std140_size<UniformLayoutSpriteDefinitions>() << std::endl;

	// --- Mesh

	auto type = mesh.attribute(attribute_type);
	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto color0 = mesh.attribute(attribute_color0);
	auto rotation_scale = mesh.attribute(attribute_custom0_f2);

	auto index = mesh.index();

//	type.resize(entries.size());
//	position.resize(entries.size());
//	normal.resize(entries.size());
//	rotation_scale.resize(entries.size());
//	color0.resize(entries.size());
//	index.resize(entries.size());
//
//	for (uint32_t i = 0; i < entries.size(); ++i) {
//		const auto& vertex = entries[i];
//
//		type[i] = vertex.type;
//		position[i] = vertex.position;
//		normal[i] = vertex.normal;
//		rotation_scale[i] = libv::vec2f(vertex.rotation, vertex.scale);
//		color0[i] = libv::vec4f(vertex.hsv_color_shift, 1.f);
//		index[i] = i;
//	}

	log_surface.error("entries_type.size(): {}", entries_type.size());

	type.set_from_range(entries_type);
	position.set_from_range(entries_position);
	normal.set_from_range(entries_normal);
	rotation_scale.set_from_range(entries_rotation_scale);
	color0.set_from_range(entries_hsv_color_shift);

	index.resize(entries_type.size());
	for (uint32_t i = 0; i < entries_type.size(); ++i)
		index[i] = i;
}

int32_t RendererSprite::registerSprite(std::string modelPath, float sizeMultiplier) {
	spriteTypes.emplace_back(std::move(modelPath), sizeMultiplier);

	return static_cast<int32_t>(spriteTypes.size()) - 1;
}

void RendererSprite::clearSprites() {
	spriteTypes.clear();
}

void RendererSprite::bakeSprites(libv::rev::ResourceManager& loader, libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	// --- Texture Bake

	libv::vec2i atlasSize = {2048, 512};
//	libv::vec2i atlasSize = {4096, 1024};
//	libv::vec2i atlasSize = {256, 2048};
//	libv::vec2i atlasSize = {256, 4096};
//	libv::vec2i atlasSize = {512, 512};
//	libv::vec2i atlasSize = {3 * 64, 512};
//	libv::vec2i atlasSize = {8 * 64, 8 * 64};
	BakerSpriteAtlas baker(loader, atlasSize, 1, 16);

	spriteAtlas = std::make_shared<SpriteAtlas>();
	spriteAtlas->definitions.resize(spriteTypes.size());
	spriteAtlas->textureColor.storage(1, libv::vec3i{atlasSize, static_cast<int>(spriteTypes.size())});
	spriteAtlas->textureColor.set(libv::gl::MagFilter::Linear);
	spriteAtlas->textureColor.set(libv::gl::MinFilter::LinearMipmapLinear);
	spriteAtlas->textureNormal.storage(1, libv::vec3i{atlasSize, static_cast<int>(spriteTypes.size())});
	spriteAtlas->textureNormal.set(libv::gl::MagFilter::Linear);
	spriteAtlas->textureNormal.set(libv::gl::MinFilter::LinearMipmapLinear);

	for (uint32_t i = 0; i < spriteTypes.size(); ++i)
		baker.bake(*spriteAtlas, i,
				glr, uniform_stream,
				spriteTypes[i].modelPath, spriteTypes[i].sizeMultiplier);

	// --- Uniform Sprite Definitions

	spriteDefinitionsBlock = uniform_stream.block_shared(layout_definitions);
	for (uint32_t i = 0; i < spriteTypes.size(); ++i) {
		const auto& def = spriteAtlas->definitions[i];

		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].tile_size] = def.tile_size;
		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].tile_num_x] = def.tile_num_x;
		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].tile_num_y] = def.tile_num_y;

		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].model_scale] = def.model_scale;
		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].model_offset] = def.model_offset;

		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].angle_x_min] = def.angle_x_min;
		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].angle_x_max] = def.angle_x_max;
		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].angle_y_min] = def.angle_y_min;
		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[i].angle_y_max] = def.angle_y_max;
	}
}

void RendererSprite::clear() {
	entries_type.clear();
	entries_position.clear();
	entries_normal.clear();
	entries_rotation_scale.clear();
	entries_hsv_color_shift.clear();

//	entries.clear();
}

void RendererSprite::add(int32_t type, libv::vec3f position, libv::vec3f normal, float rotation, float scale, libv::vec3f hsv_color_shift) {
//	entries.emplace_back(type, position, normal, rotation, scale, hsv_color_shift);
	entries_type.emplace_back(type);
	entries_position.emplace_back(position);
	entries_normal.emplace_back(normal);
	entries_rotation_scale.emplace_back(rotation, scale);
	entries_hsv_color_shift.emplace_back(hsv_color_shift, 1.f);
	dirty = true;
}

void RendererSprite::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	if (dirty)
		updateMesh();

	if (!active)
		return;

	glr.program(shader.program());
	glr.uniform(shader.uniform().fogEnabled, fogEnabled);
	glr.uniform(shader.uniform().fogIntensity, fogIntensity);
	glr.uniform(shader.uniform().fogColor, fogColor);
	auto block_matrices = uniform_stream.block_shared(layout_matrices);
	block_matrices[layout_matrices.matMVP] = glr.mvp();
	block_matrices[layout_matrices.matM] = glr.model;
	block_matrices[layout_matrices.matP] = glr.projection;
	block_matrices[layout_matrices.eye] = glr.eye();
	glr.uniform(std::move(block_matrices));
	glr.uniform(spriteDefinitionsBlock);
	glr.texture(spriteAtlas->textureColor, textureChannel_diffuse);
	glr.texture(spriteAtlas->textureNormal, textureChannel_normal);
	glr.render(mesh);
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
