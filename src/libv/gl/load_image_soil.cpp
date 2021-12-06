// Project: libv.gl, File: src/libv/gl/load_image_soil.cpp

// hpp
#include <libv/gl/image.hpp>
// ext
#include <SOIL2/SOIL2.h>
// libv
#include <libv/container/dyn_array.hpp>
// std
#include <cstring>
#include <optional>
// pro
#include <libv/gl/check.hpp>
#include <libv/gl/texture_object.hpp>


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
	[[nodiscard]] virtual libv::vec4f pixel(int32_t level, int32_t x, int32_t y) const noexcept override;
	[[nodiscard]] virtual Texture createTexture() const noexcept override;
	virtual ~ImageSOIL() noexcept override = default;
};

std::optional<Image> load_image_SOIL(const std::string_view data) noexcept {
	int32_t channels;
	libv::vec2i size;
	std::unique_ptr<uint8_t, decltype(&SOIL_free_image_data)> storage{nullptr, SOIL_free_image_data};
	storage.reset(
			SOIL_load_image_from_memory(
					reinterpret_cast<const unsigned char*>(data.data()),
					static_cast<int>(data.size()),
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

libv::vec4f ImageSOIL::pixel(int32_t level, int32_t x, int32_t y) const noexcept {
	(void) level;

	const auto raw = storage.get();
	const auto index = size_.x * y * 4 + x * 4;
	return {
		static_cast<float>(raw[index + 0]),
		static_cast<float>(raw[index + 1]),
		static_cast<float>(raw[index + 2]),
		static_cast<float>(raw[index + 3])
	};
}

Texture ImageSOIL::createTexture() const noexcept {
	// NOTE: SOIL_create_OGL_texture Fails in OpenGL 3.3 core profile
	// SOIL_create_OGL_texture(storage.get(), size_.x, size_.y, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	// TODO P3: Would be nice to have on-the-fly compression requests option

	{
		// Flip Y axis
		const auto row_size = size_.x * channels;
		auto row_tmp = libv::dyn_array<uint8_t>{static_cast<std::size_t>(row_size)};

		for (int32_t y = 0; y < size_.y / 2; ++y) {
			auto* const tmp = row_tmp.data();
			auto* const front = storage.get() + row_size * y;
			auto* const back = storage.get() + row_size * (size_.y - 1 - y);

			std::memcpy(tmp, front, row_size);
			std::memcpy(front, back, row_size);
			std::memcpy(back, tmp, row_size);
		}
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
	GLenum Format = externalFormat;

	glGenTextures(1, &textureID);
	glBindTexture(target, textureID);
	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(target, 0, Format, size_.x, size_.y, 0, externalFormat, externalType, storage.get());

	libv::gl::checkGL();
	return {textureID, TextureTarget{target}};
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
