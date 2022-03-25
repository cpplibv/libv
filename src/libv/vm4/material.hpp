// Project: libv.vm4, File: src/libv/vm4/material.hpp

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
#include <vector>


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

enum class TextureType : int32_t {
	diffuse = 1,            /// The texture is combined with the result of the diffuse lighting equation.
	specular = 2,           /// The texture is combined with the result of the specular lighting equation.
	ambient = 3,            /// The texture is combined with the result of the ambient lighting equation.
	emissive = 4,           /// The texture is added to the result of the lighting calculation. It isn't influenced by incoming light.
	height = 5,             /// The texture is a height map. By convention, higher gray-scale values stand for higher elevations from the base height.
	normals = 6,            /// The texture is a (tangent space) normal-map.
	shininess = 7,          /// The texture defines the glossiness of the material.
	opacity = 8,            /// The texture defines per-pixel opacity.
	displacement = 9,       /// Displacement texture. Application-dependent (Higher color values stand for higher vertex displacements.)
	lightmap = 10,          /// Lightmap texture (aka Ambient Occlusion). Both 'Lightmaps' and dedicated 'ambient occlusion maps' are covered by this material property.
	reflection = 11,        /// Reflection texture (Contains the color of a perfect mirror reflection.)

	base_color = 12,        /// PBR Material
	normal_camera = 13,     /// PBR Material
	emission_color = 14,    /// PBR Material
	metalness = 15,         /// PBR Material
	diffuse_roughness = 16, /// PBR Material
	ambient_occlusion = 17, /// PBR Material

	unknown = 18,
};

[[nodiscard]] constexpr inline std::string_view to_string(TextureType type) noexcept {
	switch (type) {
	case TextureType::diffuse: return "diffuse";
	case TextureType::specular: return "specular";
	case TextureType::ambient: return "ambient";
	case TextureType::emissive: return "emissive";
	case TextureType::height: return "height";
	case TextureType::normals: return "normals";
	case TextureType::shininess: return "shininess";
	case TextureType::opacity: return "opacity";
	case TextureType::displacement: return "displacement";
	case TextureType::lightmap: return "lightmap";
	case TextureType::reflection: return "reflection";

	case TextureType::base_color: return "base_color";
	case TextureType::normal_camera: return "normal_camera";
	case TextureType::emission_color: return "emission_color";
	case TextureType::metalness: return "metalness";
	case TextureType::diffuse_roughness: return "diffuse_roughness";
	case TextureType::ambient_occlusion: return "ambient_occlusion";

	case TextureType::unknown: return "unknown";
	}
	assert(false && "Invalid TextureType enum value"); // unknown would be valid, but random values are not
	return "<<invalid>>";
}

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
	boost::container::flat_map<std::string, Property> properties;
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
