// Project: libv.ctrl, File: src/libv/ctrl/duration.hpp

#pragma once

// std
#include <chrono>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

using duration = std::chrono::duration<double, std::chrono::seconds::period>;

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
