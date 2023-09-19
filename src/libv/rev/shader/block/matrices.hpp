// Project: libv.rev, File: src/libv/rev/shader/block/matrices.hpp

#pragma once

// libv
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/meta/reflection_access.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct UniformLayoutMatrices {
	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f eye;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matP);
	LIBV_REFLECTION_ACCESS(matM);
	LIBV_REFLECTION_ACCESS(eye);
};

const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{1, "Matrices"};
const auto layout_matrices = libv::glr::layout_std140<UniformLayoutMatrices>(uniformBlock_matrices);

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
