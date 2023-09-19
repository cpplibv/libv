// Project: libv.rev, File: src/libv/rev/shader.hpp

#pragma once

// hpp
#include <libv/rev/post_processing.hpp>
// libv
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/math/vec.hpp>
// std
#include <vector>
// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/render_target.hpp>
#include <libv/rev/resource/shader.hpp>
//#include <libv/rev/resource/shader_base.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct UniformsPPTexture {
	libv::glr::Uniform_texture texture0;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture0, "texture0", libv::gl::TextureUnit{0});
	}

	template <typename Access> void access_blocks(Access&) {
	}
};

struct UniformsPPBloomPrefilter {
	libv::glr::Uniform_texture texture0;
	libv::glr::Uniform_float bloomThreshold;
	libv::glr::Uniform_float bloomKnee;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture0, "texture0", libv::gl::TextureUnit{0});
		access(bloomThreshold, "bloomThreshold");
		access(bloomKnee, "bloomKnee");
	}

	template <typename Access> void access_blocks(Access&) {
	}
};

struct UniformsPPCombine {
	libv::glr::Uniform_texture textureMain;
	libv::glr::Uniform_texture textureBloom;

	libv::glr::Uniform_float bloomIntensity;

	libv::glr::Uniform_float vignetteIntensity;
	libv::glr::Uniform_float vignetteSmoothness;
	libv::glr::Uniform_float vignetteRoundness;
	libv::glr::Uniform_vec3f vignetteColor;

	template <typename Access> void access_uniforms(Access& access) {
		access(textureMain, "textureMain", libv::gl::TextureUnit{0});
		access(textureBloom, "textureBloom", libv::gl::TextureUnit{1});
		access(bloomIntensity, "bloomIntensity");
		access(vignetteIntensity, "vignetteIntensity");
		access(vignetteSmoothness, "vignetteSmoothness");
		access(vignetteRoundness, "vignetteRoundness");
		access(vignetteColor, "vignetteColor");
	}

	template <typename Access> void access_blocks(Access&) {
	}
};

// -------------------------------------------------------------------------------------------------

class ImplPostProcessing {
public:
	struct Mip {
		RenderTarget main; /// During downsample: Used as vertical blur result, During upsample: used for merge
		RenderTarget temp; /// Used for horizontal downsample blur temp storage

		explicit Mip(libv::vec2i size) :
				main(size, 0),
				temp(size, 0) {}
	};

public:
	libv::vec2i framebufferSize;
	std::vector<Mip> bloomMips;

public:
	libv::rev::Shader<UniformsPPBloomPrefilter> shaderDownsamplePrefilter;
	libv::rev::Shader<UniformsPPTexture> shaderDownsampleBlurH;
	libv::rev::Shader<UniformsPPTexture> shaderDownsampleBlurV;
	libv::rev::Shader<UniformsPPTexture> shaderUpsample;
	libv::rev::Shader<UniformsPPCombine> shaderCombine;

public:
//	bool bloom_ = true;
	float bloomIntensity_ = 1.0f; /// Strength of the bloom effect
	float bloomThreshold_ = 1.0f; /// Filter out pixels under this level of brightness.
	float bloomKnee_ = 0.2f;

//	bool vignette_ = true;
	float vignetteIntensity_ = 0.25f;  /// [0..1] Amount of vignetting on the screen
	float vignetteSmoothness_ = 0.40f; /// (0..1] Smoothness of the vignette borders (Cannot be zero)
	float vignetteRoundness_ = 1.00f;  /// [0, 1] Should the vignette be round or dependent on the aspect ratio
	libv::vec3f vignetteColor_ = {0.f, 0.f, 0.f}; /// Vignette color (Used as a multiple operation)

public:
	ImplPostProcessing(ShaderLoader& shaderLoader, libv::vec2i framebufferSize);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
