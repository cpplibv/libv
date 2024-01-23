// Project: libv, File: app/star/game/scene/scene_sp_session.hpp

#pragma once

#include <libv/ui/component/scene_container.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/nexus_fwd.hpp>

#include <star/game/fwd.hpp>
#include <star/game/sim/universe.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class SceneSPSession {
	Universe universe;

	TimeController_sp timeController;

	libv::ui::SceneContainer gsc;

public:
	explicit SceneSPSession(libv::Nexus& nexus);

public:
	void controls(ControllerContext& ctx);
	void update(libv::time_point time, libv::time_duration_d delta);
	[[nodiscard]] libv::ui::Component createScene(libv::Nexus& nexus);

private:
	[[nodiscard]] libv::ui::Component createInfoBar(libv::Nexus& nexus);
	[[nodiscard]] libv::ui::Component createMenu(libv::Nexus& nexus);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
