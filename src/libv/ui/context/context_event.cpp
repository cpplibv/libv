// Project: libv.ui, File: src/libv/ui/context/context_event.cpp

// hpp
#include <libv/ui/context/context_event.hpp>
// libv
#include <libv/algo/erase_all_unstable.hpp>
// std
#include <cassert>
#include <functional>
#include <unordered_map>
#include <vector>
#include <mutex>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextEvent {
public:
	using ptr = ContextEvent::ptr;

	struct Target {
		ptr slot;
		libv::type_uid event_type;
		std::function<void(void*, const void*)> callback;
	};

public:
	std::recursive_mutex mutex;
	std::unordered_map<ptr, std::vector<Target>> signals;
	std::unordered_map<ptr, std::vector<ptr>> slots;
};

// -------------------------------------------------------------------------------------------------

ContextEvent::ContextEvent() :
	self(std::make_unique<ImplContextEvent>()) { }

ContextEvent::~ContextEvent() {
	// For the sake of forward declared ptr
}

void ContextEvent::connect(ptr signal, ptr slot, libv::type_uid event_type, bool front, std::function<void(void*, const void*)>&& func) {
	auto lock = std::unique_lock(self->mutex);

	self->slots[slot].emplace_back(signal);
	auto& targets = self->signals[signal];
	if (front)
		targets.emplace(targets.begin(), slot, event_type, std::move(func));
	else
		targets.emplace_back(slot, event_type, std::move(func));
}

void ContextEvent::connect_global(ptr slot, libv::type_uid event_type, bool front, std::function<void(void*, const void*)>&& func) {
	auto lock = std::unique_lock(self->mutex);

	self->slots[slot].emplace_back(nullptr);
	auto& targets = self->signals[nullptr];
	if (front)
		targets.emplace(targets.begin(), slot, event_type, std::move(func));
	else
		targets.emplace_back(slot, event_type, std::move(func));
}

void ContextEvent::fire(ptr signal, libv::type_uid event_type, const void* event_ptr) {
	auto lock = std::unique_lock(self->mutex);

	const auto it = self->signals.find(signal);
	if (it == self->signals.end()) {
		assert(false && "Attempted to fire a not connected signal");
		return;
	}

	for (const ImplContextEvent::Target& target : it->second)
		if (target.event_type == event_type)
			target.callback(signal, event_ptr);
}

void ContextEvent::fire_global(libv::type_uid event_type, const void* event_ptr) {
	auto lock = std::unique_lock(self->mutex);

	const auto it = self->signals.find(nullptr);
	if (it == self->signals.end())
		return;

	for (const ImplContextEvent::Target& target : it->second)
		if (target.event_type == event_type)
			// For global events the slot is used as component in the callbacks (as there is no real signal)
			target.callback(target.slot, event_ptr);
}

void ContextEvent::disconnect_signal(ptr signal) {
	auto lock = std::unique_lock(self->mutex);

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

		// NOTE: No need to erase slots entry as disconnect_slot will also be called
		//		if (itt->second.empty())
		//			self->slots.erase(itt);
	}

	self->signals.erase(it);
}

void ContextEvent::disconnect_slot(ptr slot) {
	auto lock = std::unique_lock(self->mutex);

	const auto it = self->slots.find(slot);
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

		// NOTE: No need to erase slots entry as disconnect_signal will also be called
		//		if (itt->second.empty())
		//			self->signals.erase(itt);
	}

	self->slots.erase(it);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
