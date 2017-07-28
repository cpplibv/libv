// File: module.hpp - Created on 2017.10.08. 03:00 - Author: Vader

#pragma once

// libv
#include <libv/gl/image.hpp>
#include <libv/glr/texture.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Texture2D {
	libv::glr::Texture2D::R8_G8_B8_A8 texture_;
	libv::vec2i size_;

public:
	Texture2D(libv::gl::Image image) : size_(image.size()) {
		texture_.load(std::move(image));
	}

	libv::vec2i size() const {
		return size_;
	}

	auto& texture() {
		return texture_;
	}

	const auto& texture() const {
		return texture_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
