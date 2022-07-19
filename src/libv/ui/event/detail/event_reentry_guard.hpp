//

#pragma once

// fwd
#include <libv/ui/fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ReentryGuard {
private:
	bool allow_entry;
	bool source_lock_owned;
	const void* source;
	ContextUI& contextUI;

public:
	ReentryGuard(ContextUI& contextUI, const void* source, const void* target);
	~ReentryGuard();

public:
	/// Returns true if the operation would not result in a reentry
	[[nodiscard]] explicit constexpr inline operator bool() const noexcept {
		return allow_entry;
	}

	[[nodiscard]] constexpr inline bool operator!() const noexcept {
		return !allow_entry;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
