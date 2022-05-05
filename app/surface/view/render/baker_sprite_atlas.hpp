// Created by dbobula on 4/23/2022.

#pragma once

// libv
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/math/vec.hpp>
// pro
#include <surface/view/render/renderer_sprite.hpp>
#include <surface/view/render/shaders.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct UniformsSpriteBakerDownsample {
	libv::glr::Uniform_texture textureColor;
	libv::glr::Uniform_texture textureNormal;
	libv::glr::Uniform_int32 ssaaSamples;

	template <typename Access> void access_uniforms(Access& access) {
		access(textureColor, "textureColor", textureChannel_diffuse);
		access(textureNormal, "textureNormal", textureChannel_normal);
		access(ssaaSamples, "ssaaSamples");
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderSpriteBakerDownsample = libv::rev::Shader<UniformsSpriteBakerDownsample>;

// -------------------------------------------------------------------------------------------------

class BakerSpriteAtlas {
private:
	libv::vec2i atlasSize;
	int32_t ssaaSamples;

	libv::vec2i bakeSize;
	libv::rev::ResourceManager& loader;

	libv::glr::Framebuffer bakeMSFBO;
	libv::glr::Texture2DMultisample::R8_G8_B8_A8 bakeMSColor;
	libv::glr::Texture2DMultisample::R8_G8_B8 bakeMSNormal;
	libv::glr::Texture2DMultisample::D16 bakeMSDepth;

	libv::glr::Framebuffer bakeSSFBO;
	libv::glr::Texture2D::R8_G8_B8_A8 bakeSSColor;
	libv::glr::Texture2D::R8_G8_B8 bakeSSNormal;
	libv::glr::Texture2D::D16 bakeSSDepth;

	ShaderSpriteBakerDownsample shaderSpriteBakerDownsample;

public:
	BakerSpriteAtlas(libv::rev::ResourceManager& loader, libv::vec2i atlasSize, int msaaSamples = 16, int ssaaSamples = 16);

private:
	void renderAtlas(SpriteDefinition& def, libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::rev::Model& model, float sizeMultiplier);

public:
	void bake(SpriteAtlas& atlas, uint32_t index,
			libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream,
			std::string_view modelName, float sizeMultiplier);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
