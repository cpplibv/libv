// Created by dbobula on 4/23/2022.

#include <surface/view/render/renderer_veggie.hpp>

#include <libv/glr/queue.hpp>

#include <surface/view/render/renderer.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererVeggie::RendererVeggie(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "flat_color.vs", "flat_color.fs") {
}

//void RendererVeggie::add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count, int segment_count) {
//	const auto& sphere = Sphere(center, radius, color, ring_count, segment_count);
//	vegetationMap.insert_or_assign(index, sphere);
//	dirty = true;
//}

void RendererVeggie::addVeggies(int generation, const libv::vec2i& index, const libv::vec2f& chunkPos, std::vector<VeggieType>& veggies, bool is3D) {
	auto& chunkRenderData = vegetationMap[index];

	if (chunkRenderData.generation == generation)
		return;

	chunkRenderData.pos = chunkPos;
	chunkRenderData.generation = generation;

	std::vector<Sphere> spheres;
	spheres.reserve(veggies.size());
	for (const auto& veggie : veggies) {
		spheres.emplace_back(
				is3D ? veggie.pos : libv::vec3f{xy(veggie.pos), 0.f},
				veggie.size, veggie.color, 6, 6);
	}

	buildMesh(chunkRenderData.mesh, spheres);
}

void RendererVeggie::buildMesh(Mesh& mesh, const std::vector<Sphere>& veggies) {
	mesh.clear();

	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	for (const auto& sphere : veggies) {
		//k0-1 -- k0
		// |    / |
		// |   /  |
		// |  /   |
		//k1-1 -- k1
		const auto ring_step = libv::pi / static_cast<float>(sphere.ring_count);
		const auto segment_step = libv::tau / static_cast<float>(sphere.segment_count);

		for (int i = 0; i <= sphere.ring_count; ++i) {
			const auto fi = static_cast<float>(i);
			const auto u = libv::pi / 2.0f - fi * ring_step;
			const auto z = sphere.radius * std::sin(u);

			for (int j = 0; j <= sphere.segment_count; ++j) {
				const auto fj = static_cast<float>(j);
				const auto v = fj * segment_step;
				const auto x = sphere.radius * std::cos(u) * std::cos(v);
				const auto y = sphere.radius * std::cos(u) * std::sin(v);

				libv::vec3f point = {x, y, z};
				position(point + sphere.center);
				color0(sphere.color);

				const auto k1 = vi;
				const auto k0 = k1 - sphere.segment_count - 1;
				vi += 1;

				if (i > 0 && j > 0) {
					index.triangle(k0 - 1, k1 - 1, k0);
					index.triangle(k0 - 1, k0, k1 - 1);
					index.triangle(k0, k1 - 1, k1);
					index.triangle(k0, k1, k1 - 1);
				}
			}
		}
	}
}

void RendererVeggie::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const std::vector<libv::vec2i>& visibleChunks) {
	glr.program(shader.program());

	glr.uniform(shader.uniform().fogEnabled, fogEnabled);
	glr.uniform(shader.uniform().fogIntensity, fogIntensity);
	glr.uniform(shader.uniform().fogColor, fogColor);

	const auto& eye = glr.eye();
	for (const auto& index : visibleChunks) {
		const auto& it = vegetationMap.find(index);
		if(it == vegetationMap.end())
			//visible chunk not ready to be rendered
			continue;

		if ((libv::vec3f(it->second.pos, 0.f) - eye).length() < 10.f) {
			const auto& mesh = it->second.mesh;
			auto uniforms = uniform_stream.block_unique(layout_matrices);
			uniforms[layout_matrices.matMVP] = glr.mvp();
			uniforms[layout_matrices.matM] = glr.model;
			uniforms[layout_matrices.matP] = glr.projection;
			uniforms[layout_matrices.eye] = glr.eye();
			glr.uniform(std::move(uniforms));
			glr.render(mesh);
		}
	}
}

void RendererVeggie::clear() {
	vegetationMap.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
