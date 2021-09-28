// Project: libv.gl, File: src/libv/gl/image.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cassert>
#include <memory>
// pro
#include <libv/gl/texture_object.hpp>


namespace libv {
namespace gl {
namespace detail {

// -------------------------------------------------------------------------------------------------

struct ImageImplementation {
	[[nodiscard]] virtual libv::vec2i size() const noexcept = 0;
	[[nodiscard]] virtual libv::vec4f pixel(int32_t level, int32_t x, int32_t y) const noexcept = 0;
	[[nodiscard]] virtual Texture createTexture() const noexcept = 0;
	virtual ~ImageImplementation() noexcept = default;
};

} // namespace detail ------------------------------------------------------------------------------

class Image {
private:
	libv::vec2i size_;
	std::shared_ptr<detail::ImageImplementation> impl;

public:
	explicit inline Image(std::shared_ptr<detail::ImageImplementation>&& impl) noexcept :
		size_((assert("ImageImplementation cannot be null" && impl != nullptr), impl->size())),
		impl(std::move(impl)) {
	}

public:
	[[nodiscard]] inline libv::vec2i size() const noexcept {
		return size_;
	}

	[[nodiscard]] libv::vec4f pixel(int32_t level, int32_t x, int32_t y) const noexcept {
		return impl->pixel(level, x, y);
	}

	/// @context OpenGL
	[[nodiscard]] inline Texture createTexture() const noexcept {
		return impl->createTexture();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
