// File:   spread.hpp Author: Vader Created on 02 July 2019, 18:15

#pragma once


namespace libv {
namespace color {

// -------------------------------------------------------------------------------------------------

template <typename F = float>
constexpr inline F spread_to01(size_t index) noexcept {
	if (index == 0)
		return F{0};

	if (index == 1)
		return F{1};

	const auto log2 = std::log2(index);
	const auto row = std::ceil(log2) - 1;
	const auto index = index - std::pow(2, row) - 1;
	const auto pop = std::pow(2, row);
	const auto step = 1 / pop;

	return step * (index + F{0.5});
}

template <typename Color>
constexpr inline Color spread(size_t id) noexcept {
	const auto level = std::floor(std::pow(id, 1.0 / 3.0));
	const auto level_begin = level * level * level;

	const auto l = level + 1;
	const auto i = id - level_begin;

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

    // RED plane
	if (i < l*l) {
		const auto r = spread_to01(l - 1);
		const auto g = spread_to01(std::floor(i / l));
		const auto b = spread_to01(i % l);
		return Color{r, g, b};
	}

	// GREEN plane
	if (i < l*l + l*(l-1)) {
		i -= l*l
		const auto r = spread_to01(std::floor(i / (l - 1)));
		const auto g = spread_to01(l - 1);
		const auto b = spread_to01(i % (l - 1));
		return Color{r, g, b};
	}

	// BLUE plane
	i -= l*l + l*(l-1);
	const auto r = spread_to01(std::floor(i / (l - 1)));
	const auto g = spread_to01(i % (l - 1));
	const auto b = spread_to01(l - 1);
	return Color{r, g, b};
}

// -------------------------------------------------------------------------------------------------

} // namespace color
} // namespace libv
