// Project: libv.ui, File: src/libv/ui/event_system/event_reentry_guard.cpp

// hpp
#include <libv/ui/event_system/event_reentry_guard.hpp>
// pro
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/context/context_ui_link.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

ReentryGuard::ReentryGuard(ContextUI& contextUI, const void* source, const void* target) :
	allow_entry(contextUI.reentry_test(target)),
	// NOTE: If entry isn't allowed, skip locking
	source_lock_owned(allow_entry ? contextUI.reentry_lock(source), true : false),
	source(source),
	contextUI(contextUI) {
}

ReentryGuard::~ReentryGuard() {
	if (source_lock_owned)
		contextUI.reentry_unlock(source);
}

[[nodiscard]] ReentryGuard event_reentry_guard(const void* source, const void* target) noexcept {
	return ReentryGuard{libv::ui::current_thread_context(), source, target};
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
