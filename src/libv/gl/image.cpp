// File: image.cpp - Created on 2017.11.06. 12:06 - Author: Vader

// hpp
#include <libv/gl/image.hpp>
// pro
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

std::optional<Image> load_image_GLI(const std::string_view data) noexcept;
std::optional<Image> load_image_SOIL(const std::string_view data) noexcept;

std::optional<Image> load_image(const std::string_view data) noexcept {
	std::optional<Image> result;

	if (!result)
		result = load_image_GLI(data);

	if (!result)
		result = load_image_SOIL(data);

	if (!result)
		log_gl.error("Corrupted image or not supported format");

	return result;
}

Image load_image_or_throw(const std::string_view data) {
	auto result = load_image(data);

	if (result.has_value())
		return std::move(result.value());
	else
		throw std::invalid_argument{"Corrupted image or not supported format"};
}

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
