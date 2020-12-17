// Project: libv, File: app/update/patch/log.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_update{libv::logger_stream, "libv.update"};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
