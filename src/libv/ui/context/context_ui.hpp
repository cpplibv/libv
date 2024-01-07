// Project: libv.ui, File: src/libv/ui/context/context_ui.hpp

#pragma once

#include <libv/ui/fwd.hpp>
#include <libv/utility/nexus_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextUI;

class ContextUI {
public:
	libv::Nexus& nexus;
	Settings& settings;

	ContextFocus& focus;
	ContextEvent& event;
	ContextMouse& mouse;
	ContextState& state;
	ContextResource& resource;
	// ContextRender& render; // No need to expose render context for now
	ContextStyle& style;
	ContextTooltip& tooltip;

public:
	ContextUI(
			libv::Nexus& nexus,
			Settings& settings,
			ContextFocus& focus,
			ContextEvent& event,
			ContextMouse& mouse,
			ContextState& state,
			ContextResource& resource,
			ContextStyle& style,
			ContextTooltip& tooltip);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
