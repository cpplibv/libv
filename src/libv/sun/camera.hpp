// Project: libv.sun, File: src/libv/sun/camera.hpp

#pragma once

// libv
#include <libv/math/angle.hpp>
#include <libv/math/frustum.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/screen_picker.hpp>
// std
#include <chrono>
#include <cmath>


namespace libv {
namespace sun {

// -------------------------------------------------------------------------------------------------

// TODO P2: (?) Calculate camera 'GenericCameraInterface' properties once every frame

//struct GenericCameraInterface {
//	[[nodiscard]] mat4 projection(vec2 canvas_size) const noexcept;
//	[[nodiscard]] mat4 view() const noexcept;
//	[[nodiscard]] vec3 eye() const noexcept;
//	[[nodiscard]] vec3 direction() const noexcept;
//	[[nodiscard]] float near() const noexcept;
//	[[nodiscard]] float far() const noexcept;
//  [[nodiscard]] screen_picker picker(vec2 canvas_size) const noexcept;
//  [[nodiscard]] frustum frustum() const noexcept;
//	Maybe:
//	[[nodiscard]] mat4 orientation() const noexcept; /// View without the orbit and the position translates
//	[[nodiscard]] vec3 up() const noexcept;
//	[[nodiscard]] vec3 forward() const noexcept;
//	[[nodiscard]] vec3 right() const noexcept;
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
/// Roll - A positive rolling motion lifts the left wing and lowers the right wing. (CCW)
/// Pitch - A positive pitching motion raises the nose of the aircraft and lowers the tail. (CCW)
/// Yaw - A positive yawing motion moves the nose of the aircraft to the right. (CW)
///
struct BaseCameraOrbit {
public:
	using float_type = float;
	using vec2 = libv::vec2_t<float_type>;
	using vec3 = libv::vec3_t<float_type>;
	using mat4 = libv::mat4_t<float_type>;
	using screen_picker = libv::screen_picker<float_type>;

private:
	static constexpr vec3 axis_forward = {1, 0, 0};
	static constexpr vec3 axis_right = {0, -1, 0};
	static constexpr vec3 axis_up = {0, 0, 1};

	/// OpenGL axes: Forward is Z-, Right is X+, Up is Y+
	/// Camera axes: Forward is X+, Right is Y-, Up is Z+
//	static constexpr mat4 base_camera_orientation = mat4(
//		-axis_up, 0,
//		-axis_forward, 0,
//		-axis_right, 0,
//		0, 0, 0, 1
//	);
	static constexpr mat4 base_camera_orientation = mat4(
			 0,  0, -1,  0, // Map Z- to X+
			-1,  0,  0,  0, // Map X+ to Y-
			 0,  1,  0,  0, // Map Y+ to Z+
			 0,  0,  0,  1  //
	);

private:
	vec3 orbit_point_{0, 0, 0};
	float_type orbit_distance_ = 0;

	float_type roll_ = 0;
	float_type pitch_ = 0;
	float_type yaw_ = 0;

	float_type near_ = 0.01f;
	float_type far_ = 1000.f;
	float_type fov_y_ = libv::deg_to_rad(75.0f);

public:
	[[nodiscard]] mat4 projection(vec2 canvas_size) const noexcept;
	[[nodiscard]] mat4 projectionRevereZ(vec2 canvas_size) const noexcept;
	[[nodiscard]] mat4 projectionRevereZInf(vec2 canvas_size) const noexcept;
	[[nodiscard]] mat4 view() const noexcept;
	/// View without the orbit and the position translates
	/// Includes the OpenGL base_camera_orientation so this version is useful for OpenGL operations
	[[nodiscard]] mat4 orientation_view() const noexcept;
	/// View without the orbit and the position translates
	/// Does not includes the OpenGL base_camera_orientation so this version is useful for Universe operations
	[[nodiscard]] mat4 orientation_universe() const noexcept;
	[[nodiscard]] vec3 eye() const noexcept;

	[[nodiscard]] vec3 forward() const noexcept;
	[[nodiscard]] vec3 right() const noexcept;
	[[nodiscard]] vec3 up() const noexcept;

	[[nodiscard]] screen_picker picker(vec2 canvas_size) const noexcept;
	[[nodiscard]] libv::frustum frustum(vec2 canvas_size) const noexcept;
	[[nodiscard]] libv::frustum_culler_inf frustum_culler(vec2 canvasSize) const noexcept;

public:
	void look_at(vec3 eye, vec3 target) noexcept;

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
		return {roll_, pitch_, yaw_};
	}
	constexpr inline void rotations(vec3 value) noexcept {
		roll_ = value.x;
		pitch_ = value.y;
		yaw_ = value.z;
	}

public:
	/// Moves the camera on the XY plane based on cameras view direction to forward
	constexpr inline void move_forward(float_type value) noexcept {
		// Yaw is CW but math is CCW
		orbit_point_.x += value * std::cos(-yaw_);
		orbit_point_.y += value * std::sin(-yaw_);
	}
	/// Moves the camera on the XY plane based on cameras view direction to right
	constexpr inline void move_right(float_type value) noexcept {
		// Yaw is CW but math is CCW
		orbit_point_.x += value * std::cos(-yaw_ - libv::pi / 2.0f);
		orbit_point_.y += value * std::sin(-yaw_ - libv::pi / 2.0f);
	}
	/// Moves the camera on the Z axis
	constexpr inline void move_up(float_type value) noexcept {
		orbit_point_.z += value;
	}

