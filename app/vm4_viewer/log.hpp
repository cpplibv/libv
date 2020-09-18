// Project: libv, File: app/vm4_viewer/log.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/log/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_app{libv::logger_stream, "VM4Viewer"};

// -------------------------------------------------------------------------------------------------

} // namespace app
