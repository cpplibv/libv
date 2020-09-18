// Project: libv, File: app/vm4_viewer/scene/shader_color.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <vm4_viewer/scene/shader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct UniformsShaderColor {
	libv::glr::Uniform_mat4f matMVP;

	template <typename Access>
	void update_uniforms(Access& access) {
		access(matMVP, "matMVP");
	}
};

using ShaderColor = app::Shader<UniformsShaderColor>;

// -------------------------------------------------------------------------------------------------

} // namespace app
