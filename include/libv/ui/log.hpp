// File: log.hpp Author: Vader Created on 2017. július 5., 22:26

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_ui{libv::logger, "libv.ui"};
inline libv::LoggerModule log_ui_ft{libv::logger, "libv.ui.ft"};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
