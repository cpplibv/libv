//

#pragma once

// libv
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/meta/reflection_access.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct UniformLayoutMatrices200 {
	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matM;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matM);
};

const auto uniformBlock_matrices200 = libv::glr::UniformBlockBinding{0, "Matrices200"};
const auto layout_matrices200 = libv::glr::layout_std140<UniformLayoutMatrices200>(uniformBlock_matrices200);

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
