// Project: libv, File: app/space/universe/ids.hpp

#pragma once

// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>


namespace space {

// -------------------------------------------------------------------------------------------------

enum class FactionID : int32_t {};
constexpr inline FactionID invalidFactionID = FactionID{-1};
[[nodiscard]] constexpr inline auto operator+(FactionID e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator++(FactionID& e, int) noexcept { auto r = e; e = FactionID{libv::to_underlying(e) + 1}; return r; }

enum class FleetID : int32_t {};
constexpr inline FleetID invalidFleetID = FleetID{-1};
[[nodiscard]] constexpr inline auto operator+(FleetID e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator++(FleetID& e, int) noexcept { auto r = e; e = FleetID{+e + 1}; return r; }

enum class PlanetID : int32_t {};
constexpr inline PlanetID invalidPlanetID = PlanetID{-1};
[[nodiscard]] constexpr inline auto operator+(PlanetID e) noexcept { return libv::to_underlying(e); }
[[nodiscard]] constexpr inline auto operator++(PlanetID& e, int) noexcept { auto r = e; e = PlanetID{+e + 1}; return r; }

// -------------------------------------------------------------------------------------------------

enum class TickIndex : int64_t {};
[[nodiscard]] constexpr inline auto operator+(TickIndex e) noexcept { return libv::to_underlying(e); }

enum class Timestamp : int64_t {};
[[nodiscard]] constexpr inline auto operator+(Timestamp e) noexcept { return libv::to_underlying(e); }

enum class UserID : int32_t {};
[[nodiscard]] constexpr inline auto operator+(UserID e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace space
