// File:   error.hpp Author: Vader Created on 2018. december 4., 5:52

#pragma once

// std
#include <system_error>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

std::string to_string(const std::error_code ec);

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
