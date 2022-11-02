// Project: libv.glr, File: sandbox/libv_glr_main2.cpp

// hpp
#include "libv_gl_runner.hpp"
// libv
#include <libv/gl/gl.hpp>
#include <libv/math/angle.hpp>
//#include <libv/utility/read_file.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/procedural/plane.hpp>
#include <libv/glr/procedural/progress_segmented_ring.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>


// -------------------------------------------------------------------------------------------------

//constexpr uint32_t WINDOW_WIDTH = 1280;
//constexpr uint32_t WINDOW_HEIGHT = 800;

constexpr uint32_t WINDOW_WIDTH = 1024;
constexpr uint32_t WINDOW_HEIGHT = 1024;

const auto shader_plane_vs = R"(
#version 450 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 8) in vec2 vertexTexture0;

out vec2 fragmentTexture0;

uniform mat4 matMVP;

void main() {
	fragmentTexture0 = vertexTexture0;
	gl_Position = matMVP * vec4(vertexPosition, 1);
}
)";

const auto shader_plane_fs = R"(
#version 450 core

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0Sampler;

void main() {
	result = texture(texture0Sampler, fragmentTexture0).rgba;
}
)";

const auto shader_gen_cs = R"(
#version 450 core

layout(local_size_x = 8, local_size_y = 8) in;

layout(rgba32f, binding = 0) uniform image2D target;

uniform float time;

//
//  Simplex Perlin Noise 2D
//  Return value range of -1.0 .. 1.0
//
float SimplexPerlin2D(vec2 P) {
    //  https://github.com/BrianSharpe/Wombat/blob/master/SimplexPerlin2D.glsl

    //  simplex math constants
    const float SKEWFACTOR = 0.36602540378443864676372317075294;            // 0.5*(sqrt(3.0)-1.0)
    const float UNSKEWFACTOR = 0.21132486540518711774542560974902;          // (3.0-sqrt(3.0))/6.0
    const float SIMPLEX_TRI_HEIGHT = 0.70710678118654752440084436210485;    // sqrt( 0.5 )	height of simplex triangle
    const vec3 SIMPLEX_POINTS = vec3( 1.0-UNSKEWFACTOR, -UNSKEWFACTOR, 1.0-2.0*UNSKEWFACTOR );  //  simplex triangle geo

    //  establish our grid cell.
    P *= SIMPLEX_TRI_HEIGHT;    // scale space so we can have an approx feature size of 1.0
    vec2 Pi = floor( P + dot( P, vec2( SKEWFACTOR ) ) );

    // calculate the hash
    vec4 Pt = vec4( Pi.xy, Pi.xy + 1.0 );
    Pt = Pt - floor(Pt * ( 1.0 / 71.0 )) * 71.0;
    Pt += vec2( 26.0, 161.0 ).xyxy;
    Pt *= Pt;
    Pt = Pt.xzxz * Pt.yyww;
    vec4 hash_x = fract( Pt * ( 1.0 / 951.135664 ) );
    vec4 hash_y = fract( Pt * ( 1.0 / 642.949883 ) );

    //  establish vectors to the 3 corners of our simplex triangle
    vec2 v0 = Pi - dot( Pi, vec2( UNSKEWFACTOR ) ) - P;
    vec4 v1pos_v1hash = (v0.x < v0.y) ? vec4(SIMPLEX_POINTS.xy, hash_x.y, hash_y.y) : vec4(SIMPLEX_POINTS.yx, hash_x.z, hash_y.z);
    vec4 v12 = vec4( v1pos_v1hash.xy, SIMPLEX_POINTS.zz ) + v0.xyxy;

    //  calculate the dotproduct of our 3 corner vectors with 3 random normalized vectors
    vec3 grad_x = vec3( hash_x.x, v1pos_v1hash.z, hash_x.w ) - 0.49999;
    vec3 grad_y = vec3( hash_y.x, v1pos_v1hash.w, hash_y.w ) - 0.49999;
    vec3 grad_results = inversesqrt( grad_x * grad_x + grad_y * grad_y ) * ( grad_x * vec3( v0.x, v12.xz ) + grad_y * vec3( v0.y, v12.yw ) );

    //	Normalization factor to scale the final result to a strict 1.0->-1.0 range
    //	http://briansharpe.wordpress.com/2012/01/13/simplex-noise/#comment-36
    const float FINAL_NORMALIZATION = 99.204334582718712976990005025589;

    //	evaluate and return
    vec3 m = vec3( v0.x, v12.xz ) * vec3( v0.x, v12.xz ) + vec3( v0.y, v12.yw ) * vec3( v0.y, v12.yw );
    m = max(0.5 - m, 0.0);
    m = m*m;
    return dot(m*m, grad_results) * FINAL_NORMALIZATION;
}

