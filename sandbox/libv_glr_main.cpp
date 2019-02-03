// File: libv_gl_main3.cpp, Created on 2017.11.17. at 22:38, Author: Vader

// hpp
#include "libv_gl_runner.hpp"
// ext
#include <GL/glew.h>
// libv
#include <libv/math/angle.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/image.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/procedural/cube.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_buffer.hpp>


// -------------------------------------------------------------------------------------------------

constexpr uint32_t WINDOW_WIDTH = 1280;
constexpr uint32_t WINDOW_HEIGHT = 800;

const auto shader_sky_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPos;

out vec3 fragmentDirectionW;

uniform mat4 MVPmat;

void main() {
    fragmentDirectionW = vertexPos;
	vec4 pos = MVPmat * vec4(vertexPos, 1.0);
    gl_Position = pos.xyww; // Set depth to 1.0
}
)";

const auto shader_sky_fs = R"(
#version 330 core

uniform samplerCube textureSkySampler;

in vec3 fragmentDirectionW;

out vec4 color;

void main() {
	color = texture(textureSkySampler, fragmentDirectionW).rgba;
}
)";

const auto shader_sphere_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 8) in vec2 vertexTexture0;

out vec3 fragmentNormal;
out vec2 fragmentTexture0;

const float PI = 3.1415926535897932384626433832795;

uniform float time;

layout(std140) uniform Sphere {
	mat4 MVPmat;
	mat4 Mmat;
	vec3 color;
};

void main() {
	const float effect_period = 3;
	const float full_period = 10;

	float effect = max(0, mod(time, full_period) - full_period + effect_period) / effect_period;

	gl_Position = MVPmat * vec4(vertexPosition * (cos(effect * 2 * PI) * 0.5 + 0.5), 1);
	fragmentNormal = normalize(vertexNormal);
	fragmentTexture0 = vertexTexture0;
}
)";

const auto shader_sphere_fs = R"(
#version 330 core

in vec3 fragmentNormal;
in vec2 fragmentTexture0;

out vec4 output;

uniform sampler2D texture0Sampler;
uniform sampler2D texture1Sampler;

uniform vec3 shift;

layout(std140) uniform Sphere {
	mat4 MVPmat;
	mat4 Mmat;
	vec3 color;
};

void main() {
	output = texture(texture0Sampler, fragmentTexture0) * 6;
	output = output + texture(texture1Sampler, fragmentTexture0);
	output = output + vec4(vec3(fragmentTexture0 * 0.3 + 0.4, 0.5) * color, 1.0) * 3;
	output = output / 10;

	//output = output + vec4(vec3(fragmentTexture0 * 0.3 + 0.4, 0.5) * color, 1.0);
}
)";

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal    = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_tangent   = libv::glr::Attribute<6, libv::vec3f>{};
constexpr auto attribute_bitangent = libv::glr::Attribute<7, libv::vec3f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};

const auto uniformBlock_sphere   = libv::glr::UniformBlockBinding{0, "Sphere"};
const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{1, "Matrices"};
const auto uniformBlock_material = libv::glr::UniformBlockBinding{2, "Material"};
const auto uniformBlock_lights   = libv::glr::UniformBlockBinding{3, "Lights"};
const auto uniformBlock_skeleton = libv::glr::UniformBlockBinding{4, "Skeleton"};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal  = libv::gl::TextureChannel{1};

// -------------------------------------------------------------------------------------------------

struct SphereUniformLayout {
	libv::glr::Uniform_mat4f MVPmat;
	libv::glr::Uniform_mat4f Mmat;
	libv::glr::Uniform_vec3f color;

	LIBV_REFLECTION_ACCESS(MVPmat);
	LIBV_REFLECTION_ACCESS(Mmat);
	LIBV_REFLECTION_ACCESS(color);
};
const auto sphere_layout = libv::glr::layout_std140<SphereUniformLayout>(uniformBlock_sphere);

// -------------------------------------------------------------------------------------------------

// =================================================================================================

//UniformBlockLayout<T>        : {size, binding, locations<T>...}
//		API: defines the alignment rules inside a uniform buffer block
//		> rename to UniformLayoutSTD140<T>
//		> add static functions for fetching stride information
//RemoteUniformBuffer          : {data, buffer, ...}
//		stores the UniformBuffer
//UniformBlockBinding          : {UniformBlockBindingLocation, string}
//		API: defines an uniform block binding
//UniformBlockBindingLocation  : uint32_t
//		semi-API: defines an uniform block binding index
//		> rename
//Uniform<T>                   : {location}
//		API: stores the memory offset inside uniform block for a "layouted" variable
//		API: stores the index inside program location for a "layouted" variable
//		> rename
//UniformBuffer                : shared_ptr<RemoteUniformBuffer>
//UniformBlock::Proxy<T>   : byte*
//		Hidden API, got moved into UniformBlock::Proxy<T>
//UniformBlockUniqueView : {UniformBlockBindingLocation, UniformBlock}
//UniformDataBlock             : {offset, size}
//uniformWriteToBuffer(ptr, value)

// =================================================================================================

struct Sandbox {
	float angle = 0.0f;
	float time = 0.0f;

	libv::glr::Remote remote; // Remote has to be the first data member to cleanup gl resources

	libv::glr::Mesh sphere_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Program sphere_program;
	libv::glr::UniformBuffer sphere_uniforms{libv::gl::BufferUsage::StreamDraw};
	libv::glr::Uniform_vec3f sphere_uniform_shift;
	libv::glr::Uniform_float sphere_uniform_time;
	libv::glr::Uniform_texture sphere_uniform_texture0;
	libv::glr::Uniform_texture sphere_uniform_texture1;
//	libv::glr::Texture2D sphere_texture0;
	libv::glr::Texture sphere_texture0;
	libv::glr::Texture2D::R8_G8_B8_A8 sphere_texture1;

	libv::glr::Program sky_program;
	libv::glr::Mesh sky_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Uniform_mat4f sky_uniform_MVPmat;
	libv::glr::Uniform_texture sky_uniform_texture;
	libv::glr::Texture sky_texture0;

	Sandbox() {
		// Sphere
		sphere_program.vertex(shader_sphere_vs);
		sphere_program.fragment(shader_sphere_fs);
		sphere_program.block_binding(uniformBlock_sphere);
		sphere_program.assign(sphere_uniform_shift, "shift");
		sphere_program.assign(sphere_uniform_time, "time");
		sphere_program.assign(sphere_uniform_texture0, "texture0Sampler", textureChannel_diffuse);
		sphere_program.assign(sphere_uniform_texture1, "texture1Sampler", textureChannel_normal);

		const auto dataTexture0 = libv::read_file_or_throw("res/texture/hexagon_metal_0001_diffuse.dds");
		auto imageTexture0 = libv::gl::load_image_or_throw(dataTexture0);
		sphere_texture0.load(std::move(imageTexture0));

		const libv::vec4uc tex_data[] = {
			{0, 0, 0, 255},
			{255, 0, 0, 255},
			{0, 255, 0, 255},
			{255, 255, 0, 255},
		};

		sphere_texture1.storage(1, {2, 2});
		sphere_texture1.image(0, {0, 0}, {2, 2}, tex_data);

		{
			auto position = sphere_mesh.attribute(attribute_position);
			auto normal = sphere_mesh.attribute(attribute_normal);
			auto texture0 = sphere_mesh.attribute(attribute_texture0);
			auto index = sphere_mesh.index();

			libv::glr::generateSpherifiedCube(32, position, normal, texture0, index);
		}

		// Sky
		sky_program.vertex(shader_sky_vs);
		sky_program.fragment(shader_sky_fs);
		sky_program.assign(sky_uniform_texture, "textureSkySampler", textureChannel_diffuse);
		sky_program.assign(sky_uniform_MVPmat, "MVPmat");

		const auto dataSky = libv::read_file_or_throw("res/texture/cube_debug_transparent.dds");
		auto imageSky = libv::gl::load_image_or_throw(dataSky);
		sky_texture0.load(std::move(imageSky));

		{
//			auto position = sky_mesh.attribute(attribute_position);
//			auto index = sky_mesh.index();
//
//			libv::glr::generateCube(position, libv::glr::ignore, libv::glr::ignore, libv::glr::flip(index));
//			libv::glr::generateCube(position, normal, texture0, libv::glr::flip(index));

			auto position = sky_mesh.attribute(attribute_position);
			auto index = sky_mesh.index();

			libv::glr::generateInnerCube(position, index);
		}

		remote.create();
		remote.enableDebug();

		log_sandbox.debug("{:46} [{:>10} ]", "CurrentAvailableVideoMemory",        remote.gl.getCurrentAvailableVideoMemory());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnits",               remote.gl.getMaxTextureImageUnits());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsFragment",       remote.gl.getMaxTextureImageUnitsFragment());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsGeometry",       remote.gl.getMaxTextureImageUnitsGeometry());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsVertex",         remote.gl.getMaxTextureImageUnitsVertex());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsCompute",        remote.gl.getMaxTextureImageUnitsCompute());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessControl",    remote.gl.getMaxTextureImageUnitsTessControl());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessEvaluation", remote.gl.getMaxTextureImageUnitsTessEvaluation());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureSize",                     remote.gl.getMaxTextureSize());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBlockSize",                remote.gl.getMaxUniformBlockSize());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBufferBindings",           remote.gl.getMaxUniformBufferBindings());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexAttribs",                   remote.gl.getMaxVertexAttribs());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexUniformComponents",         remote.gl.getMaxVertexUniformComponents());
		log_sandbox.debug("{:46} [{:>10} ]", "UniformBufferOffsetAlignment",       remote.gl.getUniformBufferOffsetAlignment());

	}

	~Sandbox() {
	}

	void update(const std::chrono::duration<float> deltaTime) {
		angle = std::fmod(angle + 22.5f * deltaTime.count(), 360.0f);
		time += deltaTime.count();
	}

	void render() {
		auto queue = remote.queue();

		render_remote(queue);

		remote.queue(std::move(queue));
		remote.render();
	}

	void render_remote(libv::glr::Queue& gl) {
		const auto guard_s = gl.state.push_guard();
		const auto guard_m = gl.model.push_guard();
		const auto guard_v = gl.view.push_guard();
		const auto guard_p = gl.projection.push_guard();

		gl.state.enableBlend();
		gl.state.blendSrc_SourceAlpha();
		gl.state.blendDst_One_Minus_SourceAlpha();
//		gl.state.blendEquation_Add();

		gl.state.enableCullFace();
		gl.state.frontFaceCCW();
		gl.state.cullBackFace();

		gl.state.polygonModeFill();
//		gl.state.polygonModeLine();

		gl.state.enableDepthTest();
		gl.state.depthFunctionLess();

		gl.setClearColor(0.236f, 0.311f, 0.311f, 1.0f);
		gl.clearColor();
		gl.clearDepth();

		gl.projection = libv::mat4f::perspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.f);
		gl.view = libv::mat4f::lookAt({2.f, 2.f, 1.2f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f});
		gl.view.rotate(libv::Degrees{angle}, 0.f, 0.f, 1.f);
		gl.model = libv::mat4f::identity();

		{
			for (size_t i = 0; i < 3; ++i) {
				const auto guard_m2 = gl.model.push_guard();
				gl.model.translate(0, 0, 2 * static_cast<float>(i) - 2);

				auto uniforms = sphere_uniforms.block_unique(sphere_layout);
				uniforms[sphere_layout.MVPmat] = gl.mvp();
				uniforms[sphere_layout.Mmat] = gl.model;
				uniforms[sphere_layout.color] = libv::vec3f(1.0f, 1.0f, 1.0f) * (static_cast<float>(i) * 0.2f + 0.8f);

				gl.program(sphere_program);
				gl.uniform(sphere_uniform_shift, libv::vec3f(0, angle, static_cast<float>(i)));
				gl.uniform(sphere_uniform_time, time);
				gl.uniform(std::move(uniforms));
				gl.texture(sphere_texture0, textureChannel_diffuse);
				gl.texture(sphere_texture1, textureChannel_normal);
				gl.render(sphere_mesh);
			}
		}

		{
			const auto guard_s = gl.state.push_guard();
			gl.state.depthFunctionLEqual();
			gl.program(sky_program);
			gl.uniform(sky_uniform_MVPmat, gl.projection * libv::mat4f(libv::mat3f(gl.view)));
			gl.texture(sky_texture0, textureChannel_diffuse);
			gl.render(sky_mesh);
		}
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger;
	return run_sandbox<Sandbox>("Sandbox libv.GL3", WINDOW_HEIGHT, WINDOW_WIDTH);
}