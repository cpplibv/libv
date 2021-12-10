// Project: libv, File: src/space/universe/empire.hpp

#pragma once

// std
#include <string>
// pro
#include <space/universe/kredit.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Empire {
//	Player* player;

public:
	int64_t score = 0;

public:
//	kredit balance;
	int64_t balance = 0;
//	float karma;

//	std::vector<Colony*> colonies;
//	std::vector<Fleet*> fleets;
//	std::vector<Spy*> spies;
};

// -------------------------------------------------------------------------------------------------

} // namespace space
