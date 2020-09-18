// Project: libv.glr, File: src/libv/glr/procedural/plane.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

template <typename Precision = float>
struct GeneratePlaneXY {
	using in = uint32_t;
	using F = Precision;
	using vec2 = libv::vec2_t<Precision>;
	using vec3 = libv::vec3_t<Precision>;

	vec2 size;

	template <typename Position, typename Texture0, typename Index>
	void operator()(Position& position, Texture0& texture0, Index& index) {
		const in ioffset = static_cast<in>(position.size());

		position(size.x / F{-2}, size.y / F{-2}, 0);
		position(size.x / F{+2}, size.y / F{-2}, 0);
		position(size.x / F{+2}, size.y / F{+2}, 0);
		position(size.x / F{-2}, size.y / F{+2}, 0);

		texture0(0, 0);
		texture0(1, 0);
		texture0(1, 1);
		texture0(0, 1);

		index.quad(0 + ioffset, 1 + ioffset, 2 + ioffset, 3 + ioffset);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
