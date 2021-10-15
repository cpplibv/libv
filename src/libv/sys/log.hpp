// Project: libv.sys, File: src/libv/sys/log.hpp

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
