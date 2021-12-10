// Project: libv, File: src/space/universe/force.hpp

#pragma once

// std
#include <string>


namespace space {

// -------------------------------------------------------------------------------------------------

class Faction {
private:
	std::string name;

public:

};

class Relation {

};

class FactionRelationMatrix {
	std::map<std::pair<Faction, Faction>, Relation> relations;
//	std::vector<>
};

// -------------------------------------------------------------------------------------------------

} // namespace space
