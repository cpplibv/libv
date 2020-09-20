// Project: libv.gl, File: src/libv/gl/load_image.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <optional>
#include <string_view>
// pro
#include <libv/gl/image_fwd.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

/// @supported GLI backend: KTX, KMG, DDS
/// @supported SOIL backend: BMP, PNG, JPG, TGA, DDS, PSD, HDR
/// @context ANY
[[nodiscard]] std::optional<Image> load_image(const std::string_view data) noexcept;

/// @supported GLI backend: KTX, KMG, DDS
/// @supported SOIL backend: BMP, PNG, JPG, TGA, DDS, PSD, HDR
/// @context ANY
[[nodiscard]] Image load_image_or(const std::string_view data, const Image& fallback);

/// @supported GLI backend: KTX, KMG, DDS
/// @supported SOIL backend: BMP, PNG, JPG, TGA, DDS, PSD, HDR
/// @context ANY
[[nodiscard]] Image load_image_or_throw(const std::string_view data);

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
