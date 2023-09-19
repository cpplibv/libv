// Project: libv.re, File: src/libv/re/common/block_binding.hpp

#pragma once

#include <libv/utility/to_underlying.hpp>

#include <cstdint>
#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

enum class BlockBinding : uint8_t {
	pass = 0,
	// reserved1 = 1,
	lights = 2,
	// scene = 3,
	passRelated = 4, // TODO P5: rename pass to frame, rename passRelated to pass
	// reserved5 = 5,
	model = 6,
	// reserved7 = 7,

	// NOTE: The first 8 binding index (0..7) is reserved for library usage
	custom0 = 8,
	custom1 = 9,
	custom2 = 10,
	custom3 = 11,
};

static constexpr inline std::string_view builtinBlockNamePass = "Pass300";
static constexpr inline std::string_view builtinBlockNameLights = "Lights300";
static constexpr inline std::string_view builtinBlockNamePostProcess = "PostProcess300";
static constexpr inline std::string_view builtinBlockNameModel = "Model300";

[[nodiscard]] constexpr inline auto operator+(BlockBinding e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
