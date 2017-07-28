// File: layout_line.hpp Author: Vader Created on 2017. augusztus 26., 23:33

#pragma once

// pro
#include <libv/ui/module_layout.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ComponentBase;
class ContextLayoutPass1;
class ContextLayoutPass2;

struct LayoutLine : ModuleLayout {
	Alignment alignHorizontal = {Alignment::LEFT};
	Alignment alignVertical = {Alignment::TOP};
	Orientation orient = {Orientation::LEFT_TO_RIGHT};

	void layoutPass1(const ContextLayoutPass1& environment, ComponentBase& component) override;
	void layoutPass2(const ContextLayoutPass2& environment, ComponentBase& component) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
