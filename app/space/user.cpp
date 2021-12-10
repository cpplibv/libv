// Project: libv, File: app/space/user.cpp

// hpp
#include <space/user.hpp>
// libv
#include <libv/utility/random/uniform_distribution.hpp>
// std
#include <random>
#include <string>


namespace space {

// -------------------------------------------------------------------------------------------------

std::string generate_random_name(uint64_t seed, std::size_t length) {
	static constexpr char charset[] =
			"0123456789"
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static constexpr std::size_t charset_size = sizeof(charset) - 1;

	auto rng = std::mt19937_64{seed};
	auto dist = libv::make_uniform_distribution_exclusive(charset_size);

	std::string str;
	str.resize(length);

	for (auto& c : str)
		c = charset[dist(rng)];

	return str;
}

std::string generate_random_name(std::size_t length) {
	return generate_random_name(std::random_device{}(), length);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
