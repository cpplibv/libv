// Project: libv.color, File: src/libv/color/spread.hpp

#pragma once

// std
#include <cmath>


namespace libv {
namespace color {

// -------------------------------------------------------------------------------------------------

template <typename F = float>
constexpr inline F incremental_spread_to_01(size_t index) noexcept {
	if (index == 0)
		return F{0};

	if (index == 1)
		return F{1};

	const auto f_index = static_cast<F>(index);

	const auto log2 = std::log2(f_index);
	const auto row = std::ceil(log2) - 1;
	const auto pop = std::pow(F{2}, row);
	const auto index_adj = f_index - pop - 1;
	const auto step = 1 / pop;

	return step * (index_adj + F{0.5});
}

template <typename Color>
constexpr inline Color incremental_spread(size_t global_index) noexcept {

	//         | Y/G                           R R R R R     G G G G |     ---(G)--+
	//         |                               R R R R R     G G G G |     B B B B |
	//         |_ _ _ __ _ _ _                 R R R R R     G G G G(R)    B B B B(R)
	//        /|           /X/|                R R R R R     G G G G |     B B B B |
	//       / |  GREEN   /X/ |                R R R R R     G G G G |     B B B B |
	//      /  |         /X/  |
	//     /_ _|_ _ __ _/_/   |
	//    |_X_X|_X_X_X_X_X|RED|
	//    |    |_ _ _ __|_|_ _|_ _ _ _ _ X/R
	//    |   /  BLUE   |X|   /
	//    |  /          |X|  /
	//    | /           |X| /
	//    |/_ _ _ _ __ _|_|/
	//    /
	//   /
	//  / Z/B

	const auto level = static_cast<size_t>(std::pow(global_index, 1.0 / 3.0));
	const auto level_start = level * level * level;
	const auto level_index = global_index - level_start;

	const auto r_count = (level + 1) * (level + 1);
	const auto g_count = (level + 1) * (level + 0);

	const auto r_start = 0;
	const auto g_start = r_count;
	const auto b_start = r_count + g_count;

	// RED plane
	if (level_index < g_start) {
		const auto plane_index = level_index - r_start;
		const auto row_length = level + 1;

		const auto r = incremental_spread_to_01(level);
		const auto g = incremental_spread_to_01(static_cast<size_t>(std::floor(plane_index / row_length)));
		const auto b = incremental_spread_to_01(static_cast<size_t>(std::fmod(plane_index, row_length)));
		return Color{r, g, b};
	}

	// GREEN plane
	if (level_index < b_start) {
		const auto plane_index = level_index - g_start;
		const auto row_length = level + 1;

		const auto r = incremental_spread_to_01(static_cast<size_t>(std::floor(plane_index / row_length)));
		const auto g = incremental_spread_to_01(level);
		const auto b = incremental_spread_to_01(static_cast<size_t>(std::fmod(plane_index, row_length)));
		return Color{r, g, b};
	}

	// BLUE plane
	{
		const auto plane_index = level_index - b_start;
		const auto row_length = level;

		const auto r = incremental_spread_to_01(static_cast<size_t>(std::floor(plane_index / row_length)));
		const auto g = incremental_spread_to_01(static_cast<size_t>(std::fmod(plane_index, row_length)));
		const auto b = incremental_spread_to_01(level);
		return Color{r, g, b};
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace color
} // namespace libv
