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

	struct Sphere {
		libv::vec3f center;
		float radius;
		libv::vec4f color;
		int ring_count;
		int segment_count;

		Sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count, int segment_count) :
		center(center), radius(radius), color(color), ring_count(ring_count), segment_count(segment_count) {}
	};

	struct ChunkVegetation {
		libv::vec2f pos;
		std::vector<Sphere> veggies;
		Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	};

private:
	std::unordered_map<libv::vec2i, ChunkVegetation, VecHash> vegetationMap;

//	Mesh mesh_triangle{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	ShaderTestMode shader;
	bool dirty = true;

	Mesh build_mesh(const std::vector<Sphere>& veggies);
public:
	explicit RendererDebug(RendererResourceContext& rctx);

//	void add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count = 10, int segment_count = 10);

	void addVeggies(const libv::vec2i& index, const libv::vec2f& chunkPos,
			std::vector<VeggieType>& veggies, bool is3D);
//	void clear_spheres();

	//surface/canvas.cpp need these
//	void clear_debug_shapes();

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};


// -------------------------------------------------------------------------------------------------

} // namespace surface
