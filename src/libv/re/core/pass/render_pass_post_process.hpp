// Project: libv.re, File: src/libv/re/node/_post_process.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/common/block_binding.hpp>
#include <libv/re/core/pass/render_pass.hpp>
#include <libv/re/uniform/uniform_block.hpp>

#include <optional>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialPPCompute)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialPPCombine)
class UniformBlockPostProcess;

class RenderPassPostProcess : public RenderPassNodeless {
public:
	struct SettingsPostProcess {
		struct Bloom {
			bool enable = true;
			float bloomThreshold = 0.0f;   /// [0..inf] Filter out pixels under this level of brightness.
			float bloomKnee = 0.0f;        /// [0..bloomThreshold)
			float bloomDecay = 1.0f;       /// [0..1] Decay between downsample levels
			float bloomIntensity = 1.0f;   /// [0..inf] Strength of the bloom effect
			float bloomCombineAdd = 0.0f;  /// [0..1]
			float bloomCombineMix = 0.04f; /// [0..1]
		};

		static constexpr Bloom defaultBloomAdditive {
			.enable = true,
			.bloomThreshold = 1.2f,
			.bloomKnee = 0.2f,
			.bloomDecay = 1.0f,
			.bloomIntensity = 1.0f,
			.bloomCombineAdd = 1.0f,
			.bloomCombineMix = 0.0f,
		};

		static constexpr Bloom defaultBloomMix {
			.enable = true,
			.bloomThreshold = 0.0f,
			.bloomKnee = 0.0f,
			.bloomDecay = 1.0f,
			.bloomIntensity = 1.0f,
			.bloomCombineAdd = 0.0f,
			.bloomCombineMix = 0.04f,
		};

		// struct BloomDirt {
		// };

		struct Vignette {
			bool enable = true;
			float vignetteIntensity = 0.25f;  /// [0..1] Amount of vignetting on the screen
			float vignetteSmoothness = 0.40f; /// (0..1] Smoothness of the vignette borders (Cannot be zero)
			float vignetteRoundness = 1.00f;  /// [0, 1] Should the vignette be round or dependent on the aspect ratio

			libv::vec3f vignetteColor = {0.f, 0.f, 0.f}; /// Vignette color (Used as a multiple operation)
		};

		// struct VignetteTexture {
		// };

		// struct Exposure {
		// };

		struct Tonemap {
			bool enable = false;
			/// Possible values are:
			/// 	tonemapAcesFilmic
			/// 	tonemapNeutral
			/// 	tonemapAces
			/// 	tonemapReinhard
			/// 	tonemapReinhard2
			/// 	tonemapLottes
			/// 	tonemapFilmic
			/// 	tonemapUchimura
			/// 	tonemapUncharted2
			/// 	tonemapUncharted2Tonemap
			///		tonemapUnreal
			std::string tonemapFunction = "tonemapAcesFilmic";
		};

		struct TonemapWithWhite {
			bool enable = true;
			libv::vec3f whitePoint = {5, 5, 5}; // [0..inf) White point in the input (what should yield white output after tonemapping)
			libv::vec3f whiteCurve = {7, 7, 7}; // [0..inf) White on the tonemap curve (how much should be used from the tonemap curve)
		};

		struct GammaCorrection {
			bool enable = true;
			float gamma = 2.2f;
		};

		Bloom bloom = defaultBloomMix;
		Vignette vignette = Vignette{};
		Tonemap tonemap = Tonemap{};
		TonemapWithWhite tonemapWithWhite = TonemapWithWhite{};
		GammaCorrection gammaCorrection = GammaCorrection{};
	};

private:
	SettingsPostProcess settings;

	UniformBlock_ptr<UniformBlockPostProcess> uniformBlock;

	MaterialPPCombine_ptr materialCombine;
	Shader_ptr shaderCombine;

private:
	struct BloomState {
		MaterialPPCompute_ptr materialDownsamplePrefilter;
		MaterialPPCompute_ptr materialDownsample;
		MaterialPPCompute_ptr materialUpsample;

		Texture_ptr texturePyramid;

		libv::vec2ui storageSize;
		uint32_t numMips;
		libv::vec2ui bloomBoundingPosition;
		libv::vec2ui bloomBoundingSize;

		explicit BloomState(const UniformBlock_ptr<UniformBlockPostProcess>& uniformBlock);
		void calculateBloomPyramidInfo(libv::vec2ui framebufferSize, uint32_t maxBloomStepSize) noexcept;
	};

	std::optional<BloomState> bloomState;

public:
	RenderPassPostProcess();
	// RenderPassPostProcess(Texture_ptr bloomDirtTexture, Texture_ptr vignetteTexture);

private:
	// //	void bloom(bool value) noexcept;
	// //	[[nodiscard]] bool bloom() const noexcept;
	// void bloomIntensity(float value) noexcept;
	// [[nodiscard]] float bloomIntensity() const noexcept;
	// void bloomThreshold(float value) noexcept;
	// [[nodiscard]] float bloomThreshold() const noexcept;
	// void bloomKnee(float value) noexcept;
	// [[nodiscard]] float bloomKnee() const noexcept;
	//
	// //	void vignette(bool value) noexcept;
	// //	[[nodiscard]] bool vignette() const noexcept;
	// void vignetteIntensity(float value) noexcept;
	// [[nodiscard]] float vignetteIntensity() const noexcept;
	// void vignetteRoundness(float value) noexcept;
	// [[nodiscard]] float vignetteRoundness() const noexcept;
	// void vignetteSmoothness(float value) noexcept;
	// [[nodiscard]] float vignetteSmoothness() const noexcept;

private:
	virtual void doUpdate(ContextUpdate& ctx) override;
	virtual void doPass(ContextPipeline& pipeline, ContextRender& ctx) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
