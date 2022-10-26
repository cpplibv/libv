// Project: libv.ui, File: src/libv/ui/event_system/event_hub.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/utility/type_key.hpp>
// std
#include <memory>
// pro
#include <libv/ui/context/context_event.hpp>
#include <libv/ui/context/context_ui_link.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class EventHub {
	ContextUI* contextUI;
	std::weak_ptr<ContextEvent> contextEvent;

public:
	explicit inline EventHub(ContextUI* contextUI, std::weak_ptr<ContextEvent>&& wp) :
		contextUI(contextUI),
		contextEvent(std::move(wp)) { }

public:
	/// Enters the UI context
	/// Broadcasts the event synchronously
	template <typename Event>
	inline void broadcast(const Event& event);

	/// Broadcasts the event asynchronously as part of the event loop
	template <typename Event>
	inline void broadcast_in_ui_loop(const Event& event);
};

// -------------------------------------------------------------------------------------------------

template <typename Event>
inline void EventHub::broadcast(const Event& event) {
	const auto sp = contextEvent.lock();
	if (sp) {
		auto* previous_context = has_current_thread_context() ? &current_thread_context() : nullptr;
		current_thread_context(*contextUI);

		sp->nexus.broadcast_global<Event>(event);

		if (previous_context == nullptr)
			clear_current_thread_context();
		else
			current_thread_context(*previous_context);
	}
}

//template <typename Event>
//inline void EventHub::broadcast_in_ui_loop(const Event& event) {
////	aux_broadcast_in_ui_loop(libv::type_key<Event>(), &event);
//}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
