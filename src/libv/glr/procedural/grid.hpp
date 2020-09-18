// Project: libv.glr, File: src/libv/glr/procedural/grid.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

template <typename Precision = float, typename Position, typename LineID, typename Index>
void generateGrid(int32_t x, int32_t y, Position& position, LineID& lineID, Index& index) {
	using F = Precision;
	using in = typename Index::value_type;

	const in ioffset = static_cast<in>(position.size());

	const float fx = static_cast<float>(x - 1);
	const float fy = static_cast<float>(y - 1);

	for (int32_t i = 0; i < x; ++i) {
		const float fi = static_cast<float>(i);

		position(fx * F(-0.5) + fi, fy * F(-0.5), F(0));
		position(fx * F(-0.5) + fi, fy * F(+0.5), F(0));
		lineID(i, 0);
		lineID(i, 0);
		index.line(i * 2, i * 2 + 1);
	}

	for (int32_t i = 0; i < y; ++i) {
		const float fi = static_cast<float>(i);

		position(fx * F(-0.5), fy * F(-0.5) + fi, F(0));
		position(fx * F(+0.5), fy * F(-0.5) + fi, F(0));
		lineID(i, 1);
		lineID(i, 1);
		index.line(x * 2 + i * 2, x * 2 + i * 2 + 1);
	}

	// Adjust indices if geometry was non-empty
	if (ioffset != 0)
		for (auto& i : index.view_last(x * 2 + y * 2))
			i += ioffset;
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
