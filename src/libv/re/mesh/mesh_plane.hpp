// Project: libv.re, File: src/libv/re/mesh/mesh_plane.hpp

#pragma once

#include <libv/math/vec.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/mesh/mesh.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MeshPlane : public Mesh {
public:
	using ptr = MeshPlane_ptr;
	using cptr = MeshPlane_cptr;

public:
	explicit MeshPlane(libv::vec2i divisions, libv::vec2f uvTiles = {1, 1});

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(libv::vec2i divisions, libv::vec2f uvTiles = {1, 1}) {
		return ptr::make(divisions, uvTiles);
	}

public:
	virtual BoundingSphere boundingSphere() const override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
