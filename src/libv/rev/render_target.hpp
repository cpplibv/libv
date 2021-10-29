// Project: libv.rev, File: src/libv/rev/shader.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
#include <libv/glr/texture.hpp>
#include <libv/math/vec.hpp>
// std
#include <memory>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class ImplRenderTarget;

class RenderTarget {
private:
	std::unique_ptr<ImplRenderTarget> self;

public:
	RenderTarget(libv::vec2i size_, int sampleCount_);
	RenderTarget(RenderTarget&&);
	RenderTarget& operator=(RenderTarget&&);
	~RenderTarget();

public:
	void sampleCount(int sampleCount_) noexcept;
	[[nodiscard]] int sampleCount() const noexcept;

	void size(libv::vec2i framebufferSize) noexcept;
	[[nodiscard]] libv::vec2i size() const noexcept;

public:
	const libv::glr::Texture2D::R11F_G11F_B10F& resolve(libv::glr::Queue& glr);

	[[nodiscard]] const libv::glr::Framebuffer& framebuffer() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
