// Project: libv.utility, File: src/libv/utility/optional_ref.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstddef>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Close enough for now
template <typename T>
using optional_ref = T*;

static constexpr inline std::nullptr_t opt_ref_none = nullptr;

template <typename T>
constexpr inline optional_ref<T> make_optional_ref(T* var) {
	return var;
}

template <typename T>
constexpr inline optional_ref<T> make_optional_ref(T& var) {
	return &var;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
