// Project: libv.net, File: src/libv/net/error.hpp

#pragma once

// std
#include <string>
#include <system_error>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

std::string to_string(const std::error_code ec);

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
