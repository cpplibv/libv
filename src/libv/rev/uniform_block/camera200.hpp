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

struct UniformLayoutCamera200 {
	libv::glr::Uniform_mat4f matP;
	libv::glr::Uniform_mat4f matV;
	libv::glr::Uniform_vec3f eye;
	libv::glr::Uniform_vec3f cameraForwardW;
	libv::glr::Uniform_vec3f cameraRightW;
	libv::glr::Uniform_vec3f cameraUpW;

	LIBV_REFLECTION_ACCESS(matP);
	LIBV_REFLECTION_ACCESS(matV);
	LIBV_REFLECTION_ACCESS(eye);
	LIBV_REFLECTION_ACCESS(cameraForwardW);
	LIBV_REFLECTION_ACCESS(cameraRightW);
	LIBV_REFLECTION_ACCESS(cameraUpW);
};

const auto uniformBlock_camera200 = libv::glr::UniformBlockBinding{1, "Camera200"};
const auto layout_camera200 = libv::glr::layout_std140<UniformLayoutCamera200>(uniformBlock_camera200);

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
