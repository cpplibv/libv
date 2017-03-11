// File: log.hpp, Created on 2013. október 16. 23:02, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_LOG_UI_TRACE(...) LIBV_TRACE("libv.ui", __VA_ARGS__);
#define LIBV_LOG_UI_DEBUG(...) LIBV_DEBUG("libv.ui", __VA_ARGS__);
#define LIBV_LOG_UI_INFO( ...) LIBV_INFO( "libv.ui", __VA_ARGS__);
#define LIBV_LOG_UI_WARN( ...) LIBV_WARN( "libv.ui", __VA_ARGS__);
#define LIBV_LOG_UI_ERROR(...) LIBV_ERROR("libv.ui", __VA_ARGS__);
#define LIBV_LOG_UI_FATAL(...) LIBV_FATAL("libv.ui", __VA_ARGS__);

#define LIBV_LOG_UI_FT_TRACE(...) LIBV_TRACE("libv.ui.ft", __VA_ARGS__);
#define LIBV_LOG_UI_FT_DEBUG(...) LIBV_DEBUG("libv.ui.ft", __VA_ARGS__);
#define LIBV_LOG_UI_FT_INFO( ...) LIBV_INFO( "libv.ui.ft", __VA_ARGS__);
#define LIBV_LOG_UI_FT_WARN( ...) LIBV_WARN( "libv.ui.ft", __VA_ARGS__);
#define LIBV_LOG_UI_FT_ERROR(...) LIBV_ERROR("libv.ui.ft", __VA_ARGS__);
#define LIBV_LOG_UI_FT_FATAL(...) LIBV_FATAL("libv.ui.ft", __VA_ARGS__);

#define LIBV_LOG_UI_COMPONENT_TRACE(...) LIBV_TRACE("libv.ui.component", __VA_ARGS__);
#define LIBV_LOG_UI_COMPONENT_DEBUG(...) LIBV_DEBUG("libv.ui.component", __VA_ARGS__);
#define LIBV_LOG_UI_COMPONENT_INFO( ...) LIBV_INFO( "libv.ui.component", __VA_ARGS__);
#define LIBV_LOG_UI_COMPONENT_WARN( ...) LIBV_WARN( "libv.ui.component", __VA_ARGS__);
#define LIBV_LOG_UI_COMPONENT_ERROR(...) LIBV_ERROR("libv.ui.component", __VA_ARGS__);
#define LIBV_LOG_UI_COMPONENT_FATAL(...) LIBV_FATAL("libv.ui.component", __VA_ARGS__);

#define LIBV_LOG_UI_RESOURCE_TRACE(...) LIBV_TRACE("libv.ui.resource", __VA_ARGS__);
#define LIBV_LOG_UI_RESOURCE_DEBUG(...) LIBV_DEBUG("libv.ui.resource", __VA_ARGS__);
#define LIBV_LOG_UI_RESOURCE_INFO( ...) LIBV_INFO( "libv.ui.resource", __VA_ARGS__);
#define LIBV_LOG_UI_RESOURCE_WARN( ...) LIBV_WARN( "libv.ui.resource", __VA_ARGS__);
#define LIBV_LOG_UI_RESOURCE_ERROR(...) LIBV_ERROR("libv.ui.resource", __VA_ARGS__);
#define LIBV_LOG_UI_RESOURCE_FATAL(...) LIBV_FATAL("libv.ui.resource", __VA_ARGS__);

#define LIBV_LOG_UI_LAYOUT_TRACE(...) LIBV_TRACE("libv.ui.layout", __VA_ARGS__);
#define LIBV_LOG_UI_LAYOUT_DEBUG(...) LIBV_DEBUG("libv.ui.layout", __VA_ARGS__);
#define LIBV_LOG_UI_LAYOUT_INFO( ...) LIBV_INFO( "libv.ui.layout", __VA_ARGS__);
#define LIBV_LOG_UI_LAYOUT_WARN( ...) LIBV_WARN( "libv.ui.layout", __VA_ARGS__);
#define LIBV_LOG_UI_LAYOUT_ERROR(...) LIBV_ERROR("libv.ui.layout", __VA_ARGS__);
#define LIBV_LOG_UI_LAYOUT_FATAL(...) LIBV_FATAL("libv.ui.layout", __VA_ARGS__);
