//

#include <libv/ctrl/binding_register.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/utility/chrono.hpp>
// #include <libv/ctrl/fwd.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct TimeControlFeatures {
	bool pause = true;
	bool slowMode = true;
	bool stepFrame = true;
	// .slowMode = devMode,
	// .stepFrame = devMode,
};

struct TimeControl {
	bool paused = false;
	// float simulationSpeed = 1.f;

	bool debugUnpauseNextFrame = false;
	bool debugSlowMode = false;
	int debugSlowModeCounter = 0;

	libv::time_point timeRealStart = libv::clock::now();
	libv::time_point timeRealFrame = timeRealStart;
	libv::time_duration_d timeSim{0};

public:
	void update(libv::ui::time_duration timeDelta) {
		// !!!
		// this->timeRealFrame = ui().state.time_frame();
		this->timeRealFrame = libv::clock::now();

		if ((!paused && !debugSlowMode) || debugUnpauseNextFrame || (debugSlowMode && (++debugSlowModeCounter % 10) == 0)) {
			debugUnpauseNextFrame = false;
			timeSim += timeDelta;
		}
	}
	[[nodiscard]] libv::time_duration_d timeReal() const noexcept {
		return timeRealFrame - timeRealStart;
	}

public:
	static void register_controls(libv::ctrl::FeatureRegister controls, TimeControlFeatures features = {}) {
		using ControlState = TimeControl;

		const auto act = [&](bool enable, std::string&& name, auto&& fn) {
			if (enable) {
				controls.feature_action<ControlState>(std::move(name), [f = std::move(fn)](libv::ctrl::arg_action, ControlState& state) {
					f(state);
				});
			}
		};
		const auto bin = [&](bool enable, std::string&& name, auto&& fn) {
			if (enable) {
				controls.feature_binary<ControlState>(std::move(name), [f = std::move(fn)](libv::ctrl::arg_binary event, ControlState& state) {
					f(state, event.value);
				});
			}
		};

		act(features.pause, "time.pause", [](TimeControl& state) {
			state.paused = !state.paused;
		});
		bin(features.slowMode, "time.debug.slow-mode", [](TimeControl& state, bool value) {
			state.debugSlowMode = value;
			state.debugSlowModeCounter = 0;
		});
		act(features.stepFrame, "time.debug.step-frame", [](TimeControl& state) {
			state.debugUnpauseNextFrame = true;
		});
	}

	static void bind_default_controls(libv::ctrl::BindingRegister controls) {
		controls.bind("time.pause", "space");
		controls.bind("time.debug.slow-mode", "B");
		controls.bind("time.debug.step-frame", "N");
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace star
