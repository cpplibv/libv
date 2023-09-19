// Project: libv.utility, File: src/libv/utility/memory/intrusive2_ptr.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>
// std
#include <atomic>
#include <cassert>
#include <concepts>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------
//
// Usage:
//
//	class Resource : public libv::ref_count_base<Resource> {
//		friend libv::ref_count_access;
//		// ...
//	private:
//		void ref_count_one(); // Optional. Defaults to noop. NOTE: Multi-threading can mess the call order up: Implicit strand is required if ref_count_one is observed
//		void ref_count_zero(); // Optional. Defaults to noop.
//	};
//
//	const auto resource = libv::make_intrusive2_ptr<Resource>();
//
// OR:
//
//	class ResourceIncomplete;
// 	void increase_ref_count(ResourceIncomplete* ptr);
// 	void decrease_ref_count(ResourceIncomplete* ptr);
//	libv::intrusive2_ptr<ResourceIncomplete> incomplete_usage;
//
//	class ResourceIncomplete : public libv::ref_count_base<Resource> {
//		friend libv::ref_count_access;
//		// ...
//	private:
//		void ref_count_one(); // Optional. Defaults to noop. NOTE: Multi-threading can mess the call order up: Implicit strand is required if ref_count_one is observed
//		void ref_count_zero(); // Optional. Defaults to noop.
//	};
// 	void increase_ref_count(ResourceIncomplete* ptr) {
// 		libv::ref_count_access::increase_ref_count(ptr);
// 	}
// 	void decrease_ref_count(ResourceIncomplete* ptr) {
// 		libv::ref_count_access::decrease_ref_count(ptr);
// 	}
//	const auto resource = libv::make_intrusive2_ptr<Resource>();
//
// -------------------------------------------------------------------------------------------------

struct ref_count_access {
	template <typename T>
	static constexpr LIBV_FORCE_INLINE void increase_ref_count(T* var) noexcept {
		++var->ref_count_;
	}

	template <typename T>
	static constexpr LIBV_FORCE_INLINE void decrease_ref_count(T* var) noexcept {
		const auto new_ref_count = --var->ref_count_;
		if (new_ref_count == 1)
			// NOTE: Multi-threading can mess the call order up
			// Implicit strand is required if ref_count_one is observed
			var->ref_count_one();
		else if (new_ref_count == 0) {
			var->ref_count_zero();
			delete var;
		}
	}
};

// --- ADL Accepting customization point -----------------------------------------------------------
// Support forward declared types with forward declared functions

template <typename T>
constexpr LIBV_FORCE_INLINE void increase_ref_count(T* var, ...) noexcept {
	static_assert(sizeof(T), "increase_ref_count is not reachable by ADL for incomplete type T");
	ref_count_access::increase_ref_count(var);
}

template <typename T>
constexpr LIBV_FORCE_INLINE void decrease_ref_count(T* var, ...) noexcept {
	static_assert(sizeof(T), "decrease_ref_count is not reachable by ADL for incomplete type T");
	ref_count_access::decrease_ref_count(var);
}

// -------------------------------------------------------------------------------------------------

class ref_count_base {
	friend ref_count_access;

private:
	mutable std::atomic<int32_t> ref_count_ = 0;

private:
	// constexpr LIBV_FORCE_INLINE void increase_ref_count() noexcept {
	// 	ref_count_access::increase_ref_count(*this);
	// }
	//
	// constexpr inline void decrease_ref_count() noexcept {
	// 	ref_count_access::decrease_ref_count(var);
	// }

	constexpr LIBV_FORCE_INLINE void ref_count_one() const noexcept {
		// Noop, entry point for static polymorphism
	}

	constexpr LIBV_FORCE_INLINE void ref_count_zero() const noexcept {
		// Noop, entry point for static polymorphism
	}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE int32_t ref_count() const noexcept {
		return ref_count_;
	}
//	[[nodiscard]] constexpr inline ... make_intrusive_from_this() const noexcept {
// 		Deducing this;
//		return ...;
//	}

