// Project: libv.serial, File: src/libv/serial/types/std_variant.hpp

#pragma once

// std
#include <variant>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <std::size_t N, typename Archive, typename... Types>
inline void libv_load_std_variant(std::size_t index, Archive& ar, std::variant<Types...>& variant) {
	if (N == index) {
		variant.template emplace<N>();
		ar & LIBV_NVP_NAMED("value", std::get<N>(variant));
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

/// Serializing a std::monostate
template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(Archive&, const std::monostate&) {}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE
