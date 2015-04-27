// File: Log.cpp, Created on 2013. augusztus 22. 21:29, Author: Vader

// hpp
#include "vl/ui/log.hpp"
// std
#include <mutex>
#include <set>
#include <sstream>
#include <string>

namespace vl {
namespace ui {
namespace detail {

void errorCallbackGLFW(int code, const char * msg) {
	vl::ui::log().log(VLOG_SEVERITY_ERROR, 
			vl::format("[GLFW3]: %d - %s", code, msg), "LIB:GLFW3", "-", -1);
}

} //namespace detail
} //namespace ui
} //namespace vl