// File: log.hpp, Created on 2015. március 20. 2:37, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_VM3_TRACE(...) LIBV_TRACE("libv.vm3", __VA_ARGS__);
#define LIBV_VM3_DEBUG(...) LIBV_DEBUG("libv.vm3", __VA_ARGS__);
#define LIBV_VM3_INFO( ...) LIBV_INFO( "libv.vm3", __VA_ARGS__);
#define LIBV_VM3_WARN( ...) LIBV_WARN( "libv.vm3", __VA_ARGS__);
#define LIBV_VM3_ERROR(...) LIBV_ERROR("libv.vm3", __VA_ARGS__);
#define LIBV_VM3_FATAL(...) LIBV_FATAL("libv.vm3", __VA_ARGS__);
