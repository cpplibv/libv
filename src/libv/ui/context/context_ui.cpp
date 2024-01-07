// Project: libv.ui, File: src/libv/ui/context/context_ui.cpp

#include <libv/ui/context/context_ui.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

ContextUI::ContextUI(
			libv::Nexus& nexus,
			Settings& settings,
			ContextFocus& focus,
			ContextEvent& event,
			ContextMouse& mouse,
			ContextState& state,
			ContextResource& resource,
			ContextStyle& style,
			ContextTooltip& tooltip) :
	nexus(nexus),
	settings(settings),
	focus(focus),
	event(event),
	mouse(mouse),
	state(state),
	resource(resource),
	style(style),
	tooltip(tooltip) {
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
