// Project: libv.re, File: src/libv/re/log.hpp

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

#define LIBV_RE_ASSERT(...) assert(__VA_ARGS__)

inline LoggerModule log_re{libv::logger_stream, "libv.re"};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
