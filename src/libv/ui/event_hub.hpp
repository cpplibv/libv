// Project: libv.ui, File: src/libv/ui/event_hub.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/utility/type_key.hpp>
// std
#include <memory>


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

private:
	void aux_broadcast(uintptr_t event_type, const void* event_ptr);
	void aux_broadcast_in_ui_loop(uintptr_t event_type, const void* event_ptr);

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
	aux_broadcast(libv::type_key<Event>(), &event);
}

template <typename Event>
inline void EventHub::broadcast_in_ui_loop(const Event& event) {
	aux_broadcast_in_ui_loop(libv::type_key<Event>(), &event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
