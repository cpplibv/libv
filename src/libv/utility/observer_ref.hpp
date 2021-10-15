// Project: libv.utility, File: src/libv/utility/observer_ref.hpp

#pragma once


#ifndef LIBV_USE_STRICT_OBSERVER_PTR
#	define LIBV_USE_STRICT_OBSERVER_PTR 0
#endif


#if !LIBV_USE_STRICT_OBSERVER_PTR // ===============================================================


// std
#include <cassert>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// A non-owning non-nullable pointer without lifetime management
template <typename T>
using observer_ref = T*;

template <typename T>
[[nodiscard]] constexpr inline observer_ref<T> make_observer_ref(T* p) noexcept {
	assert(p != nullptr);
	return p;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

#else // ===========================================================================================

#include <cassert>
#include <functional> // For std::hash
#include <type_traits>

namespace libv {

// -------------------------------------------------------------------------------------------------

/// A non-owning non-nullable pointer without lifetime management
template <typename T>
class observer_ref {
	T* ptr;

public:
	constexpr inline observer_ref() noexcept = delete;
	constexpr inline observer_ref(std::nullptr_t) noexcept = delete;

	explicit constexpr inline observer_ref(T* p) noexcept : ptr(p) {
		assert(ptr != nullptr);
	}

	template <typename K, typename = std::enable_if_t<std::is_base_of_v<T, K>>>
	explicit constexpr inline observer_ref(K* other) noexcept : ptr(other) { }

	template <typename K, typename = std::enable_if_t<std::is_base_of_v<T, K>>>
	explicit(false) constexpr inline observer_ref(observer_ref<K> other) noexcept : ptr(&*other) { }

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
	friend constexpr inline bool operator==(T2* p1, observer_ref p2) noexcept {
		return p1 == p2.ptr;
	}
	template <typename T2>
	friend constexpr inline bool operator==(observer_ref p1, T2* p2) noexcept {
		return p1.ptr == p2;
	}
	template <typename T2>
	friend constexpr inline bool operator!=(T2* p1, observer_ref p2) noexcept {
		return !(p1 == p2);
	}
	template <typename T2>
	friend constexpr inline bool operator!=(observer_ref p1, T2* p2) noexcept {
		return !(p1 == p2);
	}
	template <typename T2>
	friend constexpr inline bool operator==(observer_ref p1, observer_ref<T2> p2) noexcept {
		return p1.ptr == &*p2;
	}
	template <typename T2>
	friend constexpr inline bool operator!=(observer_ref p1, observer_ref<T2> p2) noexcept {
		return !(p1 == p2);
	}
	friend constexpr inline bool operator==(observer_ref p, std::nullptr_t) noexcept {
		return p.ptr == nullptr;
	}
	friend constexpr inline bool operator==(std::nullptr_t, observer_ref p) noexcept {
		return nullptr == p.ptr;
	}
	friend constexpr inline bool operator!=(observer_ref p, std::nullptr_t) noexcept {
		return p.ptr != nullptr;
	}
	friend constexpr inline bool operator!=(std::nullptr_t, observer_ref p) noexcept {
		return nullptr != p.ptr;
	}
	template <typename T2>
	friend constexpr inline bool operator<(observer_ref p1, observer_ref<T2> p2) noexcept {
		return p1.ptr < p2.ptr;
		// return std::less<T3>()( p1.get(), p2.get() );
		// where T3 is the composite T* type (C++14 §5) of T1* and T2*.
	}
	template <typename T2>
	friend constexpr inline bool operator>(observer_ref p1, observer_ref<T2> p2) noexcept {
		return p2 < p1;
	}
	template <typename T2>
	friend constexpr inline bool operator<=(observer_ref p1, observer_ref<T2> p2) noexcept {
		return !(p2 < p1);
	}
	template <typename T2>
	friend constexpr inline bool operator>=(observer_ref p1, observer_ref<T2> p2) noexcept {
		return !(p1 < p2);
	}
};

// specialized algorithms --------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline observer_ref<T> make_observer_ref(T* p) noexcept {
	return observer_ref<T>(p);
}

} // namespace libv --------------------------------------------------------------------------------
namespace std {

template <typename T>
struct hash<::libv::observer_ref<T>> {
	constexpr inline size_t operator()(::libv::observer_ref<T> p) const noexcept {
		return hash<T*>{}(&*p);
	}
};

} // namespace std

#endif // ==========================================================================================
