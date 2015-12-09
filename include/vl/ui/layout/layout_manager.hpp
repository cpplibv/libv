// File: LayoutManager.hpp, Created on 2014. április 24. 13:01, Author: Vader

#pragma once

// pro
#include <vl/ui/component/container.hpp>

namespace vl {
namespace ui {

struct LayoutManager {
	virtual void layout(Container::iterator begin, Container::iterator end, Component* target) = 0;
};

} //namespace ui
} //namespace vl