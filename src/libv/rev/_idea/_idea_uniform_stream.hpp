// Project: libv.rev, File: src/libv/rev/_idea/_idea_uniform_stream.hpp

#pragma once

// libv
#include <libv/gl/uniform.hpp>
#include <libv/utility/align.hpp>
// std
#include <cstdint>
#include <vector>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class UniformStream {
private:
	struct Header {
		uint32_t location_index;
		uint8_t size;
		bool end;
		void (*upload_function)(const GLint location, const std::byte* ptr);
	};
	static_assert(sizeof(Header) == 16);

private:
	std::vector<Header> header;
	std::vector<std::byte> data;
	std::size_t current_header = 0;
	std::size_t current_data = 0;

private:
	inline void clear() {
		header.clear();
		data.clear();
		current_data = 0;
		current_header = 0;
	}

public:
	//	libv::gl::TextureUnit,
	//	bool, int32_t, uint32_t, int64_t, uint64_t, float, double,
	//	libv::vec2b, libv::vec3b, libv::vec4b,
	//	libv::vec2i, libv::vec3i, libv::vec4i, libv::vec2ui, libv::vec3ui, libv::vec4ui,
	//	libv::vec2l, libv::vec3l, libv::vec4l, libv::vec2ul, libv::vec3ul, libv::vec4ul,
	//	libv::vec2f, libv::vec3f, libv::vec4f, libv::vec2d, libv::vec3d, libv::vec4d,
	//	libv::mat2x2f, libv::mat3x2f, libv::mat4x2f, libv::mat2x2d, libv::mat3x2d, libv::mat4x2d,
	//	libv::mat2x3f, libv::mat3x3f, libv::mat4x3f, libv::mat2x3d, libv::mat3x3d, libv::mat4x3d,
	//	libv::mat2x4f, libv::mat3x4f, libv::mat4x4f, libv::mat2x4d, libv::mat3x4d, libv::mat4x4d
	template <typename T>
	void set(const uint32_t location_index, const T& value) {
		const auto orig_last = data.size();
		const auto aligned_last = libv::align(orig_last, alignof(T));
		const auto alignment_loss = aligned_last - orig_last;
		const auto upload_function = +[](const GLint location, const std::byte* ptr) {
			libv::gl::uniform(location, *reinterpret_cast<const T*>(ptr));
		};

		header.emplace_back(
				location_index,
				static_cast<uint8_t>(sizeof(T) + alignment_loss),
				false,
				upload_function);

		data.resize(aligned_last + sizeof(T));
		new (data.data() + aligned_last) T(value);
	}

	void endBatch() {
		header.emplace_back(0, 0, true, nullptr);
	}

public:
	void stream(const std::vector<int32_t>& locations) {
		while (true) {
			const auto& head = header[current_header++];

			if (head.end)
				break;

			const auto location = locations[head.location_index];
			const auto ptr = data.data() + current_data;

			head.upload_function(location, ptr);
			current_data += head.size;
		}

		if (current_header == header.size())
			clear();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
