// File:   view.hpp Author: Vader Created on 2016. december 27., 19:39

#pragma once

// std
#include <atomic>
#include <utility>

namespace libv {

// -------------------------------------------------------------------------------------------------

// View ptr could find a better name
template <typename T, typename Counter = std::atomic<int64_t>>
class view_ptr final {
private:
	T* ptr;
	mutable Counter* ref_count;

private:
	inline void inc() const {
		if (ptr)
			++*ref_count;
	}
	inline void dec() const {
		if (ptr)
			--*ref_count;
	}

public:
	using element_type = T;
	using pointer = T*;

	// ---------------------------------------------------------------------------------------------

	constexpr inline view_ptr() noexcept : ptr(nullptr), ref_count(nullptr) { }

	constexpr inline view_ptr(std::nullptr_t) noexcept : ptr(nullptr), ref_count(nullptr)  { }

	constexpr inline explicit view_ptr(Counter* counter, T* data) noexcept :
		ptr(data),
		ref_count(counter) {
		inc();
	}

	constexpr inline view_ptr(const view_ptr<T>& other) noexcept :
		ptr(other.ptr),
		ref_count(other.ref_count) {
		if (ref_count)
			inc();
	}

	constexpr inline view_ptr(view_ptr<T>&& other) noexcept :
		ptr(other.ptr),
		ref_count(other.ref_count) {
		other.ptr = nullptr;
		other.ref_count = nullptr;
	}

	~view_ptr() noexcept {
		if (ref_count)
			dec();
	}

	// ---------------------------------------------------------------------------------------------

	view_ptr<T>& operator=(const view_ptr<T>& rhs) noexcept {
		if (ref_count)
			dec();
		ptr = rhs.ptr;
		if (ref_count)
			inc();
		return *this;
	}

	view_ptr<T>& operator=(view_ptr<T>&& rhs) noexcept {
		ptr = rhs.ptr;
		rhs.ptr = nullptr;
		ref_count = rhs.ref_count;
		rhs.ref_count = nullptr;
		return *this;
	}

	// ---------------------------------------------------------------------------------------------

	constexpr inline T* get() const noexcept {
		return ptr;
	}
	constexpr inline T& operator*() const noexcept {
		return assert(ptr != nullptr), *ptr;
	}
	constexpr inline T* operator->() const noexcept {
		return assert(ptr != nullptr), ptr;
	}
	constexpr inline explicit operator bool() const noexcept {
		return ptr != nullptr;
	}
	constexpr inline explicit operator T*() const noexcept {
		return ptr;
	}

	// ---------------------------------------------------------------------------------------------

	constexpr inline void reset() noexcept {
		dec();
		ptr = nullptr;
	}
	template <typename K>
	constexpr inline void swap(view_ptr<K>& other) noexcept {
		using std::swap;
		swap(ptr, other.ptr);
	}
};

// -------------------------------------------------------------------------------------------------
template <typename T1, typename T2>
inline bool operator==(view_ptr<T1> p1, view_ptr<T2> p2) noexcept {
	return p1.get() == p2.get();
}
template <typename T1, typename T2>
inline bool operator!=(view_ptr<T1> p1, view_ptr<T2> p2) noexcept {
	return !(p1 == p2);
}
template <typename T>
inline bool operator==(view_ptr<T> p, std::nullptr_t) noexcept {
	return p.get() == nullptr;
}
template <typename T>
inline bool operator==(std::nullptr_t, view_ptr<T> p) noexcept {
	return nullptr == p.get();
}
template <typename T>
inline bool operator!=(view_ptr<T> p, std::nullptr_t) noexcept {
	return p.get() != nullptr;
}
template <typename T>
inline bool operator!=(std::nullptr_t, view_ptr<T> p) noexcept {
	return nullptr != p.get();
}
template <typename T1, typename T2>
inline bool operator<(view_ptr<T1> p1, view_ptr<T2> p2) noexcept {
	return p1.get() < p2.get();
	//	return std::less<T1*>()(p1.get(), p2.get());
	// return std::less<T3>()( p1.get(), p2.get() );
	// where T3 is the composite T* type (C++14 §5) of T1* and T2*.
}
template <typename T>
inline bool operator>(view_ptr<T> p1, view_ptr<T> p2) noexcept {
	return p2 < p1;
}
template <typename T>
inline bool operator<=(view_ptr<T> p1, view_ptr<T> p2) noexcept {
	return !(p2 < p1);
}
template <typename T>
inline bool operator>=(view_ptr<T> p1, view_ptr<T> p2) noexcept {
	return !(p1 < p2);
}
// specialized algorithms --------------------------------------------------------------------------
template <typename T>
inline void swap(view_ptr<T>& p1, view_ptr<T>& p2) noexcept {
	p1.swap(p2);
}
//template <typename... Args>
//inline view_ptr<T> make_view(Args&&... args) noexcept {
//	return view_ptr<T>(std::forward<Args>(args)...);
//}

} //namespace libv

// -------------------------------------------------------------------------------------------------

namespace std {

template <typename T>
struct hash<::libv::view_ptr<T>>
{
	inline size_t operator()(::libv::view_ptr<T> p) {
		return hash<T*>()(p.get());
	}
};

} //namespace std

