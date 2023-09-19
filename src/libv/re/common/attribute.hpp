// Project: libv.re, File: src/libv/re/common/attribute.hpp

#pragma once

#include <libv/gl/attribute.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using AttributeIndex = int32_t;

// Default attribute locations:
//   0 |    position |             |
//   1 |          uv |             |
//   2 |      normal |   pointSize |
//   3 |     tangent |             |
//   4 |   bitangent |             |
//   5 | boneIndices |             |
//   6 |  boneWeight |             |
//   7 |     diffuse |     custom0 |
//   8 |    specular |     custom1 |
//   9 |             |  tessFactor |
//  10 |             |    instVec0 |
//  11 |             |    instVec1 |
//  12 |             | matInstance |
//  13 |             |    ^^^^^    |
//  14 |             |    ^^^^^    |
//  15 |             |    ^^^^^    |

// 0
static constexpr auto attrPosition         = libv::gl::Attribute<0, libv::vec3f>{};
// 1
static constexpr auto attrUV               = libv::gl::Attribute<1, libv::vec2f>{};
// 2
static constexpr auto attrNormal           = libv::gl::Attribute<2, libv::vec3f>{};
// 3
static constexpr auto attrTangent          = libv::gl::Attribute<3, libv::vec3f>{};
// 4
static constexpr auto attrBitangent        = libv::gl::Attribute<4, libv::vec3f>{};
// 5
static constexpr auto attrBoneIndices      = libv::gl::Attribute<5, libv::vec4i>{};
// 6
static constexpr auto attrBondWeight       = libv::gl::Attribute<6, libv::vec4f>{};
// 7
static constexpr auto attrDiffuse          = libv::gl::Attribute<7, libv::vec4f>{};
// 8
static constexpr auto attrSpecular         = libv::gl::Attribute<8, libv::vec4f>{};
// 9
static constexpr auto attrInstanceMatRow0  = libv::gl::Attribute<9, libv::vec4f>{};
// 10
static constexpr auto attrInstanceMatRow1  = libv::gl::Attribute<10, libv::vec4f>{};
// 11
static constexpr auto attrInstanceMatRow2  = libv::gl::Attribute<11, libv::vec4f>{};

// static constexpr auto attrInstanceVec0 = libv::gl::Attribute<10, libv::vec4f>{};
// static constexpr auto attrInstanceVec1 = libv::gl::Attribute<11, libv::vec4f>{};

static constexpr auto attrCustom1          = libv::gl::Attribute<14, libv::vec4f>{};
static constexpr auto attrCustom0          = libv::gl::Attribute<15, libv::vec4f>{};
// static constexpr auto attrTessFactor  = libv::gl::Attribute<9, libv::vec4f>{};


// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
