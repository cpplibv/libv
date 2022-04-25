// Created by dbobula on 4/23/2022.

#pragma once

#include <libv/math/vec.hpp>

#include <array>
#include <vector>
#include <surface/view/render/renderer.hpp>


namespace surface {


class RendererVeggie {

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

	Mesh mesh_point{libv::gl::Primitive::Points, libv::gl::BufferUsage::StaticDraw};
	Mesh mesh_line{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	Mesh mesh_triangle{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	ShaderTestMode shader;
	bool dirty = true;

	void build_points_mesh(Mesh& mesh);
	void build_lines_mesh(Mesh& mesh);
	void build_triangles_mesh(Mesh& mesh);
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

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};


// -------------------------------------------------------------------------------------------------

} // namespace surface
