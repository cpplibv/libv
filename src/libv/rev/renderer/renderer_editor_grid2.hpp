// Created by Vader on 2023.01.09..

#pragma once

// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/material.hpp>
#include <libv/rev/static_mesh.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct RendererEditorGrid2 {
	// libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	// libv::rev::Model model;
	// libv::rev::Shader<UniformsEditorGrid> shader;
	libv::rev::StaticMesh mesh;
	libv::rev::Material material;

public:
	explicit RendererEditorGrid2(libv::rev::ResourceManager& loader);
	void render(libv::rev::Engine& rev);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
