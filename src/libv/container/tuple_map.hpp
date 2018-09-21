// File:   cs_map.hpp Author: Vader Created on 2018. szeptember 21., 14:56

#pragma once

// libv
#include <libv/meta/n_times.hpp>
// std
#include <algorithm>
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: move identity_with_pack its own header
template <typename..., typename T>
constexpr inline decltype(auto) identity_with_pack(T&& value) noexcept {
	return std::forward<T>(value);
}

// -------------------------------------------------------------------------------------------------

/// Column sequential flat ordered associative container
template <typename Key, typename... Values>
struct tuple_map {
	using Comparator = std::less<>; // TODO P5: make it template argument

	template <typename LKey>
	using requires_lookupable = decltype(
			Comparator{}(std::declval<const Key&>(), std::declval<const LKey&>()) &&
			Comparator{}(std::declval<const LKey&>(), std::declval<const Key&>())
	);

private:
	std::vector<Key> keys;
	std::tuple<std::vector<Values>...> values;
	// NOTE: There is a slight overhead of maintaining multiple vectors
	//			capacity and size stored and calculated multiple times

private:
	template <typename LKey, typename = requires_lookupable<LKey>>
	inline size_t _index(const LKey& key) const {
		const auto it = std::lower_bound(keys.begin(), keys.end(), key, Comparator{});
		return std::distance(keys.begin(), it);
	}

public:
	template <typename FKey, typename... FValues,
			typename = std::enable_if_t<sizeof...(Values) == sizeof...(FValues)>,
			typename = requires_lookupable<FKey>>
	void emplace(FKey&& newkey, FValues&&... newvalues) {
		const auto index = _index(newkey);

		keys.emplace(keys.begin() + index, std::forward<FKey>(newkey));
		libv::meta::call_with_n_index<sizeof...(Values)>([&](auto... i) {
			(std::get<i>(values).emplace(std::get<i>(values).begin() + index, std::forward<FValues>(newvalues)), ...);
		});
	}

public:
	inline void reserve(size_t n) {
		keys.reserve(n);
		libv::meta::n_times_index<sizeof...(Values)>([&](auto i) {
			std::get<i>(values).reserve(n);
		});
	}

	inline void resize(size_t n) {
		keys.resize(n);
		libv::meta::n_times_index<sizeof...(Values)>([&](auto i) {
			std::get<i>(values).resize(n);
		});
	}

	inline void clear() {
		return keys.clear();
		libv::meta::n_times_index<sizeof...(Values)>([&](auto i) {
			std::get<i>(values).clear();
		});
	}

public:
	template <size_t Column, typename LKey,
			typename = requires_lookupable<LKey>>
	auto& at(const LKey& key) {
		const auto index = _index(key);
		
		if (index >= keys.size())
			throw std::out_of_range("libv::cs_map::at");
		// TODO P5: include the key in the exception message if it is ostreamable

		return *(std::get<Column>(values).begin() + index);
	}

	template <typename Column, typename LKey,
			typename = requires_lookupable<LKey>>
	auto& at(const LKey& key) {
		const auto index = _index(key);

		if (index >= keys.size())
			throw std::out_of_range("libv::cs_map::at");

		return *(std::get<Column>(values).begin() + index);
	}

	template <size_t Column, typename LKey,
			typename = requires_lookupable<LKey>>
	const auto& at(const LKey& key) const {
		const auto index = _index(key);

		if (index >= keys.size())
			throw std::out_of_range("libv::cs_map::at");
		// TODO P5: include the key in the exception message if it is ostreamable

		return *(std::get<Column>(values).begin() + index);
	}

	template <typename Column, typename LKey,
			typename = requires_lookupable<LKey>>
	const auto& at(const LKey& key) const {
		const auto index = _index(key);

		if (index >= keys.size())
			throw std::out_of_range("libv::cs_map::at");

		return *(std::get<Column>(values).begin() + index);
	}

//	template <size_t Column>
//	auto& column() {}
//
//	template <typename Column>
//	auto& column() {}
//
//	template <size_t Column>
//	const auto& column() const {}
//
//	template <typename Column>
//	const auto& column() const {}

public:
	inline size_t size() const {
		return keys.size();
	}

	inline size_t capacity() const {
		return keys.capacity();
	}

public:
	inline tuple_map() = default;
	inline explicit tuple_map(const size_t initial_size) :
		keys(initial_size),
		values{identity_with_pack<Values>(initial_size)...} {}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
