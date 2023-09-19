// Project: libv.ui, File: src/libv/ui/component_system/switch_scene.hpp

#pragma once

// std
#include <string_view>
// pro
#include <libv/ui/fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void switchParentScene(std::string_view sceneContainerID, const libv::ui::Component& component, libv::ui::Component newScene);

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
