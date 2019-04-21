// File: panel.cpp - Created on 2017.11.05. 07:48 - Author: Vader

// hpp
#include <libv/ui/component/panel.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

Panel::Panel() :
	ComponentStatic<Panel>(UnnamedTag{}, "panel") {
}

Panel::Panel(std::string name) :
	ComponentStatic<Panel>(std::move(name)) {
}

void Panel::add(std::shared_ptr<ComponentBase> component) {
	const auto& comp = components.emplace_back(std::move(component));

	// TODO P3: libv.ui: implement setParent(this)
	comp->parent = libv::make_observer_ref(this);
	invalidate(comp->flags & Flag::mask_propagateUp);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
