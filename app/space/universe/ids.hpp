// Project: libv, File: app/space/command.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>


namespace app {

// -------------------------------------------------------------------------------------------------

enum class FrameIndex : int64_t {};
enum class Timestamp : int64_t {};

enum class PlayerID : int32_t {};

enum class FactionID : int32_t {};
enum class FleetID : int32_t {};

[[nodiscard]] constexpr inline auto operator+(FrameIndex e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator+(Timestamp e) noexcept { return libv::to_underlying(e); }

[[nodiscard]] constexpr inline auto operator+(PlayerID e) noexcept { return libv::to_underlying(e); }

[[nodiscard]] constexpr inline auto operator+(FactionID e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator+(FleetID e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace app
