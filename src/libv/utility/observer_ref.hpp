// Project: libv.utility, File: src/libv/utility/observer_ref.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------
// This implementation is no 100% percent, but close enough for now. Fix it on demand.

/// A non-owning non-nullable pointer without lifetime management
template <typename T>
class observer_ref {
	T* ptr;
public:
	using element_type = T;
	using value_type = T;
	using pointer = T*;

	constexpr inline observer_ref() noexcept = delete;
	constexpr inline observer_ref(std::nullptr_t) noexcept = delete;
	constexpr explicit inline observer_ref(T* p) noexcept : ptr(p) {
		assert(ptr != nullptr);
	}
	constexpr inline observer_ref(T& r) noexcept : ptr(std::addressof(r)) { }
	constexpr explicit inline observer_ref(const std::shared_ptr<T>& p) noexcept : ptr(p.get()) {
		assert(ptr != nullptr);
	}
	constexpr explicit inline observer_ref(const std::unique_ptr<T>& p) noexcept : ptr(p.get()) {
		assert(ptr != nullptr);
	}
	template <typename K, typename = std::enable_if_t<std::is_base_of_v<T, K>>>
	constexpr inline observer_ref(const observer_ref<K>& other) noexcept : ptr(other.get()) { }

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
//	constexpr inline observer_ref& operator=(const observer_ref& other) & noexcept {
//		ptr = other.get();
//		return *this;
//	}
//	template <typename T2,
//			typename = std::enable_if_t<!std::is_same_v<observer_ref, T2>>>
//	constexpr inline observer_ref& operator=(T2& other) & noexcept {
//		ptr = std::addressof(other);
//		return *this;
//	}
//	template <typename T2>
//	constexpr inline observer_ref& operator=(const observer_ref<T2>& other) & noexcept {
//		ptr = other.get();
//		return *this;
//	}

public:
	template <typename K>
	constexpr inline void swap(observer_ref<K>& other) noexcept {
		std::swap(ptr, other.ptr);
	}

public:
	template <typename T2>
	friend constexpr inline bool operator==(T2* p1, observer_ref p2) noexcept {
		return p1 == p2.get();
	}
	template <typename T2>
	friend constexpr inline bool operator==(observer_ref p1, T2* p2) noexcept {
		return p1.get() == p2;
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
		return p1.get() == p2.get();
	}
	template <typename T2>
	friend constexpr inline bool operator!=(observer_ref p1, observer_ref<T2> p2) noexcept {
		return !(p1 == p2);
	}
	friend constexpr inline bool operator==(observer_ref p, std::nullptr_t) noexcept {
		return p.get() == nullptr;
	}
	friend constexpr inline bool operator==(std::nullptr_t, observer_ref p) noexcept {
		return nullptr == p.get();
	}
	friend constexpr inline bool operator!=(observer_ref p, std::nullptr_t) noexcept {
		return p.get() != nullptr;
	}
	friend constexpr inline bool operator!=(std::nullptr_t, observer_ref p) noexcept {
		return nullptr != p.get();
	}
	template <typename T2>
	friend constexpr inline bool operator<(observer_ref p1, observer_ref<T2> p2) noexcept {
		return p1.get() < p2.get();
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
constexpr inline void swap(observer_ref<T> & p1, observer_ref<T> & p2) noexcept {
	p1.swap(p2);
}
template <typename T>
constexpr inline observer_ref<T> make_observer_ref(T* p) noexcept {
	return observer_ref<T>(p);
}
template <typename T>
constexpr inline observer_ref<T> make_observer_ref(T& p) noexcept {
	return observer_ref<T>(std::addressof(p));
}
template <typename T>
constexpr inline observer_ref<T> make_observer_ref(const std::shared_ptr<T>& p) noexcept {
	return observer_ref<T>(p);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

namespace std {

template <typename T>
struct hash<::libv::observer_ref<T>> {
	constexpr inline size_t operator()(::libv::observer_ref<T> p) const noexcept {
		return hash<T*>{}(p.get());
	}
};

} // namespace std
