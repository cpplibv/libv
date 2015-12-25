// File: log.hpp, Created on 2013. október 16. 23:02, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_UI_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.ui", __VA_ARGS__);
#define LIBV_UI_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.ui", __VA_ARGS__);
#define LIBV_UI_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv.ui", __VA_ARGS__);
#define LIBV_UI_WARN( ...) ::libv::log(LIBV_POC, ::libv::Warn , "libv.ui", __VA_ARGS__);
#define LIBV_UI_ERROR(...) ::libv::log(LIBV_POC, ::libv::Error, "libv.ui", __VA_ARGS__);
#define LIBV_UI_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.ui", __VA_ARGS__);

#define LIBV_UI_FT_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info, "libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_WARN( ...) ::libv::log(LIBV_POC, ::libv::Warn, "libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_ERROR(...) ::libv::log(LIBV_POC, ::libv::Error, "libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.ui.ft", __VA_ARGS__);

#define LIBV_UI_GLFW_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_WARN( ...) ::libv::log(LIBV_POC, ::libv::Warn , "libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_ERROR(...) ::libv::log(LIBV_POC, ::libv::Error, "libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.ui.glfw", __VA_ARGS__);

#define LIBV_UI_CORE_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_WARN( ...) ::libv::log(LIBV_POC, ::libv::Warn , "libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_ERROR(...) ::libv::log(LIBV_POC, ::libv::Error, "libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.ui.core", __VA_ARGS__);

#define LIBV_UI_FRAME_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_WARN( ...) ::libv::log(LIBV_POC, ::libv::Warn , "libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_ERROR(...) ::libv::log(LIBV_POC, ::libv::Error, "libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.ui.frame", __VA_ARGS__);

#define LIBV_UI_EVENT_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_WARN( ...) ::libv::log(LIBV_POC, ::libv::Warn , "libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_ERROR(...) ::libv::log(LIBV_POC, ::libv::Error, "libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.ui.event", __VA_ARGS__);

namespace libv {
namespace ui {
namespace detail {
template<typename = void>
void errorCallbackGLFW(int code, const char* msg) {
	::libv::log(LIBV_POC, ::libv::Error, "libv.ui.glfw", "%d - %s", code, msg);
}

} //namespace detail
} //namespace ui
} //namespace libv