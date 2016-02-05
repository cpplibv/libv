// File:   resource_cache.hpp

#pragma once

// libv
#include <libv/tuple.hpp>
#include <libv/type_traits.hpp>
#include <libv/utility.hpp>
// std
#include <algorithm>
#include <cassert>
#include <memory>
#include <mutex>
#include <set>

namespace libv {

// TODO P5: Possible that the redundant code with loader cache can be eliminated with a cache
//  wrapper where the cached object is an other wrapper and with the use of the shared_ptr member
//  magic ctor

// TODO P2: i cant use std::make_shared in caches but i can emulate it! Allocation and Placement
//	newing the sharedptr and the resource

// TODO P4: Loader cache helyett cache foreign construtorral!! ZSENIÁLLIS!!
//A chacheinternalWrapping osztálya kap egy +arg-ot amivel ő foreign constructálja
//http://gerardmeier.com/foreign-constructors-cpp
//és talán kaphatna egy másik objektumot ami descturcálja és igazából a shared_ptr deleterje!!

// TODO P1: cache for forward declared types?

// -------------------------------------------------------------------------------------------------

template<typename BaseComparator, typename T>
struct ChachedArgumentComparator : BaseComparator {
	template<typename... Args>
	inline bool operator()(const std::tuple<Args...>& args, const std::weak_ptr<T>& cr) const {
		assert(!cr.expired());
		return BaseComparator::operator()(args, *cr.lock());
	}
	template<typename... Args>
	inline bool operator()(const std::weak_ptr<T>& cr, const std::tuple<Args...>& args) const {
		assert(!cr.expired());
		return BaseComparator::operator()(*cr.lock(), args);
	}
	template<typename L,
	typename = libv::disable_if<libv::is_less_comparable<std::tuple<L>, T>>,
	typename = libv::enable_if<libv::is_less_comparable<L, T>>>
	inline bool operator()(const std::tuple<L>& args, const std::weak_ptr<T>& cr) const {
		assert(!cr.expired());
		return BaseComparator::operator()(std::get<0>(args), *cr.lock());
	}
	template<typename L,
	typename = libv::disable_if<libv::is_less_comparable<std::tuple<L>, T>>,
	typename = libv::enable_if<libv::is_less_comparable<L, T>>>
	inline bool operator()(const std::weak_ptr<T>& cr, const std::tuple<L>& args) const {
		assert(!cr.expired());
		return BaseComparator::operator()(*cr.lock(), std::get<0>(args));
	}
};

template<typename BaseComparator, typename T>
struct ChachedComparator : BaseComparator {
	inline bool operator()(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>&rhs) const {
		assert(!lhs.expired());
		assert(!rhs.expired());
		return BaseComparator::operator()(*lhs.lock(), *rhs.lock());
	}
	inline bool operator()(const std::weak_ptr<T>& lhs, const T& rhs) const {
		return !lhs.expired() && BaseComparator::operator()(*lhs.lock(), rhs);
	}
	inline bool operator()(const T& lhs, const std::weak_ptr<T>&rhs) const {
		return !rhs.expired() && BaseComparator::operator()(lhs, *rhs.lock());
	}
};

// -------------------------------------------------------------------------------------------------

template <size_t... Is>
struct use {
};

template <size_t... Is>
struct ignore {
};

// -------------------------------------------------------------------------------------------------

template<typename T, typename Comparator = std::less<void>>
class Cache {

	struct CacheInternal {
		std::mutex mutex;
		bool alive = true;
	};

	struct CachedObject : T {
		std::shared_ptr<CacheInternal> internal;
		using T::T;
	};
private:
	using Type = CachedObject;
	using Container = std::set<std::weak_ptr<Type>, ChachedComparator<Comparator, Type>>;

private:
	std::shared_ptr<CacheInternal> internal;
	Container cache;

private:
	void remove(Type* ptr) {
		{
			std::lock_guard<std::mutex> lk(ptr->internal->mutex);
			if (ptr->internal->alive) {
				auto result = std::equal_range(cache.begin(), cache.end(), *ptr,
						ChachedComparator<Comparator, Type>());
				assert(result.first != result.second);
				cache.erase(result.first);
			}
		}
		delete ptr;
	}

	// ---------------------------------------------------------------------------------------------

private:
	template<typename CompareOptions, typename... Args>
	struct Arguments;

	template<typename... Args>
	struct Arguments<void, Args...> {
		std::tuple<Args&&...> all;
		std::tuple<Args&&...>& comp;
		Arguments(Args&&... args) :
			all(std::forward<Args>(args)...),
			comp(all) { }
	};

	template<size_t... Is, typename... Args>
	struct Arguments<use<Is...>, Args...> {
		std::tuple<Args&&...> all;
		decltype(libv::mask_tuple(all, std::index_sequence<Is...>())) comp;
		Arguments(Args&&... args) :
			all(std::forward<Args>(args)...),
			comp(libv::mask_tuple(all, std::index_sequence<Is...>())) { }
	};

	//	template<size_t... Is, typename... Args>
	//	struct Arguments<ignore<Is...>, Args...> {
	//		std::tuple<Args&&...> all;
	//		decltype(libv::mask_tuple(all, std::index_sequence<Is...>())) comp;
	//		Arguments(Args&&... args) :
	//			all(std::forward<Args>(args)...),
	//			comp(libv::mask_tuple(all, std::index_sequence<Is...>())) { }
	//	};

