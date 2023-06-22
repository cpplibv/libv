// Project: libv, File: app/surface/view/vec_hash.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/hash.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct VecHash {
	static constexpr inline auto operator()(libv::vec2i vec) const noexcept {
		return libv::hash_combine(
				libv::hash_int(static_cast<uint32_t>(vec.x)),
				libv::hash_int(static_cast<uint32_t>(vec.y)));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
