// File: image_gli.hpp - Created on 2017.11.06. 04:30 - Author: Vader

#pragma once

// ext
#include <gli/gli.hpp>
// pro
#include <libv/gl/image.hpp>
#include <libv/gl/image_pimpl.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct ImageGLI : public Image::Pimpl {
	gli::texture texture;

	ImageGLI(const char* data, std::size_t dataSize);
	~ImageGLI();

	bool ok() const;
	GLuint createTexture() override;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
