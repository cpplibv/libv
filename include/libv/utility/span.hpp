// File: span.hpp

#pragma once

// ext
#include <range/v3/span.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
using span = ranges::span<T>; // C++20

// -------------------------------------------------------------------------------------------------

} // namespace libv
