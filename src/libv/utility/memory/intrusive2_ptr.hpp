// Project: libv.utility, File: src/libv/utility/memory/intrusive2_ptr.hpp

#pragma once

// std
#include <atomic>
#include <cassert>
#include <concepts>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct ref_count_access {
	template <typename T>
	static constexpr inline void increase_ref_count(T* var) noexcept {
		var->increase_ref_count();
	}

	template <typename T>
	static constexpr inline void decrease_ref_count(T* var) noexcept {
		var->decrease_ref_count();
	}

	template <typename T>
	static constexpr inline void ref_count_one(T* var) noexcept {
		var->ref_count_one();
	}

	/// NOTE: Multi-threading can mess the call order up: Implicit strand is required if ref_count_one is observed
	template <typename T>
	static constexpr inline void ref_count_zero(T* var) noexcept {
		var->ref_count_zero();
	}

	template <typename T>
	static constexpr inline void delete_(T* var) noexcept {
		delete var;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename CRTP>
class ref_count_base {
	friend ref_count_access;

private:
	std::atomic<int32_t> ref_count_ = 0;

private:
	constexpr inline void increase_ref_count() noexcept {
		++ref_count_;
	}

	constexpr inline void decrease_ref_count() noexcept {
		// NOTE: Multi-threading can mess the call order up: Implicit strand is required if ref_count_one is observed
		const auto new_ref_count = --ref_count_;
		if (new_ref_count == 1)
			ref_count_access::ref_count_one(static_cast<CRTP*>(this));
		else if (new_ref_count == 0) {
			ref_count_access::ref_count_zero(static_cast<CRTP*>(this));
			ref_count_access::delete_(static_cast<CRTP*>(this));
		}
	}

	constexpr inline void ref_count_one() noexcept {
		// Noop, entry point for static polymorphism
	}

	constexpr inline void ref_count_zero() noexcept {
		// Noop, entry point for static polymorphism
	}

public:
	[[nodiscard]] constexpr inline int32_t ref_count() const noexcept {
		return ref_count_;
	}
//	[[nodiscard]] constexpr inline ... make_intrusive_from_this() const noexcept {
//		return ...;
//	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class intrusive2_ptr {
public:
	struct no_increment_tag{};

private:
	T* ptr_ = nullptr;

public:
	constexpr inline intrusive2_ptr() noexcept {
	}

	explicit constexpr inline intrusive2_ptr(T* ptr) noexcept :
		ptr_(ptr) {
		if (ptr_ != nullptr)
			ref_count_access::increase_ref_count(ptr_);
	}

	explicit constexpr inline intrusive2_ptr(T* ptr, no_increment_tag) noexcept :
		ptr_(ptr) {
	}

	template <std::derived_from<T> K>
	explicit(false) constexpr inline intrusive2_ptr(const intrusive2_ptr<K>& other) noexcept :
		intrusive2_ptr(other.get()) { // Delegate
	}

	template <std::derived_from<T> K>
	explicit(false) constexpr inline intrusive2_ptr(intrusive2_ptr<K>&& other) noexcept :
		ptr_(other.detach()) {
	}

	constexpr inline intrusive2_ptr(const intrusive2_ptr& other) noexcept :
		ptr_(other.ptr_) {
		if (ptr_ != nullptr)
			ref_count_access::increase_ref_count(ptr_);
	}

	constexpr inline intrusive2_ptr& operator=(const intrusive2_ptr& other) & noexcept {
		// NOTE: Self assigment handled by increase ref count before decrease

		if (other.ptr_ != nullptr)
			ref_count_access::increase_ref_count(other.ptr_);

		if (ptr_ != nullptr)
			ref_count_access::decrease_ref_count(ptr_);

		ptr_ = other.ptr_;
		return *this;
	}

	constexpr inline intrusive2_ptr(intrusive2_ptr&& other) noexcept :
		ptr_(other.detach()) {
	}

	constexpr inline intrusive2_ptr& operator=(intrusive2_ptr&& other) & noexcept {
		if (this == &other)
			return *this;

		if (ptr_ != nullptr)
			ref_count_access::decrease_ref_count(ptr_);

		ptr_ = other.ptr_;
		other.ptr_ = nullptr;

		return *this;
	}

	constexpr inline ~intrusive2_ptr() {
		if (ptr_ != nullptr)
			ref_count_access::decrease_ref_count(ptr_);
	}

public:
	constexpr inline T* operator->() const noexcept {
		if (ptr_ == nullptr)
			return nullptr;
		assert(ptr_ != nullptr);
		return ptr_;
	}

	[[nodiscard]] constexpr inline T& operator*() const noexcept {
		assert(ptr_ != nullptr);
		return *ptr_;
	}

	[[nodiscard]] constexpr inline T* ptr() const noexcept {
		return ptr_;
	}

	[[nodiscard]] constexpr inline T* get() const noexcept {
		return ptr_;
	}

	[[nodiscard]] explicit constexpr inline operator bool() const noexcept {
		return ptr_ != nullptr;
	}

	[[nodiscard]] constexpr inline bool operator!() const noexcept {
		return ptr_ == nullptr;
	}

public:
	[[nodiscard]] constexpr inline T* detach() noexcept {
		T* p = ptr_;
		ptr_ = nullptr;
		return p;
	}

	constexpr inline void reset() noexcept {
		if (ptr_ != nullptr)
			ref_count_access::decrease_ref_count(ptr_);
		ptr_ = nullptr;
	}

	constexpr inline void reset(T* ptr) noexcept {
		if (ptr_ != nullptr)
			ref_count_access::decrease_ref_count(ptr_);
		ptr_ = ptr;
		if (ptr_ != nullptr)
			ref_count_access::increase_ref_count(ptr_);
	}

public:
	[[nodiscard]] constexpr inline bool operator==(const intrusive2_ptr& rhs) const noexcept = default;
	[[nodiscard]] constexpr inline std::strong_ordering operator<=>(const intrusive2_ptr& rhs) const noexcept = default;
//	[[nodiscard]] constexpr inline bool operator==(const T* rhs) const noexcept = default;
//	[[nodiscard]] constexpr inline std::strong_ordering operator<=>(const T* rhs) const noexcept = default;
//	[[nodiscard]] constexpr inline bool operator==(nullptr_t) const noexcept = default;
//	[[nodiscard]] constexpr inline std::strong_ordering operator<=>(nullptr_t) const noexcept = default;
};

// ---

template <typename T, typename... Args>
[[nodiscard]] constexpr inline intrusive2_ptr<T> make_intrusive2_ptr(Args&&... args) {
	return intrusive2_ptr<T>(new T(std::forward<Args>(args)...));
}

// ---

template <typename T, typename U>
[[nodiscard]] constexpr inline intrusive2_ptr<T> static_pointer_cast(const intrusive2_ptr<U>& p) noexcept {
	return static_cast<T*>(p.get());
}

template <typename T, typename U>
[[nodiscard]] constexpr inline intrusive2_ptr<T> const_pointer_cast(const intrusive2_ptr<U>& p) noexcept {
	return const_cast<T*>(p.get());
}

template <typename T, typename U>
[[nodiscard]] constexpr inline intrusive2_ptr<T> dynamic_pointer_cast(const intrusive2_ptr<U>& p) noexcept {
	return dynamic_cast<T*>(p.get());
}

template <typename T, typename U>
[[nodiscard]] constexpr inline intrusive2_ptr<T> static_pointer_cast(intrusive2_ptr<U>&& p) noexcept {
	return intrusive2_ptr<T>(static_cast<T*>(p.detach()), typename intrusive2_ptr<T>::no_increment_tag{});
}

template <typename T, typename U>
[[nodiscard]] constexpr inline intrusive2_ptr<T> const_pointer_cast(intrusive2_ptr<U>&& p) noexcept {
	return intrusive2_ptr<T>(const_cast<T*>(p.detach()), typename intrusive2_ptr<T>::no_increment_tag{});
}

template <typename T, typename U>
[[nodiscard]] constexpr inline intrusive2_ptr<T> dynamic_pointer_cast(intrusive2_ptr<U>&& p) noexcept {
	T* p2 = dynamic_cast<T*>(p.get());
	intrusive2_ptr<T> r(p2, typename intrusive2_ptr<T>::no_increment_tag{});
	if (p2)
		dynamic_cast<T*>(p.detach());
	return r;
}

} // namespace libv --------------------------------------------------------------------------------
namespace std {

template <typename T>
struct hash<::libv::intrusive2_ptr<T>> {
	std::size_t operator()(const ::libv::intrusive2_ptr<T>& p) const noexcept {
		return std::hash<T*>()(p.get());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace std
