// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <chrono>


namespace libv {
namespace control {

// -------------------------------------------------------------------------------------------------

using duration = std::chrono::duration<double, std::chrono::seconds::period>;

// -------------------------------------------------------------------------------------------------

} //namespace control
} //namespace libv
