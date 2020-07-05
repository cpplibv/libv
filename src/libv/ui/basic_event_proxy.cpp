// File: Component.cpp, Created on 2014. január 7. 7:58, Author: Vader

// hpp
#include <libv/ui/basic_event_proxy.hpp>
// pro
#include <libv/ui/core_component.hpp>
#include <libv/ui/component.hpp>


namespace libv {
namespace ui {
namespace detail {

// -------------------------------------------------------------------------------------------------

void internal_connect(Component& signal, Component& slot, std::type_index type, std::function<void(void*, const void*)>&& callback) {
	AccessConnect::connect(signal.core(), slot.core(), type, std::move(callback));
}

// -------------------------------------------------------------------------------------------------

} // namespace detail
} // namespace ui
} // namespace libv
