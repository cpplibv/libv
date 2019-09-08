// File:   quaternion.hpp Author: Vader Created on 09 September 2019, 10:40

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/constants.hpp>


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

template <typename Quaternion>
[[nodiscard]] inline auto toEulerAngles(Quaternion q) noexcept {
	using value_type = decltype(q.x);
	libv::vec3_t<value_type> angles;

	// roll (x-axis rotation)
	value_type sinr_cosp = value_type{2} * (q.w * q.x + q.y * q.z);
	value_type cosr_cosp = value_type{1} - value_type{2} * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	value_type sinp = value_type{2} * (q.w * q.y - q.z * q.x);
	if (std::fabs(sinp) >= value_type{1})
		angles.y = std::copysign(libv::PI_2<value_type>, sinp); // use 90 degrees if out of range
	else
		angles.y = std::asin(sinp);

	// yaw (z-axis rotation)
	value_type siny_cosp = value_type{2} * (q.w * q.z + q.x * q.y);
	value_type cosy_cosp = value_type{1} - value_type{2} * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
