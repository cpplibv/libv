// Project: libv, File: app/surface/log.hpp

#pragma once

// libv
#include <libv/log/log.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_surface{libv::logger_stream, "surface"};

// -------------------------------------------------------------------------------------------------

} // namespace surface
