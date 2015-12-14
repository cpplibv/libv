// File: LayoutManager.hpp, Created on 2014. április 24. 13:01, Author: Vader

#pragma once

// pro
#include <libv/ui/component/container.hpp>

namespace libv {
namespace ui {

struct LayoutManager {
	virtual void layout(Container::iterator begin, Container::iterator end, Component* target) = 0;
};

} //namespace ui
} //namespace libv