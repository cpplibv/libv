// Project: libv, File: app/star/view/frustum.hpp

#pragma once

// fwd
#include <libv/math/plane_fwd.hpp>
// libv
#include <libv/meta/force_inline.hpp>
// std
#include <ostream>
// pro
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class plane_t {
	using vec3 = libv::vec3_t<T>;

public:
	using value_type = T;

public:
	vec3 normal{libv::uninitialized}; // = {0, 0, 1}
	T dist; // = 0

public:
	// constructors --------------------------------------------------------------------------------

	constexpr LIBV_FORCE_INLINE plane_t() noexcept : normal(0, 0, 1), dist(0) {};
	explicit(false) constexpr LIBV_FORCE_INLINE plane_t(libv::uninitialized_t) noexcept { }

	constexpr LIBV_FORCE_INLINE plane_t(const plane_t&) noexcept = default;
	constexpr LIBV_FORCE_INLINE plane_t& operator=(const plane_t&) & noexcept = default;
	constexpr LIBV_FORCE_INLINE plane_t(plane_t&&) noexcept = default;
	constexpr LIBV_FORCE_INLINE plane_t& operator=(plane_t&&) & noexcept = default;

	constexpr LIBV_FORCE_INLINE plane_t(T vx, T vy, T vz, T dist) noexcept : plane_t(
			from_vector_dist(vec3(vx, vy, vz), dist)) {}

	constexpr LIBV_FORCE_INLINE plane_t(vec3 vector, T dist) noexcept : plane_t(
			from_vector_dist(vector, dist)) {}

private:
	// Raw constructor
	constexpr LIBV_FORCE_INLINE plane_t(vec3 vector, T dist, std::nullptr_t) noexcept :
			normal(vector), dist(dist) {}

public:
	// named constructors --------------------------------------------------------------------------

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE plane_t identity() noexcept {
		return {};
	}

	static constexpr LIBV_FORCE_INLINE plane_t from_normal_dist(vec3 normal, T dist) noexcept {
		return {normal, dist, nullptr}; // Access raw constructor
	}

	static constexpr LIBV_FORCE_INLINE plane_t from_normal_dist(T nx, T ny, T nz, T dist) noexcept {
		return from_normal_dist(vec3(nx, ny, nz), dist);
	}

	static constexpr LIBV_FORCE_INLINE plane_t from_normal_point(vec3 normal, vec3 point) noexcept {
		return from_normal_dist(normal, -dot(normal, point));
	}

	static constexpr LIBV_FORCE_INLINE plane_t from_vector_dist(vec3 vector, T dist) noexcept {
		return from_normal_dist(vector.normalize(), dist);
	}

	static constexpr LIBV_FORCE_INLINE plane_t from_vector_point(vec3 vector, vec3 point) noexcept {
		return from_normal_point(vector.normalize(), point);
	}

	/// CCW order is expected for p1, p2, p3
	static constexpr LIBV_FORCE_INLINE plane_t from_three_point(vec3 p1, vec3 p2, vec3 p3) noexcept {
		const auto aux1 = p1 - p2;
		const auto aux2 = p3 - p2;
		return from_normal_point(normalize(cross(aux2, aux1)), p2);
	}

	// ---------------------------------------------------------------------------------------------

public:
	/// >0 - plane normal is facing toward the point
	/// =0 - precise intersection
	/// <0 - plane normal is facing away from the point
	[[nodiscard]] constexpr LIBV_FORCE_INLINE T distance_from_point(T px, T py, T pz) const noexcept {
		return distance_from_point(vec3(px, py, pz));
	}
	/// >0 - plane normal is facing toward the point
	/// =0 - precise intersection
	/// <0 - plane normal is facing away from the point
	[[nodiscard]] constexpr LIBV_FORCE_INLINE T distance_from_point(vec3 point) const noexcept {
		return dist + dot(normal, point);
	}

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator<=>(const plane_t&) const = default;

public:
	friend std::ostream& operator<<(std::ostream& os, const plane_t& var) {
		return os << "N:" << var.normal << " D:" << var.dist;
	}
};

// -------------------------------------------------------------------------------------------------

using planef = plane_t<float>;
using planed = plane_t<double>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
