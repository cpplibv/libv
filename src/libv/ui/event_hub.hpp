// Project: libv.ui, File: src/libv/ui/ui.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// std
#include <memory>
#include <typeindex>


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
	void _broadcast(std::type_index event_type, const void* event_ptr);
	void _broadcast_in_ui_loop(std::type_index event_type, const void* event_ptr);

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
	_broadcast(std::type_index(typeid(Event)), &event);
}

template <typename Event>
inline void EventHub::broadcast_in_ui_loop(const Event& event) {
	_broadcast_in_ui_loop(std::type_index(typeid(Event)), &event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
