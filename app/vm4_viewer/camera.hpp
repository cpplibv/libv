// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/mat.hpp>
// std
#include <cmath>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Camera {
	static constexpr libv::vec3f up = {0, 0, 1};

	float fov = libv::deg_to_rad(90.0f);
	float near = 0.1f;
	float far = 1000.f;

	libv::vec3f position{0, 0, 0};
	libv::vec3f rotation{0, 0, 0}; // RAD
	float zoom_ = 0;

public:
	libv::mat4f projection(libv::vec2f canvas_size) const noexcept {
		return libv::mat4f::perspective(fov, canvas_size.x / canvas_size.y, near, far);
	}

	libv::mat4f view() const noexcept {
		libv::mat4f mat(1.f);

		mat.translate(libv::vec3f(0, 0, zoom_));
		mat.rotate(-libv::deg_to_rad(90.f), libv::vec3f(1, 0, 0));
		mat.rotate(rotation.y, libv::vec3f(0, 1, 0));
		mat.rotate(rotation.x, libv::vec3f(1, 0, 0));
		mat.rotate(rotation.z, libv::vec3f(0, 0, 1));

		mat.translate(-position);
		return mat;
	}

public:
	libv::vec3f eye() const noexcept {
		return libv::vec::xyz(view().inverse()[3]);
	}

public:
	/// zoom
	void zoom(float amount);

	float zoom() const {
		return zoom_;
	}

public:
//	void lookat(libv::vec3f eye, libv::vec3f center) const noexcept {
//		position = center;
//		zoom_ = (eye - center).lenght();
//		rotation = ...;
//	}

public:
	void translateX(float x) {
		// Strafing
		position.y += x * std::cos(rotation.z + libv::deg_to_rad(90.f));
		position.x += x * std::sin(rotation.z + libv::deg_to_rad(90.f));
	}
	void translateY(float x) {
		position.y -= x * std::cos(rotation.z);
		position.x -= x * std::sin(rotation.z);
	}
	void translateZ(float x) {
		position.z += x;
	}
	void translateZoom(float x) {
		zoom_ += x;
	}
	void rotateX(float x) {
		rotation.x += x;
	}
	void rotateY(float x) {
		rotation.y += x;
	}
	void rotateZ(float x) {
		rotation.z += x;
	}

//public:
//	/// rotate left-right
//	/// \param amount - pan angle in radian
//	void pan(float amount);
//	/// rotate up-down
//	/// \param amount - tilt angle in radian
//	void tilt(float amount);
//
//public:
//	/// rotate up-down around the pivot
//	/// \param amount - pan angle in radian
//	void orbit_pan(float amount) {
//		const auto diff = libv::xy(position) - libv::xy(pivot);
//		const float angle = std::atan2(diff.y, diff.x) + amount;
//		const float dist = diff.length();
//		position.x = pivot.x + std::cos(angle) * dist;
//		position.y = pivot.y + std::sin(angle) * dist;
//	}
//	/// rotate left-right around the pivot
//	/// \param amount - tilt angle in radian
//	void orbit_tilt(float amount) {
//		const auto diff = libv::xy(position) - libv::xy(pivot);
//		const float angle = std::atan2(diff.y, diff.x) + amount;
//		const float dist = diff.length();
//		position.x = pivot.x + std::cos(angle) * dist;
//		position.y = pivot.y + std::sin(angle) * dist;
//	}
//
//public:
//	/// move forward - backward
//	/// \param amount - dolly movement in units
//	void dolly(float amount) {
//		const auto dir = (libv::xy(position) - libv::xy(pivot)).normalize_copy();
//		pivot += libv::vec3f(dir * amount, 0);
//		position += libv::vec3f(dir * amount, 0);
//	}
//	/// move left-right
//	/// \param amount - truck movement in units
//	void truck(float amount);
//	/// move up-down
//	/// \param amount - pedestal movement in units
//	void pedestal(float amount) {
//		pivot.z += amount;
//		position.z += amount;
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
