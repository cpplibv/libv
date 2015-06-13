// File:   operator.hpp

#pragma once

#include <functional>

namespace vl {

// -------------------------------------------------------------------------------------------------

using greater = std::greater<void>;
using greater_equal = std::greater_equal<void>;
using less = std::less<void>;
using less_equal = std::less_equal<void>;

// -------------------------------------------------------------------------------------------------

} //namespace vl