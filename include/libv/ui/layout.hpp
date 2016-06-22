// File: layout.hpp Author: Vader Created on 2016. április 6., 18:38

#pragma once

#include <libv/vec.hpp>

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct LayoutInfo {
	vec3 size;

	LayoutInfo(vec3 size = vec3()) : size(size) { }
	LayoutInfo& operator=(const LayoutInfo& right) = default;

	bool operator!=(const LayoutInfo& rhs) const {
		return size != rhs.size;
	}
	bool operator==(const LayoutInfo& rhs) const {
		return size == rhs.size;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv