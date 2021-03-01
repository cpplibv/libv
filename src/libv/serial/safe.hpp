// Project: libv.serial, File: src/libv/serial/safe.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <concepts>
#include <ranges>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

// TODO P3: This is only an initial draft, needs further research
//          Might need to explicitly mark every serialization function

/// The goal of is_bound_safe is to prevent memory allocation attack from serialized data read from the network

/// serial_safe member and is_bound_safe traits only determines if the type itself is safe to serialize,
/// but it does not account for any members

// Usage:
//
//struct Address {
//	libv::serial::limit<std::string, 256> address;
//	libv::serial::limit<std::string, 256> service;
//	LIBV_REFLECTION_ACCESS(address);
//	LIBV_REFLECTION_ACCESS(service);
//	LIBV_SERIALIZATION_ENABLE_REFLECTION();
//};
//
//struct Address {
//	libv::serial::limit<std::string, 256> address;
//	libv::serial::limit<std::string, 256> service;
//	LIBV_SERIALIZATION_ENABLE_MEMBERWISE();
//};
//
//struct Address {
//	libv::serial::limit<std::string, 256> address;
//	libv::serial::limit<std::string, 256> service;
//	template <typename Archive> auto serialize(Archive& ar) {
//		ar("address", address);
//		ar("service", service);
//		return Address::bound_safe;
//	}
//};
//
//struct Address {
//	libv::serial::limit<std::string, 256> address;
//	libv::serial::limit<std::string, 256> service;
//	template <typename Archive> auto serialize(Archive& ar) {
//		ar & LIBV_NVP(address);
//		ar & LIBV_NVP(service);
//		return Address::bound_safe;
//	}
//};
//
//struct Address {
//	libv::serial::limit<std::string, 256> address;
//	libv::serial::limit<std::string, 256> service;
//	template <typename Archive> auto save(Archive& ar) {
//		ar & LIBV_NVP(address);
//		ar & LIBV_NVP(service);
//		return Address::bound_safe;
//	}
//	template <typename Archive> auto load(Archive& ar) {
//		ar & LIBV_NVP(address);
//		ar & LIBV_NVP(service);
//		return Address::bound_safe;
//	}
//};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct safety_traits {
	static constexpr bool is_bound_safe = false;
};

template <typename T> requires requires{ T::serial_bound_safe; }
struct safety_traits<T> {
	static constexpr bool is_bound_safe = T::serial_bound_safe;
};

template <typename T> requires std::integral<T> || std::floating_point<T>
struct safety_traits<T> {
	static constexpr bool is_bound_safe = true;
};

//template <typename T>
//	requires reflection_serialized<T>
//struct safety_traits<T> {
//	static constexpr bool is_bound_safe = is_every_member_bound_safe<T>;
//};

//template <typename T>
//	requires memberwise_serialized<T>
//struct safety_traits<T> {
//	static constexpr bool is_bound_safe = is_every_member_bound_safe<T>;
//};

//template <typename T>
//	requires std::ranges::range<Range>
//struct safety_traits<T> {
//	static constexpr bool is_bound_safe = false;
//};

//template <typename T>
//	requires fixed_size_range<Range>
//struct safety_traits<T> {
//	static constexpr bool is_bound_safe = true;
//};

// -------------------------------------------------------------------------------------------------

template <typename Range, int ElementLimit> requires std::ranges::range<Range>
struct actually_implemented_limit : Range {
	static constexpr int serial_element_limit = ElementLimit;
	static constexpr bool serial_bound_safe = safety_traits<typename Range::element_type>::is_bound_safe;
//	static constexpr bool serial_safe = true;

	template <typename... Args>
	constexpr inline limit(Args&& ... args) : Range(std::forward<Args>(args)...) {}
};

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
