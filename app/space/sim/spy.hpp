// Project: libv, File: app/space/sim/spy.hpp

// std
#include <vector>
#include <cstdint>
#include <string>
// pro
#include <space/sim/engine/loc_string.hpp>
#include <space/sim/engine/tick.hpp>
#include <space/sim/kredit.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct SpyMissionTable {
	loc_string name;
//	loc_string name_menu;
//	loc_string name_action;
	loc_string description;

	tick duration;
	kredit cost;
//	int required_tech;

	float chance_success_silent;
	float chance_success_detect;
	float chance_success_capture;
	float chance_failure_silent;
	float chance_failure_detect;
	float chance_failure_capture;
};

struct SpyMission {
	tick start_tick;
	Empire* a;
	Empire* b;
	Spy* spy;
	SpyMissionTable* mission;
};

struct Spy {};

// -------------------------------------------------------------------------------------------------

} // namespace space
