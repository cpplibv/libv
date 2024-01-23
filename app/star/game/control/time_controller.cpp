// Project: libv, File: app/star/game/control/time_controller.cpp

#include <star/game/control/time_controller.hpp>

#include <libv/ctrl/controls.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

TimeController::TimeController(libv::Nexus nexus) : nexus(std::move(nexus)) {
}

TimeController::~TimeController() {
	nexus.disconnect_all(this);
}

// -------------------------------------------------------------------------------------------------

CommandTimeSpeed TimeController::currentCommandTimeSpeed() const{
	return CommandTimeSpeed{paused ? 0 : simulationSpeedMode};
}

void TimeController::request(RequestTimeTogglePause) {
	paused = !paused;
	nexus.broadcast_channel<CommandTimeSpeed>(this, currentCommandTimeSpeed());
}

void TimeController::request(RequestTimeSpeed req) {
	paused = false;
	simulationSpeedMode = std::min(req.mode, static_cast<uint32_t>(std::size(simulationSpeeds)));
	nexus.broadcast_channel<CommandTimeSpeed>(this, CommandTimeSpeed{req.mode});
}

void TimeController::request(RequestTimeDebugSlowMode req) {
	debugSlowMode = req.active;
	debugSlowModeCounter = 0;
}

void TimeController::request(RequestTimeDebugStepFrame) {
	debugUnpauseNextFrame = true;
}

// -------------------------------------------------------------------------------------------------

void TimeController::update(libv::time_point time, libv::time_duration_d delta) {
	this->timeRealFrame = time;

	if ((!paused && !debugSlowMode) || debugUnpauseNextFrame || (debugSlowMode && (++debugSlowModeCounter % 10) == 0)) {
		debugUnpauseNextFrame = false;
		deltaSim = delta * simulationSpeeds[simulationSpeedMode];
		timeSim += deltaSim;
	}
}

// -------------------------------------------------------------------------------------------------

void TimeController::register_controls(libv::ctrl::Controls& controls, TimeControllerFeatures features) {
	using ControlState = TimeController;

	const auto act = [&](bool enable, std::string&& name, auto&& reqProto) {
		if (enable) {
			controls.feature_action<ControlState>(std::move(name), [req = std::move(reqProto)](libv::ctrl::arg_action, ControlState& state) {
				state.request(req);
			});
		}
	};
	const auto bin = [&](bool enable, std::string&& name, auto&& fn) {
		if (enable) {
			controls.feature_binary<ControlState>(std::move(name), [f = std::move(fn)](libv::ctrl::arg_binary event, ControlState& state) {
				state.request(f(event.value));
			});
		}
	};

	act(features.pause, "time.pause-toggle", RequestTimeTogglePause{});
	act(features.speedControl, "time.speed1", RequestTimeSpeed{1});
	act(features.speedControl, "time.speed2", RequestTimeSpeed{2});
	act(features.speedControl, "time.speed3", RequestTimeSpeed{3});
	bin(features.slowMode, "time.debug.slow-mode", [](bool value) { return RequestTimeDebugSlowMode{value}; });
	act(features.stepFrame, "time.debug.step-frame", RequestTimeDebugStepFrame{});
}

void TimeController::bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("time.pause-toggle", "space");
	controls.bind("time.pause-toggle", "`");
	controls.bind("time.speed1", "1");
	controls.bind("time.speed2", "2");
	controls.bind("time.speed3", "3");
	controls.bind("time.debug.slow-mode", "B");
	controls.bind("time.debug.step-frame", "N");
}


// -------------------------------------------------------------------------------------------------

} // namespace star
