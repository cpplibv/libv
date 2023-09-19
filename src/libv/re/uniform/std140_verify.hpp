// Project: libv.re, File: src/libv/re/uniform/std140_verify.hpp

#pragma once

#include <libv/re/uniform/std140_rules.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

namespace detail {

// Verify std140 works differently from layout. It works on native C++ types and not rev::Uniform<T> types
template <typename T>
constexpr inline bool verify_layout_std140(T& object, uint32_t offset, uint32_t preceding_padding) {
	static_assert(!is_uniform_wrapper<T>, "Packed std140 data cannot contain Uniform wrappers");

	const auto align = layout_std140_align<T>();
	const auto size = layout_std140_size<T>();
	bool result = true;
	result &= preceding_padding < align;
	result &= size == sizeof(T);
	result &= offset % align == 0;

	if constexpr (is_uniform_structure<T>) {
		uint32_t last_padding = 0;
		const auto visitor = [&]<typename Member>(Member& member, auto... /*name*/) {
			result &= verify_layout_std140(member, offset, last_padding);
			offset += layout_std140_size<Member>();
			last_padding = is_uniform_padding<Member> ? sizeof(member) : 0;
		};
		object.access_uniforms(visitor);

	} else if constexpr (libv::meta::is_array_v<T>) {
		for (auto& item : object) {
			result &= verify_layout_std140(item, offset, 0);
			offset += layout_std140_size<typename T::value_type>();
		}
	}

	return result;
}

} // namespace detail ------------------------------------------------------------------------------

// Verify std140 works differently from layout. It works on native C++ types and not rev::Uniform<T> types
template <typename T>
constexpr inline bool verify_std140() {
	static_assert(is_uniform_structure<T>);
	T block{};
	return detail::verify_layout_std140(block, 0, 0);
}

template <typename T>
concept is_std140_packed = verify_std140<T>();

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv

