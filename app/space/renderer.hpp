// Project: libv, File: app/space/renderer.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/fwd.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/rev/shader.hpp>
#include <libv/rev/shader_loader.hpp>
#include <libv/ui/component/canvas.hpp>
//#include <libv/glr/layout_to_string.hpp>
// pro
#include <space/universe/universe.hpp>
#include <space/view/camera.hpp>
#include <space/view/render/model.hpp>
#include <space/view/render/shaders.hpp>


// =================================================================================================

// TODO P1: kill these ones, aka implement 'pass' / 'program' uniform block (requires glr shared block allocation)
inline float global_time = 0.0f;
inline int32_t global_test_mode = 0;

// =================================================================================================

namespace app {

// -------------------------------------------------------------------------------------------------

struct RendererResourceContext {
	libv::rev::ShaderLoader shader_loader{"shader/"};
//	libv::rev::ModelLoader model_loader{"model/"};
	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};

	RendererResourceContext() {
		// Include the res/shader/ folder from libv
		shader_loader.add_include_directory("", "../../res/shader/");
//		// Include the res/model/ folder from libv
//		model_loader.add_include_directory("", "../../res/model/");
	}
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
		float animation_offset;
		ArrowStyle style;
		bool start_of_chain;
	};

	float curr_animation_offset = 0.f;
	bool curr_start_of_chain = true;
	std::vector<ArrowData> arrows;
	libv::glr::Mesh mesh{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StreamDraw};
	ShaderCommandArrow shader;

public:
	explicit RendererCommandArrow(RendererResourceContext& rctx);

public:
	void restart_chain(float animation_offset);
	void add_arrow(libv::vec3f source, libv::vec3f target, ArrowStyle style);

public:
	ArrowStyle debug_arrow_style{libv::vec4f(0.48f, 0.65f, 0.70f, 0.5f), libv::vec4f(0.48f, 0.65f, 0.70f, 0.5f)};

	void add_debug_spiral();
	void add_debug_view01();
	void add_debug_view02();
	void add_debug_view03();
	void add_debug_view04();
	void add_debug_view05();

public:
	void rebuild_mesh();
	void render(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream);
};

struct RendererDebug {
	struct Point {
		libv::vec3f a;
		libv::vec4f color;
//		float width;
	};

	struct Line {
		libv::vec3f a;
		libv::vec3f b;
		libv::vec4f color;
	};

	struct Triangle {
		libv::vec3f a;
		libv::vec3f b;
		libv::vec3f c;
		libv::vec4f color;
	};

	struct Quad {
		libv::vec3f a;
		libv::vec3f b;
		libv::vec3f c;
		libv::vec3f d;
		libv::vec4f color;
	};

	struct Frustum {
		std::array<libv::vec3f, 5> points;
		libv::vec4f color_sides;
		libv::vec4f color_wire;
	};

	struct Sphere {
		libv::vec3f center;
		float radius;
		libv::vec4f color;
		int ring_count;
		int segment_count;
	};

public:
	std::vector<Point> points;
	std::vector<Line> lines;
	std::vector<Triangle> triangles;
	std::vector<Quad> quads;
	std::vector<Frustum> frustums;
	std::vector<Sphere> spheres;

private:
	libv::glr::Mesh mesh_point{libv::gl::Primitive::Points, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh mesh_line{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh mesh_triangle{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	ShaderTestMode shader;

public:
	explicit RendererDebug(RendererResourceContext& rctx);

	void build_points_mesh(libv::glr::Mesh& mesh);
	void build_lines_mesh(libv::glr::Mesh& mesh);
	void build_triangles_mesh(libv::glr::Mesh& mesh);
	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream);
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

//	void build_mesh();
	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream);
};

struct RendererFleet {
	Model model;
	ShaderFleet shader;

public:
	explicit RendererFleet(RendererResourceContext& rctx);

	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream, bool selected);
};

//struct RendererText {
//	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
//	ShaderFleet shader;
//
//public:
//	explicit RendererFleet(RendererResourceContext& rctx);
//
//	void build_mesh(libv::glr::Mesh& mesh);
//	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream, bool selected);
//};

// -------------------------------------------------------------------------------------------------

struct Renderer {
	RendererResourceContext resource_context;

	RendererEditorBackground editorBackground{resource_context};
	RendererEditorGrid editorGrid{resource_context};
	RendererGizmo gizmo{resource_context};
	RendererDebug debug{resource_context};
	RendererCommandArrow arrow{resource_context};
	RendererFleet fleet{resource_context};
//	RendererText text{resource_context};

public:
	explicit Renderer(libv::ui::UI& ui);
	void prepare_for_render(libv::glr::Queue& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
