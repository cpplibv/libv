// Project: libv.ui, File: src/libv/ui/context/context_ui.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// std
#include <memory>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextUI;

class ContextUI {
public:
	Settings& settings;

	ContextEvent& event;
	ContextMouse& mouse;
	ContextState& state;
	ContextResource& resource;
	// ContextRender& render; // No need to expose render context for now
	ContextStyle& style;
	ContextTooltip& tooltip;

private:
	std::unique_ptr<ImplContextUI> self;

public:
	explicit ContextUI(UI& ui,
			Settings& settings,
			ContextEvent& event,
			ContextMouse& mouse,
			ContextState& state,
			ContextResource& resource,
			ContextStyle& style,
			ContextTooltip& tooltip);
	~ContextUI();

	ContextUI(const ContextUI&) = delete;
	ContextUI(ContextUI&&) = delete;
	ContextUI& operator=(const ContextUI&) = delete;
	ContextUI& operator=(ContextUI&&) = delete;

public:
	// TODO P4: Move to a new focus context
	void focus(CoreComponent& component);
	void detachFocused(CoreComponent& component);
	void detachFocusLinked(CoreComponent& component);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
