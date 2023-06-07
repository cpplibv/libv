// Created by Vader on 2023.01.09..

#pragma once

// libv
#include <libv/glr/fwd.hpp>
#include <libv/glr/mesh.hpp>
// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/shader.hpp>
#include <libv/rev/shader/editor_grid.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct RendererEditorGrid {
	libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::rev::Shader<UniformsEditorGrid> shader;

public:
	explicit RendererEditorGrid(libv::rev::ResourceManager& loader);

//	void build_mesh();
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