	[[nodiscard]] friend constexpr inline bool operator==(const ref_count_base& lhs, const ref_count_base& rhs) noexcept {
		(void) lhs;
		(void) rhs;
		return true;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class intrusive2_ptr {
public:
	struct no_increment_tag{};
	using value_type = T;

private:
	T* ptr_ = nullptr;

public:
	constexpr LIBV_FORCE_INLINE intrusive2_ptr() noexcept = default;

	explicit(false) constexpr LIBV_FORCE_INLINE intrusive2_ptr(std::nullptr_t) noexcept { }

	explicit constexpr LIBV_FORCE_INLINE intrusive2_ptr(T* ptr) noexcept :
		ptr_(ptr) {
		if (ptr_ != nullptr)
			increase_ref_count(ptr_); // Unqualified call to ADL customization point for increase_ref_count
	}

	explicit constexpr LIBV_FORCE_INLINE intrusive2_ptr(T* ptr, no_increment_tag) noexcept :
		ptr_(ptr) {
	}

	template <std::derived_from<T> K>
	explicit(false) constexpr LIBV_FORCE_INLINE intrusive2_ptr(const intrusive2_ptr<K>& other) noexcept :
		intrusive2_ptr(other.get()) { // Delegate
	}

	template <std::derived_from<T> K>
	explicit(false) constexpr LIBV_FORCE_INLINE intrusive2_ptr(intrusive2_ptr<K>&& other) noexcept :
		ptr_(other.detach()) {
	}

	constexpr LIBV_FORCE_INLINE intrusive2_ptr(const intrusive2_ptr& other) noexcept :
		ptr_(other.ptr_) {
		if (ptr_ != nullptr)
			increase_ref_count(ptr_); // Unqualified call to ADL customization point for increase_ref_count
	}

	constexpr inline intrusive2_ptr& operator=(const intrusive2_ptr& other) & noexcept {
		// NOTE: Self assigment handled by increase ref count before decrease

		if (other.ptr_ != nullptr)
			increase_ref_count(other.ptr_); // Unqualified call to ADL customization point for increase_ref_count

		if (ptr_ != nullptr)
			decrease_ref_count(ptr_); // Unqualified call to ADL customization point for decrease_ref_count

		ptr_ = other.ptr_;
		return *this;
	}

	constexpr LIBV_FORCE_INLINE intrusive2_ptr(intrusive2_ptr&& other) noexcept :
		ptr_(other.detach()) {
	}

public:
	template <typename... Args>
		requires requires { new T(std::declval<Args>()...); }
	static constexpr LIBV_FORCE_INLINE intrusive2_ptr make(Args&&... args) {
		return intrusive2_ptr<T>(new T(std::forward<Args>(args)...));
	}

public:
	constexpr inline intrusive2_ptr& operator=(intrusive2_ptr&& other) & noexcept {
		if (this == &other)
			return *this;

		if (ptr_ != nullptr)
			decrease_ref_count(ptr_); // Unqualified call to ADL customization point for decrease_ref_count

		ptr_ = other.ptr_;
		other.ptr_ = nullptr;

		return *this;
	}

	constexpr LIBV_FORCE_INLINE ~intrusive2_ptr() {
		if (ptr_ != nullptr)
			decrease_ref_count(ptr_); // Unqualified call to ADL customization point for decrease_ref_count
	}

public:
	constexpr LIBV_FORCE_INLINE T* operator->() const noexcept {
		if (ptr_ == nullptr)
			return nullptr;
		assert(ptr_ != nullptr);
		return ptr_;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T& operator*() const noexcept {
		assert(ptr_ != nullptr);
		return *ptr_;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* ptr() const noexcept {
		return ptr_;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* get() const noexcept {
		return ptr_;
	}

	[[nodiscard]] explicit constexpr LIBV_FORCE_INLINE operator bool() const noexcept {
		return ptr_ != nullptr;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator!() const noexcept {
		return ptr_ == nullptr;
	}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* detach() noexcept {
		T* p = ptr_;
		ptr_ = nullptr;
		return p;
	}

	constexpr LIBV_FORCE_INLINE void reset() noexcept {
		if (ptr_ != nullptr)
			decrease_ref_count(ptr_); // Unqualified call to ADL customization point for decrease_ref_count
		ptr_ = nullptr;
	}

	constexpr inline void reset(T* ptr) noexcept {
		if (ptr_ != nullptr)
			decrease_ref_count(ptr_); // Unqualified call to ADL customization point for decrease_ref_count
		ptr_ = ptr;
		if (ptr_ != nullptr)
			increase_ref_count(ptr_); // Unqualified call to ADL customization point for increase_ref_count
	}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(std::nullptr_t) const noexcept {
		return ptr_ == nullptr;
	};
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator!=(std::nullptr_t) const noexcept {
		return ptr_ != nullptr;
	};
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const intrusive2_ptr& rhs) const noexcept = default;
	[[nodiscard]] constexpr LIBV_FORCE_INLINE std::strong_ordering operator<=>(const intrusive2_ptr& rhs) const noexcept = default;
//	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const T* rhs) const noexcept = default;
//	[[nodiscard]] constexpr LIBV_FORCE_INLINE std::strong_ordering operator<=>(const T* rhs) const noexcept = default;
};

// ---

template <typename T, typename... Args>
[[nodiscard]] constexpr LIBV_FORCE_INLINE intrusive2_ptr<T> make_intrusive2_ptr(Args&&... args) {
	return intrusive2_ptr<T>(new T(std::forward<Args>(args)...));
}

// ---

template <typename T, typename U>
[[nodiscard]] constexpr LIBV_FORCE_INLINE intrusive2_ptr<T> static_pointer_cast(const intrusive2_ptr<U>& p) noexcept {
	return intrusive2_ptr<T>{static_cast<T*>(p.get())};
}

template <typename T, typename U>
[[nodiscard]] constexpr LIBV_FORCE_INLINE intrusive2_ptr<T> const_pointer_cast(const intrusive2_ptr<U>& p) noexcept {
	return intrusive2_ptr<T>{const_cast<T*>(p.get())};
}

template <typename T, typename U>
[[nodiscard]] constexpr LIBV_FORCE_INLINE intrusive2_ptr<T> dynamic_pointer_cast(const intrusive2_ptr<U>& p) noexcept {
	return intrusive2_ptr<T>{dynamic_cast<T*>(p.get())};
}

template <typename T, typename U>
[[nodiscard]] constexpr LIBV_FORCE_INLINE intrusive2_ptr<T> static_pointer_cast(intrusive2_ptr<U>&& p) noexcept {
	return intrusive2_ptr<T>(static_cast<T*>(p.detach()), typename intrusive2_ptr<T>::no_increment_tag{});
}

template <typename T, typename U>
[[nodiscard]] constexpr LIBV_FORCE_INLINE intrusive2_ptr<T> const_pointer_cast(intrusive2_ptr<U>&& p) noexcept {
	return intrusive2_ptr<T>(const_cast<T*>(p.detach()), typename intrusive2_ptr<T>::no_increment_tag{});
}

template <typename T, typename U>
[[nodiscard]] constexpr inline intrusive2_ptr<T> dynamic_pointer_cast(intrusive2_ptr<U>&& p) noexcept {
	T* p2 = dynamic_cast<T*>(p.get());
	intrusive2_ptr<T> r(p2, typename intrusive2_ptr<T>::no_increment_tag{});
	if (p2)
		(void) p.detach();
	return r;
}

// -------------------------------------------------------------------------------------------------

template <typename T>
using intrusive2_ref = intrusive2_ptr<T>;

template <typename T, typename... Args>
[[nodiscard]] constexpr LIBV_FORCE_INLINE intrusive2_ref<T> make_intrusive2_ref(Args&&... args) {
	return intrusive2_ref<T>(new T(std::forward<Args>(args)...));
}

} // namespace libv --------------------------------------------------------------------------------
namespace std {

template <typename T>
struct hash<::libv::intrusive2_ptr<T>> {
	[[nodiscard]] constexpr LIBV_FORCE_INLINE std::size_t operator()(const ::libv::intrusive2_ptr<T>& p) const noexcept {
		return std::hash<T*>()(p.get());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace std
