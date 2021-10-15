// Project: libv.update, File: app/update/common/log.hpp

#pragma once

// libv
#include <libv/log/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_app{libv::logger_stream, "updater"};

// -------------------------------------------------------------------------------------------------

} // namespace app
