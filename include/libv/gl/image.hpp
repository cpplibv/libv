// File: image.hpp - Created on 2017.11.05. 08:59 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cassert>
#include <memory>
#include <optional>
#include <string_view>
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

/// @supported KTX, KMG, DDS: GLI backend
/// @supported BMP, PNG, JPG, TGA, DDS, PSD, HDR: SOIL backend
/// @context ANY
[[nodiscard]] std::optional<Image> load_image(const std::string_view data) noexcept;

/// @supported KTX, KMG, DDS: GLI backend
/// @supported BMP, PNG, JPG, TGA, DDS, PSD, HDR: SOIL backend
/// @context ANY
// TODO P5: implement fallback image
//[[nodiscard]] Image load_image_or(const std::string_view data, Fallback&& fallback = "");

/// @supported KTX, KMG, DDS: GLI backend
/// @supported BMP, PNG, JPG, TGA, DDS, PSD, HDR: SOIL backend
/// @context ANY
[[nodiscard]] Image load_image_or_throw(const std::string_view data);

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
