// Project: libv, File: app/vm4_viewer/scene/camera_2.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/angle.hpp>
#include <libv/math/ease.hpp>
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

//struct GenericCameraInterface {
//	[[nodiscard]] mat4 projection(vec2 canvas_size) const noexcept;
//	[[nodiscard]] mat4 view() const noexcept;
//	[[nodiscard]] vec3 eye() const noexcept;
//	[[nodiscard]] vec3 direction() const noexcept;
//  [[nodiscard]] screen_picker picker(vec2 canvas_size);
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
struct BaseCameraOrbit {
protected:
	using float_type = float;
	using vec2 = libv::vec2_t<float_type>;
	using vec3 = libv::vec3_t<float_type>;
	using mat4 = libv::mat4_t<float_type>;
	using screen_picker = libv::screen_picker<float_type>;

private:
	static constexpr vec3 axis_up = {0, 0, 1};

private:
	vec3 orbit_point_{0, 0, 0};
	vec3 rotation_{0, 0, 0}; // roll, pitch, yaw in RAD on orbit
	float_type orbit_distance_ = 0;
	float_type near_ = 0.1f;
	float_type far_ = 1000.f;
	float_type fov_y_ = libv::deg_to_rad(75.0f);

public:
	[[nodiscard]] mat4 projection(vec2 canvas_size) const noexcept {
		return mat4::perspective(fov_y_, canvas_size.x / canvas_size.y, near_, far_);
	}

	[[nodiscard]] mat4 view() const noexcept {
		mat4 mat(1.f);

		// OpenGL default axes: Forward is Z-, Right is X+, Up is Y+
		mat.rotate(libv::deg_to_rad(90.f), vec3(0, 0, 1));
		mat.rotate(libv::deg_to_rad(90.f), vec3(0, 1, 0));
		// Custom default axes: Forward is X+, Right is Y-, Up is Z+

		mat.translate(vec3(orbit_distance_, 0, 0));
		mat.rotate(rotation_.x, vec3(1, 0, 0));
		mat.rotate(rotation_.y, vec3(0, 1, 0));
		mat.rotate(rotation_.z, vec3(0, 0, 1));
		mat.translate(-orbit_point_);

		return mat;
	}

	[[nodiscard]] vec3 eye() const noexcept {
		return orbit_point_ + direction() * -orbit_distance_;
	}

	[[nodiscard]] vec3 direction() const noexcept {
		const auto pitch = rotation_.y;
		const auto yaw = rotation_.z;
		const auto xy_len = std::cos(pitch);
		const auto x = xy_len * std::cos(yaw);
		const auto y = xy_len * std::sin(-yaw);
		const auto z = std::sin(pitch);
		return vec3(x, y, z);
	}

	screen_picker picker(vec2 canvas_size) {
		return screen_picker(projection(canvas_size) * view(), canvas_size);
	}

public:
	void look_at(vec3 eye, vec3 target) noexcept  {
		orbit_point_ = target;
		// TODO P5: libv.math: length_and_dir
//		const auto [distance, dir] = length_and_dir(eye - target);
		const auto diff = eye - target;
		const auto dir = diff.normalize_copy();
		rotation_.x = 0;
		rotation_.y = -std::asin(dir.z);
		rotation_.z = std::atan2(dir.y, dir.x);
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
		return rotation_;
	}
	constexpr inline void rotations(vec3 value) noexcept {
		rotation_ = value;
	}

public:
	void move_forward(float_type value) {
		orbit_point_.x += value * std::cos(rotation_.z);
		orbit_point_.y -= value * std::sin(rotation_.z);
	}
	void move_right(float_type value) {
		orbit_point_.x += value * std::cos(rotation_.z + libv::pi / 2.0f);
		orbit_point_.y -= value * std::sin(rotation_.z + libv::pi / 2.0f);
	}
	void move_up(float_type value) {
		orbit_point_.z += value;
	}

	void orbit_roll(float_type x) {
		rotation_.x += x;
	}
	void orbit_pitch(float_type x) {
		rotation_.y += x;
	}
	void orbit_yaw(float_type x) {
		rotation_.z += x;
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
