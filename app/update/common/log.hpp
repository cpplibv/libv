// Project: libv, File: app/update/common/log.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/log/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_app{libv::logger_stream, "updater"};

// -------------------------------------------------------------------------------------------------

} // namespace app
