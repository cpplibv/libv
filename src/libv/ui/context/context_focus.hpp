// Project: libv.ui, File: src/libv/ui/context/context_focus.hpp

#pragma once

#include <libv/math/vec.hpp>
#include <libv/ui/fwd.hpp>
#include <libv/ui/property/focus_mode.hpp>
#include <libv/utility/memory/observer_ptr.hpp>

#include <memory>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextFocus;

class ContextFocus {
private:
	std::unique_ptr<ImplContextFocus> self;

public:
	ContextFocus();
	void init(libv::observer_ptr<CoreComponent> root) noexcept;
	~ContextFocus();

	ContextFocus(const ContextFocus&) = delete;
	ContextFocus(ContextFocus&&) = delete;
	ContextFocus& operator=(const ContextFocus&) = delete;
	ContextFocus& operator=(ContextFocus&&) = delete;

public: // Client related functions
	void clear();
	void traverse(libv::vec2f direction);
	void traverseForward();
	void traverseBackward();

public: // System related functions
	[[nodiscard]] libv::observer_ptr<CoreComponent> current() const noexcept;
	void focus(CoreComponent& component, FocusMode mode);
	void detachFocused(CoreComponent& component);
	void detachFocusLinked(CoreComponent& component);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
