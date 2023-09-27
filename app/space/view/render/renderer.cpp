// Project: libv, File: app/space/view/render/renderer.cpp

// hpp
#include <space/view/render/renderer.hpp>
// libv
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/queue.hpp>
#include <libv/noise/noise_texture.hpp>
#include <libv/ui/context/context_resource.hpp>
#include <libv/ui/resource/font_2D.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/vm4/load.hpp>
//#include <libv/sun/camera.hpp>
// pro
//#include <space/command.hpp>
//#include <space/game/game_instance.hpp>
//#include <space/sim/playout/playout.hpp>
//#include <space/sim/universe.hpp>
#include <space/log.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

RendererEditorBackground::RendererEditorBackground(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "editor/editor_background.vs", "editor/editor_background.fs") {

	// TODO P1: Switch to blue noise once implemented
	//  		| It will not be implemented anytime soon so burn in a couple of textures from it
	const auto tex_data = libv::noise_white_2D_3uc(0x5EED, noise_size.x, noise_size.y);

	background_texture_pattern.storage(1, noise_size);
	background_texture_pattern.image(0, {0, 0}, noise_size, tex_data.data());
	background_texture_pattern.set(libv::gl::MagFilter::Nearest);
	background_texture_pattern.set(libv::gl::MinFilter::Nearest);
	background_texture_pattern.set(libv::gl::Wrap::Repeat, libv::gl::Wrap::Repeat);

	{
		auto position = mesh_background.attribute(attribute_position);
		auto index = mesh_background.index();

		position(-1, -1, 1);
		position(1, -1, 1);
		position(1, 1, 1);
		position(-1, 1, 1);

		index.quad(0, 1, 2, 3);
	}
}

void RendererEditorBackground::render(libv::glr::Queue& glr, libv::vec2f canvas_size) {
	glr.program(shader.program());
	const auto bg_noise = libv::vec4f(1, 1, 1, 0) * (5.f / 255.f);
	const auto bg_color = libv::vec4f(0.098f, 0.2f, 0.298f, 1.0f);
	glr.uniform(shader.uniform().render_resolution, canvas_size);
	glr.uniform(shader.uniform().noise_scale, bg_noise);
	glr.uniform(shader.uniform().base_color, bg_color);
	glr.texture(background_texture_pattern, textureUnit_pattern);
	glr.render(mesh_background);
}

// -------------------------------------------------------------------------------------------------

RendererCommandArrow::RendererCommandArrow(RendererResourceContext& rctx) :
		shader{rctx.loader.shader, "command_arrow.vs", "command_arrow.gs", "command_arrow.fs"} {
}

void RendererCommandArrow::restart_chain(float animation_offset) {
//	if(!arrows.empty()){
//		arrows.back().start_of_chain = true;
//	}
	curr_animation_offset = animation_offset;
	curr_start_of_chain = true;
}

void RendererCommandArrow::add_arrow(libv::vec3f source, libv::vec3f target, ArrowStyle style) {
	if (source == target) // Sanity check
		return;

	arrows.emplace_back(source, target, curr_animation_offset, style, std::exchange(curr_start_of_chain, false));
}

