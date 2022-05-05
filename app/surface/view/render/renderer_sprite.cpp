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


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererSprite::RendererSprite(libv::rev::ResourceManager& loader) :
		shader(loader.shader, "builtin/sprite.vs", "builtin/sprite.gs", "builtin/sprite.fs") {}

void RendererSprite::updateMesh() {
	dirty = false;
	active = !entries.empty();

	mesh.clear();

	if (!active)
		return;

//		std::cout << libv::glr::layout_to_string<UniformLayoutSpriteDefinitions>("SpriteDefinitions") << std::endl;
//		std::cout << libv::glr::layout_std140_size<UniformLayoutSpriteDefinitions>() << std::endl;

	// --- Mesh

	auto position = mesh.attribute(attribute_position);
	auto type = mesh.attribute(attribute_type);
//		auto normal = mesh.attribute(attribute_normal);

	auto index = mesh.index();

	position.reserve(entries.size());
	for (const auto& vertex : entries)
		position(vertex.position);

	type.reserve(entries.size());
	for (const auto& vertex : entries)
		type(vertex.type);

//		normal.reserve(entries.size());
//		for (const auto& vertex : entries)
//			normal(vertex.normal);

//		auto position = mesh.attribute();

	for (uint32_t i = 0; i < entries.size(); ++i)
		index(i);
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
	entries.clear();
}

void RendererSprite::add(int32_t type, libv::vec3f position) {
	entries.emplace_back(type, position);
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
