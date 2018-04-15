// File: observer_ptr.hpp

#pragma once

// std
#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class adaptive_ptr;

// observer_ptr ====================================================================================
// This implementation is no 100% percent, but close enough for now.

template <typename T>
class observer_ptr {
	T* ptr;
public:
	using element_type = T;
	using value_type = T;
	using pointer = T*;

	constexpr inline observer_ptr() noexcept : ptr(nullptr) { }
	constexpr explicit inline observer_ptr(std::nullptr_t) noexcept : ptr(nullptr) { }
	constexpr explicit inline observer_ptr(T* p) noexcept : ptr(p) { }
	constexpr explicit inline observer_ptr(const adaptive_ptr<T>& p) noexcept : ptr(p.get()) { }
	constexpr explicit inline observer_ptr(const std::shared_ptr<T>& p) noexcept : ptr(p.get()) { }
	template <typename K, typename = std::enable_if_t<std::is_base_of_v<T, K>>>
	constexpr inline observer_ptr(const observer_ptr<K>& other) noexcept : ptr(other.get()) { }

public:
	constexpr inline T* get() const noexcept {
		return ptr;
	}
	constexpr inline T& operator*() const {
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
		using std::swap;
		swap(ptr, other.ptr);
	}
};

template <typename T1, typename T2>
inline bool operator==(observer_ptr<T1> p1, observer_ptr<T2> p2) {
	return p1.get() == p2.get();
}
template <typename T1, typename T2>
inline bool operator!=(observer_ptr<T1> p1, observer_ptr<T2> p2) {
	return !(p1 == p2);
}
template <typename T>
inline bool operator==(observer_ptr<T> p, std::nullptr_t) noexcept {
	return p.get() == nullptr;
}
template <typename T>
inline bool operator==(std::nullptr_t, observer_ptr<T> p) noexcept {
	return nullptr == p.get();
}
template <typename T>
inline bool operator!=(observer_ptr<T> p, std::nullptr_t) noexcept {
	return p.get() != nullptr;
}
template <typename T>
inline bool operator!=(std::nullptr_t, observer_ptr<T> p) noexcept {
	return nullptr != p.get();
}
template <typename T1, typename T2>
inline bool operator<(observer_ptr<T1> p1, observer_ptr<T2> p2) {
	return std::less<T1*>()(p1.get(), p2.get());
	// return std::less<T3>()( p1.get(), p2.get() );
	// where T3 is the composite T* type (C++14 §5) of T1* and T2*.
}
template <typename T>
inline bool operator>(observer_ptr<T> p1, observer_ptr<T> p2) {
	return p2 < p1;
}
template <typename T>
inline bool operator<=(observer_ptr<T> p1, observer_ptr<T> p2) {
	return !(p2 < p1);
}
template <typename T>
inline bool operator>=(observer_ptr<T> p1, observer_ptr<T> p2) {
	return !(p1 < p2);
}

// specialized algorithms --------------------------------------------------------------------------

template <typename T>
inline void swap(observer_ptr<T> & p1, observer_ptr<T> & p2) noexcept {
	p1.swap(p2);
}
template <typename T>
inline observer_ptr<T> make_observer(T* p) noexcept {
	return observer_ptr<T>(p);
}
template <typename T>
inline observer_ptr<T> make_observer(T& p) noexcept {
	return observer_ptr<T>(&p);
}
template <typename T>
inline observer_ptr<T> make_observer(const std::shared_ptr<T>& p) noexcept {
	return observer_ptr<T>(p);
}

} // namespace libv

namespace std {

template <typename T>
struct hash<::libv::observer_ptr<T>> {
	inline size_t operator()(::libv::observer_ptr<T> p) {
		return hash<T*>()(p.get());
	}
};

} // namespace std

// adaptive_ptr ====================================================================================

namespace libv {

template <typename T>
class adaptive_ptr {
private:
	observer_ptr<T> ptr;
	std::shared_ptr<T> sp;

public:
	inline adaptive_ptr() noexcept : ptr(nullptr) { }
	explicit inline adaptive_ptr(std::nullptr_t) noexcept : ptr(nullptr) { }
	explicit inline adaptive_ptr(const observer_ptr<T>& ptr) noexcept : ptr(ptr) { }
	explicit inline adaptive_ptr(observer_ptr<T>&& ptr) noexcept : ptr(ptr) { }
	explicit inline adaptive_ptr(const std::shared_ptr<T>& sp) noexcept : ptr(sp.get()), sp(sp) { }
	explicit inline adaptive_ptr(std::shared_ptr<T>&& sp) noexcept : ptr(sp.get()), sp(std::move(sp)) { }

	adaptive_ptr& operator=(const observer_ptr<T>& ptr) & {
		this->ptr = ptr;
		return *this;
	}
	adaptive_ptr& operator=(observer_ptr<T>&& ptr) & {
		this->ptr = ptr;
		return *this;
	}
	adaptive_ptr& operator=(const std::shared_ptr<T>& sp) & {
		this->ptr = observer_ptr<T>(sp.get());
		this->sp = sp;
		return *this;
	}
	adaptive_ptr& operator=(std::shared_ptr<T>&& sp) & {
		this->ptr = observer_ptr<T>(sp.get());
		this->sp = std::move(sp);
		return *this;
	}

	inline T* get() const noexcept {
		return ptr.get();
	}
	inline T& operator*() const {
		return *ptr.get();
	}
	inline T* operator->() const noexcept {
		return ptr.get();
	}
	inline operator T*() const noexcept {
		return ptr.get();
	}
	inline operator observer_ptr<T>() const noexcept {
		return ptr;
	}
	explicit inline operator bool() const noexcept {
		return ptr != nullptr;
	}
};

template <typename T>
inline adaptive_ptr<T> make_adaptive(const observer_ptr<T>& ptr) noexcept {
	return adaptive_ptr<T>(ptr);
}
template <typename T>
inline adaptive_ptr<T> make_adaptive(observer_ptr<T>&& ptr) noexcept {
	return adaptive_ptr<T>(ptr);
}
template <typename T>
inline adaptive_ptr<T> make_adaptive(const std::shared_ptr<T>& ptr) noexcept {
	return adaptive_ptr<T>(ptr);
}
template <typename T>
inline adaptive_ptr<T> make_adaptive(std::shared_ptr<T>&& ptr) noexcept {
	return adaptive_ptr<T>(ptr);
}

} // namespace libv
