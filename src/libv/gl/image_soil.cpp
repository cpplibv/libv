// File: soil_loader.cpp - Created on 2017.11.05. 11:24 - Author: Vader

// hpp
#include <libv/gl/image_soil.hpp>
// ext
#include <SOIL/SOIL.h>
// pro
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

ImageSOIL::ImageSOIL(const char* data, std::size_t dataSize) {
	textureData = SOIL_load_image_from_memory(reinterpret_cast<const unsigned char*>(data), dataSize, &imageSize.x, &imageSize.y, &channels, SOIL_LOAD_AUTO);
}

GLuint ImageSOIL::createTexture() {
	const auto id = SOIL_create_OGL_texture(textureData, imageSize.x, imageSize.y, channels, SOIL_CREATE_NEW_ID, 0);
	LIBV_GL_DEBUG_CHECK();
	return id;
}

bool ImageSOIL::ok() const {
	return textureData != nullptr;
}

ImageSOIL::~ImageSOIL() {
	SOIL_free_image_data(textureData);
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
