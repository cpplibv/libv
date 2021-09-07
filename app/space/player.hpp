// Project: libv, File: app/space/player.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/utility/random/uniform_distribution.hpp>
// std
#include <random>
#include <string>
#include <string_view>


namespace app {

// -------------------------------------------------------------------------------------------------

inline std::string generate_random_name(uint64_t seed, size_t length) {
	static constexpr std::string_view charset =
			"0123456789"
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	auto rng = std::mt19937_64{seed};
	auto dist = libv::make_uniform_distribution_exclusive(charset.size());

	std::string str;
	str.resize(length);

	for (auto& c : str)
		c = charset[dist(rng)];

	return str;
}

inline std::string generate_random_name(size_t length) {
	return generate_random_name(std::random_device{}(), length);
}

struct Player {
	std::string name = generate_random_name(4);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
