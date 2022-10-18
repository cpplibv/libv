// Project: libv, File: app/star/game/scene/scene_settings.hpp

#pragma once

// libv
#include <libv/ui/component/component.hpp>
#include <libv/utility/nexus_fwd.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] libv::ui::Component createSceneControls(libv::Nexus& nexus);
[[nodiscard]] libv::ui::Component createSceneMainMenu(libv::Nexus& nexus);
[[nodiscard]] libv::ui::Component createSceneSettings(libv::Nexus& nexus);
[[nodiscard]] libv::ui::Component createSceneSurface(libv::Nexus& nexus);

// -------------------------------------------------------------------------------------------------

} // namespace star
