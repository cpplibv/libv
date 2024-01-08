//

#include <star/game/control/time_control.hpp>

#include <libv/ctrl/controls.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

TimeControl::TimeControl(libv::Nexus nexus) : nexus(std::move(nexus)) {
}

TimeControl::~TimeControl() {
	nexus.disconnect_channel_all(this);
}

// -------------------------------------------------------------------------------------------------

CommandTimeSpeed TimeControl::currentCommandTimeSpeed() const{
	return CommandTimeSpeed{paused ? 0 : simulationSpeedMode};
}

void TimeControl::request(RequestTimeTogglePause) {
	paused = !paused;
	nexus.broadcast_channel<CommandTimeSpeed>(this, currentCommandTimeSpeed());
}

void TimeControl::request(RequestTimeSpeed req) {
	paused = false;
	simulationSpeedMode = std::min(req.mode, static_cast<uint32_t>(std::size(simulationSpeeds)));
	nexus.broadcast_channel<CommandTimeSpeed>(this, CommandTimeSpeed{req.mode});
}

void TimeControl::request(RequestTimeDebugSlowMode req) {
	debugSlowMode = req.active;
	debugSlowModeCounter = 0;
}

void TimeControl::request(RequestTimeDebugStepFrame) {
	debugUnpauseNextFrame = true;
}

// -------------------------------------------------------------------------------------------------

void TimeControl::update(libv::time_duration timeDelta) {
	// !!!
	// this->timeRealFrame = ui().state.time_frame();
	this->timeRealFrame = libv::clock::now();

	if ((!paused && !debugSlowMode) || debugUnpauseNextFrame || (debugSlowMode && (++debugSlowModeCounter % 10) == 0)) {
		debugUnpauseNextFrame = false;
		timeSim += timeDelta * simulationSpeeds[simulationSpeedMode];
	}
}

// -------------------------------------------------------------------------------------------------

void TimeControl::register_controls(libv::ctrl::Controls& controls, TimeControlFeatures features) {
	using ControlState = TimeControl;

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

void TimeControl::bind_default_controls(libv::ctrl::Controls& controls) {
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
