// Project: libv.ui, File: src/libv/ui/style_state.hpp

#pragma once

// hpp
#include <cstdint>
// libv
#include <libv/utility/to_underlying.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//class tag_StyleState;
//using StyleState_t = libv::flag_enum<uint32_t, tag_StyleState>;
//
//namespace StyleState {
//
//static constexpr StyleState_t none = StyleState_t{};
//
//static constexpr StyleState_t disable  = libv::bit(0);
//
//} // namespace StyleState

enum class StyleState : uint32_t {
	none           = 0,

	disable        = 1u << 0u,
	hover          = 1u << 1u,
	focus          = 1u << 2u,

	active         = 1u << 3u,

	progress       = 1u << 4u,
	done           = 1u << 5u,
	error          = 1u << 6u,

//	_              = 1u << 7u,
//	_              = 1u << 8u,
//	_              = 1u << 9u,
//	_              = 1u << 10u,
//	_              = 1u << 11u,
//	_              = 1u << 12u,
//	_              = 1u << 13u,
//	_              = 1u << 14u,
//	_              = 1u << 15u,

	custom0        = 1u << 16u,
	custom1        = 1u << 17u,
	custom2        = 1u << 18u,
	custom3        = 1u << 19u,
	custom4        = 1u << 20u,
	custom5        = 1u << 21u,
	custom6        = 1u << 22u,
	custom7        = 1u << 23u,
	custom8        = 1u << 24u,
	custom9        = 1u << 25u,
	custom10       = 1u << 26u,
	custom11       = 1u << 27u,
	custom12       = 1u << 28u,
	custom13       = 1u << 29u,
	custom14       = 1u << 30u,
	custom15       = 1u << 31u,
};

[[nodiscard]] constexpr inline StyleState operator|(StyleState lhs, StyleState rhs) noexcept {
	return StyleState{libv::to_underlying(lhs) | libv::to_underlying(rhs)};
}

[[nodiscard]] constexpr inline StyleState operator&(StyleState lhs, StyleState rhs) noexcept {
	return StyleState{libv::to_underlying(lhs) & libv::to_underlying(rhs)};
}

[[nodiscard]] constexpr inline StyleState operator~(StyleState var) noexcept {
	return StyleState{~libv::to_underlying(var)};
}

[[nodiscard]] constexpr inline auto operator+(StyleState var) noexcept {
	return libv::to_underlying(var);
}

/// Returns true if \c state_new will always shadow \c state_old
[[nodiscard]] constexpr inline bool always_shadows(StyleState state_mask_new, StyleState state_value_new, StyleState state_mask_old, StyleState state_value_old) noexcept {
	if (state_mask_new == state_mask_old)
		return (state_mask_new & state_value_new) == (state_mask_old & state_value_old);

	const auto mask_is_smaller = (+state_mask_new & ~+state_mask_old) == 0;
	return mask_is_smaller && (state_mask_new & state_value_new) == (state_mask_new & state_value_old);
}

[[nodiscard]] constexpr inline bool state_matches_requirement(StyleState value, StyleState requirement_mask, StyleState requirement_value) noexcept {
	return (value & requirement_mask) == (requirement_value & requirement_mask);
}

using StyleState_t = StyleState;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
