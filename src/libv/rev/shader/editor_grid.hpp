// Project: libv.rev, File: src/libv/rev/shader/editor_grid.hpp
// Generated source code
// Generator version: v5.0.0
// Input file: src/libv/rev/shader/editor_grid.in.lua

#pragma once

//
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/rev/shader/block/camera200.hpp>
#include <libv/rev/shader/block/matrices200.hpp>


namespace libv::rev {

// -------------------------------------------------------------------------------------------------

struct UniformsEditorGrid {
	template <typename Access> void access_uniforms(Access& access) {
		(void) access;
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_Matrices200);
		access(uniformBlock_Camera200);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv::rev
