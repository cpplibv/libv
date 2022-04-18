// Project: libv.rev, File: src/libv/rev/post_processing.cpp

// hpp
#include <libv/rev/post_processing.hpp>
#include <libv/rev/post_processing.hxx>
// libv
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/queue.hpp>
// pro
//#include <libv/rev/render_target.hpp>
//#include <libv/rev/resource/shader.hpp>
//#include <libv/rev/resource/shader_base.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] static constexpr inline int calculateMipCount(libv::vec2i framebufferSize) noexcept {
	const auto longerSide = std::max(framebufferSize.x, framebufferSize.y);
	constexpr auto minMipCount = 2;
	constexpr auto maxMipCount = 8;
	return std::clamp(
			static_cast<int>(std::log2(longerSide - 1)) - 3,
			minMipCount,
			maxMipCount
	);
}

// -------------------------------------------------------------------------------------------------

PostProcessing::PostProcessing(ShaderLoader& shaderLoader, libv::vec2i framebufferSize) :
	self(std::make_unique<ImplPostProcessing>(shaderLoader, framebufferSize)) {
}

PostProcessing::~PostProcessing() {
	// For the sake of forward declared ptr
}

void PostProcessing::size(libv::vec2i framebufferSize) noexcept {
	self->framebufferSize = framebufferSize;

	const auto mipCountOld = static_cast<int>(self->bloomMips.size());
	const auto mipCountNew = calculateMipCount(framebufferSize);
	const auto mipCountCommon = std::min(mipCountOld, mipCountNew);

	// Modify common mips
	auto mipSize = framebufferSize / 2;
	for (int i = 0; i < mipCountCommon; ++i) {
		self->bloomMips[i].main.size(mipSize);
		self->bloomMips[i].temp.size(mipSize);
		mipSize /= 2;
	}

	if (mipCountNew < mipCountOld)
		// Drop excess mips
		self->bloomMips.erase(self->bloomMips.begin() + mipCountCommon, self->bloomMips.end());
	else
		// Add required mips
		for (int i = mipCountCommon; i < mipCountNew; ++i) {
			self->bloomMips.emplace_back(mipSize);
			mipSize /= 2;
		}
}

libv::vec2i PostProcessing::size() const noexcept {
	return self->framebufferSize;
}

//	void PostProcessing::bloom(bool value) noexcept {
//		bloom_ = value;
//	}
//	bool PostProcessing::bloom() const noexcept {
//		return bloom_;
//	}
void PostProcessing::bloomIntensity(float value) noexcept {
	self->bloomIntensity_ = value;
}
float PostProcessing::bloomIntensity() const noexcept {
	return self->bloomIntensity_;
}
void PostProcessing::bloomThreshold(float value) noexcept {
	self->bloomThreshold_ = value;
}
float PostProcessing::bloomThreshold() const noexcept {
	return self->bloomThreshold_;
}
void PostProcessing::bloomKnee(float value) noexcept {
	self->bloomKnee_ = value;
}
float PostProcessing::bloomKnee() const noexcept {
	return self->bloomKnee_;
}

//	void PostProcessing::vignette(bool value) noexcept {
//		self->vignette_ = value;
//	}
//	bool PostProcessing::vignette() const noexcept {
//		return self->vignette_;
//	}
void PostProcessing::vignetteIntensity(float value) noexcept {
	self->vignetteIntensity_ = value;
}
float PostProcessing::vignetteIntensity() const noexcept {
	return self->vignetteIntensity_;
}
void PostProcessing::vignetteRoundness(float value) noexcept {
	self->vignetteRoundness_ = value;
}
float PostProcessing::vignetteRoundness() const noexcept {
	return self->vignetteRoundness_;
}
void PostProcessing::vignetteSmoothness(float value) noexcept {
	self->vignetteSmoothness_ = value;
}
float PostProcessing::vignetteSmoothness() const noexcept {
	return self->vignetteSmoothness_;
}