void RendererCommandArrow::add_debug_spiral() {
	restart_chain(0);

	add_arrow({0, 0, 0}, {1, 0.5f, 0.5f}, debug_arrow_style);
	add_arrow({1, 0.5f, 0.5f}, {1, 1, 0}, debug_arrow_style);
	add_arrow({1, 1, 0}, {1, 2, 2}, debug_arrow_style);
	add_arrow({1, 2, 2}, {-1, -1, -1}, debug_arrow_style);

	libv::vec3f prev_point;
	for (int i = 0; i < 60; i++) {
		const auto if_ = static_cast<float>(i);

		const auto r = if_ / 30.f;
		const auto x = std::sin(libv::deg_to_rad(if_ * 15.f)) * r;
		const auto y = std::cos(libv::deg_to_rad(if_ * 15.f)) * r;

		const auto curr_point = libv::vec3f{x, y, 0};
		add_arrow(prev_point, curr_point, debug_arrow_style);
		prev_point = curr_point;
	}
	for (int i = 60; i < 120; i++) {
		const auto if_ = static_cast<float>(i);

		const auto r = 2.f - (if_ - 60.f) / 30.f * 0.5f;
		const auto x = std::sin(libv::deg_to_rad(if_ * 15.f)) * r;
		const auto y = std::cos(libv::deg_to_rad(if_ * 15.f)) * r;
		const auto z = std::sin(libv::deg_to_rad((if_ - 60.f) * 30.f)) * 0.25f;

		const auto curr_point = libv::vec3f{x, y, z};
		add_arrow(prev_point, curr_point, debug_arrow_style);
		prev_point = curr_point;
	}
}

void RendererCommandArrow::add_debug_view01() {
	restart_chain(0);
	for (int i = 0; i < 10; i++) {
		const auto if_ = static_cast<float>(i);

		add_arrow({5, std::pow(2.f, if_) - 1, 0}, {5, std::pow(2.f, if_ + 1) - 1, 0}, debug_arrow_style);
	}
}

void RendererCommandArrow::add_debug_view02() {
	const auto d = 0.5f;
	const auto max_n = 30;

	for (int i = 3; i <= max_n; ++i) {
		const auto if_ = static_cast<float>(i);

		restart_chain(0);
		for (int j = 0; j < i; ++j) {
			const auto jf = static_cast<float>(j);

			const auto x1 = std::sin(libv::tau / if_ * jf) * d;
			const auto y1 = std::cos(libv::tau / if_ * jf) * d;
			const auto x2 = std::sin(libv::tau / if_ * (jf - 1)) * d;
			const auto y2 = std::cos(libv::tau / if_ * (jf - 1)) * d;
			add_arrow({x1 + 3, if_ - 3, y1}, {x2 + 3, if_ - 3, y2}, debug_arrow_style);
		}
	}
}

void RendererCommandArrow::add_debug_view03() {
	const auto d = 0.5f;
	const auto max_n = 30;

	for (int i = 3; i <= max_n; ++i) {
		const auto if_ = static_cast<float>(i);

		restart_chain(0);
		for (int j = 0; j < i; ++j) {
			const auto jf = static_cast<float>(j);

			const auto x1 = std::sin(libv::tau / if_ * jf) * d;
			const auto y1 = std::cos(libv::tau / if_ * jf) * d;
			add_arrow({x1 + 6, if_ - 3, y1}, {6, if_ - 3, 0}, debug_arrow_style);
		}
	}
}

void RendererCommandArrow::add_debug_view04() {
	const auto d = 0.5f;
	const auto max_n = 30;

	for (int i = 3; i <= max_n; ++i) {
		const auto if_ = static_cast<float>(i);

		restart_chain(0);
		for (int j = 0; j < i; ++j) {
			const auto jf = static_cast<float>(j);

			const auto x1 = std::sin(libv::tau / if_ * jf) * d;
			const auto y1 = std::cos(libv::tau / if_ * jf) * d;
			add_arrow({9, if_ - 3, 0}, {x1 + 9, if_ - 3, y1}, debug_arrow_style);
		}
	}
}

void RendererCommandArrow::add_debug_view05() {
	for (int i = 0; i < 100; i++) {
		const auto if_ = static_cast<float>(i);

		restart_chain(0);
		add_arrow({10, if_, 0}, {10 + (if_ + 1) * 0.25f, if_, 0}, debug_arrow_style);
	}
}

