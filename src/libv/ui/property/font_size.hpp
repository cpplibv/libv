// Project: libv.ui, File: src/libv/ui/property/font_size.hpp

#pragma once

// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class FontSize : int16_t {};
[[nodiscard]] constexpr inline auto operator+(FontSize e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
