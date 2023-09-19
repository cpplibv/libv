// Project: libv.gl, File: src/libv/gl/load_image_soil.cpp

// hpp
#include <libv/gl/image.hpp>
// ext
#include <GL/glew.h>
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
	[[nodiscard]] virtual TextureTarget type() const noexcept override;
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

TextureTarget ImageSOIL::type() const noexcept {
	return libv::gl::TextureTarget::_2D;
}

Texture ImageSOIL::createTexture() const noexcept {
	// TODO P3: Would be nice to expose SOIL_FLAG_COMPRESS_TO_DXT
	auto sizeMutable = size_; // SOIL has feature to change the size but that is not exposed, so we ignore it
	auto textureID = SOIL_create_OGL_texture(storage.get(), &sizeMutable.x, &sizeMutable.y, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	const GLenum target = GL_TEXTURE_2D;

	libv::gl::checkGL();
	return {textureID, TextureTarget{target}};
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
