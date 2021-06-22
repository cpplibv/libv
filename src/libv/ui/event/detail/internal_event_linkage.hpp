// Project: libv.ui, File: src/libv/ui/basic_event_proxy.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// std
#include <functional>
#include <typeindex>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace detail {

void internal_connect(Component& signal, Component& slot, std::type_index event_type, bool front, bool system, std::function<bool(void*, const void*)>&& callback);
void internal_connect_global(Component& slot, std::type_index event_type, bool front, bool system, std::function<bool(void*, const void*)>&& callback);
void internal_disconnect(CoreComponent* component);
void internal_fire(Component& signal, std::type_index event_type, const void* event_ptr);
void internal_fire(CoreComponent* signal, std::type_index event_type, const void* event_ptr);
void internal_fire_global(Component& ctx, std::type_index event_type, const void* event_ptr);
void internal_fire_global(ContextEvent& ctx, std::type_index event_type, const void* event_ptr);

} // namespace detail

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
