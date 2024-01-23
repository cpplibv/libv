// Project: libv.re, File: src/libv/re/mesh/mesh_cube.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/mesh/mesh.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MeshCube : public Mesh {
public:
	using ptr = MeshCube_ptr;
	using cptr = MeshCube_cptr;

public:
	MeshCube();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}

public:
	virtual BoundingSphere boundingSphere() const override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
