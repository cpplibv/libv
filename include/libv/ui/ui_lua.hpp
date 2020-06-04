// File: pcui.hpp Author: Vader Created on 2017. augusztus 24., 22:32

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