	/// Roll - A positive rolling motion lifts the left 'wing' and lowers the right 'wing'.
	constexpr inline void orbit_roll(float_type x) noexcept {
		roll_ += x;
	}
	/// Pitch the camera around the orbit point
	/// A positive pitching motion raises the nose of the 'aircraft' and lowers the 'tail'.
	constexpr inline void orbit_pitch(float_type x) noexcept {
		pitch_ += x;
	}
	/// Yaws the camera around the orbit point
	/// A positive yawing motion moves the nose of the 'aircraft' to the right.
	constexpr inline void orbit_yaw(float_type x) noexcept {
		yaw_ += x;
	}
};

// =================================================================================================

struct CameraPlayer : public BaseCameraOrbit {
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
	void update(duration delta_time);
	void warp_to(vec3 target);
//	void warp_to(vec3 eye, vec3 target);
//	void shake(float_type magnitude, duration time) {}
//	void shake(vec3 magnitude, duration time) {}
};

// -------------------------------------------------------------------------------------------------

struct CameraDeveloper : public BaseCameraOrbit {
private:
	using duration = std::chrono::duration<float, std::chrono::seconds::period>;

public:
	void update(duration delta_time);
	void warp_to(vec3 target);
//	void warp_to(vec3 eye, vec3 target);
//	void shake(float_type magnitude, duration time) {}
//	void shake(vec3 magnitude, duration time) {}
};

// -------------------------------------------------------------------------------------------------

struct CameraOrtho {
public:
	using float_type = float;
	using vec2 = libv::vec2_t<float_type>;
	using vec3 = libv::vec3_t<float_type>;
	using mat4 = libv::mat4_t<float_type>;
	using screen_picker = libv::screen_picker<float_type>;

private:
	static constexpr vec3 axis_forward = {1, 0, 0};
	static constexpr vec3 axis_right = {0, -1, 0};
	static constexpr vec3 axis_up = {0, 0, 1};

	/// OpenGL axes: Forward is Z-, Right is X+, Up is Y+
	/// Camera axes: Forward is X+, Right is Y-, Up is Z+
	static constexpr mat4 base_camera_orientation = mat4(
			 0,  0, -1,  0, // Map Z- to X+
			-1,  0,  0,  0, // Map X+ to Y-
			 0,  1,  0,  0, // Map Y+ to Z+
			 0,  0,  0,  1  //
	);

private:
	vec3 position_{0, 0, 0};
	float zoom_ = 0.01f; /// world unit/pixel
//	float_type orbit_distance_ = 1.f;

//	float_type roll_ = 0;
//	float_type pitch_ = 0;
//	float_type yaw_ = 0;

	float_type near_ = 50;
	float_type far_ = -50;
//	float_type fov_y_ = libv::deg_to_rad(75.0f);

public:
	[[nodiscard]] mat4 projection(vec2 canvas_size) const noexcept;
	[[nodiscard]] mat4 view() const noexcept;

//public:
//	void look_at(vec3 newPosition) noexcept;

public:
	[[nodiscard]] constexpr inline float_type zoom() const noexcept {
		return zoom_;
	}
	constexpr inline void zoom(float_type value) noexcept {
		zoom_ = value;
	}
	[[nodiscard]] constexpr inline vec3 position() const noexcept {
		return position_;
	}
	constexpr inline void position(vec3 value) noexcept {
		position_ = value;
	}

	[[nodiscard]] constexpr inline libv::frustum frustum(vec2 canvas_size) const noexcept {
		// TODO P5: CameraOrtho::frustum
		(void) canvas_size;
		return {};
	}
//	[[nodiscard]] constexpr inline float_type near() const noexcept {
//		return near_;
//	}
//	[[nodiscard]] constexpr inline float_type far() const noexcept {
//		return far_;
//	}

public:
	/// Moves the camera on the XY plane based on cameras view direction to forward
	constexpr inline void move_forward(float_type value) noexcept {
		// Yaw is CW but math is CCW
//		orbit_point_.x += value;
		position_.y += value;
	}
	/// Moves the camera on the XY plane based on cameras view direction to right
	constexpr inline void move_right(float_type value) noexcept {
		// Yaw is CW but math is CCW
		position_.x += value;
//		orbit_point_.y += value;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace sun
} // namespace libv
