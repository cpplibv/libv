//

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/meta/force_inline.hpp>
// std
#include <utility>


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

/// Reentry-locks the source and reentry-tests the target.
/// ReentryGuard evaluates to true if entry is allowed (without causing a reentry)
[[nodiscard]] ReentryGuard event_reentry_guard(const void* source, const void* target) noexcept;

/// Creates a reentry guarded callback
//template <typename S, typename T, typename Func>
//[[nodiscard]] constexpr LIBV_FORCE_INLINE auto reentry_guarded(S* source, T* target, Func&& func) {
//	return [source, target, f = std::forward<Func>(func)]<typename... Args>(Args&&... args) mutable
//		requires requires { f(std::forward<Args>(args)...); } {
//		const auto reentry_guard = event_reentry_guard(source, target);
//		if (!reentry_guard)
//			return;
//
//		f(std::forward<Args>(args)...);
//	};
//}

/// Reentry guard
template <typename S, typename T, typename Func>
LIBV_FORCE_INLINE void reentry_guard(S* source, T* target, Func&& func) {
	const auto reentry_guard = event_reentry_guard(source, target);
	if (!reentry_guard)
		return;

	std::forward<Func>(func)();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
