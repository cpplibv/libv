// Project: libv.gl, File: src/libv/gl/indirect_command.hpp

#pragma once

#include <cstdint>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

struct IndirectCommandDrawElements {
	uint32_t count;
	uint32_t instanceCount;
	uint32_t firstIndex;
	int32_t baseVertex;
	uint32_t baseInstance;
};

struct IndirectCommandDrawArrays {
	uint32_t count;
	uint32_t instanceCount;
	uint32_t first;
	uint32_t baseInstance;
};

struct IndirectCommandDispatch {
	uint32_t num_groups_x;
	uint32_t num_groups_y;
	uint32_t num_groups_z;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
