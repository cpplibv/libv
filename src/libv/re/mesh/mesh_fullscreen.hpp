// Project: libv.re, File: src/libv/re/mesh/mesh_fullscreen.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/mesh/mesh.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class MeshFullscreen : public Mesh {
public:
	using ptr = MeshFullscreen_ptr;
	using cptr = MeshFullscreen_cptr;

public:
	MeshFullscreen();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}

public:
	virtual BoundingSphere boundingSphere() const override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
