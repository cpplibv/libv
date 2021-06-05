// Project: libv.gl, File: src/libv/gl/query_object.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdint>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/gl_fwd.hpp>


namespace libv {
namespace gl {

// Query -------------------------------------------------------------------------------------------

class AccessQuery;

struct Query {
	uint32_t id = 0;
	QueryType type;

	template <typename Access = AccessQuery>
	inline Access operator()(GL&) noexcept {
		return Access{*this};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
