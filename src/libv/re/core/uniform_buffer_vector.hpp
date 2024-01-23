// Project: libv.re, File: src/libv/re/core/uniform_buffer_vector.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/meta/similar.hpp>
#include <libv/re/common/block_binding.hpp>
#include <libv/re/core/safe_gl.hpp>
#include <libv/re/uniform/concept.hpp>

#include <limits>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct BaseUniformBufferVector {
	void gl_create(libv::GL& gl, libv::gl::UniformBuffer& ubo);
	void gl_upload(libv::GL& gl, libv::gl::UniformBuffer& ubo, const void* data, std::size_t size, libv::gl::BufferUsage usage);
	void gl_bind(libv::GL& gl, libv::gl::UniformBuffer& ubo, BlockBinding binding, uint32_t offset, uint32_t size);
};

// Type T MUST BE in std140 layout, use
// #include <libv/re/uniform/std140_verify.hpp>
// static_assert(libv::re::is_std140_packed<T>);
// to verify
template <typename T>
class UniformBufferVector : BaseUniformBufferVector {
	static_assert(is_uniform_block<T>);

public:
	struct alignas(256) T_256aligned {
		T value;
	};
	static_assert(sizeof(T_256aligned) % 256 == 0);

	std::vector<T_256aligned> data;
	libv::gl::BufferUsage usage = libv::gl::BufferUsage::StreamDraw;
	bool created = false;

private:
	GLSafeUniformBuffer ubo;

public:
	explicit UniformBufferVector(libv::gl::BufferUsage usage) : usage(usage) {}

public:
	[[nodiscard]] uint32_t push_back(const T& var) {
		const auto index = data.size();
		data.emplace_back(var);
		return static_cast<uint32_t>(index);
	}

	[[nodiscard]] uint32_t push_back(T&& var) {
		const auto index = data.size();
		data.emplace_back(std::move(var));
		return static_cast<uint32_t>(index);
	}

public:
	void gl_swap(libv::GL& gl) {
		if (!created) {
			gl_create(gl);
			created = true;
		}

		data.clear();
	}
	void gl_create(libv::GL& gl) {
		BaseUniformBufferVector::gl_create(gl, ubo);
	}
	void gl_upload(libv::GL& gl) {
		BaseUniformBufferVector::gl_upload(gl, ubo, data.data(), data.size() * sizeof(T_256aligned), usage);
	}
	void gl_bind(libv::GL& gl, uint32_t index) {
		BaseUniformBufferVector::gl_bind(gl, ubo, T::binding, sizeof(T_256aligned) * index, sizeof(T));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
