// Project: libv, File: app/surface/view/render/renderer_sprite.cpp

// hpp
#include <surface/view/render/renderer_sprite.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/math/frustum.hpp>
#include <libv/rev/resource_manager.hpp>
// pro
#include <surface/log.hpp>
#include <surface/surface/surface_constants.hpp>
#include <surface/view/render/baker_sprite_atlas.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererSprite::RendererSprite(libv::rev::ResourceManager& loader) :
		shader(loader.shader, "builtin/sprite.vs", "builtin/sprite.gs", "builtin/sprite.fs") {}

void RendererSprite::updateMesh(libv::glr::Mesh& mesh) {
	mesh.clear();

	auto type = mesh.attribute(attribute_type);
	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto surfaceNormal = mesh.attribute(attribute_tangent);
	auto color0 = mesh.attribute(attribute_color0);
	auto rotation_scale = mesh.attribute(attribute_custom0_f2);

	auto index = mesh.index();

	type.set_from_range(entries_type);
	position.set_from_range(entries_position);
	normal.set_from_range(entries_normal);
	surfaceNormal.set_from_range(entries_surface_normal);
	rotation_scale.set_from_range(entries_rotation_scale);
	color0.set_from_range(entries_hsv_shift);

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

void RendererSprite::clearChunks() {
	spriteChunks.clear();
}

int RendererSprite::chunkGeneration(libv::vec2i index) {
	return spriteChunks[index].generation;
}

void RendererSprite::add(int32_t type, libv::vec3f position, libv::vec3f normal, libv::vec3f surfaceNormal, float rotation, float scale, libv::vec3f hsv_shift) {
	entries_type.emplace_back(type);
	entries_position.emplace_back(position);
	entries_normal.emplace_back(normal);
	entries_surface_normal.emplace_back(surfaceNormal);
	entries_rotation_scale.emplace_back(rotation, scale);
	entries_hsv_shift.emplace_back(hsv_shift, 1.f);
}

void RendererSprite::commitChunk(int generation, libv::vec2i index) {
	auto& spriteChunk = spriteChunks[index];
	spriteChunk.generation = generation;

	updateMesh(spriteChunk.mesh);

	entries_type.clear();
	entries_position.clear();
	entries_normal.clear();
	entries_surface_normal.clear();
	entries_rotation_scale.clear();
	entries_hsv_shift.clear();
}

void RendererSprite::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum) {
	if (spriteChunks.empty())
		return;

	glr.program(shader.program());
	glr.uniform(shader.uniform().fogEnabled, fogEnabled);
	glr.uniform(shader.uniform().fogIntensity, fogIntensity);
	glr.uniform(shader.uniform().fogColor, fogColor);
	glr.uniform(shader.uniform().sunColor, sunColor);
	glr.uniform(shader.uniform().sunDirection, sunDirection);
	auto block_matrices = uniform_stream.block_unique(layout_matrices);
	block_matrices[layout_matrices.matMVP] = glr.mvp();
	block_matrices[layout_matrices.matM] = glr.model;
	block_matrices[layout_matrices.matP] = glr.projection;
	block_matrices[layout_matrices.eye] = glr.eye();
	glr.uniform(std::move(block_matrices));
	glr.uniform(spriteDefinitionsBlock);
	glr.texture(spriteAtlas->textureColor, textureChannel_diffuse);
	glr.texture(spriteAtlas->textureNormal, textureChannel_normal);

	std::erase_if(spriteChunks, [&](const auto& pair) {
		const auto& [index, spriteChunk] = pair;

		const auto eye = glr.eye();
		const auto chunkPosition = index.template cast<float>() * 32.0f;
		const auto threshold = worldRangeUnloadRender;
		if (libv::length_sq(xy(eye) - chunkPosition) > threshold * threshold) { // TODO P1: app.surface: Constants regarding size and chunk count
//			log_surface.error("Destroying render mesh {} (range: {}) eye {}", index, libv::length(xy(eye) - chunkPosition), eye);
			return true;
		}

		if (frustum.sphere_in_frustum({chunkPosition, 0}, chunkSideLength * libv::sqrt2 * 0.5f) != libv::frustum::position::outside)
			glr.render(spriteChunk.mesh);

		return false;
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
