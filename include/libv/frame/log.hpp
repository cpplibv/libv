// File: log.hpp, Created on 2013. október 16. 23:02, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_LOG_FRAME_CORE_TRACE(...) LIBV_TRACE("libv.frame.core", __VA_ARGS__);
#define LIBV_LOG_FRAME_CORE_DEBUG(...) LIBV_DEBUG("libv.frame.core", __VA_ARGS__);
#define LIBV_LOG_FRAME_CORE_INFO( ...) LIBV_INFO( "libv.frame.core", __VA_ARGS__);
#define LIBV_LOG_FRAME_CORE_WARN( ...) LIBV_WARN( "libv.frame.core", __VA_ARGS__);
#define LIBV_LOG_FRAME_CORE_ERROR(...) LIBV_ERROR("libv.frame.core", __VA_ARGS__);
#define LIBV_LOG_FRAME_CORE_FATAL(...) LIBV_FATAL("libv.frame.core", __VA_ARGS__);

#define LIBV_LOG_FRAME_TRACE(...) LIBV_TRACE("libv.frame", __VA_ARGS__);
#define LIBV_LOG_FRAME_DEBUG(...) LIBV_DEBUG("libv.frame", __VA_ARGS__);
#define LIBV_LOG_FRAME_INFO( ...) LIBV_INFO( "libv.frame", __VA_ARGS__);
#define LIBV_LOG_FRAME_WARN( ...) LIBV_WARN( "libv.frame", __VA_ARGS__);
#define LIBV_LOG_FRAME_ERROR(...) LIBV_ERROR("libv.frame", __VA_ARGS__);
#define LIBV_LOG_FRAME_FATAL(...) LIBV_FATAL("libv.frame", __VA_ARGS__);

#define LIBV_LOG_FRAME_EVENT_TRACE(...) LIBV_TRACE("libv.frame.event", __VA_ARGS__);
#define LIBV_LOG_FRAME_EVENT_DEBUG(...) LIBV_DEBUG("libv.frame.event", __VA_ARGS__);
#define LIBV_LOG_FRAME_EVENT_INFO( ...) LIBV_INFO( "libv.frame.event", __VA_ARGS__);
#define LIBV_LOG_FRAME_EVENT_WARN( ...) LIBV_WARN( "libv.frame.event", __VA_ARGS__);
#define LIBV_LOG_FRAME_EVENT_ERROR(...) LIBV_ERROR("libv.frame.event", __VA_ARGS__);
#define LIBV_LOG_FRAME_EVENT_FATAL(...) LIBV_FATAL("libv.frame.event", __VA_ARGS__);

#define LIBV_LOG_FRAME_GLFW_TRACE(...) LIBV_TRACE("libv.ui.glfw", __VA_ARGS__);
#define LIBV_LOG_FRAME_GLFW_DEBUG(...) LIBV_DEBUG("libv.ui.glfw", __VA_ARGS__);
#define LIBV_LOG_FRAME_GLFW_INFO( ...) LIBV_INFO( "libv.ui.glfw", __VA_ARGS__);
#define LIBV_LOG_FRAME_GLFW_WARN( ...) LIBV_WARN( "libv.ui.glfw", __VA_ARGS__);
#define LIBV_LOG_FRAME_GLFW_ERROR(...) LIBV_ERROR("libv.ui.glfw", __VA_ARGS__);
#define LIBV_LOG_FRAME_GLFW_FATAL(...) LIBV_FATAL("libv.ui.glfw", __VA_ARGS__);

namespace libv {
namespace frame {
namespace detail {

template <typename = void>
void errorCallbackGLFW(int code, const char* msg) {
	::libv::log(LIBV_POC, ::libv::Error, "libv.frame.glfw", "%d - %s", code, msg);
}

} // namespace detail
} // namespace frame
} // namespace libv