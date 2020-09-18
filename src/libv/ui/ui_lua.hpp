// Project: libv.ui, File: src/libv/ui/ui_lua.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/lua/fwd.hpp>
// std
#include <filesystem>
#include <memory>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Component;
class UI;

Component script_file(UI& ui, lua::State& lua, const std::filesystem::path& file);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
