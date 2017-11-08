// File: image.cpp - Created on 2017.11.06. 12:06 - Author: Vader

// hpp
#include <libv/gl/image.hpp>
// pro
#include <libv/gl/image_gli.hpp>
#include <libv/gl/image_soil.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

std::unique_ptr<Image::Pimpl> loadSOIL(const char* data, size_t dataSize) {
	auto texture = std::make_unique<ImageSOIL>(data, dataSize);
	return texture->ok() ? std::unique_ptr<Image::Pimpl>{std::move(texture)} : nullptr;
}

std::unique_ptr<Image::Pimpl> loadGLI(const char* data, size_t dataSize) {
	auto texture = std::make_unique<ImageGLI>(data, dataSize);
	return texture->ok() ? std::unique_ptr<Image::Pimpl>{std::move(texture)} : nullptr;
}

// -------------------------------------------------------------------------------------------------

Image::Image(const char* data, size_t dataSize) {
	if (!pimpl) pimpl = loadGLI(data, dataSize);
	if (!pimpl) pimpl = loadSOIL(data, dataSize);
	if (!pimpl) throw InvalidImageFormatException{};
}

Image::Image(const std::string& data) : Image(data.data(), data.size()) {
}

Image::~Image() { }

libv::vec2i Image::size() const {
	return pimpl->imageSize;
}

GLuint Image::createTexture() {
	return pimpl->createTexture();
}


// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
