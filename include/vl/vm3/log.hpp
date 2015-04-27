// File: log.hpp, Created on 2015. március 20. 2:37, Author: Vader

#pragma once

#include <vl/log.hpp>

namespace vl {
namespace vm3 {
inline Log& log() {
	static Log log("vm3", vl::log());
	return log;
}
}
}
