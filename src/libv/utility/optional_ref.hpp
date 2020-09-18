// Project: libv.utility, File: src/libv/utility/optional_ref.hpp, Author: Császár Mátyás [Vader]

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
