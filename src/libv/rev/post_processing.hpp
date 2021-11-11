// Project: libv.rev, File: src/libv/rev/post_processing.hpp
// Project: libv.rev, File: src/libv/rev/shader.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
#include <libv/glr/texture.hpp>
#include <libv/math/vec.hpp>
// std
#include <memory>
// fwd
#include <libv/rev/fwd.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class ImplPostProcessing;

class PostProcessing {
private:
	std::unique_ptr<ImplPostProcessing> self;

public:
	PostProcessing(ShaderLoader& shaderLoader, libv::vec2i framebufferSize);
	~PostProcessing();

public:
	void size(libv::vec2i framebufferSize) noexcept;
	[[nodiscard]] libv::vec2i size() const noexcept;

//	void bloom(bool value) noexcept;
//	[[nodiscard]] bool bloom() const noexcept;
	void bloomIntensity(float value) noexcept;
	[[nodiscard]] float bloomIntensity() const noexcept;
	void bloomThreshold(float value) noexcept;
	[[nodiscard]] float bloomThreshold() const noexcept;
	void bloomKnee(float value) noexcept;
	[[nodiscard]] float bloomKnee() const noexcept;

	//	void vignette(bool value) noexcept;
	//	[[nodiscard]] bool vignette() const noexcept;
	void vignetteIntensity(float value) noexcept;
	[[nodiscard]] float vignetteIntensity() const noexcept;
	void vignetteRoundness(float value) noexcept;
	[[nodiscard]] float vignetteRoundness() const noexcept;
	void vignetteSmoothness(float value) noexcept;
	[[nodiscard]] float vignetteSmoothness() const noexcept;

public:
	void pass(libv::glr::Queue& glr, const libv::glr::Texture2D::R11F_G11F_B10F& input);

public:
	const libv::glr::Texture2D::R11F_G11F_B10F& view(libv::glr::Queue& glr, int step);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
