// File: resource_cache.hpp

#pragma once

// libv
#include <libv/utility/tuple.hpp>
#include <libv/meta/type_traits.hpp>
// std
#include <algorithm>
#include <cassert>
#include <memory>
#include <mutex>
#include <set>

namespace libv {

// TODO P2: loader cache: merge the two shared_ptr into one shared which allocator / deallocator creates another one

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

template <typename BaseComparator, typename T>
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
	template <typename... Args>
	inline bool operator()(const std::tuple<Args...>& args, const CachePtr<T>& cr) const {
		return BaseComparator::operator()(args, *cr.ptr);
	}
	template <typename... Args>
	inline bool operator()(const CachePtr<T>& cr, const std::tuple<Args...>& args) const {
		return BaseComparator::operator()(*cr.ptr, args);
	}
	template <typename L,
	typename = meta::disable_if<meta::is_less_comparable<std::tuple<L>, T>>,
	typename = meta::enable_if<meta::is_less_comparable<L, T>>>
	inline bool operator()(const std::tuple<L>& args, const CachePtr<T>& cr) const {
		return BaseComparator::operator()(std::get<0>(args), *cr.ptr);
	}
	template <typename L,
	typename = meta::disable_if<meta::is_less_comparable<std::tuple<L>, T>>,
	typename = meta::enable_if<meta::is_less_comparable<L, T>>>
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
	template <typename CompareOptions, typename... Args>
	std::shared_ptr<T> getImpl(Args&&... args) {

		// This is a member class to allow forward declared type
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
					if (&object == it->ptr) {
						storage.erase(it);
						return;
					}
					++it;
				}
				assert(false && "There must be an expired ptr in the equal range.");
			}
		};

		std::lock_guard<std::mutex> lk(cacheTracker->mutex);

		Arguments<CompareOptions, Args...> arguments(std::forward<Args>(args)...);

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
	inline std::shared_ptr<T> get(Args&&... args) {
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
	~Cache() {
		std::lock_guard<std::mutex> lk(cacheTracker->mutex);
		cacheTracker->alive = false;
	}
};

// LoaderCache =================================================================================================

template <typename T>
struct LoadableResource {
	std::shared_ptr<T> obj_ptr;

	template <typename... Args>
	inline LoadableResource(Args&&... args) : obj_ptr(std::make_shared<T>(std::forward<Args>(args)...)) {
		obj_ptr->load(obj_ptr);
	}
	virtual ~LoadableResource() {
		obj_ptr->unload(obj_ptr);
	}

	bool operator<(const LoadableResource<T>& r) const {
		return *obj_ptr < *r.obj_ptr;
	}
	template <typename... Args>
	friend inline bool operator<(const std::tuple<Args...>& args, const LoadableResource<T>& r) {
		return args < *r.obj_ptr;
	}
	template <typename... Args>
	friend inline bool operator<(const LoadableResource<T>& l, const std::tuple<Args...>& args) {
		return *l.obj_ptr < args;
	}
	template <typename L,
	typename = meta::disable_if<meta::is_less_comparable<std::tuple<L>, T>>,
	typename = meta::enable_if<meta::is_less_comparable<L, T>>>
	friend inline bool operator<(const std::tuple<L>& args, const LoadableResource<T>& r) {
		return std::get<0>(args) < *r.obj_ptr;
	}
	template <typename L,
	typename = meta::disable_if<meta::is_less_comparable<std::tuple<L>, T>>,
	typename = meta::enable_if<meta::is_less_comparable<L, T>>>
	friend inline bool operator<(const LoadableResource<T>& l, const std::tuple<L>& args) {
		return *l.obj_ptr < std::get<0>(args);
	}
};

template <typename T, typename Comparator = std::less<void>>
struct LoaderCache : private Cache<LoadableResource<T>, Comparator> {
private:
	using Base = Cache<LoadableResource<T>, Comparator>;
public:
	using Base::size;
	LoaderCache(const LoaderCache&) = delete;
	LoaderCache(LoaderCache&&) = delete;
	LoaderCache& operator=(const LoaderCache&) = delete;
	LoaderCache& operator=(LoaderCache&&) = delete;
public:
	template <typename CompareOptions = void, typename... Args>
	std::shared_ptr<T> get(Args&&... args) {
		//static_assert comparable
		//static_assert constructor
		//static_assert compare out indexing
		//static_assert and sfiane for invalid CompareOptions
		std::shared_ptr<LoadableResource<T>> sp = Base::template get<CompareOptions>(std::forward<Args>(args)...);
		return std::shared_ptr<T>(sp, sp->obj_ptr.get());
	}
public:
	LoaderCache() = default;
	~LoaderCache() = default;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv