// Project: libv.re, File: src/libv/re/node/light/light_type.hpp

#pragma once

#include <libv/utility/to_underlying.hpp>

#include <cstdint>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

enum class LightType : uint32_t {
	disable = 0,

	directional = 1,
	point = 2,
	spot = 3,
};
[[nodiscard]] constexpr inline auto operator+(LightType e) noexcept { return libv::to_underlying(e); }

enum class LightAttenuationFormula : uint32_t {
	/// attenuationParams.x = range
	/// 	(NOTE: Range is not limiting! It represents where the initial light drops to 25%)
	/// 1.0 / (1.0 + distance * (2.0 / radius) + distance * distance * (1.0 / (radius * radius)))
	/// = radius * radius / (distance + radius) * (distance + radius))
	physical = 0,

	/// attenuationParams.x = range
	/// attenuationParams.y = falloff
	lisyarus = 1,

	/// attenuationParams.x = range
	/// attenuationParams.y = falloff
	lisyarusSharp = 2,
};
[[nodiscard]] constexpr inline auto operator+(LightAttenuationFormula e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
