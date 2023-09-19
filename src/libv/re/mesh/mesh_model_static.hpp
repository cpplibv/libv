// Project: libv.re, File: src/libv/re/mesh/mesh_model_static.hpp

#pragma once

#include <libv/math/mat_fwd.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/mesh/mesh.hpp>
#include <libv/vm4/fwd.hpp>

#include <span>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MeshModelStatic : public Mesh {
public:
	using ptr = MeshModelStatic_ptr;
	using cptr = MeshModelStatic_cptr;

private:
	BoundingSphere boundingSphere_;

public:
	explicit MeshModelStatic();
	explicit MeshModelStatic(const libv::vm4::Model& vm4);

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(const libv::vm4::Model& vm4) {
		return ptr::make(vm4);
	}

	void instanceMatricesRow012(std::span<const libv::mat4x3f> transformsTransposedRow012);

public:
	virtual BoundingSphere boundingSphere() const override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
