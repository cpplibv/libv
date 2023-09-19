// Project: libv.math, File: src/libv/re/math/transform.hpp

#pragma once

#include <libv/math/mat.hpp>
#include <libv/math/quat.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct Transform {
	libv::vec3f position;
	mutable libv::quatf rotation;
	libv::vec3f scale{1.f, 1.f, 1.f};

	inline Transform() = default;
	explicit inline Transform(libv::vec3f position) :
		position(position) { }
	inline Transform(libv::vec3f position, libv::quatf rotation, libv::vec3f scale) :
		position(position), rotation(rotation), scale(scale) { }

	[[nodiscard]] constexpr inline libv::mat4f toMatrix() const {
		rotation.normalize();

		// TODO P2: Implement mat4f(pos, rot_quat, scale) ctor
		auto result = libv::mat4f::identity();
		result.translate(position);
		result = result * rotation.to_mat4();
		// TODO P3: Implement matrix rotate member function with a quaternion
		result.scale(scale);
		return result;
	}

	// libv::mat4f where_to_place_this_function(libv::vec3f position, libv::quatf rotation, libv::vec3f scale) {
	//	Test, most likely orientation will be wrong!
	// 	libv::mat4f result;
	//
	// 	const auto x = rotation.x, y = rotation.y, z = rotation.z, w = rotation.w;
	// 	const auto x2 = x + x;
	// 	const auto y2 = y + y;
	// 	const auto z2 = z + z;
	//
	// 	const auto xx = x * x2;
	// 	const auto xy = x * y2;
	// 	const auto xz = x * z2;
	//
	// 	const auto yy = y * y2;
	// 	const auto yz = y * z2;
	// 	const auto zz = z * z2;
	//
	// 	const auto wx = w * x2;
	// 	const auto wy = w * y2;
	// 	const auto wz = w * z2;
	//
	// 	const auto sx = scale.x, sy = scale.y, sz = scale.z;
	//
	// 	result.data()[0] = (1 - (yy + zz)) * sx;
	// 	result.data()[1] = (xy + wz) * sx;
	// 	result.data()[2] = (xz - wy) * sx;
	// 	result.data()[3] = 0;
	//
	// 	result.data()[4] = (xy - wz) * sy;
	// 	result.data()[5] = (1 - (xx + zz)) * sy;
	// 	result.data()[6] = (yz + wx) * sy;
	// 	result.data()[7] = 0;
	//
	// 	result.data()[8] = (xz + wy) * sz;
	// 	result.data()[9] = (yz - wx) * sz;
	// 	result.data()[10] = (1 - (xx + yy)) * sz;
	// 	result.data()[11] = 0;
	//
	// 	result.data()[12] = position.x;
	// 	result.data()[13] = position.y;
	// 	result.data()[14] = position.z;
	// 	result.data()[15] = 1;
	//
	// 	return result;
	// }
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
