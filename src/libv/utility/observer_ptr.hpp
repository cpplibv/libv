// Project: libv.utility, File: src/libv/utility/observer_ptr.hpp

#pragma once


#ifndef LIBV_USE_STRICT_OBSERVER_PTR
#	define LIBV_USE_STRICT_OBSERVER_PTR 0
#endif


#if !LIBV_USE_STRICT_OBSERVER_PTR // ===============================================================

namespace libv {

// -------------------------------------------------------------------------------------------------

/// A non-owning nullable pointer without lifetime management
template <typename T>
using observer_ptr = T*;

template <typename T>
[[nodiscard]] constexpr inline observer_ptr<T> make_observer_ptr(T* p) noexcept {
	return p;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

#else // ===========================================================================================

// std
#include <cassert>
#include <functional> // For std::hash
#include <type_traits>

namespace libv {

// -------------------------------------------------------------------------------------------------

/// A non-owning nullable pointer without lifetime management
template <typename T>
class observer_ptr {
	T* ptr;

public:
	constexpr inline observer_ptr() noexcept : ptr(nullptr) { }
	constexpr inline observer_ptr(std::nullptr_t) noexcept : ptr(nullptr) { }

	explicit constexpr inline observer_ptr(T* p) noexcept : ptr(p) { }

	template <typename K, typename = std::enable_if_t<std::is_base_of_v<T, K>>>
	explicit constexpr inline observer_ptr(K* other) noexcept : ptr(other) { }

	template <typename K, typename = std::enable_if_t<std::is_base_of_v<T, K>>>
	explicit(false) constexpr inline observer_ptr(observer_ptr<K> other) noexcept : ptr(&*other) { }

public:
	constexpr inline T& operator*() const noexcept {
		return assert(ptr != nullptr), *ptr;
	}
	constexpr inline T* operator->() const noexcept {
		return assert(ptr != nullptr), ptr;
	}
	explicit constexpr inline operator bool() const noexcept {
		return ptr != nullptr;
	}
	explicit(false) constexpr inline operator T*() const noexcept {
		return ptr;
	}

public:
	template <typename T2>
	friend constexpr inline bool operator==(T2* p1, observer_ptr p2) noexcept {
		return p1 == p2.ptr;
	}
	template <typename T2>
	friend constexpr inline bool operator==(observer_ptr p1, T2* p2) noexcept {
		return p1.ptr == p2;
	}
	template <typename T2>
	friend constexpr inline bool operator!=(T2* p1, observer_ptr p2) noexcept {
		return !(p1 == p2);
	}
	template <typename T2>
	friend constexpr inline bool operator!=(observer_ptr p1, T2* p2) noexcept {
		return !(p1 == p2);
	}
	template <typename T2>
	friend constexpr inline bool operator==(observer_ptr p1, observer_ptr<T2> p2) noexcept {
		return p1.ptr == p2.ptr;
	}
	template <typename T2>
	friend constexpr inline bool operator!=(observer_ptr p1, observer_ptr<T2> p2) noexcept {
		return !(p1 == p2);
	}
	friend constexpr inline bool operator==(observer_ptr p, std::nullptr_t) noexcept {
		return p.ptr == nullptr;
	}
	friend constexpr inline bool operator==(std::nullptr_t, observer_ptr p) noexcept {
		return nullptr == p.ptr;
	}
	friend constexpr inline bool operator!=(observer_ptr p, std::nullptr_t) noexcept {
		return p.ptr != nullptr;
	}
	friend constexpr inline bool operator!=(std::nullptr_t, observer_ptr p) noexcept {
		return nullptr != p.ptr;
	}
	template <typename T2>
	friend constexpr inline bool operator<(observer_ptr p1, observer_ptr<T2> p2) noexcept {
		return p1.ptr < p2.ptr;
		// return std::less<T3>()( p1.get(), p2.get() );
		// where T3 is the composite T* type (C++14 §5) of T1* and T2*.
	}
	template <typename T2>
	friend constexpr inline bool operator>(observer_ptr p1, observer_ptr<T2> p2) noexcept {
		return p2 < p1;
	}
	template <typename T2>
	friend constexpr inline bool operator<=(observer_ptr p1, observer_ptr<T2> p2) noexcept {
		return !(p2 < p1);
	}
	template <typename T2>
	friend constexpr inline bool operator>=(observer_ptr p1, observer_ptr<T2> p2) noexcept {
		return !(p1 < p2);
	}
};

// specialized algorithms --------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline observer_ptr<T> make_observer_ptr(T* p) noexcept {
	return observer_ptr<T>(p);
}

} // namespace libv --------------------------------------------------------------------------------
namespace std {

template <typename T>
struct hash<::libv::observer_ptr<T>> {
	constexpr inline size_t operator()(::libv::observer_ptr<T> p) const noexcept {
		return hash<T*>{}(&*p);
	}
};

} // namespace std

#endif // ==========================================================================================
