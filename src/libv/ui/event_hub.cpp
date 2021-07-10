// Project: libv.ui, File: src/libv/ui/event_hub.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/event_hub.hpp>
// pro
#include <libv/ui/context/context_ui_link.hpp>
#include <libv/ui/event/detail/internal_event_linkage.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void EventHub::_broadcast(std::type_index event_type, const void* event_ptr) {
	const auto sp = contextEvent.lock();
	if (sp) {
		auto* previous_context = has_current_thread_context() ? &current_thread_context() : nullptr;
		current_thread_context(*contextUI);

		detail::internal_fire_global(*sp, event_type, event_ptr);

		if (previous_context == nullptr)
			clear_current_thread_context();
		else
			current_thread_context(*previous_context);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
