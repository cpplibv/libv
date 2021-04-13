// Project: libv, File: app/vm4_viewer/scene/camera_2.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/angle.hpp>
//#include <libv/math/ease.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/screen_picker.hpp>
// ext
//#include <glm/ext/quaternion_transform.hpp>
//#include <glm/gtx/easing.hpp>
// std
#include <chrono>
#include <cmath>
// pro
//#include <vm4_viewer/scene/object.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

// TODO P2: (?) Calculate camera 'GenericCameraInterface' properties once every frame

//struct GenericCameraInterface {
//	[[nodiscard]] mat4 projection(vec2 canvas_size) const noexcept;
//	[[nodiscard]] mat4 view() const noexcept;
//	[[nodiscard]] vec3 eye() const noexcept;
//	[[nodiscard]] vec3 direction() const noexcept;
//  [[nodiscard]] screen_picker picker(vec2 canvas_size) const noexcept;
//};

// -------------------------------------------------------------------------------------------------

/// At the 0,0,0 state the camera directions are:
///      Forward  is  X+
///       Right   is  Y-
///        Up     is  Z+
///
/// In orbit mode the camera axes are slaved under each other:
///     Z   -> Y     -> X
///     Yaw -> Pitch -> Roll
///
/// Roll - A positive rolling motion lifts the left wing and lowers the right wing.
/// Pitch - A positive pitching motion raises the nose of the aircraft and lowers the tail.
/// Yaw - A positive yawing motion moves the nose of the aircraft to the right.
///
struct BaseCameraOrbit {
protected:
	using float_type = float;
	using vec2 = libv::vec2_t<float_type>;
	using vec3 = libv::vec3_t<float_type>;
	using mat4 = libv::mat4_t<float_type>;
	using screen_picker = libv::screen_picker<float_type>;

private:
	static constexpr vec3 axis_forward = {1, 0, 0};
	static constexpr vec3 axis_right = {0, -1, 0};
	static constexpr vec3 axis_up = {0, 0, 1};

private:
	vec3 orbit_point_{0, 0, 0};
	float_type roll_ = 0;
	float_type pitch_ = 0;
	float_type yaw_ = 0;
	float_type orbit_distance_ = 0;
	float_type near_ = 0.1f;
	float_type far_ = 1000.f;
	float_type fov_y_ = libv::deg_to_rad(75.0f);

public:
	[[nodiscard]] mat4 projection(vec2 canvas_size) const noexcept {
		return mat4::perspective(fov_y_, canvas_size.x / canvas_size.y, near_, far_);
	}

	[[nodiscard]] mat4 view() const noexcept {
		mat4 mat = mat4::identity();

		// OpenGL default axes: Forward is Z-, Right is X+, Up is Y+
		mat.rotate(libv::deg_to_rad(90.f), vec3(0, 0, 1));
		mat.rotate(libv::deg_to_rad(90.f), vec3(0, 1, 0));
		// Corrected camera axes: Forward is X+, Right is Y-, Up is Z+

		mat.translate(vec3(orbit_distance_, 0, 0));
		mat.rotate(-roll_, axis_forward);
		mat.rotate(-pitch_, axis_right);
		mat.rotate(yaw_, axis_up);
		mat.translate(-orbit_point_);

		return mat;
	}

	[[nodiscard]] vec3 eye() const noexcept {
		return orbit_point_ + direction() * -orbit_distance_;
	}

	[[nodiscard]] vec3 direction() const noexcept {
		const auto xy_len = std::cos(pitch_);
		const auto x = xy_len * std::cos(-yaw_); // Yaw is CW but math is CCW
		const auto y = xy_len * std::sin(-yaw_); // Yaw is CW but math is CCW
		const auto z = std::sin(pitch_);
		return vec3(x, y, z);
	}

