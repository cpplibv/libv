// File:   context_ui_link.cpp Author: Vader Created on 13 June 2020, 09:37

// hpp
#include <libv/ui/context/context_ui_link.hpp>
// libv
#include <libv/utility/observer_ptr.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

thread_local libv::observer_ptr<ContextUI> thread_context_ = nullptr;

ContextUI& current_thread_context() noexcept {
	assert(thread_context_ != nullptr);
	return *thread_context_;
}

void current_thread_context(ContextUI& context) noexcept {
	thread_context_ = libv::make_observer(context);
}

void clear_current_thread_context() noexcept {
	thread_context_ = nullptr;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