//
//  Simplex Perlin Noise 3D
//  Return value range of -1.0 .. 1.0
//
float SimplexPerlin3D(vec3 P) {
    //  https://github.com/BrianSharpe/Wombat/blob/master/SimplexPerlin3D.glsl

    //  simplex math constants
    const float SKEWFACTOR = 1.0/3.0;
    const float UNSKEWFACTOR = 1.0/6.0;
    const float SIMPLEX_CORNER_POS = 0.5;
    const float SIMPLEX_TETRAHEDRON_HEIGHT = 0.70710678118654752440084436210485;    // sqrt( 0.5 )

    //  establish our grid cell.
    P *= SIMPLEX_TETRAHEDRON_HEIGHT;    // scale space so we can have an approx feature size of 1.0
    vec3 Pi = floor( P + dot( P, vec3( SKEWFACTOR) ) );

    //  Find the vectors to the corners of our simplex tetrahedron
    vec3 x0 = P - Pi + dot(Pi, vec3( UNSKEWFACTOR ) );
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 Pi_1 = min( g.xyz, l.zxy );
    vec3 Pi_2 = max( g.xyz, l.zxy );
    vec3 x1 = x0 - Pi_1 + UNSKEWFACTOR;
    vec3 x2 = x0 - Pi_2 + SKEWFACTOR;
    vec3 x3 = x0 - SIMPLEX_CORNER_POS;

    //  pack them into a parallel-friendly arrangement
    vec4 v1234_x = vec4( x0.x, x1.x, x2.x, x3.x );
    vec4 v1234_y = vec4( x0.y, x1.y, x2.y, x3.y );
    vec4 v1234_z = vec4( x0.z, x1.z, x2.z, x3.z );

    // clamp the domain of our grid cell
    Pi.xyz = Pi.xyz - floor(Pi.xyz * ( 1.0 / 69.0 )) * 69.0;
    vec3 Pi_inc1 = step( Pi, vec3( 69.0 - 1.5 ) ) * ( Pi + 1.0 );

    //	generate the random vectors
    vec4 Pt = vec4( Pi.xy, Pi_inc1.xy ) + vec2( 50.0, 161.0 ).xyxy;
    Pt *= Pt;
    vec4 V1xy_V2xy = mix( Pt.xyxy, Pt.zwzw, vec4( Pi_1.xy, Pi_2.xy ) );
    Pt = vec4( Pt.x, V1xy_V2xy.xz, Pt.z ) * vec4( Pt.y, V1xy_V2xy.yw, Pt.w );
    const vec3 SOMELARGEFLOATS = vec3( 635.298681, 682.357502, 668.926525 );
    const vec3 ZINC = vec3( 48.500388, 65.294118, 63.934599 );
    vec3 lowz_mods = vec3( 1.0 / ( SOMELARGEFLOATS.xyz + Pi.zzz * ZINC.xyz ) );
    vec3 highz_mods = vec3( 1.0 / ( SOMELARGEFLOATS.xyz + Pi_inc1.zzz * ZINC.xyz ) );
    Pi_1 = ( Pi_1.z < 0.5 ) ? lowz_mods : highz_mods;
    Pi_2 = ( Pi_2.z < 0.5 ) ? lowz_mods : highz_mods;
    vec4 hash_0 = fract( Pt * vec4( lowz_mods.x, Pi_1.x, Pi_2.x, highz_mods.x ) ) - 0.49999;
    vec4 hash_1 = fract( Pt * vec4( lowz_mods.y, Pi_1.y, Pi_2.y, highz_mods.y ) ) - 0.49999;
    vec4 hash_2 = fract( Pt * vec4( lowz_mods.z, Pi_1.z, Pi_2.z, highz_mods.z ) ) - 0.49999;

    //	evaluate gradients
    vec4 grad_results = inversesqrt( hash_0 * hash_0 + hash_1 * hash_1 + hash_2 * hash_2 ) * ( hash_0 * v1234_x + hash_1 * v1234_y + hash_2 * v1234_z );

    //	Normalization factor to scale the final result to a strict 1.0->-1.0 range
    //	http://briansharpe.wordpress.com/2012/01/13/simplex-noise/#comment-36
    const float FINAL_NORMALIZATION = 37.837227241611314102871574478976;

    //  evaulate the kernel weights ( use (0.5-x*x)^3 instead of (0.6-x*x)^4 to fix discontinuities )
    vec4 kernel_weights = v1234_x * v1234_x + v1234_y * v1234_y + v1234_z * v1234_z;
    kernel_weights = max(0.5 - kernel_weights, 0.0);
    kernel_weights = kernel_weights*kernel_weights*kernel_weights;

    //	sum with the kernel and return
    return dot( kernel_weights, grad_results ) * FINAL_NORMALIZATION;
}

//float noise_fractal(Seed seed, vec2 xy,
float noise_fractal_simplex_3d(
//		vec2 coord,
		vec3 coord,
		int octaves,
		float amplitude = 1.0,
		float frequency = 1.0,
		float lacunarity = 2.0,
		float persistence = 0.5) {

	float sum = 0;

	for (int i = 0; i < octaves; ++i) {
		float noise = SimplexPerlin3D(coord * frequency);
		sum += noise * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
		coord += vec3(3.1415, -2.453263, +15.4532341);
//		coord <<= reseed();
//		coord *= vec3(0.60101008, -0.453263, 0.899101);
//		coord += vec3(87.154325, 81.233311, -19.7761918);
	}

	return sum;
}

