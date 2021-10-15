// Project: libv, File: app/space/player.hpp

#pragma once

// std
#include <string>


namespace app {

// -------------------------------------------------------------------------------------------------

std::string generate_random_name(uint64_t seed, size_t length);
std::string generate_random_name(size_t length);

// -------------------------------------------------------------------------------------------------

struct Player {
	std::string name = generate_random_name(4);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
