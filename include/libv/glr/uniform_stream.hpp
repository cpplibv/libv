// File:   unfirom_state.hpp Author: Vader Created on 2019. január 19., 21:33

#pragma once

// ext
#include <boost/align/aligned_allocator.hpp>
// libv
#include <libv/gl/uniform.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/index_to_type.hpp>
#include <libv/meta/type_traits.hpp>
#include <libv/utility/align.hpp>
// std
#include <cstdint>
#include <vector>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

namespace detail {

struct BatchEndMarker {};

template <typename... Types>
class ImplUniformStream {
	static constexpr uint8_t end_type = libv::meta::type_to_index_v<BatchEndMarker, Types...>;

	struct Header {
		uint32_t location_index;
		uint8_t type;
		uint8_t size;
	};

	std::vector<Header> header;
	std::vector<std::byte, boost::alignment::aligned_allocator<std::byte, 32>> data;
	size_t current_header = 0;
	size_t current_data = 0;

public:
	template <typename T>
	void set(const uint32_t location_index, const T& value) {
		static_assert(libv::meta::is_same_as_any_v<T, Types...>, "Invalid uniform type T");

		const auto orig_last = data.size();
		const auto aligned_last = libv::align(orig_last, alignof(T));

		header.emplace_back(Header{
				location_index,
				libv::meta::type_to_index_v<T, Types...>,
				static_cast<uint8_t>(sizeof(T) + aligned_last - orig_last)});
		data.resize(aligned_last + sizeof(T));
		std::memcpy(data.data() + aligned_last, &value, sizeof(T));
	}
	void set(const uint32_t location_index, bool value) {
		set(location_index, static_cast<int32_t>(value));
	}

	void endBatch() {
		header.emplace_back(Header{0, end_type, 0});
	}

	void stream(const std::vector<int32_t>& locations) {
		static constexpr std::array<void(*)(const GLint, const std::byte*), sizeof...(Types)> dispatch = {
			[](const GLint location, const std::byte* ptr) {
				if constexpr (!std::is_same_v<Types, BatchEndMarker>) {
					const auto aligned_ptr = static_cast<const void*>(ptr); // ptr is aligned, suppress warnings
					libv::gl::uniform(location, *reinterpret_cast<const Types*>(aligned_ptr));
				} else {
					// To suppress warnings in the BatchEndMarker case
					(void) location;
					(void) ptr;
				}
			}...
        };

		while (true) {
			const auto& head = header[current_header++];
			if (head.type == end_type)
				break;

			const auto location = locations[head.location_index];
			const auto ptr = data.data() + current_data;

			dispatch[head.type](location, ptr);
			current_data += head.size;
		}

		if (current_header == header.size()) {
			data.clear();
			header.clear();
			current_data = 0;
			current_header = 0;
		}
	}
};

} // namespace detail ------------------------------------------------------------------------------

using UniformStream = detail::ImplUniformStream<
		detail::BatchEndMarker,
		libv::gl::TextureChannel,
		bool, int32_t, uint32_t, int64_t, uint64_t, float, double,
		libv::vec2b, libv::vec3b, libv::vec4b,
		libv::vec2i, libv::vec3i, libv::vec4i, libv::vec2u, libv::vec3u, libv::vec4u,
		libv::vec2l, libv::vec3l, libv::vec4l, libv::vec2ul, libv::vec3ul, libv::vec4ul,
		libv::vec2f, libv::vec3f, libv::vec4f, libv::vec2d, libv::vec3d, libv::vec4d,
		libv::mat2x2f, libv::mat3x2f, libv::mat4x2f, libv::mat2x2f, libv::mat3x2f, libv::mat4x2f,
		libv::mat2x3f, libv::mat3x3f, libv::mat4x3f, libv::mat2x3f, libv::mat3x3f, libv::mat4x3f,
		libv::mat2x4f, libv::mat3x4f, libv::mat4x4f, libv::mat2x4f, libv::mat3x4f, libv::mat4x4f
>;

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
