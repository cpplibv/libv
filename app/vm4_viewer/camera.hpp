// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/mat.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Camera {
	libv::vec3f pivot;
	libv::vec3f position;

	libv::mat4f view() const noexcept {
		return libv::mat4f::lookAt(position, pivot, {0, 0, 1});
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
