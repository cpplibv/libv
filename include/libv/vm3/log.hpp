// File: log.hpp, Created on 2015. március 20. 2:37, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_VM3_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.vm3", __VA_ARGS__);
#define LIBV_VM3_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.vm3", __VA_ARGS__);
#define LIBV_VM3_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv.vm3", __VA_ARGS__);
#define LIBV_VM3_WARN( ...) ::libv::log(LIBV_POC, ::libv::Warn , "libv.vm3", __VA_ARGS__);
#define LIBV_VM3_ERROR(...) ::libv::log(LIBV_POC, ::libv::Error, "libv.vm3", __VA_ARGS__);
#define LIBV_VM3_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.vm3", __VA_ARGS__);
