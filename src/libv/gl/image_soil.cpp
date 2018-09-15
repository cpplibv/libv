// File: soil_loader.cpp - Created on 2017.11.05. 11:24 - Author: Vader

// hpp
#include <libv/gl/image.hpp>
// ext
#include <SOIL/SOIL.h>
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
	const auto id = SOIL_create_OGL_texture(storage.get(), size_.x, size_.y, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	LIBV_GL_DEBUG_CHECK();
	return id;
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
