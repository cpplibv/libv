// Project: libv.ui, File: app/gen_ui_theme/canvas.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/gl/enum.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_buffer.hpp>
//#include <libv/log/log.hpp>
// std
#include <chrono>
#include <atomic>
#include <mutex>


// -------------------------------------------------------------------------------------------------

namespace app {
namespace {

// -------------------------------------------------------------------------------------------------

const auto shader_texture_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 8) in vec2 vertexTexture0;

out vec2 fragmentTexture0;

layout(std140) uniform Sphere {
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentTexture0 = vertexTexture0;
}
)";

const auto shader_texture_fs = R"(
#version 330 core

in vec3 fragmentNormal;
in vec2 fragmentTexture0;

out vec4 output;

uniform sampler2D texture0Sampler;

layout(std140) uniform Sphere {
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};

void main() {
	output = texture(texture0Sampler, fragmentTexture0).rgba;
}
)";

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};

const auto uniformBlock_sphere   = libv::glr::UniformBlockBinding{0, "Sphere"};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};

// -------------------------------------------------------------------------------------------------

struct SphereUniformLayout {
	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f color;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matM);
	LIBV_REFLECTION_ACCESS(color);
};
const auto sphere_layout = libv::glr::layout_std140<SphereUniformLayout>(uniformBlock_sphere);

} // namespace -------------------------------------------------------------------------------------

class Canvas : public libv::ui::Canvas {
	libv::glr::Program sphere_program;
	libv::glr::Mesh sphere_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::UniformBuffer sphere_uniforms{libv::gl::BufferUsage::StreamDraw};
	libv::glr::Uniform_vec3f sphere_uniform_shift;
	libv::glr::Uniform_float sphere_uniform_time;
	libv::glr::Uniform_texture sphere_uniform_texture0;
	libv::glr::Uniform_texture sphere_uniform_texture1;
	libv::glr::Texture2D::R8_G8_B8_A8 sphere_texture0;

private:
	std::mutex mutex;
	libv::vec2i texture_size = {128, 128};
	std::vector<libv::vec4uc> texture_data;
	bool dirty = false;

private:
	std::chrono::duration<float> time;

public:
	std::atomic_bool pulse{true};

public:
	Canvas() {
		// --- Sphere ---
		sphere_program.vertex(shader_texture_vs);
		sphere_program.fragment(shader_texture_fs);
		sphere_program.block_binding(uniformBlock_sphere);
		sphere_program.assign(sphere_uniform_texture0, "texture0Sampler", textureChannel_diffuse);

		sphere_texture0.storage(1, texture_size);
		sphere_texture0.set(libv::gl::MagFilter::Nearest);
	}

public:
	void update_texture(std::vector<libv::vec4uc> texture_data_, libv::vec2i texture_size_) {
		const auto guard = std::scoped_lock(mutex);
		texture_data = std::move(texture_data_);
		texture_size = texture_size_;
		dirty = true;
	}

private:
	virtual void update(libv::ui::time_duration deltaTime) override {
		time += deltaTime;

		const auto guard = std::scoped_lock(mutex);
		if (!dirty)
			return;

		sphere_texture0.image(0, {0, 0}, texture_size, texture_data.data());

		{
			sphere_mesh.clear();
			auto position = sphere_mesh.attribute(attribute_position);
			auto texture0 = sphere_mesh.attribute(attribute_texture0);
			auto index = sphere_mesh.index();

			const auto add_lod = [&](int i, auto pos, auto size) {
				position(pos.x +      0, pos.y +      0, 0);
				position(pos.x + size.x, pos.y +      0, 0);
				position(pos.x + size.x, pos.y + size.y, 0);
				position(pos.x +      0, pos.y + size.y, 0);

				texture0(0, 0);
				texture0(1, 0);
				texture0(1, 1);
				texture0(0, 1);

				index.quad(i * 4 + 0, i * 4 + 1, i * 4 + 2, i * 4 + 3);
			};

			const auto ts = texture_size.cast<float>();
			const auto cs = canvas_size.cast<float>();

			add_lod(0, libv::vec2f(0 + 10, (cs.y - ts.y) * 0.5f), ts);
			add_lod(1, libv::vec2f(ts.x + 20, 10), ts * 8.0f);
		}
	}

	virtual void render(libv::glr::Queue& gl) override {
		const auto guard_s = gl.state.push_guard();
		const auto guard_m = gl.model.push_guard();
		const auto guard_v = gl.view.push_guard();
		const auto guard_p = gl.projection.push_guard();

		gl.state.enableBlend();
		gl.state.blendSrc_SourceAlpha();
		gl.state.blendDst_One_Minus_SourceAlpha();

		gl.state.enableCullFace();
		gl.state.frontFaceCCW();
		gl.state.cullBackFace();

		gl.state.polygonModeFill();

		gl.state.enableDepthTest();
		gl.state.depthFunctionLess();

		const auto clear_v = !pulse ? 0.5f : std::sin(time.count()) * 0.5f + 0.5f;
		gl.setClearColor(clear_v, clear_v, clear_v, 1.0f);
		gl.clearColor();
		gl.clearDepth();

		gl.projection = libv::mat4f::ortho(0, canvas_size.x, 0, canvas_size.y);
		gl.view = libv::mat4f::identity();
		gl.model = libv::mat4f::identity();

		{
			auto uniforms = sphere_uniforms.block_unique(sphere_layout);
			uniforms[sphere_layout.matMVP] = gl.mvp();
			uniforms[sphere_layout.matM] = gl.model;
			uniforms[sphere_layout.color] = libv::vec3f(1.0f, 1.0f, 1.0f);

			gl.program(sphere_program);
			gl.uniform(std::move(uniforms));
			gl.texture(sphere_texture0, textureChannel_diffuse);
			gl.render(sphere_mesh);
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
