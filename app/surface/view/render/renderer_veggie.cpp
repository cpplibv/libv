// Created by dbobula on 4/23/2022.

#include <surface/view/render/renderer_veggie.hpp>

#include <libv/glr/queue.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererDebug::RendererDebug(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "flat_color.vs", "flat_color.fs") {
}


//void RendererDebug::add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count, int segment_count) {
//	const auto& sphere = Sphere(center, radius, color, ring_count, segment_count);
//	vegetationMap.insert_or_assign(index, sphere);
//	dirty = true;
//}

void RendererDebug::addVeggies(const libv::vec2i index, std::vector<VeggieType>& veggies, bool is3D) {
	ChunkVegetation chunkVegetation;
	for (const auto& veggie : veggies) {
		chunkVegetation.veggies.emplace_back(
				is3D ? veggie.pos : libv::vec3f{xy(veggie.pos), 0.f},
				veggie.size, veggie.color, 6, 6);
	}

	const auto& pair = vegetationMap.emplace(index, chunkVegetation);
	auto& mesh = pair.first->second.mesh;
	if (!pair.second)
		mesh.clear();

	build_mesh(chunkVegetation.veggies, mesh);
}

void RendererDebug::build_mesh(const std::vector<Sphere>& veggies, Mesh& mesh) {
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

void RendererDebug::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	// !!! P3: Rebuild is too expensive on each frame

	glr.program(shader.program());
	for (const auto&[_, chunkVeggie] : vegetationMap) {
		const auto& mesh = chunkVeggie.mesh;
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh);
	}
}

//void RendererDebug::clear_spheres() {
//	spheres.clear();
//	dirty = true;
//}

// -------------------------------------------------------------------------------------------------

} // namespace surface
