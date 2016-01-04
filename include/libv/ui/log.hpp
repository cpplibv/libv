// File: log.hpp, Created on 2013. október 16. 23:02, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_UI_TRACE(...) LIBV_TRACE("libv.ui", __VA_ARGS__);
#define LIBV_UI_DEBUG(...) LIBV_DEBUG("libv.ui", __VA_ARGS__);
#define LIBV_UI_INFO( ...) LIBV_INFO( "libv.ui", __VA_ARGS__);
#define LIBV_UI_WARN( ...) LIBV_WARN( "libv.ui", __VA_ARGS__);
#define LIBV_UI_ERROR(...) LIBV_ERROR("libv.ui", __VA_ARGS__);
#define LIBV_UI_FATAL(...) LIBV_FATAL("libv.ui", __VA_ARGS__);

#define LIBV_UI_FT_TRACE(...) LIBV_TRACE("libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_DEBUG(...) LIBV_DEBUG("libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_INFO( ...) LIBV_INFO( "libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_WARN( ...) LIBV_WARN( "libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_ERROR(...) LIBV_ERROR("libv.ui.ft", __VA_ARGS__);
#define LIBV_UI_FT_FATAL(...) LIBV_FATAL("libv.ui.ft", __VA_ARGS__);

#define LIBV_UI_GLFW_TRACE(...) LIBV_TRACE("libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_DEBUG(...) LIBV_DEBUG("libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_INFO( ...) LIBV_INFO( "libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_WARN( ...) LIBV_WARN( "libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_ERROR(...) LIBV_ERROR("libv.ui.glfw", __VA_ARGS__);
#define LIBV_UI_GLFW_FATAL(...) LIBV_FATAL("libv.ui.glfw", __VA_ARGS__);

#define LIBV_UI_CORE_TRACE(...) LIBV_TRACE("libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_DEBUG(...) LIBV_DEBUG("libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_INFO( ...) LIBV_INFO( "libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_WARN( ...) LIBV_WARN( "libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_ERROR(...) LIBV_ERROR("libv.ui.core", __VA_ARGS__);
#define LIBV_UI_CORE_FATAL(...) LIBV_FATAL("libv.ui.core", __VA_ARGS__);

#define LIBV_UI_FRAME_TRACE(...) LIBV_TRACE("libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_DEBUG(...) LIBV_DEBUG("libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_INFO( ...) LIBV_INFO( "libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_WARN( ...) LIBV_WARN( "libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_ERROR(...) LIBV_ERROR("libv.ui.frame", __VA_ARGS__);
#define LIBV_UI_FRAME_FATAL(...) LIBV_FATAL("libv.ui.frame", __VA_ARGS__);

#define LIBV_UI_EVENT_TRACE(...) LIBV_TRACE("libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_DEBUG(...) LIBV_DEBUG("libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_INFO( ...) LIBV_INFO( "libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_WARN( ...) LIBV_WARN( "libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_ERROR(...) LIBV_ERROR("libv.ui.event", __VA_ARGS__);
#define LIBV_UI_EVENT_FATAL(...) LIBV_FATAL("libv.ui.event", __VA_ARGS__);

#define LIBV_UI_COMPONENT_TRACE(...) LIBV_TRACE("libv.ui.component", __VA_ARGS__);
#define LIBV_UI_COMPONENT_DEBUG(...) LIBV_DEBUG("libv.ui.component", __VA_ARGS__);
#define LIBV_UI_COMPONENT_INFO( ...) LIBV_INFO( "libv.ui.component", __VA_ARGS__);
#define LIBV_UI_COMPONENT_WARN( ...) LIBV_WARN( "libv.ui.component", __VA_ARGS__);
#define LIBV_UI_COMPONENT_ERROR(...) LIBV_ERROR("libv.ui.component", __VA_ARGS__);
#define LIBV_UI_COMPONENT_FATAL(...) LIBV_FATAL("libv.ui.component", __VA_ARGS__);

#define LIBV_UI_LAYOUT_TRACE(...) LIBV_TRACE("libv.ui.layout", __VA_ARGS__);
#define LIBV_UI_LAYOUT_DEBUG(...) LIBV_DEBUG("libv.ui.layout", __VA_ARGS__);
#define LIBV_UI_LAYOUT_INFO( ...) LIBV_INFO( "libv.ui.layout", __VA_ARGS__);
#define LIBV_UI_LAYOUT_WARN( ...) LIBV_WARN( "libv.ui.layout", __VA_ARGS__);
#define LIBV_UI_LAYOUT_ERROR(...) LIBV_ERROR("libv.ui.layout", __VA_ARGS__);
#define LIBV_UI_LAYOUT_FATAL(...) LIBV_FATAL("libv.ui.layout", __VA_ARGS__);

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