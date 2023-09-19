// Project: libv.re, File: src/libv/re/resource/material_scanner.cpp

#include <libv/re/resource/material_scanner.hpp>

#include <libv/math/vec.hpp>
#include <libv/re/resource/texture_loader.hpp>
#include <libv/vm4/material.hpp>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

Texture_ptr MaterialScanner::texture_or(libv::vm4::TextureType type, std::string_view fallback) const {
	for (const auto& texture_def : vm4Material.textures)
		if (texture_def.type == type)
			return r.texture.load(texture_def.path);

	return r.texture.load(fallback);
}

Texture_ptr MaterialScanner::texture_or(libv::vm4::TextureType type, const Texture_ptr& fallback) const {
	for (const auto& texture_def : vm4Material.textures)
		if (texture_def.type == type)
			return r.texture.load(texture_def.path);

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

int32_t MaterialScanner::int32_or(std::string_view name, int32_t fallback) const {
	auto it = vm4Material.properties.find(name);
	if (it == vm4Material.properties.end())
		return fallback;

	if (auto vf = std::get_if<float>(&it->second))
		return static_cast<int32_t>(*vf);
	if (auto vf = std::get_if<double>(&it->second))
		return static_cast<int32_t>(*vf);
	if (auto vf = std::get_if<int32_t>(&it->second))
		return static_cast<int32_t>(*vf);

	return fallback;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
