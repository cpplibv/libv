//

#pragma once

#include <libv/ctrl/fwd.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/nexus.hpp>

#include <star/game/control/requests.hpp> // fwd would be enough


namespace star {

// -------------------------------------------------------------------------------------------------

struct TimeControllerFeatures {
	bool pause = true;
	bool speedControl = true;
	bool slowMode = true;
	bool stepFrame = true;
	// .slowMode = devMode,
	// .stepFrame = devMode,
};

struct TimeController {
	bool paused = false;
	uint32_t simulationSpeedMode = 1;
	static constexpr float simulationSpeeds[] = { 0.f, 1.f, 2.f, 4.f };

	bool debugUnpauseNextFrame = false;
	bool debugSlowMode = false;
	int debugSlowModeCounter = 0;

	libv::time_point timeRealStart = libv::clock::now();
	libv::time_point timeRealFrame = timeRealStart;
	libv::time_duration_d timeSim{0};

	libv::Nexus nexus;

public:
	explicit TimeController(libv::Nexus nexus);
	~TimeController();

public:
	[[nodiscard]] CommandTimeSpeed currentCommandTimeSpeed() const;
	void request(RequestTimeTogglePause);
	void request(RequestTimeSpeed req);
	void request(RequestTimeDebugSlowMode req);
	void request(RequestTimeDebugStepFrame);

public:
	void update(libv::time_duration timeDelta);
	[[nodiscard]] libv::time_duration_d timeReal() const noexcept {
		return timeRealFrame - timeRealStart;
	}
	[[nodiscard]] float simulationSpeed() const noexcept {
		return simulationSpeeds[simulationSpeedMode];
	}

public:
	static void register_controls(libv::ctrl::Controls& controls, TimeControllerFeatures features = {});
	static void bind_default_controls(libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace star
