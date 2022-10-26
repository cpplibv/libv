// Project: libv, File: app/surface/view/render/renderer_veggie.hpp

#pragma once

#include <libv/math/vec.hpp>
#include <libv/math/frustum.hpp>
#include <libv/glr/mesh.hpp>

#include <array>
#include <vector>
//#include <surface/view/render/renderer.hpp>
#include <surface/view/vec_hash.hpp>
#include <surface/view/render/shaders.hpp>
#include <surface/surface/biome.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

class RendererResourceContext;
class VeggieType;

// -------------------------------------------------------------------------------------------------

struct UniformsVeggie {
	libv::glr::Uniform_bool fogEnabled;
	libv::glr::Uniform_float fogIntensity;
	libv::glr::Uniform_vec4f fogColor;
//	libv::glr::Uniform_vec3f sunColor;
//	libv::glr::Uniform_vec3f sunDirection;

	template <typename Access> void access_uniforms(Access& access) {
		access(fogEnabled, "fogEnabled", true);
		access(fogIntensity, "fogIntensity", 0.05f);
		access(fogColor, "fogColor", libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f});
//		access(sunColor, "sunColor", libv::vec3f{1.0f, 0.9f, 0.8f});
//		access(sunDirection, "sunDirection", libv::vec3f{0.784464f, 0.196116f, 0.588348f});
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderVeggie = libv::rev::Shader<UniformsVeggie>;

// -------------------------------------------------------------------------------------------------

struct RendererVeggie {

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
		int generation = -1;
//		libv::vec2f pos;
		libv::vec3f chunkPos;
		libv::vec2f chunkSize;
		libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	};
	bool is3D = true;

private:
	std::unordered_map<libv::vec2i, ChunkVegetation, VecHash> vegetationMap;
	ShaderVeggie shader;

public:
	bool fogEnabled = true;
	float fogIntensity = 0.05f;
	libv::vec4f fogColor = libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f};
//	libv::vec3f sunColor = libv::vec3f{1.0f, 0.9f, 0.8f};
//	libv::vec3f sunDirection = libv::vec3f{0.784464f, 0.196116f, 0.588348f};

private:
	void buildMesh(libv::glr::Mesh& mesh, const std::vector<Sphere>& veggies);
public:
	explicit RendererVeggie(RendererResourceContext& rctx);

//	void add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count = 10, int segment_count = 10);

	void addVeggies(int generation, const libv::vec2i& index, const libv::vec2f& chunkPos, const libv::vec2f& chunkSize,
			std::vector<Veggie>& veggies, bool is3D_);

	void clear();

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::frustum& frustum);
};


// -------------------------------------------------------------------------------------------------

} // namespace surface
