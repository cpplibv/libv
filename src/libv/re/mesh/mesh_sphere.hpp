// Project: libv.re, File: src/libv/re/mesh/mesh_sphere.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/mesh/mesh.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MeshSphere : public Mesh {
public:
	using ptr = MeshSphere_ptr;
	using cptr = MeshSphere_cptr;

public:
	explicit MeshSphere(int32_t divisions);

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(int32_t divisions) {
		return ptr::make(divisions);
	}

public:
	virtual BoundingSphere boundingSphere() const override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
