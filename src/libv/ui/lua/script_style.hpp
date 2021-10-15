// Project: libv.lua, File: src/libv/ui/lua/script_style.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/lua/fwd.hpp>
// std
#include <string_view>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void script_style(UI& ui, lua::State& lua, const std::string_view script);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
