// Project: libv.net, File: src/libv/net/log.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace sys {

// -------------------------------------------------------------------------------------------------

inline LoggerModule log_sys{libv::logger_stream, "libv.sys"};

// -------------------------------------------------------------------------------------------------

} // namespace sys
} // namespace libv
