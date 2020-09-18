// Project: libv.ui, File: src/libv/ui/basic_event_proxy.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/meta/always.hpp>
// std
#include <functional>
#include <type_traits>
#include <typeindex>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreComponent;
class Component;

namespace detail {

void internal_connect(Component& signal, Component& slot, std::type_index type, std::function<void(void*, const void*)>&& callback);

} // namespace detail

// -------------------------------------------------------------------------------------------------

template <typename ComponentT, typename EventT>
struct BasicEventProxy {
	using component_type = ComponentT;
	using event_type = EventT;

private:
	component_type& component;

public:
	inline BasicEventProxy(component_type& component) noexcept : component(component) { }

private:
	template <typename Func>
	std::function<void(void*, const void*)> internal_callback(Func&& func) {
		if constexpr (std::is_invocable_r_v<void, Func, ComponentT&, const EventT&>) {
			return [f = std::forward<Func>(func)](void* component_ptr, const void* event_ptr) {
					auto handler = ComponentT{static_cast<CoreComponent*>(component_ptr)};
					f(handler, *static_cast<const EventT*>(event_ptr));
			};

		} else if constexpr (std::is_invocable_r_v<void, Func, const EventT&>) {
			return [f = std::forward<Func>(func)](void* component_ptr, const void* event_ptr) {
					(void) component_ptr; // Callback is not interested in the component
					f(*static_cast<const EventT*>(event_ptr));
			};

		} else if constexpr (std::is_invocable_r_v<void, Func, ComponentT&>) {
			return [f = std::forward<Func>(func)](void* component_ptr, const void* event_ptr) {
					(void) event_ptr; // Callback is not interested in the event
					auto handler = ComponentT{static_cast<CoreComponent*>(component_ptr)};
					f(handler);
			};

		} else {
			static_assert(libv::meta::always_false_v<Func>,
					"Callback function has invalid signature, either"
					" (const Event&),"
					" (Component&) or"
					" (Component&, const Event&) call operator should be accessible");
			return {};
		}
	}

public:
	template <typename F>
	inline void operator()(F&& func) {
		detail::internal_connect(component, component, std::type_index{typeid(EventT)}, internal_callback(std::forward<F>(func)));
	}

	template <typename F>
	inline void operator()(Component& slot, F&& func) {
		detail::internal_connect(component, slot, std::type_index{typeid(EventT)}, internal_callback(std::forward<F>(func)));
	}

	template <typename F>
	inline void connect(F&& func) {
		detail::internal_connect(component, component, std::type_index{typeid(EventT)}, internal_callback(std::forward<F>(func)));
	}

	template <typename F>
	inline void connect(Component& slot, F&& func) {
		detail::internal_connect(component, slot, std::type_index{typeid(EventT)}, internal_callback(std::forward<F>(func)));
	}

//	void connect_front();
//	void connect_unignorable();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
