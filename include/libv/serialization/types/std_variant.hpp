// File: std_variant.hpp Author: Vader Created on 2017. június 11., 5:15

#pragma once

// std
#include <variant>
// pro
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <size_t N, typename Archive, typename... Types>
inline void libv_load_std_variant(size_t index, Archive& ar, std::variant<Types...>& variant) {
	if (N == index) {
		std::decay_t<decltype(std::get<N>(variant))> value;
		ar & LIBV_NVP(value);
		variant = std::move(value);
	} else if constexpr (N + 1 < sizeof...(Types))
		libv_load_std_variant<N + 1>(index, ar, variant);
}

} // namespace detail ------------------------------------------------------------------------------

template <typename Archive, typename... Types>
inline void LIBV_SERIALIZATION_SAVE_FUNCTION_NAME(Archive& ar, const std::variant<Types...>& variant) {
	static_assert(sizeof...(Types) < 256, "Variant serialization only supported up to 255 type.");

	const auto index = static_cast<uint8_t>(variant.index());
	ar & LIBV_NVP(index);
	std::visit([&](const auto& value) {
		ar & LIBV_NVP(value);
	}, variant);
}

template <typename Archive, typename... Types>
inline void LIBV_SERIALIZATION_LOAD_FUNCTION_NAME(Archive& ar, std::variant<Types...>& variant) {
	static_assert(sizeof...(Types) < 256, "Variant serialization only supported up to 255 type.");

	uint8_t index;
	ar & LIBV_NVP(index);
	if (index >= sizeof...(Types))
		throw cereal::Exception("Invalid 'index' when deserializing std::variant");

	detail::libv_load_std_variant<0>(index, ar, variant);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
