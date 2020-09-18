// Project: libv.net, File: src/libv/net/error.hpp, Author: Császár Mátyás [Vader]

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
