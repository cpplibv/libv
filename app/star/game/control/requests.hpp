// Project: libv, File: app/star/game/control/requests.hpp

#pragma once

#include <cstdint>


namespace star {

// -------------------------------------------------------------------------------------------------

struct RequestClientExit {
	[[nodiscard]] inline bool operator==(const RequestClientExit& other) const noexcept = default;
};

// -------------------------------------------------------------------------------------------------

struct RequestTimeTogglePause {
};

struct RequestTimeSpeed {
	uint32_t mode = 1; /// 1, 2, 3
};

struct RequestTimeDebugSlowMode {
	bool active = false;
};

struct RequestTimeDebugStepFrame {
};

struct CommandTimeSpeed {
	uint32_t speed = 0; /// 0, 1, 2, 3
	[[nodiscard]] inline bool operator==(const CommandTimeSpeed& other) const noexcept = default;
};

// -------------------------------------------------------------------------------------------------

struct RequestSurfacePlaceBallUnderCursor {
};
struct RequestSurfaceSkyNext {
};
struct RequestSurfaceSkyPrev {
};
struct RequestSurfaceToggleGrid {
};

struct RequestStarmapPlaceBallUnderCursor {
};
struct RequestStarmapSkyNext {
};
struct RequestStarmapSkyPrev {
};
struct RequestStarmapToggleGrid {
};

// -------------------------------------------------------------------------------------------------

} // namespace star
