// Project: libv, File: app/star/game/scene/scene_settings.hpp

#pragma once

// libv
#include <libv/ui/component/component.hpp>
// pro
#include <star/game/fwd.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] libv::ui::Component createSceneControls(GameClient& gameClient);

// -------------------------------------------------------------------------------------------------

} // namespace star
