// File: image.hpp - Created on 2017.11.05. 08:59 - Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// ext
#include <GL/glew.h>
// std
#include <memory>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct Image {
	struct Pimpl;

private:
	std::unique_ptr<Pimpl> pimpl;

public:
	/// @supported GLI backend: KTX, KMG, DDS
	/// @supported SOIL backend: BMP, PNG, JPG, TGA, DDS, PSD, HDR
	Image(const char* data, std::size_t dataSize);
	/// @supported GLI backend: KTX, KMG, DDS
	/// @supported SOIL backend: BMP, PNG, JPG, TGA, DDS, PSD, HDR
	Image(const std::string& data);
	~Image();

	libv::vec2i size() const;

	GLuint createTexture();
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
