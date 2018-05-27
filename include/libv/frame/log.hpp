// File: log.hpp, Created on 2013. október 16. 23:02, Author: Vader

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

inline LoggerModule log_core{libv::logger, "libv.frame.core"};
inline LoggerModule log_event{libv::logger, "libv.frame.event"};
inline LoggerModule log_frame{libv::logger, "libv.frame"};
inline LoggerModule log_glfw{libv::logger, "libv.frame.glfw"};

#define LIBV_LOG_FRAME_CORE_TRACE(...) ::libv::frame::log_core.trace(__VA_ARGS__)
#define LIBV_LOG_FRAME_CORE_DEBUG(...) ::libv::frame::log_core.debug(__VA_ARGS__)
#define LIBV_LOG_FRAME_CORE_INFO( ...) ::libv::frame::log_core.info (__VA_ARGS__)
#define LIBV_LOG_FRAME_CORE_WARN( ...) ::libv::frame::log_core.warn (__VA_ARGS__)
#define LIBV_LOG_FRAME_CORE_ERROR(...) ::libv::frame::log_core.error(__VA_ARGS__)
#define LIBV_LOG_FRAME_CORE_FATAL(...) ::libv::frame::log_core.fatal(__VA_ARGS__)

#define LIBV_LOG_FRAME_EVENT_TRACE(...) ::libv::frame::log_event.trace(__VA_ARGS__)
#define LIBV_LOG_FRAME_EVENT_DEBUG(...) ::libv::frame::log_event.debug(__VA_ARGS__)
#define LIBV_LOG_FRAME_EVENT_INFO( ...) ::libv::frame::log_event.info (__VA_ARGS__)
#define LIBV_LOG_FRAME_EVENT_WARN( ...) ::libv::frame::log_event.warn (__VA_ARGS__)
#define LIBV_LOG_FRAME_EVENT_ERROR(...) ::libv::frame::log_event.error(__VA_ARGS__)
#define LIBV_LOG_FRAME_EVENT_FATAL(...) ::libv::frame::log_event.fatal(__VA_ARGS__)

#define LIBV_LOG_FRAME_TRACE(...) ::libv::frame::log_frame.trace(__VA_ARGS__)
#define LIBV_LOG_FRAME_DEBUG(...) ::libv::frame::log_frame.debug(__VA_ARGS__)
#define LIBV_LOG_FRAME_INFO( ...) ::libv::frame::log_frame.info (__VA_ARGS__)
#define LIBV_LOG_FRAME_WARN( ...) ::libv::frame::log_frame.warn (__VA_ARGS__)
#define LIBV_LOG_FRAME_ERROR(...) ::libv::frame::log_frame.error(__VA_ARGS__)
#define LIBV_LOG_FRAME_FATAL(...) ::libv::frame::log_frame.fatal(__VA_ARGS__)

#define LIBV_LOG_FRAME_GLFW_TRACE(...) ::libv::frame::log_glfw.trace(__VA_ARGS__)
#define LIBV_LOG_FRAME_GLFW_DEBUG(...) ::libv::frame::log_glfw.debug(__VA_ARGS__)
#define LIBV_LOG_FRAME_GLFW_INFO( ...) ::libv::frame::log_glfw.info (__VA_ARGS__)
#define LIBV_LOG_FRAME_GLFW_WARN( ...) ::libv::frame::log_glfw.warn (__VA_ARGS__)
#define LIBV_LOG_FRAME_GLFW_ERROR(...) ::libv::frame::log_glfw.error(__VA_ARGS__)
#define LIBV_LOG_FRAME_GLFW_FATAL(...) ::libv::frame::log_glfw.fatal(__VA_ARGS__)

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
