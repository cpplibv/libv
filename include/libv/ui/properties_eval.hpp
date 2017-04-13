// File: properties_eval.hpp Author: Vader Created on 2016. június 26., 18:27

#pragma once

#include <libv/ui/component/component.hpp>
#include <libv/ui/layout.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------
inline vec3f evalLayoutSize(const LayoutInfo& parent, const Component& component) {
	auto size = component.getOrDefault(Property::Size);
	vec3f result;

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

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv