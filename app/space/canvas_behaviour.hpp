// Project: libv, File: app/space/canvas_behaviour.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>
#include <space/universe/ids.hpp>
#include "canvas.hpp"


namespace app {

// -------------------------------------------------------------------------------------------------

struct CanvasBehaviour {
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void bind_default_controls(libv::ctrl::Controls& controls);
	static std::optional<FleetID> getSelected(app::SpaceCanvas& ctx);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
