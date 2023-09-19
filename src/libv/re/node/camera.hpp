// Project: libv.re, File: src/libv/re/node/camera.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/sun/camera.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

// TODO P1: Is Camera a Node?
class Camera : public libv::ref_count_base {
public:
	friend libv::ref_count_access;
	using ptr = Camera_ptr;
	using cptr = Camera_cptr;

public: // TODO P5: Proper placement of camera code, Just dropping this here for now
	libv::sun::CameraPlayer tmpCameraPlayer;

public:
	Camera() { }
	virtual ~Camera() = default;
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}

public:
	[[nodiscard]] virtual libv::mat4f projection(libv::vec2f canvasSize) const;
	[[nodiscard]] virtual libv::mat4f projectionRevereZ(libv::vec2f canvasSize) const;
	[[nodiscard]] virtual libv::mat4f projectionRevereZInf(libv::vec2f canvasSize) const;
	[[nodiscard]] virtual libv::mat4f view() const;
	[[nodiscard]] virtual float near() const;
	[[nodiscard]] virtual float far() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
