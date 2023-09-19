// Project: libv.re, File: src/libv/re/uniform/std140_layout.hpp

#pragma once

#include <libv/re/uniform/std140_rules.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <typename T>
constexpr inline void assign_layout_std140(T& object, uint32_t offset) {
	if constexpr (is_uniform_wrapper<T>)
		object.location.block.offset = offset;

	if constexpr (is_uniform_structure<T>) {
		const auto visitor = [&]<typename Member>(Member& member, auto... /*name*/) {
			if constexpr (is_uniform_wrapper<T>) {
				const auto align = layout_std140_align<typename Member::value_type>();
				const auto size = layout_std140_size<typename Member::value_type>();
				const auto member_offset = libv::align(offset, align);
				assign_layout_std140(member, member_offset);
				offset = member_offset + size;
			} else {
				const auto align = layout_std140_align<Member>();
				const auto size = layout_std140_size<Member>();
				const auto member_offset = libv::align(offset, align);
				assign_layout_std140(member, member_offset);
				offset = member_offset + size;
			}
		};
		object.access_uniforms(visitor);
	} else if constexpr (libv::meta::is_array_v<T>) {
		for (auto& item : object) {
			const auto align = layout_std140_align<typename T::value_type>();
			const auto size = layout_std140_size<typename T::value_type>();

			offset = libv::align(offset, align);
			assign_layout_std140(item, offset);
			offset += size;
		}
	}
}

} // namespace detail

template <typename T>
constexpr inline void layout_std140(T& block) {
	detail::assign_layout_std140(block, 0);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
