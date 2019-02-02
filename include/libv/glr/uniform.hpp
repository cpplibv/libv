// File: mesh.hpp - Created on 2017.11.25. 05:06 - Author: Vader

#pragma once

// std
#include <cstdint>
#include <type_traits>
// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
// pro
#include <libv/gl/enum.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct Uniform {
	/// - For freestanding uniforms stores the indirect location index
	/// - For uniform blocks stores the byte offset from the beginning of the block
	uint32_t location = 0;
};

template <typename T>
struct Uniform_t : Uniform {
	using value_type = T;
};

template <typename T> struct isUniform : std::false_type {};
template <typename T> struct isUniform<Uniform_t<T>> : std::true_type {};
template <typename T> constexpr bool isUniform_v = isUniform<T>::value;

// -------------------------------------------------------------------------------------------------

using Uniform_texture = Uniform_t<libv::gl::TextureChannel>;

using Uniform_bool = Uniform_t<bool>;
using Uniform_int32 = Uniform_t<int32_t>;
using Uniform_uint32 = Uniform_t<uint32_t>;
using Uniform_int64 = Uniform_t<int64_t>;
using Uniform_uint64 = Uniform_t<uint64_t>;
using Uniform_float = Uniform_t<float>;
using Uniform_double = Uniform_t<double>;

using Uniform_vec2b = Uniform_t<libv::vec2b>;
using Uniform_vec3b = Uniform_t<libv::vec3b>;
using Uniform_vec4b = Uniform_t<libv::vec4b>;
using Uniform_vec2i = Uniform_t<libv::vec2i>;
using Uniform_vec3i = Uniform_t<libv::vec3i>;
using Uniform_vec4i = Uniform_t<libv::vec4i>;
using Uniform_vec2ui = Uniform_t<libv::vec2ui>;
using Uniform_vec3ui = Uniform_t<libv::vec3ui>;
using Uniform_vec4ui = Uniform_t<libv::vec4ui>;
using Uniform_vec2l = Uniform_t<libv::vec2l>;
using Uniform_vec3l = Uniform_t<libv::vec3l>;
using Uniform_vec4l = Uniform_t<libv::vec4l>;
using Uniform_vec2ul = Uniform_t<libv::vec2ul>;
using Uniform_vec3ul = Uniform_t<libv::vec3ul>;
using Uniform_vec4ul = Uniform_t<libv::vec4ul>;
using Uniform_vec2f = Uniform_t<libv::vec2f>;
using Uniform_vec3f = Uniform_t<libv::vec3f>;
using Uniform_vec4f = Uniform_t<libv::vec4f>;
using Uniform_vec2d = Uniform_t<libv::vec2d>;
using Uniform_vec3d = Uniform_t<libv::vec3d>;
using Uniform_vec4d = Uniform_t<libv::vec4d>;

using Uniform_mat2f = Uniform_t<libv::mat2f>;
using Uniform_mat3f = Uniform_t<libv::mat3f>;
using Uniform_mat4f = Uniform_t<libv::mat4f>;
using Uniform_mat2d = Uniform_t<libv::mat2d>;
using Uniform_mat3d = Uniform_t<libv::mat3d>;
using Uniform_mat4d = Uniform_t<libv::mat4d>;

using Uniform_mat2x2f = Uniform_t<libv::mat2x2f>;
using Uniform_mat2x3f = Uniform_t<libv::mat2x3f>;
using Uniform_mat2x4f = Uniform_t<libv::mat2x4f>;
using Uniform_mat3x2f = Uniform_t<libv::mat3x2f>;
using Uniform_mat3x3f = Uniform_t<libv::mat3x3f>;
using Uniform_mat3x4f = Uniform_t<libv::mat3x4f>;
using Uniform_mat4x2f = Uniform_t<libv::mat4x2f>;
using Uniform_mat4x3f = Uniform_t<libv::mat4x3f>;
using Uniform_mat4x4f = Uniform_t<libv::mat4x4f>;
using Uniform_mat2x2d = Uniform_t<libv::mat2x2d>;
using Uniform_mat2x3d = Uniform_t<libv::mat2x3d>;
using Uniform_mat2x4d = Uniform_t<libv::mat2x4d>;
using Uniform_mat3x2d = Uniform_t<libv::mat3x2d>;
using Uniform_mat3x3d = Uniform_t<libv::mat3x3d>;
using Uniform_mat3x4d = Uniform_t<libv::mat3x4d>;
using Uniform_mat4x2d = Uniform_t<libv::mat4x2d>;
using Uniform_mat4x3d = Uniform_t<libv::mat4x3d>;
using Uniform_mat4x4d = Uniform_t<libv::mat4x4d>;

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
