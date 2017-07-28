// File:   program_font.hpp Author: Vader Created on 2019. február 4., 20:32

#pragma once

// libv
#include <libv/glr/mesh.hpp>
// pro
#include <libv/ui/context_render.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct MeshQuad : libv::glr::Mesh {
	MeshQuad() :
		libv::glr::Mesh(libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw) {
	}

	void build(float left, float right, float bottom, float top) {
		clear();
		auto position = attribute(attribute_position);
		auto texture0 = attribute(attribute_texture0);
		auto index_ = index();

		const auto ioffset = static_cast<libv::glr::VertexIndex>(position.size());

		position(left, bottom, 0);
		position(right, bottom, 0);
		position(right, top, 0);
		position(left, top, 0);

		texture0(0, 0);
		texture0(1, 0);
		texture0(1, 1);
		texture0(0, 1);

		index_.quad(0 + ioffset, 1 + ioffset, 2 + ioffset, 3 + ioffset);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
