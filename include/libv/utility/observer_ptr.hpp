// File: observer_ptr.hpp - Created on 2018.04.15. 03:37 - Author: Vader

#pragma once

// std
#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------
// This implementation is no 100% percent, but close enough for now. Fix it on demand.

/// A non-owning nullable pointer without lifetime management
template <typename T>
class observer_ptr {
	T* ptr;
public:
	using element_type = T;
	using value_type = T;
	using pointer = T*;

	constexpr inline observer_ptr() noexcept : ptr(nullptr) { }
	constexpr inline observer_ptr(std::nullptr_t) noexcept : ptr(nullptr) { }
	constexpr inline observer_ptr(T* p) noexcept : ptr(p) { }
	constexpr inline observer_ptr(T& r) noexcept : ptr(std::addressof(r)) { }
	constexpr explicit inline observer_ptr(const std::shared_ptr<T>& p) noexcept : ptr(p.get()) { }
	constexpr explicit inline observer_ptr(const std::unique_ptr<T>& p) noexcept : ptr(p.get()) { }
	template <typename K, typename = std::enable_if_t<std::is_base_of_v<T, K>>>
	constexpr inline observer_ptr(const observer_ptr<K>& other) noexcept : ptr(other.get()) { }

public:
	constexpr inline T* get() const noexcept {
		return ptr;
	}
	constexpr inline T& operator*() const noexcept {
		return assert(ptr != nullptr), *ptr;
	}
	constexpr inline T* operator->() const noexcept {
		return assert(ptr != nullptr), ptr;
	}
	constexpr explicit inline operator bool() const noexcept {
		return ptr != nullptr;
	}
	constexpr explicit inline operator T*() const noexcept {
		return ptr;
	}
	constexpr inline observer_ptr& operator=(std::nullptr_t) & noexcept {
		ptr = nullptr;
		return *this;
	}

public:
	constexpr inline T* release() noexcept {
		T * p(ptr);
		reset();
		return p;
	}
	constexpr inline void reset(T* p = nullptr) noexcept {
		ptr = p;
	}
	template <typename K>
	constexpr inline void swap(observer_ptr<K>& other) noexcept {
		std::swap(ptr, other.ptr);
	}
};

template <typename T1, typename T2>
constexpr inline bool operator==(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
	return p1.get() == p2.get();
}
template <typename T1, typename T2>
constexpr inline bool operator!=(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
	return !(p1 == p2);
}
template <typename T>
constexpr inline bool operator==(observer_ptr<T> p, std::nullptr_t) noexcept {
	return p.get() == nullptr;
}
template <typename T>
constexpr inline bool operator==(std::nullptr_t, observer_ptr<T> p) noexcept {
	return nullptr == p.get();
}
template <typename T>
constexpr inline bool operator!=(observer_ptr<T> p, std::nullptr_t) noexcept {
	return p.get() != nullptr;
}
template <typename T>
constexpr inline bool operator!=(std::nullptr_t, observer_ptr<T> p) noexcept {
	return nullptr != p.get();
}
template <typename T1, typename T2>
constexpr inline bool operator<(observer_ptr<T1> p1, observer_ptr<T2> p2) noexcept {
	return std::less<T1*>()(p1.get(), p2.get());
	// return std::less<T3>()( p1.get(), p2.get() );
	// where T3 is the composite T* type (C++14 §5) of T1* and T2*.
}
template <typename T>
constexpr inline bool operator>(observer_ptr<T> p1, observer_ptr<T> p2) noexcept {
	return p2 < p1;
}
template <typename T>
constexpr inline bool operator<=(observer_ptr<T> p1, observer_ptr<T> p2) noexcept {
	return !(p2 < p1);
}
template <typename T>
constexpr inline bool operator>=(observer_ptr<T> p1, observer_ptr<T> p2) noexcept {
	return !(p1 < p2);
}

// specialized algorithms --------------------------------------------------------------------------

template <typename T>
constexpr inline void swap(observer_ptr<T> & p1, observer_ptr<T> & p2) noexcept {
	p1.swap(p2);
}
template <typename T>
constexpr inline observer_ptr<T> make_observer(T* p) noexcept {
	return observer_ptr<T>(p);
}
template <typename T>
constexpr inline observer_ptr<T> make_observer(T& p) noexcept {
	return observer_ptr<T>(std::addressof(p));
}
template <typename T>
constexpr inline observer_ptr<T> make_observer(const std::shared_ptr<T>& p) noexcept {
	return observer_ptr<T>(p);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

namespace std {

template <typename T>
struct hash<::libv::observer_ptr<T>> {
	constexpr inline size_t operator()(::libv::observer_ptr<T> p) const noexcept {
		return hash<T*>{}(p.get());
	}
};

} // namespace std
