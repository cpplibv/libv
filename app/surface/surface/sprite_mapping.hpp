// Created by dbobula on 3/25/2022.

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
