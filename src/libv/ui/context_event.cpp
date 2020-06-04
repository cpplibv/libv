// File:   context_ui.cpp Author: Vader Created on 2019. február 9., 13:01

// hpp
#include <libv/ui/context_event.hpp>
// libv
#include <libv/algorithm/erase_all_unstable.hpp>
// std
#include <cassert>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ImplContextEvent {
	using ptr = ContextEvent::ptr;

	struct Target {
		ptr slot;
		std::type_index type;
		std::function<void(void*, const void*)> callback;

		Target(ptr slot, std::type_index type, std::function<void(void*, const void*)>&& callback) :
			slot(slot),
			type(type),
			callback(std::move(callback)) { }
	};

public:
	std::unordered_map<ptr, std::vector<Target>> signals;
	std::unordered_map<ptr, std::vector<ptr>> slots;
};

// -------------------------------------------------------------------------------------------------

ContextEvent::ContextEvent() :
	self(std::make_unique<ImplContextEvent>()) { }

ContextEvent::~ContextEvent() {
	// For the sake of forward declared unique_ptr
}

void ContextEvent::connect(ptr signal, ptr slot, std::type_index type, std::function<void(void*, const void*)>&& func) {
	self->slots[slot].emplace_back(signal);
	self->signals[signal].emplace_back(slot, type, std::move(func));
}

void ContextEvent::fire(ptr signal, std::type_index type, const void* event_ptr) {
	const auto it = self->signals.find(signal);
	if (it == self->signals.end()) {
		assert(false && "Attempted to fire a not connected signal");
		return;
	}

	for (const ImplContextEvent::Target& target : it->second)
		if (target.type == type)
			target.callback(&*signal, event_ptr);
}

void ContextEvent::disconnect_signal(ptr signal) {
	const auto it = self->signals.find(signal);
	if (it == self->signals.end()) {
		assert(false && "Attempted to disconnect a not connected signal");
		return;
	}

	for (const ImplContextEvent::Target& target : it->second) {
		const auto itt = self->slots.find(target.slot);
		if (itt == self->slots.end()) {
			assert(false && "Internal consistency violation");
			continue;
		}

		libv::erase_all_unstable(itt->second, signal);
	}

	self->signals.erase(it);
}

void ContextEvent::disconnect_slot(ptr slot) {
	auto it = self->slots.find(slot);
	if (it == self->slots.end()) {
		assert(false && "Attempted to disconnect a not connected slot");
		return;
	}

	for (const ptr signal : it->second) {
		const auto itt = self->signals.find(signal);
		if (itt == self->signals.end()) {
			assert(false && "Internal consistency violation");
			continue;
		}

		libv::erase_all_unstable(itt->second, slot, &ImplContextEvent::Target::slot);
	}

	self->slots.erase(it);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
