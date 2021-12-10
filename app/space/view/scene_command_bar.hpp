// Project: libv, File: app/space/view/scene_command_bar.hpp

#pragma once

// libv
#include <libv/ui/component/fwd.hpp>
#include <libv/utility/nexus_fwd.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class SceneCommandBar {
public:
	static libv::ui::Component create(libv::Nexus& nexus);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
