// File: log.hpp Author: Vader Created on 2017. április 13., 21:28

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

inline LoggerModule log_net{libv::logger_stream, "libv.net"};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
