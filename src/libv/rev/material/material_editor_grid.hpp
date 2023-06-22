// Project: libv.rev, File: src/libv/rev/materials/material_sprite_baker.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/material.hpp>
#include <libv/rev/shader.hpp>
// #include <libv/rev/shader/block/matrices.hpp>
#include <libv/rev/shader/editor_grid.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class MaterialEditorGrid : public BaseMaterial {
public:
	libv::rev::Shader<UniformsEditorGrid> shader;

	// Texture diffuse;
	//Texture normal;
	//Texture specular;

public:
	inline explicit MaterialEditorGrid(libv::rev::Shader<UniformsEditorGrid> shader) :
		shader(std::move(shader)) {}

public:
	static Material create(const MaterialScanner& scan);
	static Material create(libv::rev::ResourceManager& rm);

public:
	virtual void bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override;
	// virtual bool utilizes(ModelVertexAttribute attribute) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
