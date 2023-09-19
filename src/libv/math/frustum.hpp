// Project: libv.math, File: src/libv/math/frustum.hpp

#pragma once

// std
#include <array>
// pro
#include <libv/math/plane.hpp>
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

class frustum {
public:
	enum class position {
		outside,
		intersect,
		inside,
	};

private:
	using plane = planef;

	enum PlaneID {
		left = 0,
		right = 1,
		up = 2,
		down = 3,
		near = 4,
		far = 5,
	};

	enum CornerID {
		nbl = 0,
		nbr = 1,
		ntr = 2,
		ntl = 3,
		fbl = 4,
		fbr = 5,
		ftr = 6,
		ftl = 7,
	};

	std::array<plane, 6> planes_;
	std::array<vec3f, 8> corners_;

public:
	constexpr inline frustum() noexcept = default;
	/// a, b, c and d are expected in CCW
//	frustum(vec3f eye, vec3f fbl_, vec3f fbr_, vec3f ftr_, vec3f ftl_, float near) noexcept;
	frustum(vec3f nbl_, vec3f nbr_, vec3f ntr_, vec3f ntl_,
			vec3f fbl_, vec3f fbr_, vec3f ftr_, vec3f ftl_) noexcept;
//	frustum(vec3f eye, libv::quat dir, libv::vec2f fov, float near, float far) noexcept;
//	frustum(vec3f eye, libv::vec3f dir_bl, libv::vec3f dir_br, libv::vec3f dir_tr, libv::vec3f dir_tl, float near, float far) noexcept;

//	libv::frustum view(eye, fovx, fovy, near, far);
//	view.expand(5)
//	view.expand(frustum::plane::left, 5)

//	libv::frustum(eye, dir, fovx, fovy, near, far);
//	libv::frustum(eye, fbl, fbr, ftr, ftl, near);
//	libv::frustum(proj_view_matrix);
//	libv::frustum(enclosed_frustum, orientation_dir);

public:
	[[nodiscard]] const std::array<vec3f, 8>& corners() const noexcept;
	[[nodiscard]] const std::array<plane, 6>& planes() const noexcept;

public:
	[[nodiscard]] position point_in_frustum(vec3f point) const noexcept;
	[[nodiscard]] position sphere_in_frustum(vec3f point, float radius) const noexcept;

public:
//	frustum contain_from_different_dir(vec3f dir);
};

// -------------------------------------------------------------------------------------------------

class frustum_culler_inf {
private:
	enum PlaneID {
		left = 0,
		right = 1,
		up = 2,
		down = 3,
		near = 4,
		// far = 5, For inf frustum_culler there is no far plane
	};

	std::array<planef, 5> planes_;

public:
	constexpr inline frustum_culler_inf() noexcept = default;
	frustum_culler_inf(planef left, planef right, planef up, planef down, planef near) noexcept;

public:
	[[nodiscard]] bool test_point(vec3f point) const noexcept;
	[[nodiscard]] bool test_sphere(vec3f position, float radius) const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
