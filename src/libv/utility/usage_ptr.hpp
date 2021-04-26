// Created by Vader on 2021.04.26..

#pragma once

// std
#include <atomic>
#include <cassert>
#include <type_traits>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct Usable {
	std::atomic<uint32_t> ref_count = 0;

	inline void increment_ref_count() noexcept {
		++ref_count;
	}

	inline void decrement_ref_count() noexcept {
		if (--ref_count == 0)
			handle_last_ref();
	}

	inline void handle_last_ref() noexcept {
//		disconnect_from_anything_connected_to_this();
//		delete this;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class usage_ptr {
	T* ptr_ = nullptr;

public:
	template <typename... Args>
	inline explicit usage_ptr(Args&&... args) {
		ptr_ = new T(std::forward<Args>(args)...);
		ptr_->increment_ref_count();
	}
//	inline explicit usage_ptr(raw_ptr<BaseHandler> ptr_ptr) noexcept :
//		ptr_(ptr_ptr) {
//		ptr_->increment_ref_count();
//	}

public:
//	template <typename K>
//	inline /*implicit*/ usage_ptr(const usage_ptr<BaseHandler, K>& orig) noexcept
//		requires std::derived_from<K, T> || std::same_as<const K*, T*> || std::same_as<T, void> :
//		ptr_(orig.ptr_) {
//		ptr_->increment_ref_count();
//	}
//	template <typename K>
//	inline /*implicit*/ usage_ptr(usage_ptr<BaseHandler, K>&& orig) noexcept
//		requires std::derived_from<K, T> || std::same_as<const K*, T*> || std::same_as<T, void> :
//		ptr_(orig.ptr_) {
//		orig.ptr_ = nullptr;
//		orig.user_object = nullptr;
//	}

public:
	inline usage_ptr(const usage_ptr& orig) noexcept :
		ptr_(orig.ptr_) {
		ptr_->increment_ref_count();
	}

	inline usage_ptr(usage_ptr&& orig) noexcept :
		ptr_(orig.ptr_) {
		orig.ptr_ = nullptr;
	}
	inline usage_ptr& operator=(const usage_ptr& orig) & noexcept {
		ptr_ = orig.ptr_;
		ptr_->increment_ref_count();
		return *this;
	}
	inline usage_ptr& operator=(usage_ptr&& orig) & noexcept {
		ptr_ = std::move(orig.ptr_);
		orig.ptr_ = nullptr;
		return *this;
	}
	inline ~usage_ptr() {
		if (ptr_ != nullptr)
			ptr_->decrement_ref_count();
	}

public:
	inline void clear() noexcept {
		assert(ptr_ != nullptr);
		ptr_->decrement_ref_count();
		ptr_ = nullptr;
	}

public:
	inline operator bool() const noexcept {
		return ptr_ != nullptr;
	}

public:
	inline T* operator->() const noexcept
			requires (!std::is_void_v<T>) {
		assert(ptr_ != nullptr);
		return ptr_;
	}

	template <typename TT = T>
			requires (!std::is_void_v<TT>)
	inline TT& operator*() const noexcept {
		assert(ptr_ != nullptr);
		return *ptr_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv

namespace std { // ---------------------------------------------------------------------------------

template <typename T>
struct hash<libv::usage_ptr<T>> {
	constexpr auto operator()(const libv::usage_ptr<T>& t) const noexcept {
		return std::hash<T*>{}(t.get());
	}
};

} // namespace std ---------------------------------------------------------------------------------