void RendererCommandArrow::rebuild_mesh() {
	mesh.clear(); // <<< Better support for Dynamic VAO data

	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto sp_ss_cp_cs = mesh.attribute(attribute_custom0); // SegmentPosition, SegmentSize, ChainPosition, ChainSize
	auto ao_nu_nu_nu = mesh.attribute(attribute_custom1); // AnimationOffset, NotUsed, NotUsed, NotUsed
	auto index = mesh.index();

	libv::glr::VertexIndex i = 0;

	auto chain_pos = 0.f;
	auto chain_size = 0.f;

	for (std::size_t j = 0; j < arrows.size(); ++j) {
		const auto& arrow = arrows[j];
		const auto length = (arrow.target - arrow.source).length();

		if (arrow.start_of_chain == true) {
			// TODO P5: Instead of forward scan on rebuild mesh, calculate and assign values on add_arrow() and on start_chain()
			chain_pos = 0.f;
			chain_size = length;
			for (std::size_t k = j + 1; k < arrows.size() && !arrows[k].start_of_chain; ++k) {
				const auto& a = arrows[k];
				chain_size += (a.target - a.source).length();
			}
		}

		position(arrow.source);
		position(arrow.target);

		color0(arrow.style.color_source);
		color0(arrow.style.color_target);

		ao_nu_nu_nu(arrow.animation_offset, 0, 0, 0);
		ao_nu_nu_nu(arrow.animation_offset, 0, 0, 0);

		sp_ss_cp_cs(0, length, chain_pos, chain_size);
		sp_ss_cp_cs(length, length, chain_pos + length, chain_size);

		index.line(i + 0, i + 1);
		i += 2;

		chain_pos += length;
	}

	arrows.clear();

	//	float chain_length = 0;
	//	float current_length = 0;
	//
	//	libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
	//		chain_length += (b - a).length();
	//	});
	//
	//	libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
	//		const auto length = (b - a).length();
	//
	//		sp_ss_tp_ts(0, length, current_length, chain_length);
	//		sp_ss_tp_ts(length, length, current_length, chain_length);
	//
	//		current_length += length;
	//	});
}

