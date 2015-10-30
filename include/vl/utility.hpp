// File:   utility.hpp

#pragma once

#include <utility>

// -------------------------------------------------------------------------------------------------

#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// -------------------------------------------------------------------------------------------------

namespace vl {

// -------------------------------------------------------------------------------------------------

template<typename T, typename... Args>
inline T* new_f(Args&&... args) {
	return new T(std::forward<Args>(args)...);
}

struct new_t {
	template<typename T, typename... Args>
	inline T* operator()(Args&&... args) {
		return new T(std::forward<Args>(args)...);
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace vl