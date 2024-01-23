// Project: libv.re, File: src/libv/re/core/context/context_frame.hpp

#pragma once

#include <libv/math/frustum.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/core/render_state.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/screen_picker.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct ContextFrame {
	// GL State
	RenderState currentState;

	// Time
	libv::time_duration_f deltaTimeRealf;
	libv::time_duration_f deltaTimeSimf;

	// Canvas / Camera
	libv::screen_picker<float> screenPicker;
	libv::frustum_culler_inf viewFrustum;
	libv::vec3f eyeW;
	float near = 0;
	libv::mat4f matV;

	libv::vec2i canvasSize; // Only for post-processing, remove once rtStack manipulation is re-organized
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
