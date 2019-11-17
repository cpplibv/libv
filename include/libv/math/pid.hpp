// File:   pid.hpp Author: Vader Created on 17 November 2019, 03:47

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T = double>
class PID {
private:
	T Kp;
	T Kd;
	T Ki;
	T pre_error = 0.0;
	T integral = 0.0;

public:
	/// Kp - proportional gain (position correction)
	/// Kd - derivative gain (speed correction)
	/// Ki - integral gain (offset correction)
	constexpr inline PID(T Kp, T Kd, T Ki) noexcept :
		Kp(Kp),
		Kd(Kd),
		Ki(Ki) { }

	/// Returns the feedback value given the delta_time, current_value and target_value
	[[nodiscard]] constexpr inline T calculate(T delta_time, T current_value, T target_value) noexcept {
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
