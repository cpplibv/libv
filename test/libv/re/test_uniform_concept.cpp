// Project: libv.re, File: test/libv/re/test_uniform_concept.cpp

#include <libv/re/uniform/concept.hpp>


// -------------------------------------------------------------------------------------------------

static_assert(libv::re::is_uniform_primitive<bool>);
static_assert(libv::re::is_uniform_primitive<int32_t>);
static_assert(libv::re::is_uniform_primitive<uint32_t>);
static_assert(libv::re::is_uniform_primitive<int64_t>);
static_assert(libv::re::is_uniform_primitive<uint64_t>);
static_assert(libv::re::is_uniform_primitive<float>);
static_assert(libv::re::is_uniform_primitive<double>);

static_assert(libv::re::is_uniform_primitive<libv::vec2b>);
static_assert(libv::re::is_uniform_primitive<libv::vec3b>);
static_assert(libv::re::is_uniform_primitive<libv::vec4b>);

static_assert(libv::re::is_uniform_primitive<libv::vec2i>);
static_assert(libv::re::is_uniform_primitive<libv::vec3i>);
static_assert(libv::re::is_uniform_primitive<libv::vec4i>);
static_assert(libv::re::is_uniform_primitive<libv::vec2ui>);
static_assert(libv::re::is_uniform_primitive<libv::vec3ui>);
static_assert(libv::re::is_uniform_primitive<libv::vec4ui>);

static_assert(libv::re::is_uniform_primitive<libv::vec2l>);
static_assert(libv::re::is_uniform_primitive<libv::vec3l>);
static_assert(libv::re::is_uniform_primitive<libv::vec4l>);
static_assert(libv::re::is_uniform_primitive<libv::vec2ul>);
static_assert(libv::re::is_uniform_primitive<libv::vec3ul>);
static_assert(libv::re::is_uniform_primitive<libv::vec4ul>);

static_assert(libv::re::is_uniform_primitive<libv::vec2f>);
static_assert(libv::re::is_uniform_primitive<libv::vec3f>);
static_assert(libv::re::is_uniform_primitive<libv::vec4f>);
static_assert(libv::re::is_uniform_primitive<libv::vec2d>);
static_assert(libv::re::is_uniform_primitive<libv::vec3d>);
static_assert(libv::re::is_uniform_primitive<libv::vec4d>);

static_assert(libv::re::is_uniform_primitive<libv::mat2x2f>);
static_assert(libv::re::is_uniform_primitive<libv::mat3x2f>);
static_assert(libv::re::is_uniform_primitive<libv::mat4x2f>);
static_assert(libv::re::is_uniform_primitive<libv::mat2x2d>);
static_assert(libv::re::is_uniform_primitive<libv::mat3x2d>);
static_assert(libv::re::is_uniform_primitive<libv::mat4x2d>);

static_assert(libv::re::is_uniform_primitive<libv::mat2x3f>);
static_assert(libv::re::is_uniform_primitive<libv::mat3x3f>);
static_assert(libv::re::is_uniform_primitive<libv::mat4x3f>);
static_assert(libv::re::is_uniform_primitive<libv::mat2x3d>);
static_assert(libv::re::is_uniform_primitive<libv::mat3x3d>);
static_assert(libv::re::is_uniform_primitive<libv::mat4x3d>);

static_assert(libv::re::is_uniform_primitive<libv::mat2x4f>);
static_assert(libv::re::is_uniform_primitive<libv::mat3x4f>);
static_assert(libv::re::is_uniform_primitive<libv::mat4x4f>);
static_assert(libv::re::is_uniform_primitive<libv::mat2x4d>);
static_assert(libv::re::is_uniform_primitive<libv::mat3x4d>);
static_assert(libv::re::is_uniform_primitive<libv::mat4x4d>);

static_assert(!libv::re::is_uniform_primitive<libv::vec_t<2, uint16_t>>);
static_assert(!libv::re::is_uniform_primitive<libv::vec_t<5, float>>);
static_assert(!libv::re::is_uniform_primitive<libv::mat_t<4, 4, uint16_t>>);
static_assert(!libv::re::is_uniform_primitive<libv::mat_t<4, 4, uint32_t>>);
static_assert(!libv::re::is_uniform_primitive<libv::mat_t<4, 5, float>>);
