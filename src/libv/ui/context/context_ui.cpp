// Project: libv.ui, File: src/libv/ui/context/context_ui.cpp

// hpp
#include <libv/ui/context/context_ui.hpp>
// libv
#include <libv/algo/erase_unstable.hpp>
// std
#include <algorithm>
#include <vector>
// pro
#include <libv/ui/ui.hpp> // Only for focus access


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextUI {
public:
	UI& ui; // Only for focus access

	std::vector<const void*> locked_reentry_anchors;

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
	// For the sake of forward declared ptr
}

// -------------------------------------------------------------------------------------------------

void ContextUI::reentry_lock(const void* anchor) {
	self->locked_reentry_anchors.emplace_back(anchor);
}

void ContextUI::reentry_unlock(const void* anchor) noexcept {
	libv::erase_unstable(self->locked_reentry_anchors, anchor);
}

bool ContextUI::reentry_test(const void* anchor) const noexcept {
	const auto it = std::ranges::find(self->locked_reentry_anchors, anchor);
	return it == self->locked_reentry_anchors.end();
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
