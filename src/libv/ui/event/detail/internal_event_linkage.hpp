// Project: libv.ui, File: src/libv/ui/event/detail/internal_event_linkage.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/utility/type_uid.hpp>
// std
#include <functional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace detail {

void internal_connect(Component& signal, Component& slot, libv::type_uid event_type, bool front, std::function<void(void*, const void*)>&& callback);
void internal_connect_global(Component& slot, libv::type_uid event_type, bool front, std::function<void(void*, const void*)>&& callback);
void internal_disconnect(CoreComponent* component);
void internal_fire(Component& signal, libv::type_uid event_type, const void* event_ptr);
void internal_fire(CoreComponent* signal, libv::type_uid event_type, const void* event_ptr);
void internal_fire_global(Component& ctx, libv::type_uid event_type, const void* event_ptr);
void internal_fire_global(ContextEvent& ctx, libv::type_uid event_type, const void* event_ptr);

} // namespace detail

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
