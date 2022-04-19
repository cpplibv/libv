// Project: libv, File: app/space/view/render/renderer.hpp

#pragma once

// fwd
#include <surface/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/fwd.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/rev/resource/shader.hpp>
#include <libv/rev/resource/shader_loader.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/text_layout.hpp>
//#include <libv/glr/layout_to_string.hpp>
// pro
//#include <surface/sim/universe.hpp>
#include <surface/view/camera.hpp>
//#include <surface/view/render/model.hpp>
#include <surface/view/render/shaders.hpp>

#include <surface/surface/chunk.hpp>


// =================================================================================================

// TODO P1: kill these ones, aka implement 'pass' / 'program' uniform block (requires glr shared block allocation)
inline float global_time = 0.0f;
inline int32_t global_test_mode = 0;

// =================================================================================================

namespace surface {

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

	struct Disc {
		libv::vec3f center;
		float radius;
		libv::vec3f normal;
		libv::vec4f color;
	};

	struct Quad {
		libv::vec3f a;
		libv::vec3f b;
		libv::vec3f c;
		libv::vec3f d;
		libv::vec4f color;
	};

//	struct Plane {
//		libv::vec3f a;
//		libv::vec3f b;
//		libv::vec3f c;
//		libv::vec3f d;
//		libv::vec4f color;
//	};

//	struct Box {
//		libv::vec3f center;
//		libv::vec3f size;
//		libv::vec3f direction; //indicates the X+ face
//		libv::vec4f color;
//	};

	struct Frustum {
		std::array<libv::vec3f, 8> points;
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

private:
	std::vector<Point> points;
	std::vector<Line> lines;
	std::vector<Triangle> triangles;
	std::vector<Quad> quads;
	std::vector<Disc> discs;
	std::vector<Frustum> frustums;
	std::vector<Sphere> spheres;

	libv::glr::Mesh mesh_point{libv::gl::Primitive::Points, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh mesh_line{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh mesh_triangle{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	ShaderTestMode shader;
	bool dirty = true;

	void build_points_mesh(libv::glr::Mesh& mesh);
	void build_lines_mesh(libv::glr::Mesh& mesh);
	void build_triangles_mesh(libv::glr::Mesh& mesh);
public:
	explicit RendererDebug(RendererResourceContext& rctx);

	void add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count = 10, int segment_count = 10);
	void clear_spheres();

	//surface/canvas.cpp need these
	void add_debug_point(libv::vec3f a, libv::vec4f color);
	void add_debug_line(libv::vec3f a, libv::vec3f b, libv::vec4f color);
	void add_debug_triangle(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec4f color);
	void add_debug_circle(libv::vec3f center, float radius, libv::vec3f normal, libv::vec4f color);
	void add_debug_frustum(
			libv::vec3f nbl, libv::vec3f nbr, libv::vec3f ntr, libv::vec3f ntl,
			libv::vec3f fbl, libv::vec3f fbr, libv::vec3f ftr, libv::vec3f ftl,
			libv::vec4f color_wire, libv::vec4f color_sides);
	void add_debug_quad(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec4f color);
	void clear_debug_shapes();



	// -------------------------------------------------------------------------------------------------

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
//	void renderTriangles(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

//struct RendererSurface {
//private:
//	libv::glr::Mesh mesh{libv::gl::Primitive::TriangleStrip, libv::gl::BufferUsage::StaticDraw};
//	ShaderSurface shader;
//	libv::glr::VertexIndex vi = 0;
////	bool dirty = true;
//
////	void build_mesh(const std::vector<surface::Chunk>& chunks);
//public:
//	explicit RendererSurface(RendererResourceContext& rctx);
//
//	void addChunk(const surface::Chunk& chunk);
//	void clear();
////	void addFirstChunk(const surface::Chunk& chunk);
//
////	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Chunk& chunk);
//	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
//};

//struct RendererSurfaceTexture {
//	struct ChunkTexture {
//		libv::glr::Texture texture;
//		libv::vec2f pos; //down-left corner
//		libv::vec2f size;
//	};
//private:
//	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
//	ShaderSurface shader;
//	std::vector<ChunkTexture> chunks;
//
//	void build_mesh();
//public:
//	void addTexture(const libv::glr::Texture& texture, const libv::vec2f chunkPos, const libv::vec2f size = {1.f, 1.f});
//	void clear();
////	void addFirstTexture(libv::glr::Texture& texture, const libv::vec2f chunkPos);
//	explicit RendererSurfaceTexture(RendererResourceContext& rctx);
//
//	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
//};

struct RendererGizmo {
	libv::glr::Mesh mesh{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader;

public:
	explicit RendererGizmo(RendererResourceContext& rctx);

	void build_gizmo_lines(libv::glr::Mesh& mesh);
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

struct RendererEditorGrid {
	libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader;

public:
	explicit RendererEditorGrid(RendererResourceContext& rctx);

//	void build_mesh();
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

//struct RendererFleet {
////	Model model;
//	std::optional<Model> model;
//	ShaderFleet shader;
//
//public:
//	explicit RendererFleet(RendererResourceContext& rctx);
//
//	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, Fleet::Selection selection_status = Fleet::Selection::notSelected);
//};

struct RendererText {
private:
	ShaderText shader;

	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::DynamicDraw};

	struct Entry {
		libv::vec3f position;
		libv::vec2f screenOffset;
		libv::vec4f color;
		libv::ui::TextLayout layout;
	};
	std::vector<Entry> texts;

public:
	// TODO P3: Make default font private, and init it in ctor
	std::shared_ptr<libv::ui::Font2D> font;

public:
	explicit RendererText(RendererResourceContext& rctx);

	// TODO P3: A better lifetime/ownership model for rendered 'asset' reuse is necessary (same for debug)
	void add_text(
			libv::vec3f position,
			libv::vec2f screenOffset,
			std::string str,
			libv::vec4f color = libv::vec4f(1, 1, 1, 1),
			libv::ui::FontSize fontSize = libv::ui::FontSize{12},
			libv::ui::AlignHorizontal alignH = libv::ui::AlignHorizontal::center,
			libv::ui::AlignVertical alignV = libv::ui::AlignVertical::center);

	void clear_texts();
	void add_debug_coordinates_if_nothing_else();

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const CameraPlayer::screen_picker& screen_picker);
};

// -------------------------------------------------------------------------------------------------

struct Renderer {
	RendererResourceContext resource_context;

	RendererEditorGrid editorGrid{resource_context};
	RendererGizmo gizmo{resource_context};
	RendererDebug debug{resource_context};
//	RendererFleet fleet{resource_context};
//	RendererSurface surface{resource_context};
//	RendererSurfaceTexture surfaceTexture{resource_context};
	RendererText text{resource_context};

public:
	explicit Renderer(libv::ui::UI& ui);
	void prepare_for_render(libv::glr::Queue& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
