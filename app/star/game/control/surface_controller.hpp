//

#pragma once

#include <libv/ctrl/fwd.hpp>
#include <libv/utility/nexus.hpp>

#include <star/game/fwd.hpp>
#include <star/game/control/requests.hpp> // fwd would be enough


namespace star {

// -------------------------------------------------------------------------------------------------

struct SurfaceControllerFeatures {
	bool placeBall = true;
};

struct SurfaceController {
	libv::Nexus nexus;
	Surface* surface = nullptr;
	CanvasSurface* canvas = nullptr;

public:
	explicit SurfaceController(libv::Nexus& nexus);
	void init(Surface* surface, CanvasSurface* canvas);
	~SurfaceController();

public:
	void request(RequestSurfacePlaceBallUnderCursor req);

public:
	static void register_controls(libv::ctrl::Controls& controls, SurfaceControllerFeatures features = {});
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
