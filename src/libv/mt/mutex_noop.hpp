// Project: libv.mt, File: src/libv/mt/mutex_noop.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>


namespace libv {
namespace mt {

// -------------------------------------------------------------------------------------------------

struct mutex_noop {
	constexpr LIBV_FORCE_INLINE void lock() noexcept {}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool try_lock() noexcept {
		return true;
	}
	constexpr LIBV_FORCE_INLINE void unlock() noexcept {}

	constexpr LIBV_FORCE_INLINE void lock_shared() noexcept {}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool try_lock_shared() noexcept {
		return true;
	}
	constexpr LIBV_FORCE_INLINE void unlock_shared() noexcept {}
};

// -------------------------------------------------------------------------------------------------

} // namespace mt
} // namespace libv
