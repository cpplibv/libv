// File: Material.hpp, Created on 2015. január 17. 7:50, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serialization/enable.hpp>
// std
#include <map>
#include <memory>
#include <variant>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

using Property = std::variant<
		bool,
		int,
		float,
		double,
		std::string,
		vec2i,
		vec3i,
		vec4i,
		vec2f,
		vec3f,
		vec4f,
		vec2d,
		vec3d,
		vec4d
	>;

// -------------------------------------------------------------------------------------------------

struct Material {
	std::string name;
	std::string shader; // Is this correct?
	boost::container::flat_map<std::string, Property> properties;

	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(shader);
	LIBV_REFLECTION_ACCESS(properties);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
