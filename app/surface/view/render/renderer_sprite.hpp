// Created by dbobula on 4/23/2022.

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/math/vec.hpp>
// std
#include <array>
// pro
#include <surface/view/render/shaders.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct UniformLayoutSpriteDefinition {
	static inline auto name() { return "SpriteDefinition"; }

	// Ordering matters: Block is packed to 48 Byte
	libv::glr::Uniform_vec2i tile_size;
	libv::glr::Uniform_int32 tile_num_x;
	libv::glr::Uniform_int32 tile_num_y;

	libv::glr::Uniform_vec3f model_offset;
	libv::glr::Uniform_float model_scale;

	libv::glr::Uniform_float angle_x_min;
	libv::glr::Uniform_float angle_x_max;
	libv::glr::Uniform_float angle_y_min;
	libv::glr::Uniform_float angle_y_max;

	LIBV_REFLECTION_ACCESS(tile_size);
	LIBV_REFLECTION_ACCESS(tile_num_x);
	LIBV_REFLECTION_ACCESS(tile_num_y);

	LIBV_REFLECTION_ACCESS(model_offset);
	LIBV_REFLECTION_ACCESS(model_scale);

	LIBV_REFLECTION_ACCESS(angle_x_min);
	LIBV_REFLECTION_ACCESS(angle_x_max);
	LIBV_REFLECTION_ACCESS(angle_y_min);
	LIBV_REFLECTION_ACCESS(angle_y_max);
};

struct UniformLayoutSpriteDefinitions {
	// With 48 byte as SpriteDefinition size the maximum sprite definition count is: 65536 / 48 = 1365
	// std::array<UniformLayoutSpriteDefinition, 65536 / 48> spriteDefinitions;
	std::array<UniformLayoutSpriteDefinition, 64> spriteDefinitions;

	LIBV_REFLECTION_ACCESS(spriteDefinitions);
};

inline const auto layout_definitions = libv::glr::layout_std140<UniformLayoutSpriteDefinitions>(uniformBlock_definitions);

// -------------------------------------------------------------------------------------------------

struct UniformsSprite {
	libv::glr::Uniform_texture textureColor;
	libv::glr::Uniform_texture textureNormal;

	template <typename Access> void access_uniforms(Access& access) {
		access(textureColor, "textureColor", textureChannel_diffuse);
		access(textureNormal, "textureNormal", textureChannel_normal);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
		access(uniformBlock_definitions);
	}
};

using ShaderSprite = libv::rev::Shader<UniformsSprite>;

// -------------------------------------------------------------------------------------------------

struct SpriteDefinition {
	libv::vec2i tile_size = {64, 64};

	float model_scale = 1.f; // Billboard size
	libv::vec3f model_offset = {0.f, 0.f, 0.f}; // Billboard offset compared to origin

	int32_t tile_num_x = 32;
	int32_t tile_num_y = 8;
	float angle_x_min = 0.f;
//	float angle_x_max = libv::tau * (static_cast<float>(tile_num_y) - 1) / static_cast<float>(tile_num_y);
	float angle_x_max = libv::tau;
	float angle_y_min = 0.f;
	float angle_y_max = libv::pi / 2.f;
};

struct SpriteAtlas {
	std::vector<SpriteDefinition> definitions;
	libv::glr::Texture2DArray::R8_G8_B8_A8 textureColor;
	libv::glr::Texture2DArray::R8_G8_B8 textureNormal;
};

// -------------------------------------------------------------------------------------------------

class RendererSprite {
private:
	struct Entry {
		int32_t type;
		libv::vec3f position;
		//libv::vec3f normal;
		//float rotation;
		//float height or scale;
		//libv::vec3f hsv_color_shift;
	};

	struct SpriteType {
		std::string modelPath;
		float sizeMultiplier;
	};

private:
	ShaderSprite shader;
	libv::glr::Mesh mesh{libv::gl::Primitive::Points, libv::gl::BufferUsage::StaticDraw};
	std::shared_ptr<SpriteAtlas> spriteAtlas;
	libv::glr::UniformBlockSharedView_std140 spriteDefinitionsBlock;

private:
	std::vector<SpriteType> spriteTypes;

private:
	std::vector<Entry> entries;
	bool dirty = false;
	bool active = false;

public:
	explicit RendererSprite(libv::rev::ResourceManager& loader);

private:
	void updateMesh();

public:
	void clearSprites();
	int32_t registerSprite(std::string modelPath, float sizeMultiplier);
	void bakeSprites(libv::rev::ResourceManager& loader, libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);

public:
	void add(int32_t type, libv::vec3f position);

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
