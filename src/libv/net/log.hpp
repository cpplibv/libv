// Project: libv.net, File: src/libv/net/log.hpp, Author: Császár Mátyás [Vader]

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
