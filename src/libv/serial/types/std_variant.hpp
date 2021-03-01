// Project: libv.serialization, File: src/libv/serialization/types/std_variant.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <variant>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <size_t N, typename Archive, typename... Types>
inline void libv_load_std_variant(size_t index, Archive& ar, std::variant<Types...>& variant) {
	if (N == index) {
		std::variant_alternative_t<N, std::variant<Types...>> value;
		ar & LIBV_NVP(value);
		variant = std::move(value);
	} else if constexpr (N + 1 < sizeof...(Types))
		libv_load_std_variant<N + 1>(index, ar, variant);
}

} // namespace detail ------------------------------------------------------------------------------

template <typename Archive, typename... Types>
inline void save(Archive& ar, const std::variant<Types...>& variant) {
	static_assert(sizeof...(Types) < 256, "Variant serialization only supported up to 255 type.");

	const auto index = static_cast<uint8_t>(variant.index());
	ar & LIBV_NVP_NAMED("type", index);
	std::visit([&](const auto& value) {
		ar & LIBV_NVP(value);
	}, variant);
}

template <typename Archive, typename... Types>
inline void load(Archive& ar, std::variant<Types...>& variant) {
	static_assert(sizeof...(Types) < 256, "Variant serialization only supported up to 255 type.");

	uint8_t index;
	ar & LIBV_NVP_NAMED("type", index);
	if (index >= sizeof...(Types))
		throw cereal::Exception("Invalid 'index' when deserializing std::variant");

	detail::libv_load_std_variant<0>(index, ar, variant);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE
