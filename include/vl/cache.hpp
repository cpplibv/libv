// File:   resource_cache.hpp

#pragma once

// vl
#include <vl/operator.hpp>
#include <vl/tuple.hpp>
#include <vl/type_traits.hpp>
#include <vl/utility.hpp>
// std
#include <algorithm>
#include <cassert>
#include <memory>
#include <mutex>
#include <set>

namespace vl {

//------------------------------------------------------------------------------------------

template<typename Comparator, typename T>
struct ChachedArgumentComparator : Comparator {
	template<typename... Args>
	inline bool operator()(const std::tuple<Args...>& args, const std::weak_ptr<T>& cr) const {
		assert(!cr.expired());
		return Comparator::operator()(args, *cr.lock());
	}
	template<typename... Args>
	inline bool operator()(const std::weak_ptr<T>& cr, const std::tuple<Args...>& args) const {
		assert(!cr.expired());
		return Comparator::operator()(*cr.lock(), args);
	}
	template<typename L,
	typename = vl::disable_if<vl::is_less_comparable<std::tuple<L>, T>>,
	typename = vl::enable_if<vl::is_less_comparable<L, T>>>
	inline bool operator()(const std::tuple<L>& args, const std::weak_ptr<T>& cr) const {
		assert(!cr.expired());
		return Comparator::operator()(std::get<0>(args), *cr.lock());
	}
	template<typename L,
	typename = vl::disable_if<vl::is_less_comparable<std::tuple<L>, T>>,
	typename = vl::enable_if<vl::is_less_comparable<L, T>>>
	inline bool operator()(const std::weak_ptr<T>& cr, const std::tuple<L>& args) const {
		assert(!cr.expired());
		return Comparator::operator()(*cr.lock(), std::get<0>(args));
	}
};

template<typename Comparator, typename T>
struct ChachedComparator : Comparator {
	inline bool operator()(const std::weak_ptr<T>& lhs, const std::weak_ptr<T>&rhs) const {
		assert(!rhs.expired() && !lhs.expired());
		return Comparator::operator()(*lhs.lock(), *rhs.lock());
	}
	inline bool operator()(const std::weak_ptr<T>& lhs, const T& rhs) const {
		return !lhs.expired() && Comparator::operator()(*lhs.lock(), rhs);
	}
	inline bool operator()(const T& lhs, const std::weak_ptr<T>&rhs) const {
		return !rhs.expired() && Comparator::operator()(lhs, *rhs.lock());
	}
};

//------------------------------------------------------------------------------------------

template<typename T, typename Comparator = vl::less>
class Cache {
	using Container = std::set<std::weak_ptr<T>, ChachedComparator<Comparator, T>>;
private:
	std::mutex cache_m;
	Container cache;
public:
	template<typename... Args>
	std::shared_ptr<T> get(Args&&... args) {
		std::shared_ptr<T> resource;
		std::unique_lock<std::mutex> lock_guard(cache_m);

		std::tuple<Args&&...> argsPack(std::forward<Args>(args)...);
		auto result = std::equal_range(cache.begin(), cache.end(),
				argsPack, ChachedArgumentComparator<Comparator, T>());

		if (result.first != result.second)
			resource = result.first->lock();

		if (!resource) {
			T* (*newAddr)(Args&&...) = &vl::new_f;
			resource.reset(vl::forward_from_tuple(newAddr, std::move(argsPack)), [this](T*& ptr) {
				std::unique_lock<std::mutex> lock_guard(cache_m);
				auto result = std::equal_range(cache.begin(), cache.end(), *ptr, ChachedComparator<Comparator, T>());
						assert(result.first != result.second);
						cache.erase(result.first);
						delete ptr;
			});
			cache.emplace(resource);
		}
		return resource;
	}
	inline typename Container::size_type size() {
		return cache.size();
	}
public:
	Cache() = default;
	Cache(const Cache&) = delete;
	Cache& operator=(const Cache&) = delete;
	virtual ~Cache() = default;
};

//------------------------------------------------------------------------------------------

} //namespace vl