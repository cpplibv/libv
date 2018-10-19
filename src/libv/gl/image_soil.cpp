// File: soil_loader.cpp - Created on 2017.11.05. 11:24 - Author: Vader

// hpp
#include <libv/gl/image.hpp>
// ext
#include <SOIL/SOIL.h>
// libv
#include <libv/container/dynarray.hpp>
// pro
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class ImageSOIL : public detail::ImageImplementation {
	libv::vec2i size_;
	int32_t channels;
	std::unique_ptr<uint8_t, decltype(&SOIL_free_image_data)> storage;

public:
	ImageSOIL(libv::vec2i size_, int32_t channels, std::unique_ptr<uint8_t, decltype(&SOIL_free_image_data)>&& storage) :
		size_(size_), channels(channels), storage(std::move(storage)) { }

	[[nodiscard]] virtual libv::vec2i size() const noexcept override;
	[[nodiscard]] virtual uint32_t createTexture() const noexcept override;
	virtual ~ImageSOIL() noexcept override = default;
};

std::optional<Image> load_image_SOIL(const std::string_view data) {
	int32_t channels;
	libv::vec2i size;
	std::unique_ptr<uint8_t, decltype(&SOIL_free_image_data)> storage{nullptr, SOIL_free_image_data};
	storage.reset(
			SOIL_load_image_from_memory(
					reinterpret_cast<const unsigned char*>(data.data()),
					data.size(),
					&size.x, &size.y, &channels,
					SOIL_LOAD_AUTO));

	if (storage == nullptr)
		return std::nullopt;
	else
		return std::optional<Image>{std::make_shared<ImageSOIL>(size, channels, std::move(storage))};
}

// -------------------------------------------------------------------------------------------------

libv::vec2i ImageSOIL::size() const noexcept {
	return size_;
}

uint32_t ImageSOIL::createTexture() const noexcept {
	// NOTE: SOIL_create_OGL_texture Fails in OpenGL 3.3 core profile
	// SOIL_create_OGL_texture(storage.get(), size_.x, size_.y, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	// Flip Y axis
	const auto row_size = size_.x * channels;
	auto row_tmp = libv::dynarray<uint8_t>{static_cast<size_t>(row_size)};

	for (int32_t y = 0; y < size_.y / 2; ++y) {
		auto* const tmp = row_tmp.data();
		auto* const front = storage.get() + row_size * y;
		auto* const back = storage.get() + row_size * (size_.y - 1 - y);

		std::memcpy(tmp, front, row_size);
		std::memcpy(front, back, row_size);
		std::memcpy(back, tmp, row_size);
	}

	// Upload texture image
	GLuint textureID = 0;
	GLenum target = GL_TEXTURE_2D;
	GLenum externalType = GL_UNSIGNED_BYTE;
	GLenum externalFormat =
			channels == 4 ? GL_RGBA :
			channels == 3 ? GL_RGB :
			channels == 2 ? GL_RG :
			channels == 1 ? GL_R : 0;
	GLenum internalFormat = externalFormat;

	glGenTextures(1, &textureID);
	glBindTexture(target, textureID);
	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(target, 0, internalFormat, size_.x, size_.y, 0, externalFormat, externalType, storage.get());

	libv::gl::checkGL();
	return textureID;
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
