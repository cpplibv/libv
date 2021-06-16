// Project: libv.ui, File: src/libv/ui/basic_event_proxy.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/basic_event_proxy.hpp>
// pro
#include <libv/ui/component.hpp>
#include <libv/ui/context/context_event.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/core_component.hpp>


namespace libv {
namespace ui {
namespace detail {

// -------------------------------------------------------------------------------------------------

void internal_connect(Component& signal, Component& slot, std::type_index event_type, bool front, bool system, std::function<bool(void*, const void*)>&& callback) {
	auto signal_c = &signal.core();
	auto slot_c = &slot.core();

	signal_c->flagDirect(Flag::signal);
	slot_c->flagDirect(Flag::slot);

	slot_c->context().event.connect(signal_c, slot_c, event_type, front, system, std::move(callback));
}

void internal_connect_global(Component& slot, std::type_index event_type, bool front, bool system, std::function<bool(void*, const void*)>&& callback) {
	auto slot_c = &slot.core();
	slot_c->flagDirect(Flag::slot);
	slot_c->context().event.connect_global(slot_c, event_type, front, system, std::move(callback));
}

void internal_disconnect(CoreComponent* component) {
	if (component->isSignal())
		component->context().event.disconnect_signal(component);

	if (component->isSlot())
		component->context().event.disconnect_slot(component);
}

void internal_fire(Component& signal, std::type_index event_type, const void* event_ptr) {
	internal_fire(&signal.core(), event_type, event_ptr);
}

void internal_fire(CoreComponent* signal, std::type_index event_type, const void* event_ptr) {
	// NOTE: isAttached() is an experiment to stop early events that would occur in setup (during attach) codes
	if (signal->isAttached() && signal->isSignal())
		signal->context().event.fire(signal, event_type, event_ptr);
}

void internal_fire_global(Component& ctx, std::type_index event_type, const void* event_ptr) {
	internal_fire_global(ctx.core().context().event, event_type, event_ptr);
}

void internal_fire_global(ContextEvent& ctx, std::type_index event_type, const void* event_ptr) {
	ctx.fire_global(event_type, event_ptr);
}

// -------------------------------------------------------------------------------------------------

} // namespace detail
} // namespace ui
} // namespace libv
