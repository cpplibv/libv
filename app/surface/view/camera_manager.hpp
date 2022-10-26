// Project: libv, File: app/surface/view/camera_manager.hpp

#pragma once

// libv
#include <libv/ctrl/controls.hpp>
#include <libv/math/frustum.hpp>
// pro
#include <surface/view/camera.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

enum class CameraMode {
	_2d,
	_3d,
};

inline CameraMode currentCameraMode = CameraMode::_3d;
inline CameraMode previousCameraMode = currentCameraMode;

// -------------------------------------------------------------------------------------------------

class CameraManager {
	libv::ctrl::Controls& controls;

	CameraPlayer camera3D;
	CameraOrtho camera2D;

public:
	explicit inline CameraManager(libv::ctrl::Controls& controls) :
			controls(controls) {

		camera3D.look_at({6.f, 6.f, 4.8f}, {0.5f, 0.5f, 0.f});
	}

private:
	void enable(CameraMode mode) {
		switch (mode) {
		case CameraMode::_2d:
			controls.context_enter<CameraOrtho>(&camera2D);
			break;
		case CameraMode::_3d:
			controls.context_enter<BaseCameraOrbit>(&camera3D);
			break;
		}
	}

	void disable(CameraMode mode) {
		switch (mode) {
		case CameraMode::_2d:
			controls.context_leave_if_matches<CameraOrtho>(&camera2D);
			break;
		case CameraMode::_3d:
			controls.context_leave_if_matches<BaseCameraOrbit>(&camera3D);
			break;
		}
	}

public:
	void cycle() {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			currentCameraMode = CameraMode::_3d;
			break;
		case CameraMode::_3d:
			currentCameraMode = CameraMode::_2d;
			break;
		}
	}

	void switchTo(CameraMode mode) {
		currentCameraMode = mode;
	}

	void enableControls(bool value) {
		if (value)
			enable(currentCameraMode);
		else
			disable(previousCameraMode);
	}

	void update() {
		if (currentCameraMode == previousCameraMode)
			return;

		disable(previousCameraMode);
		enable(currentCameraMode);

		previousCameraMode = currentCameraMode;
	}

public:
	[[nodiscard]] libv::mat4f projection(libv::vec2f canvas_size) const noexcept {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			return camera2D.projection(canvas_size);
		case CameraMode::_3d:
			return camera3D.projection(canvas_size);
		}

		assert(false && "Invalid CameraMode enum value");
		return libv::mat4f::identity();
	}

	[[nodiscard]] libv::mat4f view() const noexcept {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			return camera2D.view();
		case CameraMode::_3d:
			return camera3D.view();
		}

		assert(false && "Invalid CameraMode enum value");
		return libv::mat4f::identity();
	}

	[[nodiscard]] float getHeight() const noexcept {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			return camera2D.position().z;
		case CameraMode::_3d:
			return camera3D.eye().z;
		}

		assert(false && "Invalid CameraMode enum value");
		return 0.f;
	}

	[[nodiscard]] libv::frustum getCameraFrustum(libv::vec2f canvas_size) const noexcept {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			return camera2D.frustum(canvas_size);
		case CameraMode::_3d:
			return camera3D.frustum(canvas_size);
		}
		assert(false && "Invalid CameraMode enum value");
		return {};
	}

	[[nodiscard]] libv::vec3f position() const noexcept {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			return camera2D.position();
		case CameraMode::_3d:
			return camera3D.eye();
		}
		assert(false && "Invalid CameraMode enum value");
		return {0, 0, 0};
	}

	[[nodiscard]] libv::vec3f forward() const noexcept {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			return libv::vec3f{0, 0, -1};
		case CameraMode::_3d:
			return camera3D.forward();
		}
		assert(false && "Invalid CameraMode enum value");
		return {1, 0, 0};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
