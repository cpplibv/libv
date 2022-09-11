// Project: libv.ui, File: src/libv/ui/basic_event_proxy.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/meta/always.hpp>
#include <libv/meta/force_inline.hpp>
#include <libv/utility/nexus.hpp>
// std
#include <type_traits>
// pro
#include <libv/ui/component/detail/core_ptr.hpp>
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace detail { // ------------------------------------------------------------------------------

template <typename SignalComponentT, typename EventT, bool IsSystem, typename Func>
[[nodiscard]] LIBV_FORCE_INLINE auto internal_callback_component(Func&& func) {
	static constexpr bool is_base_event = std::is_base_of_v<BaseEvent, EventT>;

	if constexpr (std::is_invocable_v<Func, SignalComponentT&, const EventT&>)
		return [f = std::forward<Func>(func)](typename SignalComponentT::CoreT* signal_ptr, const EventT& event) mutable {
			if (IsSystem || !is_base_event || !reinterpret_cast<const BaseEvent&>(event).propagation_stopped()) {
				auto handler = SignalComponentT::from_core(signal_ptr);
				f(handler, event);
			}
		};

	else if constexpr (std::is_invocable_v<Func, const EventT&>)
		return [f = std::forward<Func>(func)](typename SignalComponentT::CoreT*, const EventT& event) mutable {
			// Callback is not interested in the component
			if (IsSystem || !is_base_event || !reinterpret_cast<const BaseEvent&>(event).propagation_stopped())
				f(event);
		};

	else if constexpr (std::is_invocable_v<Func, SignalComponentT&>)
		return [f = std::forward<Func>(func)](typename SignalComponentT::CoreT* signal_ptr, const EventT& event) mutable {
			// Callback is not interested in the event
			if (IsSystem || !is_base_event || !reinterpret_cast<const BaseEvent&>(event).propagation_stopped()) {
				auto handler = SignalComponentT::from_core(signal_ptr);
				f(handler);
			}
		};

	else if constexpr (std::is_invocable_v<Func>)
		return [f = std::forward<Func>(func)](typename SignalComponentT::CoreT*, const EventT& event) mutable {
			// Callback is not interested in the channel or event
			if (IsSystem || !is_base_event || !reinterpret_cast<const BaseEvent&>(event).propagation_stopped())
				f();
		};

	else
		static_assert(libv::meta::always_false_v<Func>,
				"Callback function has invalid signature, Expected either:"
				"\n\tvoid(Component&, const Event&)"
				"\n\tvoid(const Event&)"
				"\n\tvoid(Component&)"
				"\n\tvoid()");
}

template <typename SlotComponentT, typename EventT, bool IsSystem, typename Func>
[[nodiscard]] LIBV_FORCE_INLINE auto internal_callback_global(CoreComponent* slot_ptr, Func&& func) {
	static constexpr bool is_base_event = std::is_base_of_v<BaseEvent, EventT>;

	// For global events there is no signal component, so the slot component is mapped as fake one

	if constexpr (std::is_invocable_v<Func, SlotComponentT&, const EventT&>)
		return [slot_ptr, f = std::forward<Func>(func)](const EventT& event) mutable {
			if (IsSystem || !is_base_event || !reinterpret_cast<const BaseEvent&>(event).propagation_stopped()) {
				auto handler = SlotComponentT::from_core(slot_ptr);
				f(handler, event);
			}
		};

	else if constexpr (std::is_invocable_v<Func, const EventT&>)
		return [f = std::forward<Func>(func)](const EventT& event) mutable {
			// Callback is not interested in the component
			if (IsSystem || !is_base_event || !reinterpret_cast<const BaseEvent&>(event).propagation_stopped())
				f(event);
		};

	else if constexpr (std::is_invocable_v<Func, SlotComponentT&>)
		return [slot_ptr, f = std::forward<Func>(func)](const EventT& event) mutable {
			// Callback is not interested in the event
			if (IsSystem || !is_base_event || !reinterpret_cast<const BaseEvent&>(event).propagation_stopped()) {
				auto handler = SlotComponentT::from_core(slot_ptr);
				f(handler);
			}
		};

	else if constexpr (std::is_invocable_v<Func>)
		return [f = std::forward<Func>(func)](const EventT& event) mutable {
			// Callback is not interested in the channel or event
			if (IsSystem || !is_base_event || !reinterpret_cast<const BaseEvent&>(event).propagation_stopped())
				f();
		};

	else
		static_assert(libv::meta::always_false_v<Func>,
				"Callback function has invalid signature, Expected either:"
				"\n\tvoid(Component&, const Event&)"
				"\n\tvoid(const Event&)"
				"\n\tvoid(Component&)"
				"\n\tvoid()");
}

// -------------------------------------------------------------------------------------------------

template <typename ComponentT>
class BaseBasicEventProxy {
public:
	using component_type = ComponentT;

protected:
	component_type& component;

public:
	explicit LIBV_FORCE_INLINE BaseBasicEventProxy(component_type& component) noexcept :
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
	LIBV_FORCE_INLINE void operator()(F&& func) {
		connect(std::forward<F>(func));
	}

	template <typename F>
	LIBV_FORCE_INLINE void operator()(Component& slot, F&& func) {
		connect(slot, std::forward<F>(func));
	}

	template <typename F>
	LIBV_FORCE_INLINE void connect(F&& func) {
		mark_as_signal(this->component);
		get_nexus(this->component).template connect_channel<EventT>(
				get_core(this->component),
				nullptr,
				detail::internal_callback_component<ComponentT, EventT, false>(std::forward<F>(func)));
	}

	template <typename F>
	LIBV_FORCE_INLINE void connect(Component& slot, F&& func) {
		mark_as_signal(this->component);
		mark_as_slot(slot);
		get_nexus(this->component).template connect_channel<EventT>(
				get_core(this->component),
				get_core(slot),
				detail::internal_callback_component<ComponentT, EventT, false>(std::forward<F>(func)));
	}

	template <typename F>
	LIBV_FORCE_INLINE void connect_front(F&& func) {
		mark_as_signal(this->component);
		get_nexus(this->component).template connect_channel_front<EventT>(
				get_core(this->component),
				nullptr,
				detail::internal_callback_component<ComponentT, EventT, false>(std::forward<F>(func)));
	}

	template <typename F>
	LIBV_FORCE_INLINE void connect_front(Component& slot, F&& func) {
		mark_as_signal(this->component);
		mark_as_slot(slot);
		get_nexus(this->component).template connect_channel_front<EventT>(
				get_core(this->component),
				get_core(slot),
				detail::internal_callback_component<ComponentT, EventT, false>(std::forward<F>(func)));
	}

	template <typename F>
	LIBV_FORCE_INLINE void connect_system(F&& func) {
		mark_as_signal(this->component);
		get_nexus(this->component).template connect_channel<EventT>(
				get_core(this->component),
				nullptr,
				detail::internal_callback_component<ComponentT, EventT, true>(std::forward<F>(func)));
	}

	template <typename F>
	LIBV_FORCE_INLINE void connect_system(Component& slot, F&& func) {
		mark_as_signal(this->component);
		mark_as_slot(slot);
		get_nexus(this->component).template connect_channel<EventT>(
				get_core(this->component),
				get_core(slot),
				detail::internal_callback_component<ComponentT, EventT, true>(std::forward<F>(func)));
	}

	template <typename F>
	LIBV_FORCE_INLINE void connect_system_front(F&& func) {
		mark_as_signal(this->component);
		get_nexus(this->component).template connect_channel_front<EventT>(
				get_core(this->component),
				nullptr,
				detail::internal_callback_component<ComponentT, EventT, true>(std::forward<F>(func)));
	}

	template <typename F>
	LIBV_FORCE_INLINE void connect_system_front(Component& slot, F&& func) {
		mark_as_signal(this->component);
		mark_as_slot(slot);
		get_nexus(this->component).template connect_channel_front<EventT>(
				get_core(this->component),
				get_core(slot),
				detail::internal_callback_component<ComponentT, EventT, true>(std::forward<F>(func)));
	}

	template <typename F>
	inline F& connect_system_out_ref(F&& func) {
		struct Target {
			F f;

			void operator()(CoreComponent* signal_ptr, const EventT& event) {
				(void) signal_ptr; // Callback is not interested in the component
				(void) event; // Callback is not interested in the event
				// NOTE: It's a system event, no need to check propagation
				f();
			}
		};

		auto callback = std::function<void(CoreComponent*, const EventT&)>{Target{std::forward<F>(func)}};
		auto& target = callback.template target<Target>()->f;

		mark_as_signal(this->component);
		get_nexus(this->component).template connect_channel<EventT>(
				get_core(this->component),
				nullptr,
				std::move(callback));

		return target;
	}

	LIBV_FORCE_INLINE void fire(const EventT& event) {
		if (is_marked_as_signal(this->component))
			get_nexus(this->component).template broadcast_channel<EventT>(get_core(this->component), event);
	}

	template <typename... Args>
	LIBV_FORCE_INLINE void fire(Args&&... args) {
		EventT event{std::forward<Args>(args)...};
		fire(const_cast<const EventT&>(event));
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
	LIBV_FORCE_INLINE void operator()(F&& func) {
		connect<EventT>(std::forward<F>(func));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void operator()(Component& slot, F&& func) {
		connect<EventT>(slot, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void connect(F&& func) {
		connect<EventT>(this->component, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void connect(Component& slot, F&& func) {
		mark_as_slot(slot);
		const auto slot_ptr = get_core(slot);
		get_nexus(this->component).template connect_global<EventT>(
				slot_ptr,
				detail::internal_callback_global<ComponentT, EventT, false>(slot_ptr, std::forward<F>(func)));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void connect_front(F&& func) {
		connect_front<EventT>(this->component, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void connect_front(Component& slot, F&& func) {
		mark_as_slot(slot);
		const auto slot_ptr = get_core(slot);
		get_nexus(this->component).template connect_global_front<EventT>(
				slot_ptr,
				detail::internal_callback_global<ComponentT, EventT, false>(slot_ptr, std::forward<F>(func)));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void connect_system(F&& func) {
		connect_system<EventT>(this->component, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void connect_system(Component& slot, F&& func) {
		mark_as_slot(slot);
		const auto slot_ptr = get_core(slot);
		get_nexus(this->component).template connect_global<EventT>(
				slot_ptr,
				detail::internal_callback_global<ComponentT, EventT, true>(slot_ptr, std::forward<F>(func)));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void connect_system_front(F&& func) {
		connect_system_front<EventT>(this->component, std::forward<F>(func));
	}

	template <typename EventT, typename F>
	LIBV_FORCE_INLINE void connect_system_front(Component& slot, F&& func) {
		mark_as_slot(slot);
		const auto slot_ptr = get_core(slot);
		get_nexus(this->component).template connect_global_front<EventT>(
				slot_ptr,
				detail::internal_callback_global<ComponentT, EventT, true>(slot_ptr, std::forward<F>(func)));
	}

	template <typename EventT>
	LIBV_FORCE_INLINE void fire(const EventT& event) {
		// NOTE: Always fire, global events cannot be checked on the slot side
		get_nexus(this->component).template broadcast_global<EventT>(event);
	}

	template <typename EventT, typename... Args>
	LIBV_FORCE_INLINE void fire(Args&&... args) {
		EventT event{std::forward<Args>(args)...};
		fire(const_cast<const EventT&>(event));
	}

	//	event_connection_handler connect(...); // Hand back a handler: (?) 2 ptr: std::function ptr + slot ptr
	//	void remove(event_connection_handler handler);
	//	void remove_slot(Component& slot);
	//	void remove_signal(Component& signal);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
