// Project: libv.ctrl, File: src/libv/ctrl/time_point.hpp

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
