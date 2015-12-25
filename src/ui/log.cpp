// File: Log.cpp, Created on 2013. augusztus 22. 21:29, Author: Vader

// hpp
#include <libv/ui/log.hpp>
// std
#include <mutex>
#include <set>
#include <sstream>
#include <string>

namespace libv {
namespace ui {
namespace detail {

void errorCallbackGLFW(int code, const char * msg) {
	::libv::log(LIBV_POC, ::libv::Error, "libv.ui", "[GLFW3]: %d - %s", code, msg);
}

} //namespace detail
} //namespace ui
} //namespace libv