// File:   resource_cache.hpp

#pragma once

// libv
#include <libv/tuple.hpp>
#include <libv/type_traits.hpp>
// std
#include <algorithm>
#include <atomic>
#include <cassert>
#include <memory>
#include <mutex>
#include <set>

namespace libv {

template <typename T>
class cache_ptr;

template <typename T>
class cache_ptr_host;

// -------------------------------------------------------------------------------------------------

template <typename T>
struct cache_ptr_host {
	// For multi-thread support scoped cache's mutex will "guard" our host object
	// NOTE: Codes are based on the relative memory position between counter and data!
	mutable std::atomic_size_t counter{0};
	mutable T data;

	template <typename... Args>
	cache_ptr_host(Args&&... args) : data(std::forward<Args>(args)...) { }

	inline cache_ptr<T> make_cache_ptr() const {
		return cache_ptr<T>(&data);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class cache_ptr {
	T* ptr;
public:
	using element_type = T;
	using pointer = T*;
	// ---------------------------------------------------------------------------------------------
	constexpr inline cache_ptr() noexcept : ptr(nullptr) { }
	constexpr explicit inline cache_ptr(std::nullptr_t) noexcept : ptr(nullptr) { }
	constexpr explicit inline cache_ptr(T* p) noexcept : ptr(p) {
		if (ptr)
			++*(reinterpret_cast<std::atomic_int*>(ptr) - 1);
	}
	template<typename K, typename = enable_if<std::is_base_of<T, K>>>
	constexpr explicit inline cache_ptr(cache_ptr<K> other) noexcept : ptr(other.get()) {
		other.ptr = nullptr;
	}
	template<typename K, typename = enable_if<std::is_base_of<T, K>>>
	constexpr explicit inline cache_ptr(const cache_ptr<K>& other) noexcept : ptr(other.get()) {
		if (ptr)
			++*(reinterpret_cast<std::atomic_int*>(ptr) - 1);
	}
	template<typename K, typename = enable_if<std::is_base_of<T, K>>>
	constexpr explicit inline cache_ptr(cache_ptr<K>&& other) noexcept : ptr(other.get()) {
		other.ptr = nullptr;
	}

	constexpr inline cache_ptr(const cache_ptr<T>& other) noexcept : ptr(other.get()) {
		if (ptr)
			++*(reinterpret_cast<std::atomic_int*>(ptr) - 1);
	}
	constexpr inline cache_ptr(cache_ptr<T>&& other) noexcept : ptr(other.get()) {
		other.ptr = nullptr;
	}

	~cache_ptr() {
		if (ptr)
			--*(reinterpret_cast<std::atomic_int*>(ptr) - 1);
	}
	// ---------------------------------------------------------------------------------------------
	cache_ptr<T>& operator=(cache_ptr<T> rhs) {
		std::swap(ptr, rhs.ptr);
		return *this;
	}
	cache_ptr<T>& operator=(const cache_ptr<T>& rhs) {
		reset();
		ptr = rhs.ptr;
		if (ptr)
			++*(reinterpret_cast<std::atomic_int*>(ptr) - 1);
		return *this;
	}
	cache_ptr<T>& operator=(cache_ptr<T>&& rhs) {
		std::swap(ptr, rhs.ptr);
		return *this;
	}
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
	constexpr inline void reset() noexcept {
		if (ptr)
			--*(reinterpret_cast<std::atomic_int*>(ptr) - 1);
		ptr = nullptr;
	}
	template<typename K>
	constexpr inline void swap(cache_ptr<K>& other) noexcept {
		using std::swap;
		swap(ptr, other.ptr);
	}
};
template<typename T1, typename T2>
inline bool operator==(cache_ptr<T1> p1, cache_ptr<T2> p2) {
	return p1.get() == p2.get();
}
template<typename T1, typename T2>
inline bool operator!=(cache_ptr<T1> p1, cache_ptr<T2> p2) {
	return !(p1 == p2);
}
template<typename T>
inline bool operator==(cache_ptr<T> p, std::nullptr_t) noexcept {
	return !p;
}
template<typename T>
inline bool operator==(std::nullptr_t, cache_ptr<T> p) noexcept {
	return !p;
}
template<typename T>
inline bool operator!=(cache_ptr<T> p, std::nullptr_t) noexcept {
	return (bool)p;
}
template<typename T>
inline bool operator!=(std::nullptr_t, cache_ptr<T> p) noexcept {
	return (bool)p;
}
template<typename T1, typename T2>
inline bool operator<(cache_ptr<T1> p1, cache_ptr<T2> p2) {
	return std::less<T1*>()(p1.get(), p2.get());
	// return std::less<T3>()( p1.get(), p2.get() );
	// where T3 is the composite T* type (C++14 §5) of T1* and T2*.
}
template<typename T>
inline bool operator>(cache_ptr<T> p1, cache_ptr<T> p2) {
	return p2 < p1;
}
template<typename T>
inline bool operator<=(cache_ptr<T> p1, cache_ptr<T> p2) {
	return !(p2 < p1);
}
template<typename T>
inline bool operator>=(cache_ptr<T> p1, cache_ptr<T> p2) {
	return !(p1 < p2);
}
// specialized algorithms --------------------------------------------------------------------------
template<typename T>
inline void swap(cache_ptr<T> & p1, cache_ptr<T> & p2) noexcept {
	p1.swap(p2);
}

} //namespace libv

namespace std {

template<typename T>
struct hash<::libv::cache_ptr<T>> {
	inline size_t operator()(::libv::cache_ptr<T> p) {
		return hash<T*>()(p.get());
	}
};

} //namespace std

namespace libv {

// =================================================================================================

template <size_t... Is>
struct use {
};

template <size_t... Is>
struct ignore {
};

// -------------------------------------------------------------------------------------------------

template<typename BaseComparator, typename T>
struct ScopedCacheComparator : BaseComparator {
	using is_transparent = void;
	inline bool operator()(const std::unique_ptr<cache_ptr_host<T>>& lhs, const T& rhs) const {
		return BaseComparator::operator()(lhs->data, rhs);
	}
	inline bool operator()(const T& lhs, const std::unique_ptr<cache_ptr_host<T>>& rhs) const {
		return BaseComparator::operator()(lhs, rhs->data);
	}
	inline bool operator()(const std::unique_ptr<cache_ptr_host<T>>& lhs, const std::unique_ptr<cache_ptr_host<T>>& rhs) const {
		return BaseComparator::operator()(lhs->data, rhs->data);
	}
	//
	template<typename... Args>
	inline bool operator()(const std::tuple<Args...>& args, const std::unique_ptr<cache_ptr_host<T>>& cr) const {
		return BaseComparator::operator()(args, cr->data);
	}
	template<typename... Args>
	inline bool operator()(const std::unique_ptr<cache_ptr_host<T>>& cr, const std::tuple<Args...>& args) const {
		return BaseComparator::operator()(cr->data, args);
	}
	template<typename L,
	typename = libv::disable_if<libv::is_less_comparable<std::tuple<L>, T>>,
	typename = libv::enable_if<libv::is_less_comparable<L, T>>>
	inline bool operator()(const std::tuple<L>& args, const std::unique_ptr<cache_ptr_host<T>>& cr) const {
		return BaseComparator::operator()(std::get<0>(args), cr->data);
	}
	template<typename L,
	typename = libv::disable_if<libv::is_less_comparable<std::tuple<L>, T>>,
	typename = libv::enable_if<libv::is_less_comparable<L, T>>>
	inline bool operator()(const std::unique_ptr<cache_ptr_host<T>>& cr, const std::tuple<L>& args) const {
		return BaseComparator::operator()(cr->data, std::get<0>(args));
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename ComparatorBase = std::less<void>>
class ScopedCache {
private:
	using Comparator = ScopedCacheComparator<ComparatorBase, T>;
	using Container = std::set<std::unique_ptr<cache_ptr_host<T>>, Comparator>;

private:
	mutable std::mutex mutex;
	Container storage;

	// ---------------------------------------------------------------------------------------------

private:
	template <typename CompareOptions, typename... Args>
	struct Arguments;

	template <typename... Args>
	struct Arguments<void, Args...> {
		std::tuple<Args&&...> all;
		std::tuple<Args&&...>& comp;
		Arguments(Args&&... args) :
			all(std::forward<Args>(args)...),
			comp(all) { }
	};

	template <size_t... Is, typename... Args>
	struct Arguments<use<Is...>, Args...> {
		std::tuple<Args&&...> all;
		decltype(libv::mask_tuple(all, std::index_sequence<Is...>())) comp;
		Arguments(Args&&... args) :
			all(std::forward<Args>(args)...),
			comp(libv::mask_tuple(all, std::index_sequence<Is...>())) { }
	};

	//	template <size_t... Is, typename... Args>
	//	struct Arguments<ignore<Is...>, Args...> {
	//		std::tuple<Args&&...> all;
	//		decltype(libv::mask_tuple(all, std::index_sequence<Is...>())) comp;
	//		Arguments(Args&&... args) :
	//			all(std::forward<Args>(args)...),
	//			comp(libv::mask_tuple(all, std::index_sequence<Is...>())) { }
	//	};

	// ---------------------------------------------------------------------------------------------

private:
	template<typename CompareOptions, typename... Args>
	std::pair<cache_ptr<T>, bool> getImpl(Args&&... args) {
		Arguments<CompareOptions, Args...> arguments(std::forward<Args>(args)...);

		auto it = storage.find(arguments.comp);
		if (it != storage.end())
			return std::make_pair((*it)->make_cache_ptr(), false);

		auto resource = libv::forward_from_tuple(
				[this](auto&&... args) {
					return std::make_unique<cache_ptr_host<T>>(
							std::forward<decltype(args)>(args)...);
				}, std::move(arguments.all));

		auto ptr = resource->make_cache_ptr();
		storage.emplace(std::move(resource));
		return std::make_pair(ptr, true);
	}

public:
	template <typename CompareOptions = void, typename... Args>
	inline std::pair<cache_ptr<T>, bool> get(Args&&... args) {
		//static_assert comparable
		//static_assert constructor
		//static_assert compare out indexing
		//static_assert and sfiane for invalid CompareOptions
		std::lock_guard<std::mutex> lk(mutex);
		return getImpl<CompareOptions>(std::forward<Args>(args)...);
	}
	inline typename Container::size_type size() {
		std::lock_guard<std::mutex> lk(mutex);
		return storage.size();
	}
	void clear() {
		std::lock_guard<std::mutex> lk(mutex);
		for (auto it = storage.begin(); it != storage.end(); ++it) {
			assert((*it)->counter.load() == 0);
		}
		storage.clear();
	}
	void cleanup() {
		std::lock_guard<std::mutex> lk(mutex);
		for (auto it = storage.begin(); it != storage.end();) {
			if ((*it)->counter == 0)
				it = storage.erase(it);
			 else
				++it;
		}
	}
public:
	ScopedCache() {
	}
	ScopedCache(const ScopedCache&) = delete;
	ScopedCache(ScopedCache&&) = delete;
	ScopedCache& operator=(const ScopedCache&) = delete;
	ScopedCache& operator=(ScopedCache&&) = delete;
	~ScopedCache() {
		for (auto it = storage.begin(); it != storage.end(); ++it) {
			assert((*it)->counter.load() == 0);
		}
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace libv