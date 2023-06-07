// Created by Vader on 2023.01.09..

#pragma once

// libv
// pro
#include <libv/rev/shader/block/matrices.hpp>
// #include <libv/rev/settings.hpp>
#include <libv/mt/thread_bulk.hpp>
#include <libv/utility/chrono.hpp>
// Grid only:
#include <libv/glr/uniform_buffer.hpp>
#include <libv/glr/mesh.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct UniformsTestMode {
	libv::glr::Uniform_int32 test_mode;

	template <typename Access> void access_uniforms(Access& access) {
		access(test_mode, "test_mode", 0);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(libv::rev::uniformBlock_matrices);
	}
};

using ShaderTestMode = libv::rev::Shader<UniformsTestMode>;

struct RendererEditorGrid {
	libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader;

public:
	explicit RendererEditorGrid(libv::rev::ResourceManager& loader);

//	void build_mesh();
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

RendererEditorGrid::RendererEditorGrid(libv::rev::ResourceManager& loader) :
		shader(loader.shader, "surface/editor_grid_plane.vs", "surface/editor_grid_plane.fs") {
	auto position = mesh_grid.attribute(libv::rev::attribute_position);
	auto index = mesh_grid.index();

	position(-1, -1, 0);
	position(+1, -1, 0);
	position(+1, +1, 0);
	position(-1, +1, 0);

	index.quad(0, 1, 2, 3); // Front face quad
	index.quad(0, 3, 2, 1); // Back face quad
}

void RendererEditorGrid::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(libv::rev::layout_matrices);
	uniforms[libv::rev::layout_matrices.matMVP] = glr.mvp();
	uniforms[libv::rev::layout_matrices.matM] = glr.model;
	uniforms[libv::rev::layout_matrices.matP] = glr.projection;
	uniforms[libv::rev::layout_matrices.eye] = glr.eye();

	glr.program(shader.program());
	glr.uniform(std::move(uniforms));
	glr.render(mesh_grid);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
