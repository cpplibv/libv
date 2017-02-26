// File: layout.hpp Author: Vader Created on 2016. április 6., 18:38

#pragma once

// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/log.hpp>

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct LayoutInfo {
	vec3f size;
	LayoutInfo(vec3f size = vec3f()) : size(size) { }
	LayoutInfo& operator=(const LayoutInfo& right) = default;
	bool operator!=(const LayoutInfo& rhs) const {
		return size != rhs.size;
	}
	bool operator==(const LayoutInfo& rhs) const {
		return size == rhs.size;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv