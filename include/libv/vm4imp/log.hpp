// File: log.hpp, Created on 2015. március 20. 2:37, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_LOG_VM4_TRACE(...) LIBV_LOG_BASE_TRACE("libv.vm4", __VA_ARGS__);
#define LIBV_LOG_VM4_DEBUG(...) LIBV_LOG_BASE_DEBUG("libv.vm4", __VA_ARGS__);
#define LIBV_LOG_VM4_INFO( ...) LIBV_LOG_BASE_INFO( "libv.vm4", __VA_ARGS__);
#define LIBV_LOG_VM4_WARN( ...) LIBV_LOG_BASE_WARN( "libv.vm4", __VA_ARGS__);
#define LIBV_LOG_VM4_ERROR(...) LIBV_LOG_BASE_ERROR("libv.vm4", __VA_ARGS__);
#define LIBV_LOG_VM4_FATAL(...) LIBV_LOG_BASE_FATAL("libv.vm4", __VA_ARGS__);
