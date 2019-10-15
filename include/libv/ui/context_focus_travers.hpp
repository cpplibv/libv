// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/math/angle.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/observer_ref.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BaseComponent;

struct ContextFocusTravers {
	Degrees<float> direction;
	// libv::vec3f direction;

public:
	constexpr inline bool isForward() const noexcept {
		return Degrees<float>(45).value > direction.value || direction.value >= Degrees<float>(225).value;
	}

	constexpr inline bool isBackward() const noexcept {
		return Degrees<float>(45).value <= direction.value && direction.value < Degrees<float>(225).value;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
