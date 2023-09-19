// Project: libv.re, File: src/libv/re/mesh/mesh_skybox.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/mesh/mesh.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MeshSkybox : public Mesh {
public:
	using ptr = MeshSkybox_ptr;
	using cptr = MeshSkybox_cptr;

public:
	MeshSkybox();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}

public:
	virtual BoundingSphere boundingSphere() const override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
