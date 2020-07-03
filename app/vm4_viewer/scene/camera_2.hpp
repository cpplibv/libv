// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/math/mat.hpp>
//#include <libv/math/quat.hpp>
#include <libv/math/vec.hpp>
// ext
#include <glm/ext/quaternion_transform.hpp>
// std
#include <cmath>
// pro
//#include <vm4_viewer/scene/object.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

// TODO P1: make sure that the camera points toward its X+ and the rotations are also based on that
struct Camera2 {
private:
	//private:
//	const auto orbit = -camera.orbit();

	static constexpr libv::vec3f forward = {1, 0, 0};
	static constexpr libv::vec3f right = {0, 1, 0};
	static constexpr libv::vec3f up = {0, 0, 1};

	float fov = libv::deg_to_rad(75.0f);
	float near_ = 1.0f;
	float far_ = 1000.f;

//	libv::vec3f position_{0, 0, 0};
//	libv::quat rotation_{};
//	float orbit_ = 0;

	libv::vec3f position_{0, 0, 0};
	libv::vec3f rotation_{0, 0, 0}; // Roll, pitch, yaw in RAD
	float orbit_ = 0;

public:
	[[nodiscard]] constexpr inline float near() const noexcept {
		return near_;
	}
	[[nodiscard]] constexpr inline float far() const noexcept {
		return far_;
	}

public:
	libv::mat4f projection(libv::vec2f canvas_size) const noexcept {
		return libv::mat4f::perspective(fov, canvas_size.x / canvas_size.y, near_, far_);
	}

	libv::mat4f view() const noexcept {
		libv::mat4f mat(1.f);

		mat.translate(libv::vec3f(0, 0, -orbit_));
		mat.rotate(-libv::deg_to_rad(90.f), libv::vec3f(1, 0, 0));
		mat.rotate(rotation_.y, libv::vec3f(0, 1, 0));
		mat.rotate(rotation_.x, libv::vec3f(1, 0, 0));
		mat.rotate(rotation_.z, libv::vec3f(0, 0, 1));

		mat.translate(-position_);
		return mat;
	}

public:
	libv::vec3f eye() const noexcept {
		return libv::vec::xyz(view().inverse()[3]);
	}

public:
//	void lookat(libv::vec3f eye, libv::vec3f center) const noexcept {
//		position_ = center;
//		rotation_ = ...;
//		orbit_ = (eye - center).lenght();
//	}

public:
	constexpr inline void position(libv::vec3f value) noexcept {
		position_ = value;
	}
	[[nodiscard]] constexpr inline libv::vec3f position() const noexcept {
		return position_;
	}

	constexpr inline void rotation(libv::vec3f value) noexcept {
		rotation_ = value;
	}
	[[nodiscard]] constexpr inline libv::vec3f rotation() const noexcept {
		return rotation_;
	}

	constexpr inline void orbit(float value) noexcept {
		orbit_ += value;
	}
	constexpr inline void orbit_to(float value) noexcept {
		orbit_ = value;
	}
	[[nodiscard]] constexpr inline float orbit() const noexcept {
		return orbit_;
	}

public:
//	constexpr inline void roll(float value) noexcept {
//		glm::rotate(rotation_, value, libv::to_glm(forward));
//	}
//	constexpr inline void pitch(float value) noexcept {
//		glm::rotate(rotation_, value, libv::to_glm(right));
//	}
//	constexpr inline void yaw(float value) noexcept {
//		glm::rotate(rotation_, value, libv::to_glm(up));
//	}

//	orbit_roll
//	orbit_pich
//	orbit_yaw

	void translate_x(float x) {
		position_.y -= x * std::cos(rotation_.z);
		position_.x -= x * std::sin(rotation_.z);
	}
	void translate_y(float x) {
		position_.y += x * std::cos(rotation_.z + libv::pi / 2.0f);
		position_.x += x * std::sin(rotation_.z + libv::pi / 2.0f);
	}
	void translate_z(float x) {
		position_.z += x;
	}

	void rotate_x(float x) {
		rotation_.x += x;
	}
	void rotate_y(float x) {
		rotation_.y += x;
	}
	void rotate_z(float x) {
		rotation_.z += x;
	}

//public:
//	/// rotate left-right
//	void pan(float amount);
//	/// rotate up-down
//	void tilt(float amount);
//
//public:
//	/// rotate up-down around the pivot
//	void orbit_pan(float amount) {
//		const auto diff = libv::xy(position) - libv::xy(pivot);
//		const float angle = std::atan2(diff.y, diff.x) + amount;
//		const float dist = diff.length();
//		position_.x = pivot.x + std::cos(angle) * dist;
//		position_.y = pivot.y + std::sin(angle) * dist;
//	}
//	/// rotate left-right around the pivot
//	void orbit_tilt(float amount) {
//		const auto diff = libv::xy(position) - libv::xy(pivot);
//		const float angle = std::atan2(diff.y, diff.x) + amount;
//		const float dist = diff.length();
//		position_.x = pivot.x + std::cos(angle) * dist;
//		position_.y = pivot.y + std::sin(angle) * dist;
//	}
//
//public:
//	/// move forward - backward
//	void dolly(float amount) {
//		const auto dir = (libv::xy(position) - libv::xy(pivot)).normalize_copy();
//		pivot += libv::vec3f(dir * amount, 0);
//		position += libv::vec3f(dir * amount, 0);
//	}
//	/// move left-right
//	void truck(float amount);
//	/// move up-down
//	void pedestal(float amount) {
//		pivot.z += amount;
//		position_.z += amount;
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
