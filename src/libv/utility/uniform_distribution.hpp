// Project: libv.utility, File: src/libv/utility/uniform_distribution.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/meta/always.hpp>
// std
#include <chrono>
#include <random>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template<typename>
struct aux_is_std_chrono_duration : public std::false_type { };

template <typename Rep, typename Period>
struct aux_is_std_chrono_duration<std::chrono::duration<Rep, Period>> : public std::true_type { };

template <typename T>
struct is_std_chrono_duration : public aux_is_std_chrono_duration<std::remove_cv_t<T>>::type { };

template <typename T>
constexpr inline bool is_std_chrono_duration_v = is_std_chrono_duration<T>::value;

// -------------------------------------------------------------------------------------------------

template <typename Duration, typename Base =
		std::conditional_t<std::is_integral_v<typename Duration::rep>,
			std::uniform_int_distribution<typename Duration::rep>,
			std::uniform_real_distribution<typename Duration::rep>
		>>
class uniform_duration_distribution : public Base {
public:
	constexpr inline uniform_duration_distribution(Duration min, Duration max) noexcept :
		Base(min.count(), max.count()) {}

	template <class Generator>
	inline Duration operator()(Generator&& g) {
		return Duration(Base::operator()(g));
	}

	template <class Generator>
	inline Duration operator()(Generator&& g, const typename Base::param_type& params) {
		return Duration(Base::operator()(g, params));
	}
};

// =================================================================================================

namespace detail {

template <typename T>
T to_inclusive_max(const T value) {
	if constexpr (std::is_floating_point_v<T>)
		return std::nextafter(value, std::numeric_limits<T>::max());
	else if constexpr (std::is_integral_v<T>)
		return value;
	else if constexpr (is_std_chrono_duration_v<T>)
		return T{to_inclusive_max(value.count())};
	else
		static_assert(libv::meta::always_false_v<T>, "Uniform distribution for type T is not supported");
}

template <typename T>
T to_exclusive_max(const T value) {
	if constexpr (std::is_floating_point_v<T>)
		return value;
	else if constexpr (std::is_integral_v<T>)
		return value - T{1};
	else if constexpr (is_std_chrono_duration_v<T>)
		return T{to_exclusive_max(value.count())};
	else
		static_assert(libv::meta::always_false_v<T>, "Uniform distribution for type T is not supported");
}

template <typename C>
[[nodiscard]] constexpr inline auto aux_make_uniform_distribution(const C min, const C max) noexcept {
	if constexpr (std::is_integral_v<C>)
		return std::uniform_int_distribution<C>{min, max};
	else if constexpr (std::is_floating_point_v<C>)
		return std::uniform_real_distribution<C>{min, max};
	else if constexpr (is_std_chrono_duration_v<C>)
		return uniform_duration_distribution<C>{min, max};
	else
		static_assert(libv::meta::always_false_v<C>, "Uniform distribution for type C is not supported");
}

} // namespace detail ------------------------------------------------------------------------------

/// Inclusive 2 param: [min .. max]
template <typename A, typename B, typename C = std::common_type_t<A, B>>
[[nodiscard]] constexpr inline auto make_uniform_distribution_inclusive(const A min, const B max) noexcept {
	return detail::aux_make_uniform_distribution(static_cast<C>(min), detail::to_inclusive_max(static_cast<C>(max)));
}

/// Inclusive 1 param: [0 .. max]
template <typename C>
[[nodiscard]] constexpr inline auto make_uniform_distribution_inclusive(const C max) noexcept {
	return detail::aux_make_uniform_distribution(static_cast<C>(0), detail::to_inclusive_max(static_cast<C>(max)));
}

/// Exclusive 2 param: [min .. max)
template <typename A, typename B, typename C = std::common_type_t<A, B>>
[[nodiscard]] constexpr inline auto make_uniform_distribution_exclusive(const A min, const B max) noexcept {
	return detail::aux_make_uniform_distribution(static_cast<C>(min), detail::to_exclusive_max(static_cast<C>(max)));
}

/// Exclusive 1 param: [0 .. max)
template <typename C>
[[nodiscard]] constexpr inline auto make_uniform_distribution_exclusive(const C max) noexcept {
	return detail::aux_make_uniform_distribution(static_cast<C>(0), detail::to_exclusive_max(static_cast<C>(max)));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

