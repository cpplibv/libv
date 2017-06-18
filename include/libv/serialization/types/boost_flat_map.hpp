// File: std_variant.hpp Author: Vader Created on 2017. június 11., 5:15

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// pro
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, typename... Args>
inline void save(Archive& ar, const boost::container::flat_map<Args...>& map) {
	const auto size = map.size();
	ar & LIBV_NVP(size);

	for (const auto& [key, value] : map) {
		ar & LIBV_NVP(key);
		ar & LIBV_NVP(value);
	}
}

template <typename Archive, typename... Args>
inline void load(Archive& ar, boost::container::flat_map<Args...>& map) {
	size_t size;
	ar & LIBV_NVP(size);
	map.reserve(size);

	typename boost::container::flat_map<Args...>::key_type key;
	typename boost::container::flat_map<Args...>::mapped_type value;
	for (size_t i = 0; i < size; ++i) {
		ar & LIBV_NVP(key);
		ar & LIBV_NVP(value);
		map.emplace_hint(map.end(), std::move(key), std::move(value));
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
