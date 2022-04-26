// Created by dbobula on 4/25/2022.

#pragma once

// libv
#include <libv/ctrl/controls.hpp>
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

		camera3D.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});
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
	libv::mat4f projection(libv::vec2f canvas_size) {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			return camera2D.projection(canvas_size);
		case CameraMode::_3d:
			return camera3D.projection(canvas_size);
		}

		assert(false && "Invalid CameraMode enum value");
		return libv::mat4f::identity();
	}

	libv::mat4f view() {
		switch (currentCameraMode) {
		case CameraMode::_2d:
			return camera2D.view();
		case CameraMode::_3d:
			return camera3D.view();
		}

		assert(false && "Invalid CameraMode enum value");
		return libv::mat4f::identity();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
