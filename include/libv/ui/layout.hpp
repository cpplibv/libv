// File: layout.hpp Author: Vader Created on 2016. április 6., 18:38

#pragma once

#include <libv/vec.hpp>

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//class Layout {
//private:
//	vec3 containerSize;
//	vec3 componentSize;
//public:
//	Layout(vec3 containerSize) : containerSize(containerSize) { }
//	vec3 getComponentSize() const {
//		return componentSize;
//	}
//	vec3 getSize() const {
//		return containerSize;
//	}
//	void setComponentSize(vec3 size) {
//		componentSize = size;
//	}
//};

struct Layout {
	vec3 size;

	Layout(vec3 size) : size(size) { }
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv