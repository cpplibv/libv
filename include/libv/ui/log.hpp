// File: log.hpp, Created on 2013. október 16. 23:02, Author: Vader

#pragma once

#include <libv/log/log.hpp>

#define LIBV_UI_TRACE(...) ::libv::log(LIBV_POC, ::libv::Trace, "libv.ui", __VA_ARGS__);
#define LIBV_UI_DEBUG(...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.ui", __VA_ARGS__);
#define LIBV_UI_INFO( ...) ::libv::log(LIBV_POC, ::libv::Info , "libv.ui", __VA_ARGS__);
#define LIBV_UI_WARN( ...) ::libv::log(LIBV_POC, ::libv::Debug, "libv.ui", __VA_ARGS__);
#define LIBV_UI_ERROR(...) ::libv::log(LIBV_POC, ::libv::Warn , "libv.ui", __VA_ARGS__);
#define LIBV_UI_FATAL(...) ::libv::log(LIBV_POC, ::libv::Fatal, "libv.ui", __VA_ARGS__);

namespace libv {
namespace ui {
namespace detail {

void errorCallbackGLFW(int, const char *);

} //namespace detail
} //namespace ui
} //namespace libv