//// Project: libv.ui, File: src/libv/ui/context/context_event.cpp, Author: Császár Mátyás [Vader]
//
//// hpp
//#include <space/signal/event_hub.hpp>
//// libv
//#include <libv/algo/erase_all_unstable.hpp>
//// std
//#include <cassert>
//#include <functional>
//#include <unordered_map>
//#include <vector>
//#include <mutex>
//
//
//namespace libv {
//
//// -------------------------------------------------------------------------------------------------
//
//class ImplEventHub {
//public:
//	using ptr = EventHub::ptr;
//
//	struct Target {
//		ptr slot;
//		std::type_index event_type;
//		bool is_system; /// Callback is not ignorable
//		std::function<bool(void*, const void*)> callback;
//	};
//
//public:
//	std::recursive_mutex mutex;
//	std::unordered_map<ptr, std::vector<Target>> signals;
//	std::unordered_map<ptr, std::vector<ptr>> slots;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//EventHub::EventHub() :
//	self(std::make_unique<ImplEventHub>()) { }
//
//EventHub::~EventHub() {
//	// For the sake of forward declared unique_ptr
//}
//
//void EventHub::connect(ptr signal, ptr slot, std::type_index event_type, bool front, bool system, std::function<bool(void*, const void*)>&& func) {
//	auto lock = std::unique_lock(self->mutex);
//
//	self->slots[slot].emplace_back(signal);
//	auto& targets = self->signals[signal];
//	if (front)
//		targets.emplace(targets.begin(), slot, event_type, system, std::move(func));
//	else
//		targets.emplace_back(slot, event_type, system, std::move(func));
//}
//
//void EventHub::connect_global(ptr slot, std::type_index event_type, bool front, bool system, std::function<bool(void*, const void*)>&& func) {
//	auto lock = std::unique_lock(self->mutex);
//
//	self->slots[slot].emplace_back(nullptr);
//	auto& targets = self->signals[nullptr];
//	if (front)
//		targets.emplace(targets.begin(), slot, event_type, system, std::move(func));
//	else
//		targets.emplace_back(slot, event_type, system, std::move(func));
//}
//
//void EventHub::fire(ptr signal, std::type_index event_type, const void* event_ptr) {
//	auto lock = std::unique_lock(self->mutex);
//
//	const auto it = self->signals.find(signal);
//	if (it == self->signals.end()) {
//		assert(false && "Attempted to fire a not connected signal");
//		return;
//	}
//
//	bool stopped = false;
//	for (const ImplEventHub::Target& target : it->second)
//		if (target.event_type == event_type)
//			if (target.is_system || !stopped)
//				stopped |= target.callback(&*signal, event_ptr);
//}
//
//void EventHub::fire_global(std::type_index event_type, const void* event_ptr) {
//	auto lock = std::unique_lock(self->mutex);
//
//	const auto it = self->signals.find(nullptr);
//	if (it == self->signals.end())
//		return;
//
//	bool stopped = false;
//	for (const ImplEventHub::Target& target : it->second)
//		if (target.event_type == event_type)
//			if (target.is_system || !stopped)
//				// For global events the slot is used as component in the callbacks (as there is no real signal)
//				stopped |= target.callback(&*target.slot, event_ptr);
//}
//
//void EventHub::disconnect_signal(ptr signal) {
//	auto lock = std::unique_lock(self->mutex);
//
//	const auto it = self->signals.find(signal);
//	if (it == self->signals.end()) {
//		assert(false && "Attempted to disconnect a not connected signal");
//		return;
//	}
//
//	for (const ImplEventHub::Target& target : it->second) {
//		const auto itt = self->slots.find(target.slot);
//		if (itt == self->slots.end()) {
//			assert(false && "Internal consistency violation");
//			continue;
//		}
//
//		libv::erase_all_unstable(itt->second, signal);
//	}
//
//	self->signals.erase(it);
//}
//
//void EventHub::disconnect_slot(ptr slot) {
//	auto lock = std::unique_lock(self->mutex);
//
//	const auto it = self->slots.find(slot);
//	if (it == self->slots.end()) {
//		assert(false && "Attempted to disconnect a not connected slot");
//		return;
//	}
//
//	for (const ptr signal : it->second) {
//		const auto itt = self->signals.find(signal);
//		if (itt == self->signals.end()) {
//			assert(false && "Internal consistency violation");
//			continue;
//		}
//
//		libv::erase_all_unstable(itt->second, slot, &ImplEventHub::Target::slot);
//	}
//
//	self->slots.erase(it);
//}
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace libv
