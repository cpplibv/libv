// File: pcui.hpp Author: Vader Created on 2017. augusztus 24., 22:32

#pragma once

// std
#include <filesystem>
#include <memory>


namespace libv {
namespace lua {

class State;

} // namespace lua
namespace ui {

// -------------------------------------------------------------------------------------------------

class UI;
class ComponentBase;

std::shared_ptr<ComponentBase> script_file(UI& ui, lua::State& lua, const std::filesystem::path& file);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
