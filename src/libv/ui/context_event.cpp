// File:   context_ui.cpp Author: Vader Created on 2019. február 9., 13:01

// hpp
#include <libv/ui/context_event.hpp>
// libv
#include <libv/algorithm/erase_all_unstable.hpp>
// std
#include <cassert>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void ContextEvent::_connect(ptr signal, ptr slot, std::type_index type, std::function<void(const void* const)>&& func) {
	slots[slot].emplace_back(signal);
	signals[signal].emplace_back(slot, type, std::move(func));
}

void ContextEvent::_fire(ptr signal, std::type_index type, const void* const event_ptr) {
	const auto it = signals.find(signal);
	assert(it != signals.end() && "Attempted to fire a not connected signal");

	for (const Target& target : it->second)
		if (target.type == type)
			target.callback(event_ptr);
}

void ContextEvent::disconnect_signal(ptr signal) {
	const auto it = signals.find(signal);
	assert(it != signals.end() && "Attempted to disconnect a not connected signal");

	for (const Target& target : it->second) {
		const auto itt = slots.find(target.slot);
		assert(itt != slots.end() && "Internal consistency violation");

		libv::erase_all_unstable(itt->second, signal);
	}

	signals.erase(it);
}

void ContextEvent::disconnect_slot(ptr slot) {
	auto it = slots.find(slot);
	assert(it != slots.end() && "Attempted to disconnect a not connected slot");

	for (const ptr signal : it->second) {
		const auto itt = signals.find(signal);
		assert(itt != signals.end() && "Internal consistency violation");

		libv::erase_all_unstable(itt->second, slot, &Target::slot);
	}

	slots.erase(it);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