vec2 seed(vec2 pos, int index) {
	const mat2 seeders_rotate[1] = {
		mat2(1, 0, 0, 1),
	};
	const vec2 seeders_offset[1] = {
		vec2(0, 0),
	};

//	float a = rand(0, tau);
//	float s = sin(a);
//	float c = cos(a);
//	mat2 m = mat2(c, s, -s, c);

	return seeders_rotate[index] * pos + seeders_offset[index];
}

void main() {
	ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 coordsSize = ivec2(gl_NumWorkGroups.xy * gl_WorkGroupSize.xy);
	vec2 uv = vec2(coords) / vec2(coordsSize);

//	vec4 result = vec4(mod(uv + vec2(-time * 0.5, 0.0), 1.0), sin(time * 0.5), 1.0);
//	vec4 result = vec4(SimplexPerlin2D(mod(uv + vec2(-time * 0.5, 0.0), 1.0)), 0, sin(time * 0.5), 1.0);
//	vec4 result = vec4(SimplexPerlin3D(vec3(uv * vec2(15.0, 5.0), time)), 0, 0, 1.0);

//	float value = SimplexPerlin3D(vec3(uv * vec2(10.0, 10.0), time * 0.25)) * 0.5 + 0.5;

	vec3 pos = vec3(uv * vec2(10.0, 10.0), time * 0.25);

	pos.xy = seed(pos.xy, 0);

	float value = noise_fractal_simplex_3d(pos, 6) * 0.5 + 0.5;

	vec4 result = vec4(value, value, value, 1.0);

	imageStore(target, coords, result);
}
)";

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
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
const auto plane_layout = libv::glr::layout_std140<SphereUniformLayout>(uniformBlock_sphere);

// -------------------------------------------------------------------------------------------------

struct Sandbox {
	libv::glr::Remote remote; // Remote has to be the first data member to cleanup gl resources

	libv::glr::Mesh plane_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	libv::glr::Texture2D::RGBA32F gen_texture;

	libv::glr::Program gen_program;
	libv::glr::Uniform_float gen_uniform_time;
	libv::glr::Uniform_texture gen_uniform_texture;

	libv::glr::Program plane_program;
	libv::glr::Uniform_mat4f plane_uniform_matMVP;
	libv::glr::Uniform_texture plane_uniform_texture;

	float time = 0;

	Sandbox() {
		// Plane
		plane_program.vertex(shader_plane_vs);
		plane_program.fragment(shader_plane_fs);
		plane_program.assign(plane_uniform_matMVP, "matMVP");
		plane_program.assign(plane_uniform_texture, "texture0Sampler", textureChannel_diffuse);

		gen_program.compute(shader_gen_cs);
		gen_program.assign(gen_uniform_time, "time");
		gen_program.assign(gen_uniform_texture, "target", textureChannel_diffuse);

		gen_texture.storage(1, {1024, 1024});
		gen_texture.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);

		{
			plane_mesh.clear();

			auto position = plane_mesh.attribute(attribute_position);
			auto texture0 = plane_mesh.attribute(attribute_texture0);
			auto index = plane_mesh.index();

//			libv::glr::ProgressSegmentedRingStyle style;
//
//			style.segment_num = 16;
//			style.resolution = 4;
//			style.radius_inner = 100;
//			style.radius_outer = 500;
//			style.up = libv::to_rad(0.f);
////			style.CW = false;
////			style.fill_mode = libv::glr::ProgressSegmentedRingStyle::FillMode::floor;
//
//			libv::glr::generateProgressSegmentedRing(style, position, texture0, index);

			libv::glr::GeneratePlaneXY<float>(libv::vec2f{1024, 1024})(position, texture0, index);
		}

		remote.create();
		remote.enableDebug();
	}

	~Sandbox() {
	}

	void update(const std::chrono::duration<float> deltaTime) {
		time += deltaTime.count();
	}

	void render() {
		auto queue = remote.queue();

		render_remote(queue);

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

		{
			gl.program(gen_program);
			gl.bindImageTexture(0, gen_texture, 0, libv::gl::BufferAccessFull::Write);
			gl.uniform(gen_uniform_time, time);
			gl.dispatchCompute(1024 / 8, 1024 / 8);
			gl.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);
		}

		gl.viewport({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT});

		gl.projection = libv::mat4f::ortho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
		gl.view = libv::mat4f::identity();
		gl.model = libv::mat4f::identity();

		{
			const auto guard_m = gl.model.push_guard();
			gl.model.translate(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 0);

			gl.program(plane_program);
			gl.uniform(plane_uniform_matMVP, gl.mvp());
			gl.texture(gen_texture, textureChannel_diffuse);
			gl.render(plane_mesh);
		}
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Sandbox libv.GLR3 - Compute Shader", WINDOW_HEIGHT, WINDOW_WIDTH);
}
