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
	[[nodiscard]] virtual Texture createTexture() const noexcept = 0;
	virtual ~ImageImplementation() noexcept = default;
};

} // namespace detail ------------------------------------------------------------------------------

class Image {
private:
	libv::vec2i size_;
	std::shared_ptr<detail::ImageImplementation> impl;

public:
	inline Image(std::shared_ptr<detail::ImageImplementation>&& impl) noexcept :
		size_(impl ? impl->size() : libv::vec2i{}), // operator?: to suppress -Wnull-dereference
		impl(std::move(impl)) {
		assert("ImageImplementation cannot be null" && this->impl != nullptr);
	}

public:
	[[nodiscard]] inline libv::vec2i size() const noexcept {
		return size_;
	}

	/// @context OpenGL
	[[nodiscard]] inline Texture createTexture() const noexcept {
		return impl->createTexture();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
