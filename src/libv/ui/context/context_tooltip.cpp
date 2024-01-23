// Project: libv.ui, File: src/libv/ui/context/context_tooltip.cpp

// hpp
#include <libv/ui/context/context_tooltip.hpp>
//libv
#include <libv/ui/component/overlay/overlay_tooltip.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ImplContextTooltip {
	OverlayTooltip component{nullptr};
};

// -------------------------------------------------------------------------------------------------

ContextTooltip::ContextTooltip() :
	self(std::make_unique<ImplContextTooltip>()) {
}

ContextTooltip::~ContextTooltip() {
	// For the sake of forward declared unique_ptr
}

void ContextTooltip::init() {
	self->component = OverlayTooltip();
}

Component ContextTooltip::overlay() {
	return self->component;
}

void ContextTooltip::show_tooltip(Component owner, Component tooltip) {
	self->component.show_tooltip(std::move(owner), std::move(tooltip));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
