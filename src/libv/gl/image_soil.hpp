// File: image_soil.hpp - Created on 2017.11.06. 04:30 - Author: Vader

#pragma once

// pro
#include <libv/gl/image.hpp>
#include <libv/gl/image_pimpl.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct ImageSOIL : public Image::Pimpl {
	int32_t channels;
	unsigned char* textureData;

	ImageSOIL(const char* data, std::size_t dataSize);
	~ImageSOIL ();

	bool ok() const;
	GLuint createTexture() override;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
