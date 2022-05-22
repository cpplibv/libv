// Project: libv.utility, File: src/libv/utility/index_spiral.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline libv::vec2i index_spiral(const size_t index) {
	// +---+---+---+---+---+
	// | G | F | E | D | C |
	// +---+---+---+---+---+
	// | H | 4 | 3 | 2 | B |
	// +---+---+---+---+---+
	// | I | 5 | 0 | 1 | A |
	// +---+---+---+---+---+
	// | J | 6 | 7 | 8 | 9 |
	// +---+---+---+---+---+
	// | K | L | M | N | O |
	// +---+---+---+---+---+

	const auto i = static_cast<int32_t>(index);

	if (i == 0) // Handle ring 0 separately so we can -1 later
		return {0, 0};

	const auto if_ = static_cast<double>(i);
	const auto ring = static_cast<int32_t>(std::lround(std::sqrt(if_ / 4.0)));
	const auto ring_population = ring * 8;
	const auto ring_start = (ring * 2 - 1) * (ring * 2 - 1);
	const auto ring_index = i - ring_start;

	const auto quarter_population = ring_population / 4;
	const auto quarter = ring_index / quarter_population;
	const auto quarter_start = quarter * quarter_population;
	const auto quarter_index = ring_index - quarter_start;

	const auto secondary_index = quarter_index - quarter_population / 2 + 1;

	switch (quarter) {
	case 0:
		return {ring, secondary_index};
	case 1:
		return {-secondary_index, ring};
	case 2:
		return {-ring, -secondary_index};
	case 3:
		return {secondary_index, -ring};
	}

	return {0, 0};
}

template <typename Fn>
constexpr inline void index_spiral_loop(int num_ring, Fn&& fn) {
	// +---+---+---+---+---+
	// | G | F | E | D | C |
	// +---+---+---+---+---+
	// | H | 4 | 3 | 2 | B |
	// +---+---+---+---+---+
	// | I | 5 | 0 | 1 | A |
	// +---+---+---+---+---+
	// | J | 6 | 7 | 8 | 9 |
	// +---+---+---+---+---+
	// | K | L | M | N | O |
	// +---+---+---+---+---+

	if (num_ring < 1)
		return;

	fn(libv::vec2i{0, 0});

	for (int ring = 1; ring < num_ring; ++ring) {
		const auto quarter_population = ring * 2;
		const auto quarter_start = -quarter_population / 2 + 1;

		for (int y = 0; y < quarter_population; ++y)
			fn(libv::vec2i{ring, quarter_start + y});

		for (int x = 0; x < quarter_population; ++x)
			fn(libv::vec2i{-quarter_start - x, ring});

		for (int y = 0; y < quarter_population; ++y)
			fn(libv::vec2i{-ring, -quarter_start - y});

		for (int x = 0; x < quarter_population; ++x)
			fn(libv::vec2i{quarter_start + x, -ring});
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
