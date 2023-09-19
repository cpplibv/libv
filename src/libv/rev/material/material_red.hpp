// Project: libv.rev, File: src/libv/rev/material/material_red.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/resource/material.hpp>
#include <libv/rev/resource/shader.hpp>
#include <libv/rev/shader/block/matrices.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct UniformsRed {
	template <typename Access> void access_uniforms(Access&) {
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderRed = libv::rev::Shader<UniformsRed>;

// -------------------------------------------------------------------------------------------------

class MaterialRed : public BaseMaterial {
public:
	ShaderRed shader;

public:
	explicit inline MaterialRed(ShaderRed shader) :
		shader(std::move(shader)) {}

public:
	static Material create(const MaterialScanner& scan);

public:
	virtual void bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override;
	// virtual bool utilizes(ModelVertexAttribute attribute) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
