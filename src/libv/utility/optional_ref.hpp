// File:   optional_ref.hpp Author: Vader Created on 29 August 2019, 05:43

#pragma once

// libv
#include <libv/utility/observer_ptr.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Close enough for now
template <typename T>
using optional_ref = libv::observer_ptr<T>;

template <typename T>
constexpr inline auto make_optional_ref(T* var) {
	return libv::make_observer(var);
}

template <typename T>
constexpr inline auto make_optional_ref(T& var) {
	return libv::make_observer(var);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
