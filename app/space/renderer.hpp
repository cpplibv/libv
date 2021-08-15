// Project: libv, File: app/space/render.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue_fwd.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/rev/shader.hpp>
#include <libv/rev/shader_loader.hpp>
#include <libv/ui/component/canvas.hpp>
//#include <libv/glr/layout_to_string.hpp>
// pro
#include <space/camera.hpp>


// =================================================================================================

// TODO P1: kill these ones, aka implement 'pass' / 'program' uniform block (requires glr shared block allocation)
inline float global_time = 0.0f;
inline int32_t global_test_mode = 0;

// =================================================================================================

namespace app {

// -------------------------------------------------------------------------------------------------

constexpr auto attribute_position = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0 = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0 = libv::glr::Attribute<8, libv::vec2f>{};
constexpr auto attribute_custom0 = libv::glr::Attribute<15, libv::vec4f>{};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal = libv::gl::TextureChannel{1};
constexpr auto textureChannel_pattern = libv::gl::TextureChannel{7};

struct UniformLayoutMatrices {
	// Could be split into two: Camera and Model
	// 			Camera: matV, matP, near, far, ...
	// 			Model: matM, matMVP

	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f eye;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matP);
	LIBV_REFLECTION_ACCESS(matM);
	LIBV_REFLECTION_ACCESS(eye);
};

//struct UniformLayoutPass {
//	libv::glr::Uniform_float time_sim;
//	libv::glr::Uniform_float time_real;
//	libv::glr::Uniform_float time_real_hcap;
//	libv::glr::Uniform_vec2f render_resolution;
//	libv::glr::Uniform_vec2f pixel_size; // 1.0 / render_resolution
//
//	libv::glr::Uniform_int32 test_mode0;
//
//    libv::glr::Uniform_vec4f frame_random_vec4; // [0..1] random numbers changing each frame
//    libv::glr::Uniform_vec4i frame_random_ivec4; // [int_min..int_max] random numbers changing each frame
//
//	LIBV_REFLECTION_ACCESS(time_sim);
//	LIBV_REFLECTION_ACCESS(time_real);
//	LIBV_REFLECTION_ACCESS(time_real_hcap);
//	LIBV_REFLECTION_ACCESS(render_resolution);
//	LIBV_REFLECTION_ACCESS(pixel_size);
//
//	LIBV_REFLECTION_ACCESS(test_mode0);
//
//	LIBV_REFLECTION_ACCESS(frame_random_vec4);
//	LIBV_REFLECTION_ACCESS(frame_random_ivec4);
//};

const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{0, "Matrices"};
//const auto uniformBlock_pass = libv::glr::UniformBlockBinding{1, "Pass"};
const auto layout_matrices = libv::glr::layout_std140<UniformLayoutMatrices>(uniformBlock_matrices);
//const auto layout_pass = libv::glr::layout_std140<UniformLayoutPass>(uniformBlock_pass);

// -------------------------------------------------------------------------------------------------

// TODO P2: libv.glr: Shader automated block binding by watching the includes
//          So this should work:
//          shader_loader.register_block<UniformLayoutMatrices>("block/sphere.glsl");
//          Note that this also could generate the file block/sphere.glsl (OR just be an in memory resource)
//          And this would be called on any program that includes block/sphere.glsl
//          program.block_binding(uniformBlock_matrices) and access_blocks(Access&) will be replaced by this system
//          | issue: some struct might have already been defined, so block to string might have to skip them
//                  This mean tracking of structs OR pushing the problem back to the include system pragma once solution
//                  With additional mapping and includes to struct/my_struct_that_is_in_a_block.glsl
//			std::cout << libv::glr::layout_to_string<UniformLayoutMatrices>("Sphere") << std::endl;

struct UniformsEditorBackground {
	libv::glr::Uniform_texture texture_noise;
	libv::glr::Uniform_vec2f render_resolution;
	libv::glr::Uniform_vec4f noise_scale;
	libv::glr::Uniform_vec4f base_color;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture_noise, "texture_noise", textureChannel_pattern);
		access(render_resolution, "render_resolution");
		access(base_color, "base_color");
		access(noise_scale, "noise_scale");
	}

	template <typename Access> void access_blocks(Access&) {
	}
};

struct UniformsTestMode {
	libv::glr::Uniform_int32 test_mode;

	template <typename Access> void access_uniforms(Access& access) {
		access(test_mode, "test_mode", 0);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsColor {
	libv::glr::Uniform_vec4f base_color;

	template <typename Access> void access_uniforms(Access& access) {
		access(base_color, "base_color");
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsCommandArrow {
	libv::glr::Uniform_int32 test_mode;
	libv::glr::Uniform_float time;
	libv::glr::Uniform_vec2f render_resolution;
	libv::glr::Uniform_vec4f color;

	template <typename Access> void access_uniforms(Access& access) {
		access(color, "color");
		access(render_resolution, "render_resolution");
		access(test_mode, "test_mode", 0);
		access(time, "time", 0.f);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderEditorBackground = libv::rev::Shader<UniformsEditorBackground>;
using ShaderCommandArrow = libv::rev::Shader<UniformsCommandArrow>;
using ShaderColor = libv::rev::Shader<UniformsColor>;
using ShaderTestMode = libv::rev::Shader<UniformsTestMode>;
//using Shader = libv::rev::Shader<>;

// =================================================================================================

struct RendererResourceContext {
	libv::rev::ShaderLoader shader_manager{"shader/"};
	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};
};

// -------------------------------------------------------------------------------------------------

struct RendererEditorBackground {
	libv::glr::Mesh mesh_background{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	ShaderEditorBackground shader;
	libv::glr::Texture2D::R8_G8_B8 background_texture_pattern;

	static constexpr libv::vec2i noise_size = {128, 128};

public:
	explicit RendererEditorBackground(RendererResourceContext& rctx);
	void render(libv::glr::Queue& gl, libv::vec2f canvas_size);
};

struct RendererCommandArrow {
	struct ArrowStyle {
		libv::vec4f color_source;
		libv::vec4f color_target;
//		float size;
//		Pattern pattern;
	};
	struct ArrowData {
		libv::vec3f source;
		libv::vec3f target;
		ArrowStyle style;
	};

	std::vector<ArrowData> arrows;
	libv::glr::Mesh mesh{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StreamDraw};
	ShaderCommandArrow shader;

public:
	explicit RendererCommandArrow(RendererResourceContext& rctx);

public:
	void add_arrow(libv::vec3f source, libv::vec3f target, ArrowStyle style);

public:
	void rebuild_mesh();
	void render(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream);
};

struct RendererGizmo {
	libv::glr::Mesh mesh{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader;

public:
	explicit RendererGizmo(RendererResourceContext& rctx);

	void build_gizmo_lines(libv::glr::Mesh& mesh);
	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream);
};

struct RendererEditorGrid {
	libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader;

public:
	explicit RendererEditorGrid(RendererResourceContext& rctx);

	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream);
};

struct RendererFleet {
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderColor shader;

public:
	explicit RendererFleet(RendererResourceContext& rctx);

	void build_mesh(libv::glr::Mesh& mesh);
	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream);
};

// -------------------------------------------------------------------------------------------------

struct Renderer {
	RendererResourceContext resource_context;

	RendererEditorBackground editorBackground{resource_context};
	RendererEditorGrid editorGrid{resource_context};
	RendererGizmo gizmo{resource_context};
	RendererCommandArrow arrow{resource_context};
	RendererFleet fleet{resource_context};

public:
	explicit Renderer(libv::ui::UI& ui);
	void prepare_for_render(libv::glr::Queue& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
