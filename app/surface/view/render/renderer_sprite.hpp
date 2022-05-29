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
#include <unordered_map>
// pro
#include <surface/view/render/shaders.hpp>
#include <surface/view/vec_hash.hpp>


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

//	std::cout << libv::glr::layout_to_string<UniformLayoutSpriteDefinitions>("SpriteDefinitions") << std::endl;
//	std::cout << libv::glr::layout_std140_size<UniformLayoutSpriteDefinitions>() << std::endl;

// -------------------------------------------------------------------------------------------------

struct UniformsSprite {
	libv::glr::Uniform_texture textureColor;
	libv::glr::Uniform_texture textureNormal;

	libv::glr::Uniform_bool fogEnabled;
	libv::glr::Uniform_float fogIntensity;
	libv::glr::Uniform_vec4f fogColor;
	libv::glr::Uniform_vec3f sunColor;
	libv::glr::Uniform_vec3f sunDirection;

	template <typename Access> void access_uniforms(Access& access) {
		access(textureColor, "textureColor", textureChannel_diffuse);
		access(textureNormal, "textureNormal", textureChannel_normal);

		access(fogEnabled, "fogEnabled", true);
		access(fogIntensity, "fogIntensity", 0.05f);
		access(fogColor, "fogColor", libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f});
		access(sunColor, "sunColor", libv::vec3f{1.0f, 0.9f, 0.8f});
		access(sunDirection, "sunDirection", libv::vec3f{0.784464f, 0.196116f, 0.588348f});
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
//	libv::vec2i tile_size = {128, 128};
//	libv::vec2i tile_size = {256, 256};

	float model_scale = 1.f; // Billboard size
	libv::vec3f model_offset = {0.f, 0.f, 0.f}; // Billboard offset compared to origin

	int32_t tile_num_x = 32;
	int32_t tile_num_y = 8;


//	int32_t tile_num_x = 8;
//	int32_t tile_num_y = 8;
//	int32_t tile_num_x = 1;
//	int32_t tile_num_y = 16;
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

class Frustum;

struct SpriteChunk {
	int generation = -1;
	libv::glr::Mesh mesh{libv::gl::Primitive::Points, libv::gl::BufferUsage::StaticDraw};
};

class RendererSprite {
private:
//	struct Entry {
//		int32_t type;
//		libv::vec3f position;
//		libv::vec3f normal;
//		float rotation;
//		float scale;
//		libv::vec3f hsv_shift;
//	};

	struct SpriteType {
		std::string modelPath;
		float sizeMultiplier;
	};

private:
	ShaderSprite shader;
	std::unordered_map<libv::vec2i, SpriteChunk, VecHash> spriteChunks;
	std::shared_ptr<SpriteAtlas> spriteAtlas;
	libv::glr::UniformBlockSharedView_std140 spriteDefinitionsBlock;

private:
	std::vector<SpriteType> spriteTypes;

private:
	std::vector<int32_t> entries_type;
	std::vector<libv::vec3f> entries_position;
	std::vector<libv::vec3f> entries_normal;
	std::vector<libv::vec3f> entries_surface_normal;
	std::vector<libv::vec2f> entries_rotation_scale;
	std::vector<libv::vec4f> entries_hsv_shift;

public:
	bool fogEnabled = true;
	float fogIntensity = 0.05f;
	libv::vec4f fogColor = libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f};
	libv::vec3f sunColor = libv::vec3f{1.0f, 0.9f, 0.8f};
	libv::vec3f sunDirection = libv::vec3f{0.784464f, 0.196116f, 0.588348f};

public:
	explicit RendererSprite(libv::rev::ResourceManager& loader);

private:
	void updateMesh(libv::glr::Mesh& mesh);

public:
	int32_t registerSprite(std::string modelPath, float sizeMultiplier);
	void clearSprites();
	void bakeSprites(libv::rev::ResourceManager& loader, libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);

public:
	void clearChunks();

	int chunkGeneration(libv::vec2i index);
	void add(int32_t type, libv::vec3f position, libv::vec3f normal, libv::vec3f surfaceNormal, float rotation, float scale, libv::vec3f hsv_shift);
	void commitChunk(int generation, libv::vec2i index);

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Frustum& frustum);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
