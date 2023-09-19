//
// Generated source code
// Generator version: v5.0.0
// Input file: src/libv/rev/shader/particle.in.lua

#pragma once

//
#include <libv/rev/shader/attribute.hpp>
#include <libv/rev/shader/block/camera200.hpp>
#include <libv/rev/shader/block/matrices200.hpp>


namespace libv::rev {

// -------------------------------------------------------------------------------------------------

struct UniformsParticle {
	libv::glr::Uniform_texture texture0;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture0, "texture0", textureUnit_diffuse);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_Matrices200);
		access(uniformBlock_Camera200);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv::rev
