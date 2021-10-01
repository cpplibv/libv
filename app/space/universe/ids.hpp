// Project: libv, File: app/space/universe/ids.hpp

#pragma once

// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>


namespace app {

// -------------------------------------------------------------------------------------------------

enum class FactionID : int32_t {};
enum class FleetID : int32_t {};

[[nodiscard]] constexpr inline auto operator+(FactionID e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator+(FleetID e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

enum class TickIndex : int64_t {};

enum class Timestamp : int64_t {};

enum class UserID : int32_t {};

[[nodiscard]] constexpr inline auto operator+(TickIndex e) noexcept { return libv::to_underlying(e); }

[[nodiscard]] constexpr inline auto operator+(Timestamp e) noexcept { return libv::to_underlying(e); }

[[nodiscard]] constexpr inline auto operator+(UserID e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace app
