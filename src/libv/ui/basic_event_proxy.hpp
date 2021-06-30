// Project: libv.ui, File: src/libv/ui/basic_event_proxy.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/meta/always.hpp>
// std
#include <functional>
#include <type_traits>
#include <typeindex>
// pro
#include <libv/ui/event/base_event.hpp>
#include <libv/ui/event/detail/internal_event_linkage.hpp>



namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace detail { // ------------------------------------------------------------------------------

template <typename ComponentT, typename EventT, typename Func>
[[nodiscard]] std::function<bool(void*, const void*)> internal_callback(Func&& func) {
	static constexpr bool is_base_event = std::is_base_of_v<BaseEvent, EventT>;

	if constexpr (std::is_invocable_r_v<void, Func, ComponentT&, const EventT&>) {
		return [f = std::forward<Func>(func)](void* signal_ptr, const void* event_ptr) mutable {
			auto handler = ComponentT{static_cast<CoreComponent*>(signal_ptr)};
			f(handler, *static_cast<const EventT*>(event_ptr));
			return is_base_event && static_cast<const BaseEvent*>(event_ptr)->propagation_stopped();
		};

	} else if constexpr (std::is_invocable_r_v<void, Func, const EventT&>) {
		return [f = std::forward<Func>(func)](void* signal_ptr, const void* event_ptr) mutable {
			(void) signal_ptr; // Callback is not interested in the component
			f(*static_cast<const EventT*>(event_ptr));
			return is_base_event && static_cast<const BaseEvent*>(event_ptr)->propagation_stopped();
		};

	} else if constexpr (std::is_invocable_r_v<void, Func, ComponentT&>) {
		return [f = std::forward<Func>(func)](void* signal_ptr, const void* event_ptr) mutable {
			(void) event_ptr; // Callback is not interested in the event
			auto handler = ComponentT{static_cast<CoreComponent*>(signal_ptr)};
			f(handler);
			return is_base_event && static_cast<const BaseEvent*>(event_ptr)->propagation_stopped();
		};

	} else if constexpr (std::is_invocable_r_v<void, Func>) {
		return [f = std::forward<Func>(func)](void* signal_ptr, const void* event_ptr) mutable {
			(void) signal_ptr; // Callback is not interested in the component
			(void) event_ptr; // Callback is not interested in the event
			f();
			return is_base_event && static_cast<const BaseEvent*>(event_ptr)->propagation_stopped();
		};

	} else {
		static_assert(libv::meta::always_false_v<Func>,
				"Callback function has invalid signature, either"
				" (),"
				" (Component&, const Event&),"
				" (const Event&) or"
				" (Component&) call operator should be accessible");
		return {};
	}
}

// -------------------------------------------------------------------------------------------------

template <typename ComponentT>
class BaseBasicEventProxy {
public:
	using component_type = ComponentT;

protected:
	component_type& component;

public:
	explicit inline BaseBasicEventProxy(component_type& component) noexcept :
		component(component) { }
};

} // namespace detail ------------------------------------------------------------------------------

template <typename ComponentT, typename EventT>
class BasicEventProxy : public detail::BaseBasicEventProxy<ComponentT> {
public:
	using event_type = EventT;

public:
	using detail::BaseBasicEventProxy<ComponentT>::BaseBasicEventProxy;

public:
	template <typename F>
	inline void operator()(F&& func) {
		connect(std::forward<F>(func));
	}

	template <typename F>
	inline void operator()(Component& slot, F&& func) {
		connect(slot, std::forward<F>(func));
	}

	template <typename F>
	inline void connect(F&& func) {
		connect(this->component, std::forward<F>(func));
	}

	template <typename F>
	inline void connect(Component& slot, F&& func) {
		detail::internal_connect(
				this->component,
				slot,
				std::type_index{typeid(EventT)},
				false, // front
				false, // system
				detail::internal_callback<ComponentT, EventT>(std::forward<F>(func)));
	}

	template <typename F>
	inline void connect_front(F&& func) {
		connect_front(this->component, std::forward<F>(func));
	}

