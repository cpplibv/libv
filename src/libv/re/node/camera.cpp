// Project: libv.re, File: src/libv/re/node/camera.cpp

#include <libv/re/node/camera.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Camera* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Camera* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

libv::mat4f Camera::projection(libv::vec2f canvasSize) const {
	// return tmpCameraPlayer.projection(canvasSize);
	return tmpCameraPlayer.projection(canvasSize);
}

libv::mat4f Camera::projectionRevereZ(libv::vec2f canvasSize) const {
	// return tmpCameraPlayer.projection(canvasSize);
	return tmpCameraPlayer.projectionRevereZ(canvasSize);
}

libv::mat4f Camera::projectionRevereZInf(libv::vec2f canvasSize) const {
	// return tmpCameraPlayer.projection(canvasSize);
	return tmpCameraPlayer.projectionRevereZInf(canvasSize);
}

libv::mat4f Camera::view() const {
	return tmpCameraPlayer.view();
}

float Camera::near() const {
	return tmpCameraPlayer.near();
}

float Camera::far() const {
	return tmpCameraPlayer.far();
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
