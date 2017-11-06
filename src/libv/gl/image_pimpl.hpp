// File: image_pimpl.hpp - Created on 2017.11.06. 04:32 - Author: Vader

#pragma once


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct Image::Pimpl {
	libv::vec2i imageSize;

public:
	Pimpl() = default;
	virtual GLuint createTexture() = 0;
	virtual ~Pimpl() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
