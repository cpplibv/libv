// Project: libv.re, File: src/libv/re/resource/common_resource.hpp

#pragma once

#include <libv/utility/to_underlying.hpp>

#include <cassert>
#include <cstdint>
#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct CachedTag { };

enum class VariantID : int32_t {
	Default = 0,
	unspecified = -1,
};

[[nodiscard]] constexpr inline auto operator+(VariantID e) noexcept {
	return libv::to_underlying(e);
}

// -------------------------------------------------------------------------------------------------

enum class LoadingMode : int32_t {
	async = 0, // Resource is using its fallback value until loading is completed
	// sync = 1, // Blocks the return of the load function until the resource is loaded
	blocking = 2, // The beginning of the following frame is blocked until the resource is loaded
};

[[nodiscard]] constexpr inline std::string_view to_string(LoadingMode value) noexcept {
	switch (value) {
	case LoadingMode::async: return "async";
	case LoadingMode::blocking: return "blocking";
	}
	assert(false && "Invalid LoadingMode enum value");
	return "<<invalid-LoadingMode-enum-value>>";
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
