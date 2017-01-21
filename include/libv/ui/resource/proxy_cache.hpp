// File:   intrusive_cache.hpp Author: Vader Created on 2016. szeptember 19., 5:32

#pragma once

// ext
#include <boost/intrusive_ptr.hpp>
// libv
#include <libv/tuple.hpp>
#include <libv/type_traits.hpp>
// std
#include <cassert>
#include <memory>
#include <mutex>
#include <set>

namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename... Args>
boost::intrusive_ptr<T> make_intrusive(Args&&... args) {
	return boost::intrusive_ptr<T>(new T(std::forward<Args>(args)...));
}

// -------------------------------------------------------------------------------------------------

template <size_t... Is>
struct use {
};

template <size_t... Is>
struct ignore {
};

// -------------------------------------------------------------------------------------------------

template<typename BaseComparator, typename T>
struct IntrusiveCacheComparator : BaseComparator {
	using is_transparent = void;
	inline bool operator()(const std::unique_ptr<T>& lhs, const T& rhs) const {
		return BaseComparator::operator()(*lhs, rhs);
	}
	inline bool operator()(const T& lhs, const std::unique_ptr<T>& rhs) const {
		return BaseComparator::operator()(lhs, *rhs);
	}
	inline bool operator()(const std::unique_ptr<T>& lhs, const std::unique_ptr<T>& rhs) const {
		return BaseComparator::operator()(*lhs, *rhs);
	}
	//
	template<typename... Args>
	inline bool operator()(const std::tuple<Args...>& args, const std::unique_ptr<T>& cr) const {
		return BaseComparator::operator()(args, *cr);
	}
	template<typename... Args>
	inline bool operator()(const std::unique_ptr<T>& cr, const std::tuple<Args...>& args) const {
		return BaseComparator::operator()(*cr, args);
	}
	template<typename L,
	typename = libv::disable_if<libv::is_less_comparable<std::tuple<L>, T>>,
	typename = libv::enable_if<libv::is_less_comparable<L, T>>>
	inline bool operator()(const std::tuple<L>& args, const std::unique_ptr<T>& cr) const {
		return BaseComparator::operator()(std::get<0>(args), *cr);
	}
	template<typename L,
	typename = libv::disable_if<libv::is_less_comparable<std::tuple<L>, T>>,
	typename = libv::enable_if<libv::is_less_comparable<L, T>>>
	inline bool operator()(const std::unique_ptr<T>& cr, const std::tuple<L>& args) const {
		return BaseComparator::operator()(*cr, std::get<0>(args));
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename ComparatorBase = std::less<void>>
class IntrusiveCache {
private:
	using Comparator = IntrusiveCacheComparator<ComparatorBase, T>;
	using Container = std::set<std::unique_ptr<T>, Comparator>;

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
	std::pair<boost::intrusive_ptr<T>, bool> getImpl(Args&&... args) {
		Arguments<CompareOptions, Args...> arguments(std::forward<Args>(args)...);

		auto it = storage.find(arguments.comp);
		if (it != storage.end())
			return std::make_pair(make_intrusive<T>(it->get()), false);

		auto resource = libv::forward_from_tuple([this](auto&&... args) {
					return std::make_unique<T>(std::forward<decltype(args)>(args)...);
				}, std::move(arguments.all));

		auto raw_ptr = resource.get();
		storage.emplace(resource);
		return std::make_pair(make_intrusive<T>(raw_ptr), true);
	}

public:
	template <typename CompareOptions = void, typename... Args>
	inline std::pair<boost::intrusive_ptr<T>, bool> get(Args&&... args) {
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
	IntrusiveCache() { }
	IntrusiveCache(const IntrusiveCache&) = delete;
	IntrusiveCache(IntrusiveCache&&) = delete;
	IntrusiveCache& operator=(const IntrusiveCache&) = delete;
	IntrusiveCache& operator=(IntrusiveCache&&) = delete;
	~IntrusiveCache() {
		for (auto it = storage.begin(); it != storage.end(); ++it) {
			assert((*it)->counter.load() == 0);
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv