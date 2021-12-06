// Project: libv.glr, File: sandbox/libv_glr_main.cpp

// hpp
#include "libv_gl_runner.hpp"
// libv
#include <libv/gl/enum.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/image.hpp>
#include <libv/gl/load_image.hpp>
#include <libv/math/angle.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/glr/attribute.hpp>
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/procedural/cube.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/renderbuffer.hpp>
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

uniform mat4 matMVP;

void main() {
    fragmentDirectionW = vertexPos;
	vec4 pos = matMVP * vec4(vertexPos, 1.0);
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
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};

void main() {
	const float effect_period = 3;
	const float full_period = 10;

	float effect = max(0, mod(time, full_period) - full_period + effect_period) / effect_period;

	gl_Position = matMVP * vec4(vertexPosition * (cos(effect * 2 * PI) * 0.5 + 0.5), 1);
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
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};

void main() {
	output = texture(texture0Sampler, fragmentTexture0) * 6;
	output = output + texture(texture1Sampler, fragmentTexture0);
	output = output + vec4(vec3(fragmentTexture0 * 0.3 + 0.4, 0.5) * color, 1.0) * 3;
	output = output / 10;
	output = output * 2; // To over expose so I can test HDR

	//output = output + vec4(vec3(fragmentTexture0 * 0.3 + 0.4, 0.5) * color, 1.0);
}
)";

const auto shader_stream_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 8) in vec2 vertexTexture0;

out vec3 fragmentNormal;
out vec2 fragmentTexture0;

const float PI = 3.1415926535897932384626433832795;

layout(std140) uniform Sphere {
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentNormal = normalize(vertexNormal);
	fragmentTexture0 = vertexTexture0;
}
)";

const auto shader_stream_fs = R"(
#version 330 core

in vec3 fragmentNormal;
in vec2 fragmentTexture0;

out vec4 output;

//uniform sampler2D texture0Sampler;
//uniform sampler2D texture1Sampler;

//uniform vec3 shift;

layout(std140) uniform Sphere {
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};

void main() {
//	output = texture(texture0Sampler, fragmentTexture0) * 6;
//	output = output + texture(texture1Sampler, fragmentTexture0);
//	output = output + vec4(vec3(fragmentTexture0 * 0.3 + 0.4, 0.5) * color, 1.0) * 3;
	output = vec4(color, 1.0);
}
)";

constexpr auto shader_quad_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPos;

out vec2 fragmentUV;

void main() {
	gl_Position = vec4(vertexPos, 1);
	fragmentUV = vertexPos.xy * 0.5 + 0.5;
})";

constexpr auto shader_quad_fs = R"(
#version 330 core

uniform sampler2D texture0Sampler;

in vec2 fragmentUV;

out vec4 color;

void main() {
	vec4 sample = texture(texture0Sampler, fragmentUV, 0).rgba;
	color = vec4(fract(sample.rgb), sample.a);
//	color = sample;
})";

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
	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f color;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matM);
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

	libv::vec2i windowSize = {WINDOW_WIDTH, WINDOW_HEIGHT};
	libv::vec2i framebufferSize = windowSize / 8;
	int32_t samples = 16;

	libv::glr::Remote remote; // Remote has to be the first data member to cleanup gl resources

	libv::glr::Program sphere_program;
	libv::glr::Mesh sphere_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::UniformBuffer sphere_uniforms{libv::gl::BufferUsage::StreamDraw};
	libv::glr::Uniform_vec3f sphere_uniform_shift;
	libv::glr::Uniform_float sphere_uniform_time;
	libv::glr::Uniform_texture sphere_uniform_texture0;
	libv::glr::Uniform_texture sphere_uniform_texture1;
//	libv::glr::Texture2D sphere_texture0;
	libv::glr::Texture sphere_texture0;
	libv::glr::Texture2D::R8_G8_B8_A8 sphere_texture1;

	libv::glr::Program stream_program;
	libv::glr::Mesh stream_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::UniformBufferStream stream_uniforms;

	libv::glr::Program quad_program;
	libv::glr::Mesh quad_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Uniform_texture quad_uniform_texture0;

	libv::glr::Program sky_program;
	libv::glr::Mesh sky_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Uniform_mat4f sky_uniform_matMVP;
	libv::glr::Uniform_texture sky_uniform_texture;
	libv::glr::Texture sky_texture0;

	libv::glr::Framebuffer framebuffer;
//	libv::glr::Texture2DMultisample::R8_G8_B8_A8 framebufferColor;
	libv::glr::Texture2DMultisample::R32F framebufferColor;
//	libv::glr::Texture2DMultisample::RGBA32F framebufferColor;
	libv::glr::Renderbuffer::D32 framebufferDepth;

	libv::glr::Framebuffer framebuffer2;
//	libv::glr::Texture2D::R8_G8_B8_A8 framebuffer2Color;
	libv::glr::Texture2D::RGBA32F framebuffer2Color;

	Sandbox() {
		// --- Sphere ---

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

		static constexpr libv::vec4uc tex_data[] = {
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

		// --- Stream ---

		{
			stream_program.vertex(shader_stream_vs);
			stream_program.fragment(shader_stream_fs);
			stream_program.block_binding(uniformBlock_sphere);
	//		stream_program.assign(sphere_uniform_shift, "shift");
	//		stream_program.assign(sphere_uniform_time, "time");
	//		stream_program.assign(sphere_uniform_texture0, "texture0Sampler", textureChannel_diffuse);
	//		stream_program.assign(sphere_uniform_texture1, "texture1Sampler", textureChannel_normal);

//			const auto dataTexture0 = libv::read_file_or_throw("res/texture/hexagon_metal_0001_diffuse.dds");
//			auto imageTexture0 = libv::gl::load_image_or_throw(dataTexture0);
//			sphere_texture0.load(std::move(imageTexture0));

//			const libv::vec4uc tex_data[] = {
//				{0, 0, 0, 255},
//				{255, 0, 0, 255},
//				{0, 255, 0, 255},
//				{255, 255, 0, 255},
//			};
//
//			sphere_texture1.storage(1, {2, 2});
//			sphere_texture1.image(0, {0, 0}, {2, 2}, tex_data);

			auto position = stream_mesh.attribute(attribute_position);
			auto normal = stream_mesh.attribute(attribute_normal);
			auto texture0 = stream_mesh.attribute(attribute_texture0);
			auto index = stream_mesh.index();

			libv::glr::generateSpherifiedCube(32, position, normal, texture0, index);
		}

		// --- Quad ---

		quad_program.vertex(shader_quad_vs);
		quad_program.fragment(shader_quad_fs);
		quad_program.assign(quad_uniform_texture0, "texture0Sampler", textureChannel_diffuse);

		{
			auto position = quad_mesh.attribute(attribute_position);
			auto index = quad_mesh.index();

			// Full screen
//			position(-1, -1, 0);
//			position(+1, -1, 0);
//			position(+1, +1, 0);
//			position(-1, +1, 0);

			// Little quad in lower left of center
			position(-0.75f, -0.75f, 0);
			position(0, -0.75f, 0);
			position(0, 0, 0);
			position(-0.75f, 0, 0);

			index.quad(0, 1, 2, 3);
		}

		// --- Sky ---

		sky_program.vertex(shader_sky_vs);
		sky_program.fragment(shader_sky_fs);
		sky_program.assign(sky_uniform_texture, "textureSkySampler", textureChannel_diffuse);
		sky_program.assign(sky_uniform_matMVP, "matMVP");

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

		// --- Framebuffer ---

		framebufferColor.storage_ms(framebufferSize, samples, true);
		framebufferDepth.storage_ms(framebufferSize, samples);
		framebuffer.attach2D(libv::gl::Attachment::Color0, framebufferColor);
		framebuffer.attach(libv::gl::Attachment::Depth, framebufferDepth);

		framebuffer2Color.storage(1, framebufferSize);
		framebuffer2Color.set(libv::gl::MagFilter::Nearest);
		framebuffer2.attach2D(libv::gl::Attachment::Color0, framebuffer2Color);

		// --- Start ---

		remote.create();
		remote.enableDebug();

		log_sandbox.debug("{:46} [{:>10} ]", "CurrentAvailableVideoMemory",        remote.gl().getCurrentAvailableVideoMemory());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxSamples",                         remote.gl().getMaxSamples());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnits",               remote.gl().getMaxTextureImageUnits());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsFragment",       remote.gl().getMaxTextureImageUnitsFragment());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsGeometry",       remote.gl().getMaxTextureImageUnitsGeometry());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsVertex",         remote.gl().getMaxTextureImageUnitsVertex());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsCompute",        remote.gl().getMaxTextureImageUnitsCompute());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessControl",    remote.gl().getMaxTextureImageUnitsTessControl());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessEvaluation", remote.gl().getMaxTextureImageUnitsTessEvaluation());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureSize",                     remote.gl().getMaxTextureSize());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBlockSize",                remote.gl().getMaxUniformBlockSize());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBufferBindings",           remote.gl().getMaxUniformBufferBindings());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexAttribs",                   remote.gl().getMaxVertexAttribs());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexUniformComponents",         remote.gl().getMaxVertexUniformComponents());
		log_sandbox.debug("{:46} [{:>10} ]", "UniformBufferOffsetAlignment",       remote.gl().getUniformBufferOffsetAlignment());

	}

	~Sandbox() {
	}

	void update(const std::chrono::duration<float> deltaTime) {
		angle = std::fmod(angle + 5.0f * deltaTime.count(), 360.0f);
		time += deltaTime.count();
	}

	void render() {
		auto queue = remote.queue();

		if (std::fmod(time, 4.f) < 2.f) // NOTE: This multisample only for the 'default' (and might be for blit ms->ss too) frame buffer render
			queue.state.enableMultisample();
		else
			queue.state.disableMultisample();

		// Draw world normal
		queue.framebuffer_draw_default();
		queue.viewport({0, 0}, windowSize);
		render_remote(queue);

		// Draw world lower resolution onto background framebuffer (on the whole screen)
		queue.framebuffer_draw(framebuffer);
		queue.viewport({0, 0}, framebufferSize);
		render_remote(queue);

		// Downsample background multisampled framebuffer to framebuffer2 (on the whole screen)
		queue.blit(framebuffer, framebuffer2,
				{}, framebufferSize,
				{}, framebufferSize,
				libv::gl::BufferBit::Color, libv::gl::MagFilter::Nearest);

		// Render a quad with framebuffer2 over the normal output (on a small window)
		queue.framebuffer_draw_default();
		queue.viewport({0, 0}, windowSize);
		render_remote_quad(queue);

		remote.queue(std::move(queue));
		remote.execute();
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

		gl.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
		gl.clearColor();
		gl.clearDepth();

		gl.projection = libv::mat4f::perspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.f);
		gl.view = libv::mat4f::lookAt({2.f, 2.f, 1.2f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f});
		gl.view.rotate(libv::Degrees{angle}, 0.f, 0.f, 1.f);
		gl.model = libv::mat4f::identity();

		{
			for (std::size_t i = 0; i < 3; ++i) {
				const auto guard_m2 = gl.model.push_guard();
				gl.model.translate(0, 0, 2 * static_cast<float>(i) - 2);

				auto uniforms = sphere_uniforms.block_unique(sphere_layout);
				uniforms[sphere_layout.matMVP] = gl.mvp();
				uniforms[sphere_layout.matM] = gl.model;
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
			stream_uniforms.clear();

			gl.model.push();
			gl.model.translate(-2, 0, 2 * 0.0f - 2);
			auto block0 = stream_uniforms.block_stream(sphere_layout);
			block0[sphere_layout.matMVP] = gl.mvp();
			block0[sphere_layout.matM] = gl.model;
			block0[sphere_layout.color] = libv::vec3f(1.0f, 0.0f, 0.0f);
			gl.model.pop();

			gl.model.push();
			gl.model.translate(-2, 0, 2 * 1.0f - 2);
			auto block1 = stream_uniforms.block_stream(sphere_layout);
			block1[sphere_layout.matMVP] = gl.mvp();
			block1[sphere_layout.matM] = gl.model;
			block1[sphere_layout.color] = libv::vec3f(0.0f, 1.0f, 0.0f);
			gl.model.pop();

			gl.model.push();
			gl.model.translate(-2, 0, 2 * 2.0f - 2);
			auto block2 = stream_uniforms.block_stream(sphere_layout);
			block2[sphere_layout.matMVP] = gl.mvp();
			block2[sphere_layout.matM] = gl.model;
			block2[sphere_layout.color] = libv::vec3f(0.0f, 0.0f, 1.0f);
			gl.model.pop();

			gl.program(stream_program);

			gl.uniform(block0);
			gl.render(stream_mesh);

			gl.uniform(block1);
			gl.render(stream_mesh);

			gl.uniform(block2);
			gl.render(stream_mesh);
		}

		{
			const auto guard_s = gl.state.push_guard();
			gl.state.depthFunctionLEqual();
			gl.program(sky_program);
			gl.uniform(sky_uniform_matMVP, gl.projection * libv::mat4f(libv::mat3f(gl.view)));
			gl.texture(sky_texture0, textureChannel_diffuse);
			gl.render(sky_mesh);
		}
	}

	void render_remote_quad(libv::glr::Queue& gl) {
		const auto guard_s = gl.state.push_guard();

		gl.state.disableDepthTest();

		gl.program(quad_program);
		gl.texture(framebuffer2Color, textureChannel_diffuse);
		gl.render(quad_mesh);
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Sandbox libv.GL3", WINDOW_HEIGHT, WINDOW_WIDTH);
}
