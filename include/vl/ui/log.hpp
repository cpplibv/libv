// File: log.hpp, Created on 2013. október 16. 23:02, Author: Vader

#pragma once

#include "vl/log.hpp"

namespace vl {
namespace ui {

inline Log& log() {
	static Log log("ui", vl::log());
	return log;
}

namespace detail {

void errorCallbackGLFW(int, const char *);

} //namespace detail
} //namespace ui
} //namespace vl