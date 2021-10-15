// Project: libv.ui, File: src/libv/ui/event/detail/internal_event_linkage.cpp

// hpp
#include <libv/ui/event/detail/internal_event_linkage.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/detail/core_ptr.hpp>
#include <libv/ui/context/context_event.hpp>
#include <libv/ui/context/context_ui.hpp>


namespace libv {
namespace ui {
namespace detail {

// -------------------------------------------------------------------------------------------------

void internal_connect(Component& signal, Component& slot, libv::type_uid event_type, bool front, bool system, std::function<bool(void*, const void*)>&& callback) {
	auto signal_c = get_core(signal);
	auto slot_c = get_core(slot);

	signal_c->flagDirect(Flag::signal);
	slot_c->flagDirect(Flag::slot);

	slot_c->context().event.connect(signal_c, slot_c, event_type, front, system, std::move(callback));
}

void internal_connect_global(Component& slot, libv::type_uid event_type, bool front, bool system, std::function<bool(void*, const void*)>&& callback) {
	auto slot_c = get_core(slot);
	slot_c->flagDirect(Flag::slot);
	slot_c->context().event.connect_global(slot_c, event_type, front, system, std::move(callback));
}

void internal_disconnect(CoreComponent* component) {
	if (component->isSignal())
		component->context().event.disconnect_signal(component);

	if (component->isSlot())
		component->context().event.disconnect_slot(component);
}

void internal_fire(Component& signal, libv::type_uid event_type, const void* event_ptr) {
	internal_fire(get_core(signal), event_type, event_ptr);
}

void internal_fire(CoreComponent* signal, libv::type_uid event_type, const void* event_ptr) {
	// NOTE: isAttached() is an experiment to stop early events that would occur in setup (during attach) codes
	if (signal->isAttached() && signal->isSignal())
		signal->context().event.fire(signal, event_type, event_ptr);
}

void internal_fire_global(Component& ctx, libv::type_uid event_type, const void* event_ptr) {
	internal_fire_global(get_core(ctx)->context().event, event_type, event_ptr);
}

void internal_fire_global(ContextEvent& ctx, libv::type_uid event_type, const void* event_ptr) {
	ctx.fire_global(event_type, event_ptr);
}

// -------------------------------------------------------------------------------------------------

} // namespace detail
} // namespace ui
} // namespace libv
