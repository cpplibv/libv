// Project: libv.re, File: src/libv/re/common/debug_capture_mode.hpp

#pragma once

#include <libv/utility/to_underlying.hpp>

#include <cstdint>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

enum class DebugCaptureMode : int32_t {
	normal,
	depth,
	depth_linear,
	depth_distance,
	stencil,
	jfa,
	missing,
};

[[nodiscard]] constexpr inline auto operator+(DebugCaptureMode e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
