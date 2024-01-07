// Project: libv.ui, File: src/libv/ui/context/context_focus.cpp

#include <libv/ui/context/context_focus.hpp>

#include <libv/ui/component/component_core.hpp>
#include <libv/ui/context/context_focus_traverse.hpp>
#include <libv/ui/log.hpp>
#include <libv/utility/memory/observer_ptr.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextFocus {
	using ptr = libv::observer_ptr<CoreComponent>;

public:
	ptr root = nullptr;
	ptr currentFocus = nullptr;
	bool currentActive = true;

private:
	void changeFocus(ptr focusNew, bool activeNew) {
		const auto focusOld = currentFocus;
		const auto activeOld = currentActive;

		if (focusNew == focusOld && activeNew == activeOld)
			return;

		if (focusOld == nullptr) {
			log_ui.trace("Focus set to: {}", focusNew->path());
			AccessRoot::focusGain(*focusNew, activeNew);

		} else if (focusNew == nullptr) {
			log_ui.trace("Focus clear from: {}", focusOld->path());
			AccessRoot::focusLoss(*focusOld, activeOld);

		} else {
			log_ui.trace("Focus set from: {} to: {}", focusOld->path(), focusNew->path());
			AccessRoot::focusLoss(*focusOld, activeOld);
			AccessRoot::focusGain(*focusNew, activeNew);
		}

		currentFocus = focusNew;
	}

	void focusTraverse(ContextFocusTraverse ctx) {
		ptr focusNew = nullptr;

		if (currentFocus) // Traverse to next
			focusNew = AccessRoot::focusTraverse(*currentFocus, ctx);

		if (focusNew == nullptr) // End reached, Loop around
			focusNew = AccessRoot::focusTraverse(*root, ctx);

		changeFocus(focusNew, true);
	}

public:
	void clear() {
		changeFocus(nullptr, true);
	}

	void focus(CoreComponent& component, FocusMode mode) {
		switch (mode) {
		case FocusMode::active:
			changeFocus(libv::make_observer_ptr(&component), true);
			break;
		case FocusMode::passive:
			changeFocus(libv::make_observer_ptr(&component), false);
			break;
		case FocusMode::inactive:
			changeFocus(nullptr, true);
			break;
		}
	}

	void detachFocused(CoreComponent& component) {
		(void) component;
		assert(libv::make_observer_ptr(&component) == currentFocus && "Attempted to detachFocused the not focused element");
		focusTraverse(ContextFocusTraverse::makeForward());
	}

	void detachFocusLinked(CoreComponent& component) {
		(void) component;
		// TODO P5: implement focus link
		assert(false && "Not yet implemented");
	}
};

// -------------------------------------------------------------------------------------------------

ContextFocus::ContextFocus() :
	self(std::make_unique<ImplContextFocus>()) {
}

void ContextFocus::init(libv::observer_ptr<CoreComponent> root) noexcept {
	self->root = root;
}

ContextFocus::~ContextFocus() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

libv::observer_ptr<CoreComponent> ContextFocus::current() const noexcept {
	return self->currentActive ? self->currentFocus : nullptr;
}

void ContextFocus::clear() {
	self->clear();
}

void ContextFocus::focus(CoreComponent& component, FocusMode mode) {
	self->focus(component, mode);
}

void ContextFocus::detachFocused(CoreComponent& component) {
	self->detachFocused(component);
}

void ContextFocus::detachFocusLinked(CoreComponent& component) {
	self->detachFocusLinked(component);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
