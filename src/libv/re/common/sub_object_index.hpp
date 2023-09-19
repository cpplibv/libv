// Project: libv.re, File: src/libv/re/common/sub_object_index.hpp

#pragma once

#include <libv/utility/to_underlying.hpp>

#include <cstdint>


namespace libv::re { // --------------------------------------------------------------------------

enum class SubObjectIndex : uint32_t {};
[[nodiscard]] constexpr inline auto operator+(SubObjectIndex e) noexcept { return libv::to_underlying(e); }

} // namespace libv::re --------------------------------------------------------------------------
