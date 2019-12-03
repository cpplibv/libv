// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/mat.hpp>
// pro
#include <vm4_viewer/scene/node.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Object : Node {
	libv::vec3f position;
	// TODO P4: use quaternion for rotation
	libv::vec3f rotation; // RAD
	libv::vec3f scale;

	[[nodiscard]] constexpr inline libv::mat4f transformation() const noexcept {
		libv::mat4f result = libv::mat4f::identity();

		result.translate(position);
		result.rotate(rotation.x, {1, 0, 0});
		result.rotate(rotation.y, {0, 1, 0});
		result.rotate(rotation.z, {0, 0, 1});
		result.scale(scale);

		return result;
	}

	[[nodiscard]] constexpr inline libv::mat4f transformation_inverse() const noexcept {
		libv::mat4f result = libv::mat4f::identity();

		result.scale(1.0f / scale);
		result.rotate(-rotation.x, {1, 0, 0});
		result.rotate(-rotation.y, {0, 1, 0});
		result.rotate(-rotation.z, {0, 0, 1});
		result.translate(-position);

		return result;
	}

	[[nodiscard]] constexpr inline libv::mat4f transformation_rot() const noexcept {
		libv::mat4f result = libv::mat4f::identity();

		result.rotate(rotation.x, {1, 0, 0});
		result.rotate(rotation.y, {0, 1, 0});
		result.rotate(rotation.z, {0, 0, 1});

		return result;
	}

	[[nodiscard]] constexpr inline libv::mat4f transformation_inverse_rot() const noexcept {
		libv::mat4f result = libv::mat4f::identity();

		result.rotate(-rotation.x, {1, 0, 0});
		result.rotate(-rotation.y, {0, 1, 0});
		result.rotate(-rotation.z, {0, 0, 1});

		return result;
	}
};

struct OModel : Object {
	void* model;
	void* shader;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
