// Project: libv, File: app/space/camera.cpp

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

BaseCameraOrbit::mat4 BaseCameraOrbit::orientation_view() const noexcept {
	mat4 mat = base_camera_orientation; // Forward is X+, Right is Y-, Up is Z+

	mat.rotate(-roll_, axis_forward);
	mat.rotate(-pitch_, axis_right);
	mat.rotate(yaw_, axis_up);

	return mat;
}

BaseCameraOrbit::mat4 BaseCameraOrbit::orientation_universe() const noexcept {
	mat4 mat = mat4::identity();

	mat.rotate(-roll_, axis_forward);
	mat.rotate(-pitch_, axis_right);
	mat.rotate(yaw_, axis_up);

	return mat;
}

BaseCameraOrbit::vec3 BaseCameraOrbit::eye() const noexcept {
	return orbit_point_ + forward() * -orbit_distance_;
}

BaseCameraOrbit::vec3 BaseCameraOrbit::forward() const noexcept {
	const auto sP = std::sin(pitch_);
	const auto cP = std::cos(pitch_);
	const auto sY = std::sin(-yaw_); // Yaw is CW but math is CCW
	const auto cY = std::cos(-yaw_);

	const auto x = cY * cP;
	const auto y = sY * cP;
	const auto z = sP;

	return {x, y, z};
}

BaseCameraOrbit::vec3 BaseCameraOrbit::right() const noexcept {
	const auto sR = std::sin(roll_);
	const auto cR = std::cos(roll_);
	const auto sP = std::sin(pitch_);
	const auto cP = std::cos(pitch_);
	const auto sY = std::sin(-yaw_); // Yaw is CW but math is CCW
	const auto cY = std::cos(-yaw_);

	const auto x = sY * cR + cY * sP * sR;
	const auto y = -cY * cR + sY * sP * sR;
	const auto z = -cP * sR;

	return {x, y, z};
}

BaseCameraOrbit::vec3 BaseCameraOrbit::up() const noexcept {
	const auto sR = std::sin(roll_);
	const auto cR = std::cos(roll_);
	const auto sP = std::sin(pitch_);
	const auto cP = std::cos(pitch_);
	const auto sY = std::sin(-yaw_); // Yaw is CW but math is CCW
	const auto cY = std::cos(-yaw_);

	const auto x = sY * sR - cY * sP * cR;
	const auto y = -cY * sR - sY * sP * cR;
	const auto z = cP * cR;

	return {x, y, z};
}

BaseCameraOrbit::screen_picker BaseCameraOrbit::picker(vec2 canvas_size) const noexcept {
	return {projection(canvas_size) * view(), canvas_size};
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

	const auto [length, dir] = (target - eye).length_and_dir();
	roll_ = 0;
	pitch_ = std::asin(dir.z);
	yaw_ = -std::atan2(dir.y, dir.x); // Yaw is CW but atan2 is CCW
	orbit_distance_ = length;
}

// =================================================================================================

void CameraPlayer::update(duration delta_time) {
	(void) delta_time;

	// TODO P2: Camera movement over time: warp: out - move - in
	// shake
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
	orbit_point(target);
}

//	void warp_to(vec3 eye, vec3 target);
//	void shake(float_type magnitude, duration time) {}
//	void shake(vec3 magnitude, duration time) {}

// -------------------------------------------------------------------------------------------------

void CameraDeveloper::update(duration delta_time) {
	// Nothing: No screen shake, no warp animation, Dev cam is fixed
	(void) delta_time;
}

void CameraDeveloper::warp_to(vec3 target) {
	orbit_point(target);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
