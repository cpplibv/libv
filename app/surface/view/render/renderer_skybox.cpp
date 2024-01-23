// Project: libv, File: app/surface/view/render/renderer_skybox.cpp

// hpp
#include <surface/view/render/renderer_skybox.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/glr/procedural/cube.hpp>



#include <surface/view/render/renderer.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererSkybox::RendererSkybox(RendererResourceContext& rctx) :
//		shader(rctx.loader.shader, "skybox.vs", "skybox.fs"),
		shader(rctx.loader.shader, "surface/skybox.vs", "surface/skybox.fs") {
//		texture(rctx.loader.texture.load("sky.dds")) {
//		texture(rctx.loader.texture.load("builtin:white")) {

	auto position = mesh.attribute(attribute_position);
	auto index = mesh.index();

	libv::glr::generateInnerCube(position, index);
}

void RendererSkybox::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	const auto s_guard = glr.state.push_guard();

	glr.state.depthFunctionLEqual();
	glr.program(shader.program());

	glr.uniform(shader.uniform().fogEnabled, fogEnabled);
//	glr.uniform(shader.uniform().fogIntensity, fogIntensity);
	glr.uniform(shader.uniform().fogColor, fogColor);
//	glr.uniform(shader.uniform().sunColor, sunColor);
//	glr.uniform(shader.uniform().sunDirection, sunDirection);

	auto uniforms = uniform_stream.block_unique(layout_matrices);
//	uniforms[layout_matrices.matMVP] = glr.mvp();
	uniforms[layout_matrices.matMVP] = glr.projection * libv::mat4f(libv::mat3f(glr.view));
	uniforms[layout_matrices.matM] = glr.model;
	uniforms[layout_matrices.matP] = glr.projection;
	uniforms[layout_matrices.eye] = glr.eye();
	glr.uniform(std::move(uniforms));
	glr.texture(texture.texture(), textureUnit_diffuse);
	glr.render(mesh);
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
