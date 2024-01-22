// Project: libv, File: app/star/game/scene/scenes.hpp

#pragma once

#include <libv/ui/component/component.hpp>
#include <libv/utility/nexus_fwd.hpp>

#include <star/game/fwd.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

// Gameplay scenes
[[nodiscard]] libv::ui::Component createSceneSPSession(libv::Nexus& nexus);
[[nodiscard]] libv::ui::Component createSceneStarmap(libv::Nexus& nexus, Starmap_sp starmap, TimeController_sp timeController);
[[nodiscard]] libv::ui::Component createSceneSurface(libv::Nexus& nexus, Surface_sp surface, TimeController_sp timeController);

// Menu scenes
[[nodiscard]] libv::ui::Component createSceneMainMenu(libv::Nexus& nexus);
[[nodiscard]] libv::ui::Component createSceneSettings(libv::Nexus& nexus);
[[nodiscard]] libv::ui::Component createSceneControls(libv::Nexus& nexus);
[[nodiscard]] libv::ui::Component createSceneCredits(libv::Nexus& nexus);

// -------------------------------------------------------------------------------------------------

} // namespace star
