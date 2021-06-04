// Project: libv, File: app/space/camera.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/camera.hpp>
// libv
//#include <libv/math/ease.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

BaseCameraOrbit::mat4 BaseCameraOrbit::projection(vec2 canvas_size) const noexcept {
	return mat4::perspective(fov_y_, canvas_size.x / canvas_size.y, near_, far_);
}

BaseCameraOrbit::mat4 BaseCameraOrbit::view() const noexcept {
	mat4 mat = base_camera_orientation; // Forward is X+, Right is Y-, Up is Z+

	mat.translate(vec3(orbit_distance_, 0, 0));
	mat.rotate(-roll_, axis_forward);
	mat.rotate(-pitch_, axis_right);
	mat.rotate(yaw_, axis_up);
	mat.translate(-orbit_point_);

	return mat;
}

BaseCameraOrbit::mat4 BaseCameraOrbit::orientation() const noexcept {
	mat4 mat = base_camera_orientation; // Forward is X+, Right is Y-, Up is Z+

	mat.rotate(-roll_, axis_forward);
	mat.rotate(-pitch_, axis_right);
	mat.rotate(yaw_, axis_up);

	return mat;
}

BaseCameraOrbit::vec3 BaseCameraOrbit::eye() const noexcept {
	return orbit_point_ + direction() * -orbit_distance_;
}

BaseCameraOrbit::vec3 BaseCameraOrbit::direction() const noexcept {
	const auto xy_len = std::cos(pitch_);
	const auto x = xy_len * std::cos(-yaw_); // Yaw is CW but math is CCW
	const auto y = xy_len * std::sin(-yaw_); // Yaw is CW but math is CCW
	const auto z = std::sin(pitch_);
	return vec3(x, y, z);
}

BaseCameraOrbit::screen_picker BaseCameraOrbit::picker(vec2 canvas_size) const noexcept {
	return screen_picker(projection(canvas_size) * view(), canvas_size);
}

void BaseCameraOrbit::look_at(vec3 eye, vec3 target) noexcept  {
	orbit_point_ = target;

	if (target == eye) {
		roll_ = 0;
		pitch_ = 0;
		yaw_ = 0;
		orbit_distance_ = 0;
		return;
	}

	// TODO P5: libv.math: length_and_dir
//	const auto [distance, dir] = length_and_dir(target - eye);
	const auto diff = target - eye;
	const auto dir = diff.normalize_copy();
	roll_ = 0;
	pitch_ = std::asin(dir.z);
	yaw_ = -std::atan2(dir.y, dir.x); // Yaw is CW but atan2 is CCW
	orbit_distance_ = diff.length();
}

// =================================================================================================

void CameraPlayer::update(duration delta_time) {
	// warp: out - move - in
	// inclination correction
}

void CameraPlayer::warp_to(vec3 target) {
	// IDEA: Not constant camera direction but a change in the camera direction during warp.
	//      So eye and target are two separate ease function.
	//      The eye ease should be a slower ease in and should never really impact more than 2-5°

//		const auto warp_distance_threshold = float_type{2.0};
//		const auto warp_ease_out_duration = duration{0.2};
//		const auto warp_ease_in_duration = duration{0.2};

//		const auto v = libv::math::ease_in_cubic(x);
//		const auto v = libv::math::ease_in_out_cubic(x);
//		const auto v = libv::math::ease_out_cubic(x);
}

//	void warp_to(vec3 eye, vec3 target);
//	void shake(float_type magnitude, duration time) {}
//	void shake(vec3 magnitude, duration time) {}

// -------------------------------------------------------------------------------------------------

void CameraDeveloper::update(duration delta_time) {
	// nothing
}

void CameraDeveloper::warp_to(vec3 target) {
	// instant
}

// -------------------------------------------------------------------------------------------------

} // namespace app
