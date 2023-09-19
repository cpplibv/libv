// Project: libv.ui, File: src/libv/ui/context/context_ui.cpp

// hpp
#include <libv/ui/context/context_ui.hpp>
// pro
#include <libv/ui/ui.hpp> // Only for focus access


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextUI {
public:
	UI& ui; // Only for focus access

public:
	explicit ImplContextUI(UI& ui) :
		ui(ui) {}
};

// -------------------------------------------------------------------------------------------------

ContextUI::ContextUI(UI& ui,
			Settings& settings,
			ContextEvent& event,
			ContextMouse& mouse,
			ContextState& state,
			ContextResource& resource,
			ContextStyle& style) :
	settings(settings),
	event(event),
	mouse(mouse),
	state(state),
	resource(resource),
	style(style),
	self(std::make_unique<ImplContextUI>(ui)) {
}

ContextUI::~ContextUI() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

void ContextUI::focus(CoreComponent& component) {
	self->ui.focus(component);
}

void ContextUI::detachFocused(CoreComponent& component) {
	self->ui.detachFocused(component);
}

void ContextUI::detachFocusLinked(CoreComponent& component) {
	self->ui.detachFocusLinked(component);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
