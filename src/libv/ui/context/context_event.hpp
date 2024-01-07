// Project: libv.ui, File: src/libv/ui/context/context_event.hpp

#pragma once

#include <libv/ui/context/context_ui.hpp> // IWYU pragma: export // For convenience
#include <libv/utility/nexus.hpp>

#include <memory>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextEvent;

/// Context event is the main UI event hub. Every event signal and slot is connected inside.
///
/// Connection types are:
/// - Global: Connection between UI Global signal and a normal component or an outside callback
/// - Normal: Connection between two component
/// - External: Connection between a component and an outside callback
class ContextEvent {
public:
	libv::Nexus nexus;

	// struct EventBeforeUpdate{};
	// BasicEventProxy<ComponentT, EventBeforeUpdate> before_update{this->owner};
	// struct EventAfterUpdate{};
	// BasicEventProxy<ComponentT, EventAfterUpdate> after_update{this->owner};

private:
	std::unique_ptr<ImplContextEvent> self;

public:
	ContextEvent(libv::Nexus&& nexus);
	~ContextEvent();

public:
	void reentry_lock(const void* anchor);
	void reentry_unlock(const void* anchor) noexcept;
	[[nodiscard]] bool reentry_test(const void* anchor) const noexcept;

public:
	template <typename Event>
	inline void broadcast(const Event& event);
};

// -------------------------------------------------------------------------------------------------

template <typename Event>
inline void ContextEvent::broadcast(const Event& event) {
	nexus.broadcast_global<Event>(event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
