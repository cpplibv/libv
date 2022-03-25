// Project: libv.vm4, File: src/libv/vm4/material.hpp

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/serial/serial.hpp>
// std
#include <string>
#include <variant>
#include <vector>
// pro
#include <libv/vm4/texture_type.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

using Property = std::variant<
		bool,
		int32_t,
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

struct MaterialTexture {
	TextureType type;
	std::string path;
	int uvwsrc = 0;
	//	TEXBLEND
	//	TEXOP
	//	MAPPING
	//	MAPPINGMODE_U
	//	MAPPINGMODE_V
	//	int? TEXMAP_AXIS
	//	vec3f? TEXMAP_AXIS

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP(type));
		ar(LIBV_NVP(path));
		ar(LIBV_NVP(uvwsrc));
	}
};

// -------------------------------------------------------------------------------------------------

struct Material {
	std::string name;
	std::string shader; // Is this correct?
	boost::container::flat_map<std::string, Property, std::less<>> properties;
	std::vector<MaterialTexture> textures;

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP(name));
		ar(LIBV_NVP(shader));
		ar(LIBV_NVP(properties));
		ar(LIBV_NVP(textures));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
