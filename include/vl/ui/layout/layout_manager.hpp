// File: LayoutManager.hpp, Created on 2014. április 24. 13:01, Author: Vader

#pragma once

// vl
#include <vl/memory.hpp>
#include <vl/vec.hpp>
// pro
#include "vl/ui/component/container.hpp"


namespace vl {
namespace ui {

struct LayoutManager {
	virtual void layout(Container::iterator begin, Container::iterator end, Component* target) = 0;
};

using LayoutManagerPtr = ::vl::shared_or_raw_ptr<LayoutManager>;

} //namespace ui
} //namespace vl