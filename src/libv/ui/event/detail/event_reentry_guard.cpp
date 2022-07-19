//

// hpp
#include <libv/ui/event/detail/event_reentry_guard.hpp>
// pro
#include <libv/ui/context/context_ui.hpp>


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

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
