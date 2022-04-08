// Project: libv.utility, File: src/libv/utility/index_spiral.hpp

#pragma once

#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] inline libv::vec2i index_spiral(const size_t index) {
	//  _ _ 1 _ _
	// |_|_|D|C|B|
	// |_|4|3|2|A| 0
	//2|_|5|0|1|9|
	// |_|6|7|8|_| 4
	// |_|_|_|_|_|
	//      3
	const auto id = static_cast<int> (index);
	if (id == 0)
		return {0, 0};

	auto temp = id - 1; //bc of id=0
	auto posInRing = temp;

	//TODO P3: This could be a math formula
	int i = 0;
	while (temp >= 0) {
		posInRing = temp;
		temp -= (i + 1) * 8;
		i++;
	}
	const auto ring = i;
	const auto side = ring * 2;
	const auto halfSide = ring;
	// 0. 0 ~ halfSide, 0 ~ 2
	// 1. halfSide + 1 ~ halfSide + 1 + side - 1, 3 - 6
	// 2. halfSide + 1 + side ~  halfSide + 1 + side + side - 1, 7 - 10
	// 3. halfSide + 1 + side + side ~ halfSide + 1 + side + side + side - 1, 11 - 14
	// 4. halfSide + 1 + side + side + side ~ halfSide + 1 + side + side + side + halfSide - 1, 15 - 16
	int sideId = 0;
	int posInSide = posInRing;
	if (posInRing >= 0 && posInRing <= halfSide)
		sideId = 0;
	else if (halfSide + 1 + side + side + side <= posInRing && posInRing <= halfSide + 1 + side + side + side + halfSide - 1) {
		sideId = 4;
		posInSide -= halfSide + 1 + 3 * side;
	} else {
		posInSide -= halfSide + 1;
		for (int j = 0; j < 3; ++j) {
			if (halfSide + 1 + j * side <= posInRing && posInRing <= halfSide + 1 + (j + 1) * side - 1) {
				sideId = j + 1;
				break;
			}
			posInSide -= side;
		}
	}

	int32_t y = 0;
	int32_t x = 0;
	switch (sideId) {
	default: // To calm down compiler warnings
	case 0:
		x = ring;
		//y = [0 , ring]
		y = posInSide;
		return {x, y};
	case 1:
		//x = [-ring, ring)
		x = -posInSide + halfSide - 1;
		y = ring;
		return {x, y};
	case 2:
		x = -ring;
		//y = [-ring , ring)
		y = -posInSide + halfSide - 1;
		return {x, y};
	case 3:

		//x = (-ring, ring]
		x = posInSide - halfSide + 1;
		y = -ring;
		return {x, y};
	case 4:
		x = ring;
		//y = (0 , -ring)
		y = posInSide - halfSide + 1;
		return {x, y};
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
