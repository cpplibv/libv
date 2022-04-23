// Created by dbobula on 4/23/2022.

#include <surface/view/render/renderer_veggie.hpp>

#include <libv/glr/queue.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererDebug::RendererDebug(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "flat_color.vs", "flat_color.fs") {
}

void RendererDebug::build_points_mesh(libv::glr::Mesh& mesh) {
	mesh.clear();
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	for (const auto& point : points) {
		position(point.a);

		color0(point.color);

		index.point(vi);
		vi += 1;
	}
}

void RendererDebug::build_lines_mesh(libv::glr::Mesh& mesh) {
	mesh.clear();
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	for (const auto& line : lines) {
		position(line.a);
		position(line.b);

		color0(line.color);
		color0(line.color);

		index.line(vi + 0, vi + 1);
		vi += 2;
	}

//	for (const auto& sphere : spheres) {
//		//k0-1 -- k0
//		// |    / |
//		// |   /  |
//		// |  /   |
//		//k1-1 -- k1
//		const auto ring_step = libv::pi / static_cast<float>(sphere.ring_count);
//		const auto segment_step = libv::tau / static_cast<float>(sphere.segment_count);
//
//		for (int i = 0; i <= sphere.ring_count; ++i) {
//			const auto fi = static_cast<float>(i);
//			const auto u = libv::pi / 2.0f - fi * ring_step;
//			const auto z = sphere.radius * std::sin(u);
//
//			for (int j = 0; j <= sphere.segment_count; ++j) {
//				const auto fj = static_cast<float>(j);
//				const auto v = fj * segment_step;
//				const auto x = sphere.radius * std::cos(u) * std::cos(v);
//				const auto y = sphere.radius * std::cos(u) * std::sin(v);
//
//				libv::vec3f point = {x, y, z};
//				position(point + sphere.center);
//				color0({0.6f, 0, 1, 1});
//
//				const auto k1 = vi;
//				const auto k0 = k1 - sphere.segment_count - 1;
//				vi += 1;
//
//				if (i > 0 && j > 0) {
//					index.line(k0, k1);
//					index.line(k0 - 1, k0);
//				}
//			}
//		}
//	}

	//  7-------------6
	//  | \         / |
	//  |   \     /   |
	//  |    3---2    |
	//  |    |   |    |
	//  |    0---1    |
	//  |   /     \   |
	//  | /         \ |
	//  4-------------5
	for (const auto& frustum : frustums) {
		for (const auto& point : frustum.points) {
			position(point);
			color0(frustum.color_wire);
		}

		// Front-face
		index.line(vi + 0, vi + 1);
		index.line(vi + 1, vi + 2);
		index.line(vi + 2, vi + 3);
		index.line(vi + 3, vi + 0);

		index.line(vi + 4, vi + 5);
		index.line(vi + 5, vi + 6);
		index.line(vi + 6, vi + 7);
		index.line(vi + 7, vi + 4);

		index.line(vi + 0, vi + 4);
		index.line(vi + 1, vi + 5);
		index.line(vi + 2, vi + 6);
		index.line(vi + 3, vi + 7);

		vi += 8;
	}
}

