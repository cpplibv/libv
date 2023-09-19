// Project: libv.re, File: src/libv/re/resource/material_scanner.hpp

#pragma once

#include <libv/math/vec_fwd.hpp>
#include <libv/re/fwd.hpp>
#include <libv/vm4/fwd.hpp>
#include <libv/vm4/texture_type.hpp> // For convenience

#include <string_view>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

class MaterialScanner {
private:
	const libv::vm4::Material& vm4Material;

public:
	explicit inline MaterialScanner(const libv::vm4::Material& vm4Material) noexcept :
		vm4Material(vm4Material) {}

public:
	[[nodiscard]] Texture_ptr texture_or(libv::vm4::TextureType type, std::string_view fallback) const;
	[[nodiscard]] Texture_ptr texture_or(libv::vm4::TextureType type, const Texture_ptr& fallback) const;

	[[nodiscard]] libv::vec4f vec4f_or(std::string_view name, libv::vec4f fallback) const;

	[[nodiscard]] float float_or(std::string_view name, float fallback) const;
	[[nodiscard]] int32_t int32_or(std::string_view name, int32_t fallback) const;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
