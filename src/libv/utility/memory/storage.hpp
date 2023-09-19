// Project: libv.utility, File: src/libv/utility/memory/storage.hpp

#pragma once

#include <cstddef>
#include <memory>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct storage {
	alignas(T) std::byte storage[sizeof(T)]{};

	template <typename... Args>
	constexpr inline void create(Args&&... args) {
		std::construct_at(address(), std::forward<Args>(args)...);
	}

	constexpr inline void destroy() {
		std::destroy_at<T>(address());
	}

	constexpr inline T* address() noexcept {
		return reinterpret_cast<T*>(static_cast<void*>(storage)); // void cast to suppress alignment warning
	}
	constexpr inline const T* address() const noexcept {
		return reinterpret_cast<const T*>(static_cast<const void*>(storage)); // void cast to suppress alignment warning
	}
	constexpr inline T& value() noexcept {
		return *address();
	}
	constexpr inline const T& value() const noexcept {
		return *address();
	}
	constexpr inline operator T&() noexcept {
		return *address();
	}
	constexpr inline operator const T&() const noexcept {
		return *address();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
