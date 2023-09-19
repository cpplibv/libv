// Project: libv.gl, File: src/libv/gl/query_object.hpp

#pragma once

// std
#include <cstdint>
// pro
#include <libv/gl/enum.hpp>


namespace libv {
namespace gl {

// Query -------------------------------------------------------------------------------------------

struct Query {
	uint32_t id = 0;
	QueryType type = QueryType::SamplesPassed;
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