	// ---------------------------------------------------------------------------------------------

public:
	template<typename CompareOptions, typename... Args>
	inline std::shared_ptr<Type> getImpl(Args&&... args) {
		std::shared_ptr<Type> resource;
		{
			std::lock_guard<std::mutex> lk(internal->mutex);

			Arguments<CompareOptions, Args...> arguments(std::forward<Args>(args)...);

			auto result = std::equal_range(cache.begin(), cache.end(),
					arguments.comp, ChachedArgumentComparator<Comparator, Type>());

			if (result.first != result.second) {
				assert(!result.first->expired());
				return result.first->lock();
			}

			Type * (*newAddr)(Args&&...) = &libv::new_f;
			resource = std::shared_ptr<Type>(
					libv::forward_from_tuple(newAddr, std::move(arguments.all)),
					[this](Type * ptr) {
						remove(ptr);
					});
			resource->internal = internal;
			cache.emplace(resource);
		}
		return resource;
	}

public:
	template<typename CompareOptions = void, typename... Args>
	std::shared_ptr<T> get(Args&&... args) {
		//static_assert comparable
		//static_assert constructor
		//static_assert compare out indexing
		//static_assert and sfiane for invalid CompareOptions
		return getImpl<CompareOptions>(std::forward<Args>(args)...);
	}
	inline typename Container::size_type size() {
		return cache.size();
	}

public:
	Cache() {
		internal = std::make_shared<CacheInternal>();
	}
	Cache(const Cache&) = delete;
	Cache(Cache&&) = delete;
	Cache& operator=(const Cache&) = delete;
	Cache& operator=(Cache&&) = delete;
	virtual ~Cache() {
		std::lock_guard<std::mutex> lk(internal->mutex);
		internal->alive = false;
	}
};

// =================================================================================================

template<typename T, typename Comparator = std::less<void>>
class LoaderCache {

	struct CacheInternal {
		std::mutex mutex;
		bool alive = true;
	};

	struct CachedObject : T {
		std::shared_ptr<CacheInternal> internal;
		using T::T;
		using T::load;
		using T::unload;
	};
private:
	using Type = CachedObject;
	using Container = std::set<std::weak_ptr<Type>, ChachedComparator<Comparator, Type>>;

private:
	std::shared_ptr<CacheInternal> internal;
	Container cache;

private:
	void remove(Type* ptr) {
		{
			std::lock_guard<std::mutex> lk(ptr->internal->mutex);
			if (ptr->internal->alive) {
				auto result = std::equal_range(cache.begin(), cache.end(), *ptr,
						ChachedComparator<Comparator, Type>());
				assert(result.first != result.second);
				cache.erase(result.first);
			}
		}
		ptr->unload(std::shared_ptr<T>(ptr)); //implicit delete via shared_ptr
	}

	// ---------------------------------------------------------------------------------------------

private:
	template<typename CompareOptions, typename... Args>
	struct Arguments;

	template<typename... Args>
	struct Arguments<void, Args...> {
		std::tuple<Args&&...> all;
		std::tuple<Args&&...>& comp;
		Arguments(Args&&... args) :
			all(std::forward<Args>(args)...),
			comp(all) { }
	};

	template<size_t... Is, typename... Args>
	struct Arguments<use<Is...>, Args...> {
		std::tuple<Args&&...> all;
		decltype(libv::mask_tuple(all, std::index_sequence<Is...>())) comp;
		Arguments(Args&&... args) :
			all(std::forward<Args>(args)...),
			comp(libv::mask_tuple(all, std::index_sequence<Is...>())) { }
	};

	//	template<size_t... Is, typename... Args>
	//	struct Arguments<ignore<Is...>, Args...> {
	//		std::tuple<Args&&...> all;
	//		decltype(libv::mask_tuple(all, std::index_sequence<Is...>())) comp;
	//		Arguments(Args&&... args) :
	//			all(std::forward<Args>(args)...),
	//			comp(libv::mask_tuple(all, std::index_sequence<Is...>())) { }
	//	};

	// ---------------------------------------------------------------------------------------------

public:
	template<typename CompareOptions, typename... Args>
	inline std::shared_ptr<Type> getImpl(Args&&... args) {
		std::shared_ptr<Type> resource;
		{
			std::lock_guard<std::mutex> lk(internal->mutex);

			Arguments<CompareOptions, Args...> arguments(std::forward<Args>(args)...);

			auto result = std::equal_range(cache.begin(), cache.end(),
					arguments.comp, ChachedArgumentComparator<Comparator, Type>());

			if (result.first != result.second) {
				assert(!result.first->expired());
				return result.first->lock();
			}

			Type * (*newAddr)(Args&&...) = &libv::new_f;
			resource = std::shared_ptr<Type>(
					libv::forward_from_tuple(newAddr, std::move(arguments.all)),
					[this](Type * ptr) {
						remove(ptr);
					});
			resource->internal = internal;
			cache.emplace(resource);
		}
		resource->load(resource);
		return resource;
	}

public:
	template<typename CompareOptions = void, typename... Args>
	std::shared_ptr<T> get(Args&&... args) {
		// TODO P3: static_assert comparable
		// TODO P3: static_assert constructor
		// TODO P3: static_assert compare out indexing
		// TODO P3: static_assert and sfiane for invalid CompareOptions
		return getImpl<CompareOptions>(std::forward<Args>(args)...);
	}
	inline typename Container::size_type size() {
		return cache.size();
	}

public:
	LoaderCache() {
		internal = std::make_shared<CacheInternal>();
	}
	LoaderCache(const LoaderCache&) = delete;
	LoaderCache(LoaderCache&&) = delete;
	LoaderCache& operator=(const LoaderCache&) = delete;
	LoaderCache& operator=(LoaderCache&&) = delete;
	virtual ~LoaderCache() {
		std::lock_guard<std::mutex> lk(internal->mutex);
		internal->alive = false;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace libv