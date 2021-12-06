// Project: libv, File: app/space/user.hpp

#pragma once

// std
#include <string>


namespace app {

// -------------------------------------------------------------------------------------------------

std::string generate_random_name(uint64_t seed, std::size_t length);
std::string generate_random_name(std::size_t length);

// -------------------------------------------------------------------------------------------------

struct User {
	std::string name = generate_random_name(5);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
