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
#include <iostream>

namespace libv {

// TODO P1: loader cache
// TODO P1: loader cache for forward declared types?
// TODO P2: loader cache std::make_shared
// TODO P5: Possible that the redundant code with loader cache can be eliminated with a cache
//  wrapper where the cached object is an other wrapper and with the use of the shared_ptr member
//  magic ctor

// TODO P4: Loader cache helyett cache foreign construtorral!! ZSENIÁLLIS!!
//A chachecacheTrackerWrapping osztálya kap egy +arg-ot amivel ő foreign constructálja
//http://gerardmeier.com/foreign-constructors-cpp
//és talán kaphatna egy másik objektumot ami descturcálja és igazából a shared_ptr deleterje!!

// -------------------------------------------------------------------------------------------------

template<typename BaseComparator, typename T>
struct CachedArgumentComparator : BaseComparator {
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
struct CachedComparator : BaseComparator {
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

template <typename T>
struct CachePtr {
	std::weak_ptr<T> weak;
	T* ptr;

	CachePtr(const std::shared_ptr<T>& sp) : weak(sp), ptr(sp.get()) { }
};

template <size_t... Is>
struct use {
};

template <size_t... Is>
struct ignore {
};

// -------------------------------------------------------------------------------------------------

template<typename BaseComparator, typename T>
struct CacheComparator : BaseComparator {
	using is_transparent = void;
	inline bool operator()(const CachePtr<T>& lhs, const T& rhs) const {
		return BaseComparator::operator()(*lhs.ptr, rhs);
	}
	inline bool operator()(const T& lhs, const CachePtr<T>& rhs) const {
		return BaseComparator::operator()(lhs, *rhs.ptr);
	}
	inline bool operator()(const CachePtr<T>& lhs, const CachePtr<T>& rhs) const {
		return BaseComparator::operator()(*lhs.ptr, *rhs.ptr);
	}
	//
	template<typename... Args>
	inline bool operator()(const std::tuple<Args...>& args, const CachePtr<T>& cr) const {
		return BaseComparator::operator()(args, *cr.ptr);
	}
	template<typename... Args>
	inline bool operator()(const CachePtr<T>& cr, const std::tuple<Args...>& args) const {
		return BaseComparator::operator()(*cr.ptr, args);
	}
	template<typename L,
	typename = libv::disable_if<libv::is_less_comparable<std::tuple<L>, T>>,
	typename = libv::enable_if<libv::is_less_comparable<L, T>>>
	inline bool operator()(const std::tuple<L>& args, const CachePtr<T>& cr) const {
		return BaseComparator::operator()(std::get<0>(args), *cr.ptr);
	}
	template<typename L,
	typename = libv::disable_if<libv::is_less_comparable<std::tuple<L>, T>>,
	typename = libv::enable_if<libv::is_less_comparable<L, T>>>
	inline bool operator()(const CachePtr<T>& cr, const std::tuple<L>& args) const {
		return BaseComparator::operator()(*cr.ptr, std::get<0>(args));
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename ComparatorBase = std::less<void>>
class Cache {

	struct CacheTracker {
		std::mutex mutex;
		bool alive = true;
		Cache<T, ComparatorBase>* cache;
	};

private:
	using Comparator = CacheComparator<ComparatorBase, T>;
	using Container = std::multiset<CachePtr<T>, Comparator>;

private:
	std::shared_ptr<CacheTracker> cacheTracker;
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
	inline std::shared_ptr<T> getImpl(Args&&... args) {

		struct CachedObject {
			T object;
			std::shared_ptr<CacheTracker> cacheTracker;
			CachedObject(
					const std::shared_ptr<CacheTracker>& cacheTracker,
					Args&&... args) :
				object(std::forward<Args>(args)...),
				cacheTracker(cacheTracker) { }
			~CachedObject() {
				std::lock_guard<std::mutex> lk(cacheTracker->mutex);
				if (!cacheTracker->alive)
					return;

				// The cache is still alive, removing dead weak_ptr from it.
				auto& storage = cacheTracker->cache->storage;

				const auto equalRange = storage.equal_range(object);

				auto it = equalRange.first;
				while (it != equalRange.second) {
					if (it->weak.expired()) {
						storage.erase(it);
						return;
					}
					++it;
				}
				assert(false && "There must be an expired ptr in the equal range.");
			}
		};

		Arguments<CompareOptions, Args...> arguments(std::forward<Args>(args)...);

		std::lock_guard<std::mutex> lk(cacheTracker->mutex);

		const auto equalRange = storage.equal_range(arguments.comp);

		auto it = equalRange.first;
		while (it != equalRange.second) {
			if (auto sp = it->weak.lock())
				return sp;
			++it;
		}

		auto resource = libv::forward_from_tuple(
				[this](auto&&... args) {
					return std::make_shared<CachedObject>(
							cacheTracker, std::forward<decltype(args)>(args)...);
				}, std::move(arguments.all));

		std::shared_ptr<T> resultNew(resource, &resource->object);
		storage.emplace_hint(equalRange.second, resultNew);
		return resultNew;
	}

public:
	template <typename CompareOptions = void, typename... Args>
	std::shared_ptr<T> get(Args&&... args) {
		//static_assert comparable
		//static_assert constructor
		//static_assert compare out indexing
		//static_assert and sfiane for invalid CompareOptions
		return getImpl<CompareOptions>(std::forward<Args>(args)...);
	}
	inline typename Container::size_type size() {
		std::lock_guard<std::mutex> lk(cacheTracker->mutex);
		return storage.size();
	}

public:
	Cache() {
		cacheTracker = std::make_shared<CacheTracker>();
		cacheTracker->cache = this;
	}
	Cache(const Cache&) = delete;
	Cache(Cache&&) = delete;
	Cache& operator=(const Cache&) = delete;
	Cache& operator=(Cache&&) = delete;
	virtual ~Cache() {
		std::lock_guard<std::mutex> lk(cacheTracker->mutex);
		cacheTracker->alive = false;
	}
};

// =================================================================================================

template<typename T, typename Comparator = std::less<void>>
class LoaderCache {

	struct CacheTracker {
		std::mutex mutex;
		bool alive = true;
	};

	struct CachedObject : T {
		std::shared_ptr<CacheTracker> cacheTracker;
		using T::T;
		using T::load;
		using T::unload;
	};
private:
	using Type = CachedObject;
	using Container = std::set<std::weak_ptr<Type>, CachedComparator<Comparator, Type>>;

private:
	std::shared_ptr<CacheTracker> cacheTracker;
	Container cache;

private:
	void remove(Type* ptr) {
		{
			std::lock_guard<std::mutex> lk(ptr->cacheTracker->mutex);
			if (ptr->cacheTracker->alive) {
				auto result = std::equal_range(cache.begin(), cache.end(), *ptr,
						CachedComparator<Comparator, Type>());
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

		Arguments<CompareOptions, Args...> arguments(std::forward<Args>(args)...);
		{
			std::lock_guard<std::mutex> lk(cacheTracker->mutex);

			auto result = std::equal_range(cache.begin(), cache.end(),
					arguments.comp, CachedArgumentComparator<Comparator, Type>());

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
			resource->cacheTracker = cacheTracker;
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
		cacheTracker = std::make_shared<CacheTracker>();
	}
	LoaderCache(const LoaderCache&) = delete;
	LoaderCache(LoaderCache&&) = delete;
	LoaderCache& operator=(const LoaderCache&) = delete;
	LoaderCache& operator=(LoaderCache&&) = delete;
	virtual ~LoaderCache() {
		std::lock_guard<std::mutex> lk(cacheTracker->mutex);
		cacheTracker->alive = false;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace libv