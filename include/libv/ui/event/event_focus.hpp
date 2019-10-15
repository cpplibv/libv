// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// libv
#include <libv/math/angle.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/observer_ref.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BaseComponent;

struct EventFocus {
	bool gain = false;
	bool loss = false;

	libv::observer_ref<BaseComponent> previous;
	libv::observer_ref<BaseComponent> current;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
