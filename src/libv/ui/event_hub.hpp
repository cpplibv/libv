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
	std::weak_ptr<ContextEvent> contextEvent;

public:
	explicit inline EventHub(std::weak_ptr<ContextEvent>&& wp) : contextEvent(std::move(wp)) { }

public:
	template <typename Event>
	inline void broadcast(const Event& event);
};

// -------------------------------------------------------------------------------------------------

namespace detail {
// Skipping the inclusion for this function only #include <libv/ui/basic_event_proxy.hpp> (or the creation of a new header)
void internal_fire_global(ContextEvent& ctx, std::type_index event_type, const void* event_ptr);
} // namespace detail

template <typename Event>
inline void EventHub::broadcast(const Event& event) {
	const auto sp = contextEvent.lock();
	if (sp)
		detail::internal_fire_global(*sp, std::type_index(typeid(Event)), &event);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
