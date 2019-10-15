// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct EventFocus {
	bool focus = false;

	[[nodiscard]] constexpr inline bool gain() const noexcept {
		return focus;
	}

	[[nodiscard]] constexpr inline bool loss() const noexcept {
		return !focus;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