void RendererCommandArrow::render(libv::glr::Queue& glr, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream) {
	rebuild_mesh();

	// TODO P2: This will require to re upload to VAO every render
	//  		but it could be optimized if we give it a 'pretend' start offset
	//  		single uniform value that adjust the 'fake' starting point (not only the first section could have this)
	//			This also means VAO update only required on NEW command (SUPER GOOD), every normal render only changes some uniform
	//			| MAJOR ISSUE: Command arrows that move, like follow would break this system
	//				Resolution ideas: indirections?, uniform array as coordinates?, VBA update is okey?
	//			and/or Command arrows could be merged into a single VAO and use sub-meshes to render

	auto uniforms = uniform_stream.block_unique(libv::rev::layout_matrices);
	uniforms[libv::rev::layout_matrices.matMVP] = glr.mvp();
	uniforms[libv::rev::layout_matrices.matM] = glr.model;
	uniforms[libv::rev::layout_matrices.matP] = glr.projection;
	uniforms[libv::rev::layout_matrices.eye] = glr.eye();

	glr.program(shader.program());
	glr.uniform(std::move(uniforms));
	glr.uniform(shader.uniform().color, libv::vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	glr.uniform(shader.uniform().render_resolution, canvas_size);
	glr.uniform(shader.uniform().test_mode, global_test_mode);
	glr.uniform(shader.uniform().time, global_time);
	glr.render(mesh);
}

// -------------------------------------------------------------------------------------------------

RendererDebug::RendererDebug(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "flat_color.vs", "flat_color.fs") {
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
	// TODO P3: (space) Rebuild is too expensive on each frame

	if (dirty) {
		build_points_mesh(mesh_point);
		build_lines_mesh(mesh_line);
		build_triangles_mesh(mesh_triangle);
		dirty = false;
	}

	glr.program(shader.program());

	{
		auto uniforms = uniform_stream.block_unique(libv::rev::layout_matrices);
		uniforms[libv::rev::layout_matrices.matMVP] = glr.mvp();
		uniforms[libv::rev::layout_matrices.matM] = glr.model;
		uniforms[libv::rev::layout_matrices.matP] = glr.projection;
		uniforms[libv::rev::layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh_triangle);
	}
	{
		auto uniforms = uniform_stream.block_unique(libv::rev::layout_matrices);
		uniforms[libv::rev::layout_matrices.matMVP] = glr.mvp();
		uniforms[libv::rev::layout_matrices.matM] = glr.model;
		uniforms[libv::rev::layout_matrices.matP] = glr.projection;
		uniforms[libv::rev::layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh_line);
	}
	{
		auto uniforms = uniform_stream.block_unique(libv::rev::layout_matrices);
		uniforms[libv::rev::layout_matrices.matMVP] = glr.mvp();
		uniforms[libv::rev::layout_matrices.matM] = glr.model;
		uniforms[libv::rev::layout_matrices.matP] = glr.projection;
		uniforms[libv::rev::layout_matrices.eye] = glr.eye();
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

RendererGizmo::RendererGizmo(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "flat_color.vs", "flat_color.fs") {
	build_gizmo_lines(mesh);
}

void RendererGizmo::build_gizmo_lines(libv::glr::Mesh& mesh) {
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();

	position(0, 0, 0);
	position(1, 0, 0);
	position(0, 0, 0);
	position(0, 1, 0);
	position(0, 0, 0);
	position(0, 0, 1);

	color0(1, 0, 0, 1);
	color0(1, 0, 0, 1);
	color0(0, 1, 0, 1);
	color0(0, 1, 0, 1);
	color0(0, 0, 1, 1);
	color0(0, 0, 1, 1);

	index.line(0, 1);
	index.line(2, 3);
	index.line(4, 5);
}

void RendererGizmo::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(libv::rev::layout_matrices);
	uniforms[libv::rev::layout_matrices.matMVP] = glr.mvp();
	uniforms[libv::rev::layout_matrices.matM] = glr.model;
	uniforms[libv::rev::layout_matrices.matP] = glr.projection;
	uniforms[libv::rev::layout_matrices.eye] = glr.eye();

	glr.program(shader.program());
	glr.uniform(std::move(uniforms));
	glr.render(mesh);
}

// -------------------------------------------------------------------------------------------------

RendererFleet::RendererFleet(RendererResourceContext& rctx) :
// <<< P2: Model loader
// 		model(libv::vm4::load_or_throw(libv::read_file_or_throw("../../res/model/tree_01.vm4"))),
//		model(libv::vm4::load_or_throw(libv::read_file_or_throw("../../res/model/Tree_med.fixed.game.vm4"))),
//		model(libv::vm4::load_or_throw(libv::read_file_or_throw("../../res/model/tank_01_rocket_ring.0031_med.game.vm4"))),
		model(libv::vm4::load_or_throw(libv::read_file_or_throw("../../res/model/fighter_01_eltanin.vm4"))),
		// model(rctx.model_loader, "fighter_01_eltanin.vm4"),
		shader(rctx.loader.shader, "fleet.vs", "fleet.fs") {

//	log_space.fatal("RendererFleet...");
//
//	log_space.fatal("read_file_or_throw...");
//	auto file = libv::read_file_or_throw("../../res/model/fighter_01_eltanin.vm4");
//	log_space.fatal("load_or_throw...");
//	auto model_t = libv::vm4::load_or_throw(file);
// TODO P1: app.space: exception from here gets silently ignored
//	log_space.fatal("done...");
//
//	model.emplace(std::move(model_t));
}

void RendererFleet::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, Fleet::Selection selection_status) {
	glr.program(shader.program());
	glr.uniform(shader.uniform().base_color, libv::vec4f(0.7f, 0.7f, 0.7f, 1.0f));
	glr.uniform(shader.uniform().selected, libv::to_underlying(selection_status));

	model->render(glr, shader, uniform_stream);
}

// -------------------------------------------------------------------------------------------------

RendererPlanet::RendererPlanet(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "planet.vs", "planet.fs") {
	build_mesh(mesh);
}

void RendererPlanet::build_mesh(libv::glr::Mesh& mesh) {
	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto index = mesh.index();

	libv::glr::generateSpherifiedCube(12, position, normal, texture0, index);
	//      libv::glr::generateCube(position, normal, texture0, index);
}

void RendererPlanet::render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream, const Planet& planet) {

	const auto m_guard = gl.model.push_guard();
	gl.model.scale(planet.radius);

	auto uniforms = uniform_stream.block_unique(libv::rev::layout_matrices);
	uniforms[libv::rev::layout_matrices.matMVP] = gl.mvp();
	uniforms[libv::rev::layout_matrices.matM] = gl.model;
	uniforms[libv::rev::layout_matrices.matP] = gl.projection;
	uniforms[libv::rev::layout_matrices.eye] = gl.eye();

	gl.program(shader.program());
	gl.uniform(shader.uniform().base_color0, planet.color0);
	gl.uniform(shader.uniform().base_color1, planet.color1);
//	gl.uniform(shader.uniform().selected, selected);
	gl.uniform(std::move(uniforms));
	gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

RendererText::RendererText(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "font_2D.vs", "font_2D.fs") {
}

void RendererText::add_text(
		libv::vec3f position,
		libv::vec2f screenOffset,
		std::string str,
		libv::vec4f color,
		libv::ui::FontSize fontSize,
		libv::ui::AlignHorizontal alignH,
		libv::ui::AlignVertical alignV) {

	texts.emplace_back(
			position,
			screenOffset,
			color,
			libv::ui::LayoutText(
					font,
					fontSize,
					alignH,
					alignV,
					std::move(str)
			)
	);
}

void RendererText::clear_texts() {
	texts.clear();
}

void RendererText::add_debug_coordinates_if_nothing_else() {
	if (!texts.empty())
		return;

	add_text({0, 0, 0}, {0, 0}, "(0, 0, 0)");
	add_text({5, 0, 0}, {0, 0}, "(5, 0, 0)");
	add_text({0, 5, 0}, {0, 0}, "(0, 5, 0)");
	add_text({0, 0, 5}, {0, 0}, "(0, 0, 5)");
	add_text({-5, 0, 0}, {0, 0}, "(-5, 0, 0)", {1, 0.4f, 0.4f, 1});
	add_text({0, -5, 0}, {0, 0}, "(0, -5, 0)", {0.4f, 1, 0.4f, 1});
	add_text({0, 0, -5}, {0, 0}, "(0, 0, -5)", {0.4f, 0.4f, 1, 1});
}

void RendererText::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::sun::CameraPlayer::screen_picker& screen_picker) {

//	auto& task = std::get<ImplContextRender::TaskUI>(context.tasks.emplace_back(
//			std::in_place_type<ImplContextRender::TaskUI>,
//			glr.state.state(),
//			shader->base_ref(),
//			font->texture().base_ref(),
//			context.uniform_stream.block_stream(layout_UIInfo),
//			static_cast<uint32_t>(base_vertex),
//			static_cast<uint32_t>(base_index),
//			static_cast<uint32_t>(num_index)
//	));
//
//	task.uniform_block[layout_UIInfo.matMVP] = glr.mvp().translate({pos, 0});
//	task.uniform_block[layout_UIInfo.matM] = glr.model.top().translate_copy({pos, 0});
//	task.uniform_block[layout_UIInfo.clip_pos] = clip_pos;
//	task.uniform_block[layout_UIInfo.clip_size] = clip_size;
//	task.uniform_block[layout_UIInfo.component_pos] = current_component.layout_position2();
//	task.uniform_block[layout_UIInfo.component_size] = current_component.layout_size2();
//	task.uniform_block[layout_UIInfo.mouse_position] = context.mouse_position;
//	task.uniform_block[layout_UIInfo.ui_size] = context.ui_size;
//	task.uniform_block[layout_UIInfo.time_frame] = context.current_time;

	const auto guard_p = glr.projection.push_guard();
	const auto guard_v = glr.view.push_guard();

	glr.projection = libv::mat4f::ortho({0, 0}, screen_picker.canvas_size(), -1000, +1000);
	glr.view = libv::mat4f::identity();

	{
		mesh.clear();

		auto position = mesh.attribute(attribute_position); // Character quad vertex positions
		//	texture0_tiles.insert(context.vtx_texture0_tiles.end(), num_vertex, libv::vec4f(0, 0, 1, 1));
		auto texture0 = mesh.attribute(attribute_texture0);
		auto color0 = mesh.attribute(attribute_color0);
		auto positionOffset = mesh.attribute(attribute_custom0); // Whole text position offset (so we don't have to use uniforms, and it can be a single pass)
		auto index = mesh.index();

		auto vi_offset = libv::glr::VertexIndex{0};
		for (auto& entry : texts) {
			const auto content_size = entry.layout.content(-1, -1);
			entry.layout.limit(content_size);
			const auto& vd = entry.layout.vertices_data();

			const auto alignment_offset =
					entry.screenOffset +
							content_size *
									libv::vec2f(
											-info(entry.layout.align_vertical()).rate(),
											-info(entry.layout.align_horizontal()).rate());

			const auto[position2DAnchor, onScreen] = screen_picker.to_screen_ahead(entry.position);
			if (!onScreen)
				continue;

			position.append_from_range(vd.positions);
			texture0.append_from_range(vd.texture0s);
			color0.append_n_times(vd.positions.size(), entry.color);
			positionOffset.append_n_times(vd.positions.size(), libv::vec4f{alignment_offset + position2DAnchor, 0, 0});
			index.append_from_range(vd.indices);

			for (auto& i : index.view_last(vd.indices.size()))
				i += vi_offset;

			vi_offset += static_cast<libv::glr::VertexIndex>(vd.positions.size());
		}
	}

	const auto guard_s = glr.state.push_guard();

	glr.state.blendSrc_Source1Color();
	glr.state.blendDst_One_Minus_Source1Color();
	glr.state.disableDepthTest();
	glr.state.disableDepthMask();

	auto uniforms = uniform_stream.block_unique(libv::rev::layout_matrices);
	uniforms[libv::rev::layout_matrices.matMVP] = glr.mvp();
	uniforms[libv::rev::layout_matrices.matM] = glr.model;
	uniforms[libv::rev::layout_matrices.matP] = glr.projection;
	uniforms[libv::rev::layout_matrices.eye] = glr.eye();

	glr.program(shader.program());
	glr.texture(font->texture().base_ref(), textureUnit_diffuse);
//	glr.uniform(shader.uniform().base_color, libv::vec4f(0.7f, 0.7f, 0.7f, 1.0f));
//	glr.uniform(shader.uniform().selected, selected);
	glr.uniform(std::move(uniforms));
	glr.render(mesh);
}

// -------------------------------------------------------------------------------------------------

Renderer::Renderer(libv::ui::UI& ui) :
	resource_context(ui.nexus()) {
	text.font = ui.resource().font("consola.ttf");
}

void Renderer::prepare_for_render(libv::glr::Queue& glr) {
	// shader_loader.update MUST run before any other render queue operation
	// OTHERWISE the not loaded uniform locations are attempted to be used and placed into the streams
	// | So this is a better place than before, still not the best, When UI gets rev updates in the future maybe there will be better solutions
	resource_context.loader.shader.update(glr.out_of_order_gl());
}

// -------------------------------------------------------------------------------------------------

} // namespace space