void RendererDebug::build_triangles_mesh(libv::glr::Mesh& mesh) {
	mesh.clear();
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	for (const auto& triangle : triangles) {
		position(triangle.a);
		position(triangle.b);
		position(triangle.c);

		color0(triangle.color);
		color0(triangle.color);
		color0(triangle.color);

		index.triangle(vi + 0, vi + 1, vi + 2);
		index.triangle(vi + 0, vi + 2, vi + 1);
		vi += 3;
	}

	//  7-------------6
	//  | \         / |
	//  |   \     /   |
	//  |    3---2    |
	//  |    |   |    |
	//  |    0---1    |
	//  |   /     \   |
	//  | /         \ |
	//  4-------------5
	for (const auto& frustum : frustums) {
		for (const auto& point : frustum.points) {
			position(point);
			color0(frustum.color_sides);
		}

		// Front-face
		index.quad(vi + 0, vi + 1, vi + 2, vi + 3);
		index.quad(vi + 4, vi + 5, vi + 6, vi + 7);
		index.quad(vi + 4, vi + 5, vi + 1, vi + 0);
		index.quad(vi + 5, vi + 6, vi + 2, vi + 1);
		index.quad(vi + 6, vi + 7, vi + 3, vi + 2);
		index.quad(vi + 7, vi + 4, vi + 0, vi + 3);

		// Back-face
		index.quad(vi + 3, vi + 2, vi + 1, vi + 0);
		index.quad(vi + 7, vi + 6, vi + 5, vi + 4);
		index.quad(vi + 0, vi + 1, vi + 5, vi + 4);
		index.quad(vi + 1, vi + 2, vi + 6, vi + 5);
		index.quad(vi + 2, vi + 3, vi + 7, vi + 6);
		index.quad(vi + 3, vi + 0, vi + 4, vi + 7);
		vi += 8;
	}

	for (const auto& quad : quads) {
		position(quad.a);
		position(quad.b);
		position(quad.c);
		position(quad.d);

		color0(quad.color);
		color0(quad.color);
		color0(quad.color);
		color0(quad.color);

		index.quad(vi, vi + 1, vi + 2, vi + 3);
		index.quad(vi, vi + 3, vi + 2, vi + 1);
		vi += 4;
	}

	for (const auto& disc : discs) {
		const auto centeri = vi;
		position(disc.center);
		color0(disc.color);
		vi += 1;

		static constexpr auto count = 64;
		const auto up = libv::vec3f{0, 0, 1};
		const auto dot_prod = dot(disc.normal, up);
		const auto rotation_mat = dot_prod < 0.9999f ?
				libv::mat4f::identity().rotate(std::acos(dot_prod), cross(disc.normal, up)) :
				libv::mat4f::identity();
		for (int i = 0; i <= count; ++i) {
			float a = libv::tau / count * static_cast<float>(i);
			const auto x = disc.radius * std::cos(a);
			const auto y = disc.radius * std::sin(a);
			libv::vec3f point = {x, y, 0};
			auto rotated_point = rotation_mat * libv::vec4f{point, 1};
			position(xyz(rotated_point) + disc.center);
			color0(disc.color);

			if (i > 0) {
				index.triangle(vi - 1, centeri, vi);
				index.triangle(vi, centeri, vi - 1);
			}
			vi += 1;
		}
	}

	for (const auto& sphere : spheres) {
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

	if (dirty) {
		build_points_mesh(mesh_point);
		build_lines_mesh(mesh_line);
		build_triangles_mesh(mesh_triangle);
		dirty = false;
	}

	glr.program(shader.program());

	{
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh_triangle);
	}
	{
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh_line);
	}
	{
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh_point);
	}
}

void RendererDebug::add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count, int segment_count) {
	spheres.emplace_back(center, radius, color, ring_count, segment_count);
	dirty = true;
}

void RendererDebug::clear_spheres() {
	spheres.clear();
	dirty = true;
}

void RendererDebug::add_debug_point(libv::vec3f a, libv::vec4f color) {
	points.emplace_back(a, color);
	dirty = true;
}

void RendererDebug::add_debug_line(libv::vec3f a, libv::vec3f b, libv::vec4f color) {
	lines.emplace_back(a, b, color);
	dirty = true;
}

void RendererDebug::add_debug_triangle(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec4f color) {
	triangles.emplace_back(a, b, c, color);
	dirty = true;
}

void RendererDebug::add_debug_circle(libv::vec3f center, float radius, libv::vec3f normal, libv::vec4f color) {
	discs.emplace_back(center, radius, normal, color);
	dirty = true;
}

void RendererDebug::add_debug_frustum(libv::vec3f nbl, libv::vec3f nbr, libv::vec3f ntr,
		libv::vec3f ntl, libv::vec3f fbl, libv::vec3f fbr,
		libv::vec3f ftr, libv::vec3f ftl, libv::vec4f color_wire, libv::vec4f color_sides) {
	frustums.push_back(RendererDebug::Frustum{{nbl, nbr, ntr, ntl, fbl, fbr, ftr, ftl}, color_sides, color_wire});
	dirty = true;
}

void RendererDebug::add_debug_quad(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec4f color) {
	quads.emplace_back(a, b, c, d, color);
	dirty = true;
}

void RendererDebug::clear_debug_shapes() {
	points.clear();
	lines.clear();
	triangles.clear();
	quads.clear();
	frustums.clear();
	spheres.clear();
	dirty = true;
}

//void RendererDebug::renderTriangles(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
//	glr.program(shader.program());
//
//	{
//		auto uniforms = uniform_stream.block_unique(layout_matrices);
//		uniforms[layout_matrices.matMVP] = glr.mvp();
//		uniforms[layout_matrices.matM] = glr.model;
//		uniforms[layout_matrices.matP] = glr.projection;
//		uniforms[layout_matrices.eye] = glr.eye();
//		glr.uniform(std::move(uniforms));
//		glr.render(mesh_triangle);
//	}
//}
// -------------------------------------------------------------------------------------------------

} // namespace surface
