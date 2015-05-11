// File: resourceManager.hpp, Created on 2014. november 28. 21:18, Author: Vader

#pragma once

// std
#include <mutex>
#include <algorithm>
#include <cassert>
#include <memory>
#include <set>

#include <iostream>

namespace vl {

template<typename T> class ResourceManager;
template<typename T> class ResourceScope;

//------------------------------------------------------------------------------------------

template<typename T>
class Resource {
private:
	std::shared_ptr<T> ptr;

public:
	inline T* operator->() {
		return ptr.get();
	}
	inline const T* operator->() const {
		return ptr.get();
	}
	inline T& operator*() {
		return *ptr.get();
	}
	inline const T& operator*() const {
		return *ptr.get();
	}
	inline T& data() {
		return *ptr.get();
	}
	inline const T& data() const {
		return *ptr.get();
	}
	template<typename... Args>
	Resource(ResourceManager<T>& resourceManager, Args&&... args) : ptr(resourceManager.request(std::forward<Args>(args)...)) { }
	template<typename... Args>
	Resource(ResourceScope<T>& resourceScope, Args&&... args) : ptr(resourceScope.request(std::forward<Args>(args)...)) { }
	virtual ~Resource() = default;
};

//------------------------------------------------------------------------------------------

struct ChachedResourceArgumentComparator {
	template<typename T, typename... Args>
	inline bool operator()(const std::tuple<Args...>& args, const std::weak_ptr<T>&cr) const {
		assert(!cr.expired());
		return args < *cr.lock();
	}
	template<typename T, typename... Args>
	inline bool operator()(const std::weak_ptr<T>&cr, const std::tuple<Args...>& args) const {
		assert(!cr.expired());
		return *cr.lock() < args;
	}
};

template<typename T>
struct ChachedResourceComparator {
	inline bool operator()(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>&rhs) const {
		assert(!rhs.expired() && !lhs.expired());
		return *lhs.lock() < *rhs.lock();
	}
	inline bool operator()(const std::weak_ptr<T>& lhs, const T& rhs) const {
		assert(!lhs.expired());
		return *lhs.lock() < rhs;
	}
	inline bool operator()(const T& lhs, const std::weak_ptr<T>&rhs) const {
		assert(!rhs.expired());
		return lhs < *rhs.lock();
	}
};

//------------------------------------------------------------------------------------------

template<typename T> //, typename Allocator = std::allocator<T>>
//TODO P2: Custom allocator (VERY important) for you know... reasons
//Or maybe type ereased allocators? / deleter?
class ResourceManager {
	friend class Resource<T>;
	friend class ResourceScope<T>;
private:
	std::mutex cache_m;
	std::set<std::weak_ptr<T>, ChachedResourceComparator<T>> cache;

private:
	template<typename... Args>
	std::shared_ptr<T> request(Args&&... args) {
		std::shared_ptr<T> resource;
		std::unique_lock<std::mutex> lock_guard(cache_m);
		auto result = std::equal_range(cache.begin(), cache.end(), std::make_tuple(args...), ChachedResourceArgumentComparator());

		if (result.first != result.second)
			resource = result.first->lock();

		if (!resource)
			resource.reset(new T(args...), [this](T*& ptr) {
				std::unique_lock<std::mutex> lock_guard(cache_m);
				//auto result = std::equal_range(cache.begin(), cache.end(), *ptr, ChachedResourceComparatorAllowingExpiered<T>());
				//assert(result.first != result.second);
				//cache.erase(result.first);

				auto it = cache.begin();
				while (it != cache.end()) { //lin search!
					if (it->expired()) {
						cache.erase(it++);
						break;
					} else
						++it;
					}
				delete ptr;
			});

		cache.emplace(resource);

		return resource;
	}
public:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	virtual ~ResourceManager() = default;
};

//------------------------------------------------------------------------------------------

template<typename T>
class ResourceScope {
	friend class Resource<T>;
private:
	ResourceManager<T>& rm;
	std::mutex keep_alive_m;
	std::set<std::shared_ptr<T>> keep_alive;
private:
	template<typename... Args>
	std::shared_ptr<T> request(Args&&... args) {
		std::shared_ptr<T> result(rm.request(std::forward<Args>(args)...));
		std::unique_lock<std::mutex> lock_guard(keep_alive_m);
		keep_alive.insert(result);
		return result;
	}
public:
	ResourceScope(ResourceManager<T>& rm) : rm(rm) { }
	ResourceScope(ResourceScope<T>& rs) : rm(rs.rm) { }
};

} //namespace vl