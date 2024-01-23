// Project: libv, File: app/star/game/control/surface_controller.hpp

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

	int testSkyIndex = -1;

public:
	explicit SurfaceController(libv::Nexus& nexus);
	void init(Surface* surface, CanvasSurface* canvas);
	~SurfaceController();

private:
	void setSky(int index);

public:
	void request(RequestSurfacePlaceBallUnderCursor req);
	void request(RequestSurfaceSkyNext req);
	void request(RequestSurfaceSkyPrev req);
	void request(RequestSurfaceToggleGrid req);

public:
	static void register_controls(libv::ctrl::Controls& controls, SurfaceControllerFeatures features = {});
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
