// File:   pid.hpp Author: Vader Created on 17 November 2019, 03:47

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

///
///	libv::PID<double> pid(0.25, 0.01, 0.5);
///
///	while(true) {
///		double target_value = ...;
///		double current_value = ...;
///		double delta_time = ...;
///		double feedback = pid.calculate(delta_time, current_value, target_value);
///		apply_feedback(feedback);
/// }
///
template <typename T = double, typename S = double>
class PID {
private:
	S Kp;
	S Kd;
	S Ki;
	T pre_error{};
	T integral{};

public:
	/// Kp - proportional gain (position correction)
	/// Kd - derivative gain (speed correction)
	/// Ki - integral gain (offset correction)
	constexpr inline PID(S Kp, S Kd, S Ki) noexcept :
		Kp(Kp),
		Kd(Kd),
		Ki(Ki) { }

	/// Returns the feedback value given the delta_time, current_value and target_value
	template <typename Time>
	[[nodiscard]] constexpr inline T calculate(Time delta_time, T current_value, T target_value) noexcept {
		const auto error = target_value - current_value;
		integral += error * delta_time;
		const auto derivative = (error - pre_error) / delta_time;
		const auto output = Kp * error + Ki * integral + Kd * derivative;
		pre_error = error;
		return output;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
