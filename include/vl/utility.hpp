// File:   utility.hpp

#pragma once

#include <utility>

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