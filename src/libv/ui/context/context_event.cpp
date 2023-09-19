// Project: libv.ui, File: src/libv/ui/context/context_event.cpp

// hpp
#include <libv/ui/context/context_event.hpp>
// libv
#include <libv/algo/erase_unstable.hpp>
// std
#include <algorithm>
#include <vector>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ImplContextEvent {
	std::vector<const void*> locked_reentry_anchors;
};

// -------------------------------------------------------------------------------------------------

ContextEvent::ContextEvent() :
	self(std::make_unique<ImplContextEvent>()) {
}

ContextEvent::~ContextEvent() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

void ContextEvent::reentry_lock(const void* anchor) {
	self->locked_reentry_anchors.emplace_back(anchor);
}

void ContextEvent::reentry_unlock(const void* anchor) noexcept {
	libv::erase_unstable(self->locked_reentry_anchors, anchor);
}

bool ContextEvent::reentry_test(const void* anchor) const noexcept {
	const auto it = std::ranges::find(self->locked_reentry_anchors, anchor);
	return it == self->locked_reentry_anchors.end();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
