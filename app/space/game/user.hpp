// Project: libv, File: app/space/game/user.hpp

#pragma once

// std
#include <string>


namespace space {

// -------------------------------------------------------------------------------------------------

std::string generate_random_name(uint64_t seed, std::size_t length);
std::string generate_random_name(std::size_t length);

// -------------------------------------------------------------------------------------------------

struct User {
	std::string name = generate_random_name(5);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