	[[nodiscard]] screen_picker picker(vec2 canvas_size) const noexcept {
		return screen_picker(projection(canvas_size) * view(), canvas_size);
	}

public:
	void look_at(vec3 eye, vec3 target) noexcept  {
		orbit_point_ = target;
		// TODO P5: libv.math: length_and_dir
		// TODO P1: if eye == target or nan infection -> default cam in eye coord
//		const auto [distance, dir] = length_and_dir(target - eye);
		const auto diff = target - eye;
		const auto dir = diff.normalize_copy();
		roll_ = 0;
		pitch_ = std::asin(dir.z);
		yaw_ = -std::atan2(dir.y, dir.x); // Yaw is CW but atan2 is CCW
		orbit_distance_ = diff.length();
	}

public:
	[[nodiscard]] constexpr inline float_type near() const noexcept {
		return near_;
	}
	constexpr inline void near(float_type value) noexcept {
		near_ = value;
	}
	[[nodiscard]] constexpr inline float_type far() const noexcept {
		return far_;
	}
	constexpr inline void far(float_type value) noexcept {
		far_ = value;
	}
	[[nodiscard]] constexpr inline float_type fov_y() const noexcept {
		return fov_y_;
	}
	constexpr inline void fov_y(float_type value) noexcept {
		fov_y_ = value;
	}
	[[nodiscard]] constexpr inline float_type roll() const noexcept {
		return roll_;
	}
	constexpr inline void roll(float_type value) noexcept {
		roll_ = value;
	}
	[[nodiscard]] constexpr inline float_type pitch() const noexcept {
		return pitch_;
	}
	constexpr inline void pitch(float_type value) noexcept {
		pitch_ = value;
	}
	[[nodiscard]] constexpr inline float_type yaw() const noexcept {
		return yaw_;
	}
	constexpr inline void yaw(float_type value) noexcept {
		yaw_ = value;
	}
	[[nodiscard]] constexpr inline float_type orbit_distance() const noexcept {
		return orbit_distance_;
	}
	constexpr inline void orbit_distance(float_type value) noexcept {
		orbit_distance_ = value;
	}

	[[nodiscard]] constexpr inline vec3 orbit_point() const noexcept {
		return orbit_point_;
	}
	constexpr inline void orbit_point(vec3 value) noexcept {
		orbit_point_ = value;
	}
	[[nodiscard]] constexpr inline vec3 rotations() const noexcept {
		return vec3(roll_, pitch_, yaw_);
	}
	constexpr inline void rotations(vec3 value) noexcept {
		roll_ = value.x;
		pitch_ = value.y;
		yaw_ = value.z;
	}

public:
	/// Moves the camera on the XY plane based on cameras view direction to forward
	void move_forward(float_type value) {
		// Yaw is CW but math is CCW
		orbit_point_.x += value * std::cos(-yaw_);
		orbit_point_.y += value * std::sin(-yaw_);
	}
	/// Moves the camera on the XY plane based on cameras view direction to right
	void move_right(float_type value) {
		// Yaw is CW but math is CCW
		orbit_point_.x += value * std::cos(-yaw_ - libv::pi / 2.0f);
		orbit_point_.y += value * std::sin(-yaw_ - libv::pi / 2.0f);
	}
	/// Moves the camera on the Z axis
	void move_up(float_type value) {
		orbit_point_.z += value;
	}

	/// Roll - A positive rolling motion lifts the left 'wing' and lowers the right 'wing'.
	void orbit_roll(float_type x) {
		roll_ += x;
	}
	/// Pitch the camera around the orbit point
	/// A positive pitching motion raises the nose of the 'aircraft' and lowers the 'tail'.
	void orbit_pitch(float_type x) {
		pitch_ += x;
	}
	/// Yaws the camera around the orbit point
	/// A positive yawing motion moves the nose of the 'aircraft' to the right.
	void orbit_yaw(float_type x) {
		yaw_ += x;
	}
};

// =================================================================================================

struct CameraPlayer : BaseCameraOrbit {
private:
	using duration = std::chrono::duration<float_type, std::chrono::seconds::period>;

private:
	float inclination_hard_min_ = libv::deg_to_rad(5.0f);
	float inclination_soft_min_ = libv::deg_to_rad(15.0f);
	float inclination_soft_max_ = libv::deg_to_rad(90.0f - 15.0f);
	float inclination_hard_max_ = libv::deg_to_rad(90.0f - 5.0f);

	vec3 warp_target;
	float_type warp_edge_speed;
	float_type warp_distance_threshold_to_orbit_distance_ratio = 0.5f;
	duration warp_ease_out_duration = duration{0.2};
	duration warp_ease_in_duration = duration{0.2};

public:
	void update(duration delta_time) {
		// warp: out - move - in
		// inclination correction
	}

	void warp_to(vec3 target) {
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
};

// -------------------------------------------------------------------------------------------------

struct CameraDeveloper : BaseCameraOrbit {
private:
	using duration = std::chrono::duration<float, std::chrono::seconds::period>;

public:
	void update(duration delta_time) {
		// nothing
	}

	void warp_to(vec3 target) {
		// instant
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
