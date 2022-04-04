// Project: libv.vm4, File: src/libv/vm4/material.hpp

#pragma once

// fwd
#include <libv/vm4/fwd.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

enum class VertexAttribute : int32_t {
	position,
	normal,
	tangent,
	bitangent,
	texture0,
	boneID,
	boneWeight,
};

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
