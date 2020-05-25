// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <chrono>
// pro
#include <libv/ctrl/duration.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

using time_point = std::chrono::time_point<std::chrono::steady_clock, duration>;

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
