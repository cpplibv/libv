//
// Generated source code
// Generator version: v5.0.0
// Input file: src/libv/rev/shader/block/matrices200.in.lua

#pragma once

//
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/meta/reflection_access.hpp>


namespace libv::rev {

// -------------------------------------------------------------------------------------------------

struct UniformLayoutMatrices200 {
	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matM;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matM);
};

const auto uniformBlock_Matrices200 = libv::glr::UniformBlockBinding{1, "Matrices200"};
const auto layout_Matrices200 = libv::glr::layout_std140<UniformLayoutMatrices200>(uniformBlock_Matrices200);

// -------------------------------------------------------------------------------------------------

} // namespace libv::rev
