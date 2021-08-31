// Project: libv.ui, File: src/libv/ui/ui_lua.hpp, Author: Császár Mátyás [Vader]

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
