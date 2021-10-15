// Project: libv, File: app/theme/log.hpp

#pragma once

// libv
#include <libv/log/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_app{libv::logger_stream, "gen_ui_theme"};

// -------------------------------------------------------------------------------------------------

} // namespace app
