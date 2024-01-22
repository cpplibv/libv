//

#pragma once

#include <libv/ctrl/fwd.hpp>
#include <libv/utility/nexus.hpp>

#include <star/game/fwd.hpp>
#include <star/game/control/requests.hpp> // fwd would be enough


namespace star {

// -------------------------------------------------------------------------------------------------

struct StarmapControllerFeatures {
	bool placeBall = true;
};

struct StarmapController {
	libv::Nexus nexus;
	Starmap* starmap = nullptr;
	CanvasStarmap* canvas = nullptr;

public:
	explicit StarmapController(libv::Nexus& nexus);
	void init(Starmap* starmap, CanvasStarmap* canvas);
	~StarmapController();

public:
	void request(RequestStarmapPlaceBallUnderCursor req);
	void request(RequestStarmapToggleGrid req);

public:
	static void register_controls(libv::ctrl::Controls& controls, StarmapControllerFeatures features = {});
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
