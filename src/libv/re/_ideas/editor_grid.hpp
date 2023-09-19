// Project: libv.re, File: src/libv/re/_ideas/editor_grid.hpp

#pragma once

#include <libv/glr/fwd.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/material.hpp>
#include <libv/re/shader.hpp>
#include <libv/re/shader/block/camera200.hpp>
#include <libv/re/shader/block/matrices200.hpp>
#include <libv/re/static_mesh.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

// struct UniformsEditorGridLegacy {
// 	template <typename Access>
// 	void access_uniforms(Access& access) {
// 		(void) access;
// 	}
//
// 	template <typename Access>
// 	void access_blocks(Access& access) {
// 		access(uniformBlock_Matrices200);
// 		access(uniformBlock_Camera200);
// 	}
// };
//
// // -------------------------------------------------------------------------------------------------
//
// struct RendererEditorGrid {
// 	libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
// 	libv::re::Shader<UniformsEditorGridLegacy> shader;
//
// public:
// 	explicit RendererEditorGrid(libv::re::ResourceManager& loader);
// 	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
// };
//
// // =================================================================================================
//
// struct RendererEditorGrid200 {
// 	// libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
// 	// libv::re::Model model;
// 	// libv::re::Shader<UniformsEditorGrid> shader;
// 	libv::re::StaticMesh mesh;
// 	libv::re::Material material;
//
// public:
// 	explicit RendererEditorGrid200(libv::re::ResourceManager& loader);
// 	void render(libv::re::Engine& rev);
// };

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv


// Project: libv.re, File: src/libv/re/_ideas/editor_grid.cpp

// #include <libv/glr/queue.hpp>
// #include <libv/glr/uniform_buffer.hpp>
// #include <libv/re/material/editor_grid.hpp>
// #include <libv/re/resource_manager.hpp>
// #include <libv/re/shader/attribute.hpp>
// #include <libv/re/material_adv.hpp>
// #include <libv/re/renderer/renderer_editor_grid2.hpp>
// #include <libv/glr/queue.hpp>
// #include <libv/glr/uniform_buffer.hpp>
// #include <libv/re/shader/attribute.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

// RendererEditorGrid::RendererEditorGrid(libv::re::ResourceManager& loader) :
// 	shader(loader.shader, "editor/editor_grid.vs", "editor/editor_grid.fs") {
// 	auto position = mesh_grid.attribute(libv::re::attribute_position);
// 	auto index = mesh_grid.index();
//
// 	position(-1, -1, 0);
// 	position(+1, -1, 0);
// 	position(+1, +1, 0);
// 	position(-1, +1, 0);
//
// 	index.quad(0, 1, 2, 3); // Front face quad
// 	index.quad(0, 3, 2, 1); // Back face quad
// }
//
// void RendererEditorGrid::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
// 	auto matrices = uniform_stream.block_unique(libv::re::layout_Matrices200);
// 	matrices[libv::re::layout_Matrices200.matMVP] = glr.mvp();
// 	matrices[libv::re::layout_Matrices200.matM] = glr.model;
//
// 	auto camera = uniform_stream.block_unique(libv::re::layout_Camera200);
// 	camera[libv::re::layout_Camera200.matP] = glr.projection;
// 	camera[libv::re::layout_Camera200.matV] = glr.view;
// 	camera[libv::re::layout_Camera200.eye] = glr.eye();
// 	camera[libv::re::layout_Camera200.cameraForwardW] =
// 			libv::vec3f(glr.view.top()[0][2], glr.view.top()[1][2], glr.view.top()[2][2]);
// 	camera[libv::re::layout_Camera200.cameraRightW] =
// 			libv::vec3f(glr.view.top()[0][0], glr.view.top()[1][0], glr.view.top()[2][0]);
// 	camera[libv::re::layout_Camera200.cameraUpW] =
// 			libv::vec3f(glr.view.top()[0][1], glr.view.top()[1][1], glr.view.top()[2][1]);
//
// 	glr.program(shader.program());
// 	glr.uniform(std::move(matrices));
// 	glr.uniform(std::move(camera));
// 	glr.render(mesh_grid);
// }

// =================================================================================================

// // RendererEditorGrid200::RendererEditorGrid200(libv::re::ResourceManager& loader) {
// RendererEditorGrid200::RendererEditorGrid200(libv::re::ResourceManager& rm) :
// 		material(MaterialEditorGrid::create(rm)) {
//
// 	//rm.material.
// 	// auto position = mesh_grid.attribute(libv::re::attribute_position);
// 	// auto index = mesh_grid.index();
// 	//
// 	// position(-1, -1, 0);
// 	// position(+1, -1, 0);
// 	// position(+1, +1, 0);
// 	// position(-1, +1, 0);
// 	//
// 	// index.quad(0, 1, 2, 3); // Front face quad
// 	// index.quad(0, 3, 2, 1); // Back face quad
// }
//
// void RendererEditorGrid200::render(libv::re::Engine& rev) {
// 	// auto matrices = uniform_stream.block_unique(libv::re::layout_Matrices200);
// 	// matrices[libv::re::layout_Matrices200.matMVP] = glr.mvp();
// 	// matrices[libv::re::layout_Matrices200.matM] = glr.model;
// 	//
// 	// auto camera = uniform_stream.block_unique(libv::re::layout_Camera200);
// 	// camera[libv::re::layout_Camera200.matP] = glr.projection;
// 	// camera[libv::re::layout_Camera200.matV] = glr.view;
// 	// camera[libv::re::layout_Camera200.eye] = glr.eye();
// 	// camera[libv::re::layout_Camera200.cameraForwardW] = libv::vec3f(glr.view.top()[0][2], glr.view.top()[1][2],
// glr.view.top()[2][2]);
// 	// camera[libv::re::layout_Camera200.cameraRightW] = libv::vec3f(glr.view.top()[0][0], glr.view.top()[1][0],
// glr.view.top()[2][0]);
// 	// camera[libv::re::layout_Camera200.cameraUpW] = libv::vec3f(glr.view.top()[0][1], glr.view.top()[1][1],
// glr.view.top()[2][1]);
// 	//
// 	// glr.program(shader.program());
// 	// glr.uniform(std::move(matrices));
// 	// glr.uniform(std::move(camera));
// 	// glr.render(mesh_grid);
// }

// void RendererEditorGrid200::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
// 	auto matrices = uniform_stream.block_unique(libv::re::layout_Matrices200);
// 	matrices[libv::re::layout_Matrices200.matMVP] = glr.mvp();
// 	matrices[libv::re::layout_Matrices200.matM] = glr.model;
//
// 	auto camera = uniform_stream.block_unique(libv::re::layout_Camera200);
// 	camera[libv::re::layout_Camera200.matP] = glr.projection;
// 	camera[libv::re::layout_Camera200.matV] = glr.view;
// 	camera[libv::re::layout_Camera200.eye] = glr.eye();
// 	camera[libv::re::layout_Camera200.cameraForwardW] = libv::vec3f(glr.view.top()[0][2], glr.view.top()[1][2],
// glr.view.top()[2][2]); 	camera[libv::re::layout_Camera200.cameraRightW] = libv::vec3f(glr.view.top()[0][0],
// glr.view.top()[1][0], glr.view.top()[2][0]); 	camera[libv::re::layout_Camera200.cameraUpW] =
// libv::vec3f(glr.view.top()[0][1], glr.view.top()[1][1], glr.view.top()[2][1]);
//
// 	glr.program(shader.program());
// 	glr.uniform(std::move(matrices));
// 	glr.uniform(std::move(camera));
// 	glr.render(mesh_grid);
// }

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
