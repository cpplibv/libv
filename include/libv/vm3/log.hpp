// File: log.hpp, Created on 2015. március 20. 2:37, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_LOG_VM3_TRACE(...) LIBV_LOG_BASE_TRACE("libv.vm3", __VA_ARGS__);
#define LIBV_LOG_VM3_DEBUG(...) LIBV_LOG_BASE_DEBUG("libv.vm3", __VA_ARGS__);
#define LIBV_LOG_VM3_INFO( ...) LIBV_LOG_BASE_INFO( "libv.vm3", __VA_ARGS__);
#define LIBV_LOG_VM3_WARN( ...) LIBV_LOG_BASE_WARN( "libv.vm3", __VA_ARGS__);
#define LIBV_LOG_VM3_ERROR(...) LIBV_LOG_BASE_ERROR("libv.vm3", __VA_ARGS__);
#define LIBV_LOG_VM3_FATAL(...) LIBV_LOG_BASE_FATAL("libv.vm3", __VA_ARGS__);
