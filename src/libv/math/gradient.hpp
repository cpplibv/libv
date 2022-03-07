// Project: libv.color, File: src/libv/color/spread.hpp

#pragma once

// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/math/remap.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename K, typename V = K>
class gradient;

template <typename V = float>
using gradientf = gradient<float, V>;

template <typename V = double>
using gradientd = gradient<double, V>;

// -------------------------------------------------------------------------------------------------

template <typename K, typename V>
class gradient {
private:
	boost::container::small_vector<K, 8> keys;
	boost::container::small_vector<V, 8> values;

public:
	inline void add(K key, V value) {
		assert(keys.empty() || keys.back() <= key);
		keys.emplace_back(key);
		values.emplace_back(value);
	}

public:
	[[nodiscard]] constexpr inline K min_key() const noexcept {
		assert(!keys.empty());
		return keys.front();
	}
	[[nodiscard]] constexpr inline K max_key() const noexcept {
		assert(!keys.empty());
		return keys.back();
	}
//	[[nodiscard]] constexpr inline std::size_t upper_bound_key_index(K key) const noexcept {
//		assert(!keys.empty());
//
//		if (key < keys[0])
//			return 0;
//
//		for (std::size_t i = 1; i < keys.size(); ++i)
//			if (key < keys[i])
//				return i;
//
//		return values.size();
//	}
	[[nodiscard]] constexpr inline K closest_key(K key) const noexcept;
	[[nodiscard]] constexpr inline std::size_t closest_key_index(K key) const noexcept;

	[[nodiscard]] constexpr inline K key(std::size_t index) const noexcept {
		assert(index < keys.size());
		return keys[index];
	}
	[[nodiscard]] constexpr inline V value(std::size_t index) const noexcept {
		assert(index < values.size());
		return value[index];
	}

public:
	[[nodiscard]] constexpr inline V sample(K key) const noexcept {
		assert(!keys.empty());

		if (key < keys.front())
			return values.front();

		for (std::size_t i = 1; i < keys.size(); ++i)
			if (key < keys[i])
				return libv::remap_key(key, keys[i - 1], keys[i], values[i - 1], values[i]);

		return values.back();
	}
};

// -------------------------------------------------------------------------------------------------

//template <typename K, typename V, std::size_t Extent = std::dynamic_extent>
//struct _gradient_lut_storage {
//	std::array<K, Extent> keys;
//	std::array<V, Extent> values;
//
////	base_gradient() {
////
////	}
//};
//
//template <typename K, typename V>
//struct _gradient_lut_storage<K, V, std::dynamic_extent> {
//	std::vector<K> keys;
//	std::vector<V> values;
//
////	base_gradient() {
////
////	}
//};

//template <typename K, typename V, std::size_t Extent = std::dynamic_extent>
//class gradient_lut : public base_gradient<K, V, Extent> {
//public:
//	using base_gradient<K, V, Extent>::base_gradient;
//
//public:
//	[[nodiscard]] constexpr inline K min_key() const noexcept {
//		return this->keys[0];
//	}
//	[[nodiscard]] constexpr inline K max_key() const noexcept {
//		return this->keys[this->keys.size() - 1];
//	}
//
//public:
//	[[nodiscard]] constexpr inline V sample(K key) const noexcept {
//		if (key < this->keys[0])
//			return this->values[0];
//
//		for (std::size_t i = 1; i < this->keys.size(); ++i)
//			if (key < this->keys[i])
//				return libv::remap_key(key, this->keys[i - 1], this->keys[i], this->values[i - 1], this->values[i]);
//
//		return this->values[this->values.size() - 1];
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace libv
