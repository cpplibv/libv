// Project: libv.vm4, File: src/libv/vm4/material.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/enable.hpp>
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
		libv::vec2i,
		libv::vec3i,
		libv::vec4i,
		libv::vec2f,
		libv::vec3f,
		libv::vec4f,
		libv::vec2d,
		libv::vec3d,
		libv::vec4d
	>;

// -------------------------------------------------------------------------------------------------

struct Material {
	std::string name;
	std::string shader; // Is this correct?
	boost::container::flat_map<std::string, Property> properties;

	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(shader);
	LIBV_REFLECTION_ACCESS(properties);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
