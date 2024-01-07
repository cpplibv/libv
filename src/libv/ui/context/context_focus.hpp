// Project: libv.ui, File: src/libv/ui/context/context_style.hpp

#pragma once

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

public:
	[[nodiscard]] libv::observer_ptr<CoreComponent> current() const noexcept;
	void clear();
	void focus(CoreComponent& component, FocusMode mode);
	void detachFocused(CoreComponent& component);
	void detachFocusLinked(CoreComponent& component);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
