// File: log.hpp, Created on 2013. október 16. 23:02, Author: Vader

#pragma once

#include <libv/log.hpp>

namespace libv {
namespace ui {

inline Log& log() {
	static Log log("ui", libv::log());
	return log;
}

namespace detail {

void errorCallbackGLFW(int, const char *);

} //namespace detail
} //namespace ui
} //namespace libv