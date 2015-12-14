// File: log.hpp, Created on 2015. március 20. 2:37, Author: Vader

#pragma once

#include <libv/log.hpp>

namespace libv {
namespace vm3 {
inline Log& log() {
	static Log log("vm3", libv::log());
	return log;
}
}
}
