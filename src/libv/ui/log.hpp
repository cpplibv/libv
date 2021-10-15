// Project: libv.ui, File: src/libv/ui/log.hpp

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_ui{libv::logger_stream, "libv.ui"};
inline libv::LoggerModule log_ui_ft{libv::logger_stream, "libv.ui.ft"};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
