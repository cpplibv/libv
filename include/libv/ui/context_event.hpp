// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/utility/observer_ptr.hpp>
// std
#include <functional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BaseComponent;

class ContextEvent {
	using ptr = libv::observer_ptr<BaseComponent>;

	struct Target {
		ptr slot;
		std::type_index type;
		std::function<void(const void* const)> callback;

		Target(ptr slot, std::type_index type, std::function<void(const void* const)>&& callback) :
			slot(slot),
			type(type),
			callback(std::move(callback)) { }
	};

private:
	std::unordered_map<ptr, std::vector<Target>> signals;
	std::unordered_map<ptr, std::vector<ptr>> slots;

private:
	void _connect(ptr signal, ptr slot, std::type_index type, std::function<void(const void* const)>&& func);
	void _fire(ptr signal, std::type_index type, const void* const event_ptr);

public:
	template <typename Event, typename Func>
	void connect(ptr signal, ptr slot, Func&& func) {
		static_assert(std::is_invocable_r_v<void, Func, const Event&>);
		_connect(signal, slot, std::type_index(typeid(Event)), [f = std::forward<Func>(func)](const void* const event_ptr) {
			f(*static_cast<const Event*>(event_ptr));
		});
	}

	template <typename Event>
	void fire(ptr signal, const Event& event) {
		const void* const event_ptr = &event;
		_fire(signal, std::type_index(typeid(Event)), event_ptr);
	}

	void disconnect_signal(ptr signal);
	void disconnect_slot(ptr slot);

	void disconnect(ptr signal, ptr slot); /// Exposition only, Implement on demand
	template <typename Event>
	void disconnect(ptr signal, ptr slot); /// Exposition only, Implement on demand
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
