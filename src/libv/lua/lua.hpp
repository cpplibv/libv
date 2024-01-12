// Project: libv.lua, File: src/libv/lua/lua.hpp

#pragma once

#include <libv/lua/fwd.hpp>

#include <sol/forward.hpp>

#include <libv/utility/to_underlying.hpp>

#include <cstdint>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

enum class lib : uint32_t {
	std_base = 1u << 0u,
	std_coroutine = 1u << 1u,
	std_string = 1u << 2u,
	std_math = 1u << 3u,
	std_table = 1u << 4u,
	std_bit32 = 1u << 5u,
	std_utf8 = 1u << 6u,

	basic = std_base | std_coroutine | std_string | std_math | std_table | std_bit32 | std_utf8,

	std_debug = 1u << 7u,
	std_package = 1u << 8u,
	std_io = 1u << 9u,
	std_os = 1u << 10u,

	std = basic | std_debug | std_package | std_io | std_os,

	jit_ffi = 1u << 11u,
	jit_jit = 1u << 12u,

	// _ = 1u << 13u,
	// _ = 1u << 14u,
	// _ = 1u << 15u,

	libv_vec = 1u << 16u,
	// libv_vm4 = (1u << 17u) | libv_vec,
	// libv_ui = (1u << 18u) | libv_vec,
};

[[nodiscard]] constexpr inline lib operator|(const lib& lhs, const lib& rhs) noexcept {
	return lib{libv::to_underlying(lhs) | libv::to_underlying(rhs)};
}

[[nodiscard]] constexpr inline lib operator&(const lib& lhs, const lib& rhs) noexcept {
	return lib{libv::to_underlying(lhs) & libv::to_underlying(rhs)};
}

// -------------------------------------------------------------------------------------------------

sol::state create_state(lib libmask);
void open_libraries(sol::state& lua, lib libmask);

// -------------------------------------------------------------------------------------------------

} // namespace lua
} // namespace libv
