// File:   properties_eval.hpp Author: Vader Created on 2016. június 26., 18:27

#pragma once

#include <libv/ui/component/component.hpp>
#include <libv/ui/layout.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------
inline vec3 implEvalLayoutSize(const LayoutInfo& parent, const LayoutSizeVec& size) {
	vec3 result;

	for (int i = 0; i < 3; i++) {
		switch (size[i].type) {

		case LayoutSize::NOSPEC:
			result[i] = 0;
			break;

		case LayoutSize::PERCENT:
			result[i] = parent.size[i] * size[i].value * 0.01;
			break;

		case LayoutSize::PX:
			result[i] = size[i].value;
			break;
		}
	}
	return result;
}

inline vec3 evalLayoutSize(const LayoutInfo& parentInfo, const Component& component) {
	auto compSizeProperty = component.get(Property::Size);
	if (!compSizeProperty.is_initialized()) {
		LIBV_UI_LAYOUT_WARN("Component [%s] has no Size Property but its parent relays on it. "
				"Default value (100,100,0) is used.", component.getComponentID());
		return vec3(100, 100, 0);
	} else {
		return implEvalLayoutSize(parentInfo, compSizeProperty.value());
	}
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv