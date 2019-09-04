// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/log/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_app{libv::logger_stream, "VM4Viewer"};

// -------------------------------------------------------------------------------------------------

} // namespace app
