// Project: libv, File: app/space/sim/kredit.hpp

#pragma once

// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>


namespace space {

// -------------------------------------------------------------------------------------------------

enum class kredit : int64_t {};
[[nodiscard]] constexpr inline auto operator+(kredit e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace space
