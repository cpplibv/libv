// Project: libv.rev, File: src/libv/rev/resource/material_scanner.cpp

// hpp
#include <libv/rev/resource/material_scanner.hpp>
// libv
#include <libv/vm4/material.hpp>
// pro
#include <libv/rev/resource/texture.hpp>
#include <libv/rev/resource/texture_loader.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

Texture MaterialScanner::texture_or(libv::vm4::TextureType type, std::string_view fallback) const {
	for (const auto& texture_def : vm4Material.textures)
		if (texture_def.type == type)
			return textureLoader.load(texture_def.path);

	return textureLoader.load(fallback);
}

Texture MaterialScanner::texture_or(libv::vm4::TextureType type, Texture fallback) const {
	for (const auto& texture_def : vm4Material.textures)
		if (texture_def.type == type)
			return textureLoader.load(texture_def.path);

	return fallback;
}

libv::vec4f MaterialScanner::vec4f_or(std::string_view name, libv::vec4f fallback) const {
	auto it = vm4Material.properties.find(name);
	if (it == vm4Material.properties.end())
		return fallback;

	if (auto vf = std::get_if<libv::vec4f>(&it->second))
		return *vf;
	if (auto vf = std::get_if<libv::vec4d>(&it->second))
		return vf->cast<float>();
	if (auto vf = std::get_if<libv::vec4i>(&it->second))
		return vf->cast<float>();

	return fallback;
}

float MaterialScanner::float_or(std::string_view name, float fallback) const {
	auto it = vm4Material.properties.find(name);
	if (it == vm4Material.properties.end())
		return fallback;

	if (auto vf = std::get_if<float>(&it->second))
		return *vf;
	if (auto vf = std::get_if<double>(&it->second))
		return static_cast<float>(*vf);
	if (auto vf = std::get_if<int32_t>(&it->second))
		return static_cast<float>(*vf);

	return fallback;
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