	template <typename F>
	inline void connect_front(Component& slot, F&& func) {
		detail::internal_connect(
				this->component,
				slot,
				std::type_index{typeid(EventT)},
				true, // front
				false, // system
				detail::internal_callback<ComponentT, EventT>(std::forward<F>(func)));
	}

	template <typename F>
	inline void connect_system(F&& func) {
		connect_system(this->component, std::forward<F>(func));
	}

	template <typename F>
	inline void connect_system(Component& slot, F&& func) {
		detail::internal_connect(
				this->component,
				slot,
				std::type_index{typeid(EventT)},
				false, // front
				true, // system
				detail::internal_callback<ComponentT, EventT>(std::forward<F>(func)));
	}

	template <typename F>
	inline void connect_system_front(F&& func) {
		connect_system_front(this->component, std::forward<F>(func));
	}

	template <typename F>
	inline void connect_system_front(Component& slot, F&& func) {
		detail::internal_connect(
				this->component,
				slot,
				std::type_index{typeid(EventT)},
				true, // front
				true, // system
				detail::internal_callback<ComponentT, EventT>(std::forward<F>(func)));
	}

	inline void fire(const EventT& event) {
		detail::internal_fire(this->component, std::type_index{typeid(EventT)}, &event);
	}

	template <typename... Args>
	inline void fire(Args&&... args) {
		fire(EventT{std::forward<Args>(args)...});
	}

	//	event_connection_handler connect(...); // Hand back a handler: (?) 2 ptr: std::function ptr + slot ptr
	//	void remove(event_connection_handler handler);
	//	void remove_slot(Component& slot);
	//	void remove_signal(Component& signal);
};

template <typename ComponentT>
class BasicEventProxyGlobal : public detail::BaseBasicEventProxy<ComponentT> {
public:
	using detail::BaseBasicEventProxy<ComponentT>::BaseBasicEventProxy;

public:
	template <typename EventT, typename F>
	inline void operator()(F&& func) {
		connect<EventT>(std::forward<F>(func));
	}

	template <typename EventT, typename F>
	inline void operator()(Component& slot, F&& func) {
		connect<EventT>(slot, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	inline void connect(F&& func) {
		connect<EventT>(this->component, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	inline void connect(Component& slot, F&& func) {
		detail::internal_connect_global(
				slot,
				std::type_index{typeid(EventT)},
				false, // front
				false, // system
				detail::internal_callback<ComponentT, EventT>(std::forward<F>(func)));
	}

	template <typename EventT, typename F>
	inline void connect_front(F&& func) {
		connect_front<EventT>(this->component, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	inline void connect_front(Component& slot, F&& func) {
		detail::internal_connect_global(
				slot,
				std::type_index{typeid(EventT)},
				true, // front
				false, // system
				detail::internal_callback<ComponentT, EventT>(std::forward<F>(func)));
	}

	template <typename EventT, typename F>
	inline void connect_system(F&& func) {
		connect_system<EventT>(this->component, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	inline void connect_system(Component& slot, F&& func) {
		detail::internal_connect_global(
				slot,
				std::type_index{typeid(EventT)},
				false, // front
				true, // system
				detail::internal_callback<ComponentT, EventT>(std::forward<F>(func)));
	}

	template <typename EventT, typename F>
	inline void connect_system_front(F&& func) {
		connect_system_front<EventT>(this->component, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	inline void connect_system_front(Component& slot, F&& func) {
		detail::internal_connect_global(
				slot,
				std::type_index{typeid(EventT)},
				true, // front
				true, // system
				detail::internal_callback<ComponentT, EventT>(std::forward<F>(func)));
	}

	template <typename EventT>
	inline void fire(const EventT& event) {
		detail::internal_fire_global(this->component, std::type_index{typeid(EventT)}, &event);
	}

	template <typename EventT, typename... Args>
	inline void fire(Args&&... args) {
		fire(EventT{std::forward<Args>(args)...});
	}

	//	event_connection_handler connect(...); // Hand back a handler: (?) 2 ptr: std::function ptr + slot ptr
	//	void remove(event_connection_handler handler);
	//	void remove_slot(Component& slot);
	//	void remove_signal(Component& signal);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
