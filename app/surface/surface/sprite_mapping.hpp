// Project: libv, File: app/surface/surface/sprite_mapping.hpp

#pragma once

// std
#include <string>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct SpriteMappingEntry {
	int32_t id;
	std::string name;
	std::string path;
	float scale;
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
