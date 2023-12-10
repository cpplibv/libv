// Project: libv.re, File: src/libv/re/common/layer_mask.hpp

#pragma once

#include <cstdint>
#include <string>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct LayerMask {
	static inline std::string layerNames[32] = {
		"DepthCaster",
		"Opaque",
		"Background",
		"Transparent",
		// "Water",
		"OutlineCaster",
		"ShadowCaster",
		"Flare",
		"HUD"
	};

	uint32_t bitMask = 0;

public:
	friend constexpr inline LayerMask operator|(LayerMask lhs, LayerMask rhs) noexcept {
		return LayerMask{lhs.bitMask | rhs.bitMask};
	}
	friend constexpr inline LayerMask operator&(LayerMask lhs, LayerMask rhs) noexcept {
		return LayerMask{lhs.bitMask & rhs.bitMask};
	}
	explicit constexpr inline operator bool() const noexcept {
		return passed();
	}
	[[nodiscard]] constexpr inline bool passed() const noexcept {
		return bitMask != 0;
	}

	[[nodiscard]] constexpr inline bool operator==(const LayerMask& other) const noexcept = default;
};

constexpr LayerMask layerNone{0u};
constexpr LayerMask layerDepthCaster{1u << 0u};
constexpr LayerMask layerNoFrustumCull{1u << 1u};

constexpr LayerMask layerOpaque{1u << 2u};
constexpr LayerMask layerBackground = LayerMask{1u << 3u} | layerNoFrustumCull;
constexpr LayerMask layerTransparent{1u << 4u};
// constexpr LayerMask layerWater{1u << 5u};
constexpr LayerMask layerOutlineCaster{1u << 6u};
constexpr LayerMask layerShadowCaster{1u << 7u};
// constexpr LayerMask layerLight{1u << 8u};
constexpr LayerMask layerFlare = LayerMask{1u << 9u} | layerNoFrustumCull;
constexpr LayerMask layerHUD = LayerMask{1u << 10u} | layerNoFrustumCull;

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
