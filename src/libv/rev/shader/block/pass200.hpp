//
// Generated source code
// Generator version: v5.0.0
// Input file: src/libv/rev/shader/block/pass200.in.lua

#pragma once

//
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/meta/reflection_access.hpp>


namespace libv::rev {

// -------------------------------------------------------------------------------------------------

struct UniformLayoutPass200 {
	libv::glr::Uniform_vec2f render_resolution;
	libv::glr::Uniform_vec2f pixel_size;
	libv::glr::Uniform_float time_sim;
	libv::glr::Uniform_float time_sim_uncapped;
	libv::glr::Uniform_float time_real;
	libv::glr::Uniform_float time_real_uncapped;

	LIBV_REFLECTION_ACCESS(render_resolution);
	LIBV_REFLECTION_ACCESS(pixel_size);
	LIBV_REFLECTION_ACCESS(time_sim);
	LIBV_REFLECTION_ACCESS(time_sim_uncapped);
	LIBV_REFLECTION_ACCESS(time_real);
	LIBV_REFLECTION_ACCESS(time_real_uncapped);
};

const auto uniformBlock_Pass200 = libv::glr::UniformBlockBinding{2, "Pass200"};
const auto layout_Pass200 = libv::glr::layout_std140<UniformLayoutPass200>(uniformBlock_Pass200);

// -------------------------------------------------------------------------------------------------

} // namespace libv::rev
