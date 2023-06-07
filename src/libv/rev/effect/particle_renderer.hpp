// Created by Vader on 2023.01.09..

#pragma once

// libv
// #include <libv/math/vec.hpp>
// #include <libv/algo/erase_if_unstable.hpp>
// #include <libv/utility/random/xoroshiro128.hpp>
// #include <libv/math/quat.hpp>
#include <libv/gl/array_buffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/vertex_array.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/glr/texture.hpp>
// std
// #include <variant>
// #include <memory>
// #include <vector>
// #include <span>
// #include <random>
// pro
#include <libv/rev/shader/attribute.hpp>
#include <libv/rev/resource/resource_manager.hpp>
#include <libv/rev/resource/shader.hpp>
#include <libv/rev/resource/texture.hpp>

// #include <libv/rev/shader/block/camera200.hpp>
// #include <libv/rev/shader/block/matrices200.hpp>
#include <libv/rev/shader/particle.hpp>

#include <libv/rev/effect/particle.hpp>
// #include <libv/rev/effect/particle_engine.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

constexpr auto attribute_particle_xyz       = libv::glr::Attribute<10, libv::vec3f>{};
constexpr auto attribute_particle_packSizeRotation = libv::glr::Attribute<11, libv::vec2f>{};

// constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};

// struct UniformsParticle {
// //	libv::glr::Uniform_mat4f matMVP;
// //	libv::glr::Uniform_mat4f matM;
// //	libv::glr::Uniform_vec3f color;
// 	libv::glr::Uniform_texture texture0;
//
// 	template <typename Access> void access_uniforms(Access& access) {
// 		access(texture0, "texture0", textureChannel_diffuse);
// 	}
//
// 	template <typename Access> void access_blocks(Access& access) {
// 		access(uniformBlock_Matrices200);
// 		access(uniformBlock_Camera200);
// 	}
// };

// -------------------------------------------------------------------------------------------------

struct RendererParticle {
	libv::rev::Shader<UniformsParticle> shader_particle;
	libv::rev::Texture texture_star_01;
	libv::rev::Texture texture_star_06;
	libv::rev::Texture texture_circle_05;

	libv::gl::ArrayBuffer shape;
	libv::gl::ArrayBuffer positions;
	libv::gl::ArrayBuffer properties;
	libv::gl::VertexArray vao;

public:
	explicit RendererParticle(libv::rev::ResourceManager& rm) :
		shader_particle(rm.shader, "rev_sandbox/particle.vs", "rev_sandbox/particle.fs"),
		texture_star_01(rm.texture.load("particle/star_01.png", libv::rev::parse_swizzle_or_throw("111r"))),
		texture_star_06(rm.texture.load("particle/star_06.png", libv::rev::parse_swizzle_or_throw("111r"))),
		texture_circle_05(rm.texture.load("particle/circle_05.png", libv::rev::parse_swizzle_or_throw("111r"))) {
	}

	void create(libv::gl::GL& gl) {
		libv::vec3f quadPositions[] = {
				{-0.5f, -0.5f, 0.f},
				{+0.5f, -0.5f, 0.f},
				{-0.5f, +0.5f, 0.f},
				{+0.5f, +0.5f, 0.f},
		};

		gl(vao).create();
		gl(vao).bind();

		gl(shape).create();
		gl(shape).bind();
		gl(shape).data(quadPositions, 4 * sizeof(libv::vec3f), libv::gl::BufferUsage::StaticDraw);
		gl(vao).bindAttribute(shape, attribute_position, 0, 0);

		gl(positions).create();
		gl(positions).bind();
		gl(positions).data(nullptr, 0, libv::gl::BufferUsage::StreamDraw);
		gl(vao).bindAttribute(positions, attribute_particle_xyz, 0, 0);
		gl(vao).attributeDivisor(attribute_particle_xyz, 1);

		gl(properties).create();
		gl(properties).bind();
		gl(properties).data(nullptr, 0, libv::gl::BufferUsage::StreamDraw);
		gl(vao).bindAttribute(properties, attribute_color0, &ParticleGPUData::color);
		gl(vao).attributeDivisor(attribute_color0, 1);
		gl(vao).bindAttribute(properties, attribute_particle_packSizeRotation, &ParticleGPUData::packSizeRotation);
		gl(vao).attributeDivisor(attribute_particle_packSizeRotation, 1);

		gl(properties).unbind(); // Rebind in the middle, Unbind as the last
		gl(vao).unbind();
	}

	void destroy(libv::gl::GL& gl) {
		gl(properties).destroy();
		gl(positions).destroy();
		gl(shape).destroy();
		gl(vao).destroy();
	}

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const ParticleEffect& system) {
		const auto s_guard = glr.state.push_guard();
		glr.state.disableDepthMask();

		glr.program(shader_particle.program());

		auto uniforms = uniform_stream.block_unique(layout_Matrices200);
		uniforms[layout_Matrices200.matMVP] = glr.mvp();
		uniforms[layout_Matrices200.matM] = glr.model;
		glr.uniform(std::move(uniforms));

		auto uniformsCam = uniform_stream.block_shared(layout_Camera200);
		uniformsCam[layout_Camera200.matP] = glr.projection;
		uniformsCam[layout_Camera200.matV] = glr.view;
		uniformsCam[layout_Camera200.eye] = glr.eye();
		uniformsCam[layout_Camera200.cameraForwardW] = libv::vec3f(glr.view.top()[0][2], glr.view.top()[1][2], glr.view.top()[2][2]);
		uniformsCam[layout_Camera200.cameraRightW] = libv::vec3f(glr.view.top()[0][0], glr.view.top()[1][0], glr.view.top()[2][0]);
		uniformsCam[layout_Camera200.cameraUpW] = libv::vec3f(glr.view.top()[0][1], glr.view.top()[1][1], glr.view.top()[2][1]);
		glr.uniform(std::move(uniformsCam));

		// glr.texture(texture_star_01.texture(), textureChannel_diffuse);
		// glr.texture(texture_star_06.texture(), textureChannel_diffuse);
		glr.texture(texture_circle_05.texture(), textureChannel_diffuse);

		// !!! Test Hack &system
		// glr.callbackProgram([this](libv::gl::GL& gl) {
		glr.callbackProgram([this, &system](libv::gl::GL& gl) mutable {

			const auto sizePositions = system.numParticle() * sizeof(libv::vec3f);
			gl(positions).named_data(nullptr, sizePositions, libv::gl::BufferUsage::StreamDraw);
			gl(positions).named_subData(0, system.positionData(), sizePositions);

			const auto sizeProperties = system.numParticle() * sizeof(ParticleGPUData);
			gl(properties).named_data(nullptr, sizeProperties, libv::gl::BufferUsage::StreamDraw);
			gl(properties).named_subData(0, system.propertiesData(), sizeProperties);

			gl(vao).bind();
			gl(vao).drawArraysInstanced(libv::gl::Primitive::TriangleStrip, 0, 4, system.numParticle());
		});

		// glr.render_instanced(mesh, system.numParticle());
		// glr.render_instanced(mesh, 1);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
