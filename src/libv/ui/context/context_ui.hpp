// Project: libv.ui, File: src/libv/ui/context/context_ui.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// std
#include <memory>
// pro
#include <libv/ui/context/context_event.hpp> // Because of broadcast


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

private:
	std::unique_ptr<ImplContextUI> self;

public:
	explicit ContextUI(UI& ui,
			Settings& settings,
			ContextEvent& event,
			ContextMouse& mouse,
			ContextState& state,
			ContextResource& resource,
			ContextStyle& style);
	~ContextUI();

	ContextUI(const ContextUI&) = delete;
	ContextUI(ContextUI&&) = delete;
	ContextUI& operator=(const ContextUI&) = delete;
	ContextUI& operator=(ContextUI&&) = delete;

public:
	// TODO P4: Move to the event context
	void reentry_lock(const void* anchor);
	void reentry_unlock(const void* anchor) noexcept;
	[[nodiscard]] bool reentry_test(const void* anchor) const noexcept;

public:
	// TODO P4: Move to a new focus context
	void focus(CoreComponent& component);
	void detachFocused(CoreComponent& component);
	void detachFocusLinked(CoreComponent& component);

public:
	// TODO P4: Move to the event context
	template <typename Event>
	inline void broadcast(const Event& event);
};

// -------------------------------------------------------------------------------------------------

template <typename Event>
inline void ContextUI::broadcast(const Event& event) {
	this->event.nexus.broadcast_global<Event>(event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
