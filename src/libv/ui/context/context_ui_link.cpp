// Project: libv.ui, File: src/libv/ui/context/context_ui_link.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/context/context_ui_link.hpp>
// std
#include <cassert>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

thread_local ContextUI* thread_context_ = nullptr;

ContextUI& current_thread_context() noexcept {
	assert(thread_context_ != nullptr);
	return *thread_context_;
}

void current_thread_context(ContextUI& context) noexcept {
	thread_context_ = &context;
}

void clear_current_thread_context() noexcept {
	thread_context_ = nullptr;
}

bool has_current_thread_context() noexcept {
	return thread_context_ != nullptr;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
