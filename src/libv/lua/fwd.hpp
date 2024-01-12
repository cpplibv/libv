// Project: libv.lua, File: src/libv/lua/fwd.hpp

#pragma once

// std
#include <cstdint>


namespace libv {
namespace lua {

// -------------------------------------------------------------------------------------------------

enum class lib : uint32_t;

} // namespace lua ---------------------------------------------------------------------------------

using lualib = libv::lua::lib;

// -------------------------------------------------------------------------------------------------

} // namespace libv
