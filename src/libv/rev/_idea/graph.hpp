// Created by Vader on 2021.05.07..

#pragma once

// libv
//#include <libv/container/ring.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/math/vec.hpp>
// std
#include <vector>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class Graph {
private:
	using vec2 = libv::vec2f;
	using vec4 = libv::vec4f;
	using Color = libv::vec4f;

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::DynamicDraw};

	// TODO P5: Use libv::ring or libv::flex_ring for:
	std::vector<float> values;
	uint32_t current_index = 0;
	bool full = false;
	// ... these 3

	float max_value = 0;

	vec2 size;
	Color color = {1, 1, 1, 1};

public:
	Graph(uint32_t value_count, vec2 size) :
		values(value_count, 0.0f),
		size(size) {
	}

	Graph(const Graph&) = delete;
	Graph& operator=(const Graph&) & = delete;
	Graph(Graph&&) noexcept = delete;
	Graph& operator=(Graph&&) & noexcept = delete;

public:
	void next_value(float value = 0.0f) {
		next();
		set_last_value(value);
	}

	void next() {
		++current_index;
		if (current_index == values.size()) {
			full = true;
			current_index = 0;
		}
	}

	void set_last_value(float value) {
		values[current_index] = value;
		if (value > max_value)
			max_value = value;
	}

	void render(relevant_gl_render_type& gl) {
		dirty_region_lazy_update_not_always_full_write();
		direct_write_vbo_aka_mesh_and_update_that_region_would_be_best();

		const auto count = values.size();
		const auto bar_w = size.x / static_cast<float>(count);
		const auto height_scale = size.y / max_value;

		const auto position_ = mesh.attribute(...);
		const auto color_ = mesh.attribute(...);
		const auto index_ = mesh.index(...);

		for (size_t i = 0; i < count; ++i) {
			const auto index = full ? (current_index + i + 1) % count : i;
			dont_use_modulo_or_check_if_optimizer_kills_it();

			const auto bar_h = values[index] * height_scale;
			const auto x_curr = static_cast<float>(i);
			const auto x_next = static_cast<float>(i + 1);

			position_(x_curr * bar_w, 0);
			position_(x_next * bar_w, 0);
			position_(x_next * bar_w, bar_h);
			position_(x_curr * bar_w, bar_h);

			color_(color);
			color_(color);
			color_(color);
			color_(color);

			index_.tail_quad();
		}

		gl.shader(gl.builtin_shaders.flat_vtx_color);
		set_mvp_stuff_or_shader_assignment_handles_it_already();
		gl.render(mesh);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