void PostProcessing::pass(libv::glr::Queue& glr, const libv::glr::Texture2D::R11F_G11F_B10F& input) {

	// Bloom: Downsample and Prefilter on the first MIP
	glr.framebuffer_draw(self->bloomMips[0].main.framebuffer());
	glr.viewport({0, 0}, self->bloomMips[0].main.size());

	glr.program(self->shaderDownsamplePrefilter.program());
	glr.uniform(self->shaderDownsamplePrefilter.uniform().bloomThreshold, self->bloomThreshold_);
	glr.uniform(self->shaderDownsamplePrefilter.uniform().bloomKnee, self->bloomKnee_);
	glr.texture(input, libv::gl::TextureChannel{0});
	glr.render_full_screen();

	// Bloom: Downsample
	for (std::size_t i = 1; i < self->bloomMips.size(); ++i) {
		// Horizontal blur (using 'temp' mip as temporary place for 2 step downsample blur)
		glr.framebuffer_draw(self->bloomMips[i].temp.framebuffer());
		glr.viewport({0, 0}, self->bloomMips[i].temp.size());

		glr.program(self->shaderDownsampleBlurH.program());
		glr.texture(self->bloomMips[i - 1].main.resolve(glr), libv::gl::TextureChannel{0});
		glr.render_full_screen();

		// Vertical blur
		glr.framebuffer_draw(self->bloomMips[i].main.framebuffer());
		glr.viewport({0, 0}, self->bloomMips[i].main.size());

		glr.program(self->shaderDownsampleBlurV.program());
		glr.texture(self->bloomMips[i].temp.resolve(glr), libv::gl::TextureChannel{0});
		glr.render_full_screen();
	}

	// Bloom: Upsample
	{
		auto state_g = glr.state.push_guard();
		glr.state.blendSrc_One();
		glr.state.blendDst_One();

		for (std::size_t i = self->bloomMips.size() - 1; i > 0; --i) {
			glr.framebuffer_draw(self->bloomMips[i - 1].main.framebuffer());
			glr.viewport({0, 0}, self->bloomMips[i - 1].main.size());

			glr.program(self->shaderUpsample.program());
			glr.texture(self->bloomMips[i].main.resolve(glr), libv::gl::TextureChannel{0});
			glr.render_full_screen();
		}
	}

	// Final combine
	glr.framebuffer_draw_default();
	glr.viewport({0, 0}, self->framebufferSize);

	glr.program(self->shaderCombine.program());
	glr.texture(input, libv::gl::TextureChannel{0});
	glr.texture(self->bloomMips[0].main.resolve(glr), libv::gl::TextureChannel{1});
	glr.uniform(self->shaderCombine.uniform().bloomIntensity, self->bloomIntensity_);
	glr.uniform(self->shaderCombine.uniform().vignetteIntensity, self->vignetteIntensity_ * 3.0f); // NOTE: Multiply by 3 to allow the 0..1 configuration range to yield a reasonable 3 as max value
	glr.uniform(self->shaderCombine.uniform().vignetteSmoothness, self->vignetteSmoothness_ * 5.0f); // NOTE: Multiply by 5 to allow the 0..1 configuration range to yield a reasonable 5 as max value
	glr.uniform(self->shaderCombine.uniform().vignetteRoundness, self->vignetteRoundness_);
	glr.uniform(self->shaderCombine.uniform().vignetteColor, self->vignetteColor_);
	glr.render_full_screen();
}

const libv::glr::Texture2D::R11F_G11F_B10F& PostProcessing::view(libv::glr::Queue& glr, int step) {
	const auto size = static_cast<int>(self->bloomMips.size());
	const auto ref_step = step < 0 ? std::abs(step + size) : step;
	const auto bound_step = static_cast<std::size_t>(ref_step % size);

//	glr.viewport({0, 0}, mips[bound_step].size());

	// TODO P3: Implement inspection feature for middle render targets / textures (this could include access to temporary downsampling steps that are lost after the full pass [but only with overhead])

//	return mips[bound_step].resolve(glr);
	return self->bloomMips[bound_step].temp.resolve(glr);
}

// -------------------------------------------------------------------------------------------------

ImplPostProcessing::ImplPostProcessing(ShaderLoader& shaderLoader, libv::vec2i framebufferSize) :
	framebufferSize(framebufferSize),
	shaderDownsamplePrefilter{shaderLoader, "post_process/full_screen.vs", "post_process/pp_bloom_down_prefilter.fs"},
	shaderDownsampleBlurH{shaderLoader, "post_process/full_screen.vs", "post_process/pp_bloom_down_blur_h.fs"},
	shaderDownsampleBlurV{shaderLoader, "post_process/full_screen.vs", "post_process/pp_bloom_down_blur_v.fs"},
	shaderUpsample{shaderLoader, "post_process/full_screen.vs", "post_process/pp_bloom_up.fs"},
	shaderCombine{shaderLoader, "post_process/full_screen.vs", "post_process/pp_combine.fs"} {

	const auto mipCount = calculateMipCount(framebufferSize);

	auto mipSize = framebufferSize / 2;
	for (int i = 0; i < mipCount; ++i) {
		bloomMips.emplace_back(mipSize);
		mipSize /= 2;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
