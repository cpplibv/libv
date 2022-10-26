// Project: libv.math, File: test/libv/math/test_frustum.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
//// ext
//#include <fmt/format.h>
//// libv
//#include <libv/utility/approx.hpp>
// pro
#include <libv/math/frustum.hpp>


// -------------------------------------------------------------------------------------------------

using vec3 = libv::vec3f;

// -------------------------------------------------------------------------------------------------

TEST_CASE("Test frustum constructors", "[libv.math.frustum]") {
	libv::frustum f0;
	for (const auto& corner : f0.corners())
		CHECK(corner == vec3(0, 0, 0));

//	libv::frustum view(eye, fovx, fovy, near, far);
//	view.expand(5)
//	view.expand(frustum::plane::left, 5)

//	libv::frustum(eye, dir, fovx, fovy, near, far);
//	libv::frustum(eye, fbl, fbr, ftr, ftl, near);
//	libv::frustum(proj_view_matrix);
//	libv::frustum(enclosed_frustum, orientation_dir);
}

//#include <libv/math/mat.hpp>
//
//
//auto foo() {
//	vec3f sun_dir;
//
//	Frustum camera;
//	Frustum shadow_box = contain_from_different_dir(camera, sun_dir);
//
//	libv::mat4f light_matP = libv::mat4f::ortho(shadow_box.getWidth(), shadow_box.getHeight(), shadow_box.getLenght());
//	libv::mat4f light_matMV = libv::mat4f::lookAt(sun_dir, {0, 0, 0}, {0, 0, 1});
//
//	const auto light_matMVP = light_matMV * light_matP;
//}
