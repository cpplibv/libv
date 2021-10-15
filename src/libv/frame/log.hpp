// Project: libv.frame, File: src/libv/frame/log.hpp

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

inline LoggerModule log_core{libv::logger_stream, "libv.frame.core"};
inline LoggerModule log_event{libv::logger_stream, "libv.frame.event"};
inline LoggerModule log_frame{libv::logger_stream, "libv.frame"};
inline LoggerModule log_glfw{libv::logger_stream, "libv.frame.glfw"};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
