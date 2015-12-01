// File:   observer_ptr.hpp

#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>

namespace vl {

// shared_ptr ======================================================================================
template<typename T>
using shared_ptr = std::shared_ptr<T>;

// weak_ptr ========================================================================================
template<typename T>
using weak_ptr = std::weak_ptr<T>;

// unique_ptr ======================================================================================
template<typename T, typename Deleter = std::default_delete<T>>
using unique_ptr = std::unique_ptr<T, Deleter>;

// observer_ptr ====================================================================================

// This implementation is no 100% percent, but close enough for now.

template<typename T>
class observer_ptr {
	T* ptr;
public:
	using element_type = T;
	using pointer = T*;
	// ---------------------------------------------------------------------------------------------
	constexpr inline observer_ptr() noexcept : ptr(nullptr) { }
	constexpr inline observer_ptr(std::nullptr_t) noexcept : ptr(nullptr) { }
	constexpr explicit inline observer_ptr(T* p) noexcept : ptr(p) { }
	template<typename K>
	constexpr inline observer_ptr(observer_ptr<K> other) noexcept : ptr(other.get()) { }
	// ---------------------------------------------------------------------------------------------
	constexpr inline T* get() const noexcept {
		return ptr;
	}
	constexpr inline T& operator*() const {
		return assert(ptr != nullptr), *ptr;
	}
	constexpr inline T* operator->() const noexcept {
		return ptr;
	}
	constexpr explicit inline operator bool() const noexcept {
		return ptr != nullptr;
	}
	constexpr explicit inline operator T*() const noexcept {
		return ptr;
	}
	// ---------------------------------------------------------------------------------------------
	constexpr inline T* release() noexcept {
		T * p(ptr);
		reset();
		return p;
	}
	constexpr inline void reset(T* p = nullptr) noexcept {
		ptr = p;
	}
	template<typename K>
	constexpr inline void swap(observer_ptr<K>& other) noexcept {
		using std::swap;
		swap(ptr, other.ptr);
	}
};
template<typename T1, typename T2>
inline bool operator==(observer_ptr<T1> p1, observer_ptr<T2> p2) {
	return p1.get() == p2.get();
}
template<typename T1, typename T2>
inline bool operator!=(observer_ptr<T1> p1, observer_ptr<T2> p2) {
	return !(p1 == p2);
}
template<typename T>
inline bool operator==(observer_ptr<T> p, std::nullptr_t) noexcept {
	return !p;
}
template<typename T>
inline bool operator==(std::nullptr_t, observer_ptr<T> p) noexcept {
	return !p;
}
template<typename T>
inline bool operator!=(observer_ptr<T> p, std::nullptr_t) noexcept {
	return (bool)p;
}
template<typename T>
inline bool operator!=(std::nullptr_t, observer_ptr<T> p) noexcept {
	return (bool)p;
}
template<typename T1, typename T2>
inline bool operator<(observer_ptr<T1> p1, observer_ptr<T2> p2) {
	return std::less<T1*>()(p1.get(), p2.get());
	// return std::less<T3>()( p1.get(), p2.get() );
	// where T3 is the composite T* type (C++14 §5) of T1* and T2*.
}
template<typename T>
inline bool operator>(observer_ptr<T> p1, observer_ptr<T> p2) {
	return p2 < p1;
}
template<typename T>
inline bool operator<=(observer_ptr<T> p1, observer_ptr<T> p2) {
	return !(p2 < p1);
}
template<typename T>
inline bool operator>=(observer_ptr<T> p1, observer_ptr<T> p2) {
	return !(p1 < p2);
}
// specialized algorithms --------------------------------------------------------------------------
template<typename T>
inline void swap(observer_ptr<T> & p1, observer_ptr<T> & p2) noexcept {
	p1.swap(p2);
}
template<typename T>
inline observer_ptr<T> make_observer(T* p) noexcept {
	return observer_ptr<T>(p);
}

} //namespace vl

namespace std {

template<typename T>
struct hash<::vl::observer_ptr<T>>
{
	inline size_t operator()(::vl::observer_ptr<T> p) {
		return hash<T*>()(p.get());
	}
};

} //namespace std