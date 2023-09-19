// Project: libv.math, File: src/libv/math/quat.hpp

#pragma once

// hpp
#include <libv/math/quat_fwd.hpp>
// ext
#include <libv/fwd/ext/fmt_fwd.hpp>
// libv
#include <libv/math/constants.hpp>
#include <libv/math/mat.hpp>
#include <libv/math/mix.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/force_inline.hpp>
#include <libv/meta/uninitialized.hpp>
// std
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Representation is W-X-Y-Z based on quaternion's origin in math (w + xi + yj + zk) and other libraries
/// including glm (by default). https://github.com/clemense/quaternion-conventions
/// The coordinate system is right handed and uses Z up (Forward = X+, Right = Y-, Up = Z+)
template <typename T>
struct quat_t {
	using value_type = T;

public:
	T w; // = 1
	T x; // = 0
	T y; // = 0
	T z; // = 0

public:
	// constructors --------------------------------------------------------------------------------

	constexpr LIBV_FORCE_INLINE quat_t() noexcept : w{1}, x{0}, y{0}, z{0} {}
	explicit(false) constexpr LIBV_FORCE_INLINE quat_t(libv::uninitialized_t) noexcept { }

	constexpr LIBV_FORCE_INLINE quat_t(const quat_t&) noexcept = default;
	constexpr LIBV_FORCE_INLINE quat_t& operator=(const quat_t&) & noexcept = default;
	constexpr LIBV_FORCE_INLINE quat_t(quat_t&&) noexcept = default;
	constexpr LIBV_FORCE_INLINE quat_t& operator=(quat_t&&) & noexcept = default;

	constexpr LIBV_FORCE_INLINE quat_t(T w, T x, T y, T z) noexcept : w(w), x(x), y(y), z(z) {}

	explicit constexpr LIBV_FORCE_INLINE quat_t(const mat3_t<T>& m) noexcept : quat_t(from_mat(m)) {}

	explicit constexpr LIBV_FORCE_INLINE quat_t(const mat4_t<T>& mat) noexcept : quat_t(mat3_t<T>{mat}) {}

	// named constructors --------------------------------------------------------------------------

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE quat_t identity() noexcept {
		return quat_t();
	}

	/// \param angle - (Radians)
	[[nodiscard]] static constexpr inline quat_t angle_axis(T angle, vec3_t<T> axis) noexcept {
		// Axis of rotation must be normalised
		const auto axis_len_sq = axis.length_sq();
		if (std::abs(axis_len_sq - T{1}) > static_cast<T>(0.001))
			axis /= std::sqrt(axis_len_sq);

		const T sin = std::sin(angle * static_cast<T>(0.5));
		const T cos = std::cos(angle * static_cast<T>(0.5));

		return quat_t{cos, axis.x * sin, axis.y * sin, axis.z * sin};
	}

	/// \param angle - (Radians)
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE quat_t create_rotate(T angle, vec3_t<T> axis) noexcept {
		return angle_axis(angle, axis);
	}

	/// Creates a rotation quaternion that maps from (forward = X+; up = Z+) to (forward = \c direction; up = orthogonalized up)
	/// The provided \c direction and \c up cannot match (The implementation is NaN safe, but the result might be unexpected)
	///	\param direction - Desired forward direction onto which the X+-axis gets mapped. Needs to be normalized.
	///	\param up - Up vector, how the camera is oriented. Typically (0, 0, 1).
	[[nodiscard]] static constexpr inline quat_t look_at(vec3_t<T> direction, vec3_t<T> up) noexcept {
		vec3_t<T> right = cross(direction, up);
		// Safe normalize right
		right = right / std::sqrt(std::max(static_cast<T>(0.00001), right.length_sq()));
		const auto upOrtho = cross(right, direction);

		return quat_t(mat3_t<T>{
			to_glm(direction),
			to_glm(-right), // Matrix need Y+ (left) but right is Y-
			to_glm(upOrtho)
		});
	}

	/// Returns a quaternion such that: quat * start = dest
	/// Based on: http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
	[[nodiscard]] static constexpr inline quat_t from_unit_vectors(vec3_t<T> start, vec3_t<T> dest) noexcept {
		float m = std::sqrt(T{2} + T{2} * dot(start, dest));
		vec3_t<T> w = (T{1} / m) * cross(start, dest);
		return quat_t<T>(T{0.5} * m, w.x, w.y, w.z);
	}

	/// Returns a quaternion such that: quat * start = dest
	/// Based on: http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
	[[nodiscard]] static constexpr inline quat_t from_non_unit_vectors(vec3_t<T> start, vec3_t<T> dest) noexcept {
		vec3_t<T> w = cross(start, dest);
		quat_t q = quat_t(dot(start, dest), w.x, w.y, w.z);
		q.w += q.length();
		return q.normalize();
	}

	[[nodiscard]] static constexpr inline quat_t from_mat(mat3_t<T> m) noexcept {
		T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
		T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
		T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
		T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

		T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		int biggestIndex = 0;

		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}
		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}
		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		T biggestVal = std::sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1)) * static_cast<T>(0.5);
		T mult = static_cast<T>(0.25) / biggestVal;

		switch (biggestIndex) {
		case 0:
			return quat_t(biggestVal, (m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult);
		case 1:
			return quat_t((m[1][2] - m[2][1]) * mult, biggestVal, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult);
		case 2:
			return quat_t((m[2][0] - m[0][2]) * mult, (m[0][1] + m[1][0]) * mult, biggestVal, (m[1][2] + m[2][1]) * mult);
		case 3:
			return quat_t((m[0][1] - m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggestVal);
		default: // Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
			assert(false);
			return quat_t::identity();
		}
	}

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE quat_t from_mat(mat4_t<T> mat) noexcept {
		return from_mat(mat3_t<T>{mat});
	}

	template <typename Quat>
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE quat_t from_quat(const Quat& q) noexcept {
		return quat_t{q.w, q.x, q.y, q.z};
	}

public:
	// operator unary ------------------------------------------------------------------------------

	constexpr LIBV_FORCE_INLINE quat_t operator+() const noexcept {
		return *this;
	}

	constexpr LIBV_FORCE_INLINE quat_t operator-() const noexcept {
		return quat_t{-w, -x, -y, -z};
	}

	// operator+ -----------------------------------------------------------------------------------

	constexpr LIBV_FORCE_INLINE quat_t& operator+=(quat_t q2) noexcept {
		w += q2.w;
		x += q2.x;
		y += q2.y;
		z += q2.z;
		return *this;
	}

	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE quat_t operator+(quat_t q1, quat_t q2) noexcept {
		return quat_t(q1) += q2;
	}

	constexpr LIBV_FORCE_INLINE quat_t& operator-=(quat_t q2) noexcept {
		w -= q2.w;
		x -= q2.x;
		y -= q2.y;
		z -= q2.z;
		return *this;
	}

	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE quat_t operator-(quat_t q1, quat_t q2) noexcept {
		return quat_t(q1) -= q2;
	}

	// operator* -----------------------------------------------------------------------------------

	// Quat, Quat

	constexpr inline quat_t& operator*=(quat_t rhs) noexcept {
		auto lhs = *this;
		w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
		x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
		y = lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z;
		z = lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x;
		return *this;
	}

	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE quat_t operator*(quat_t q1, quat_t q2) noexcept {
		return quat_t(q1) *= q2;
	}

	// Quat, Vec

	[[nodiscard]] friend constexpr inline vec3_t<T> operator*(quat_t q1, vec3_t<T> vec) noexcept {
		const vec3_t<T> quatVector(q1.x, q1.y, q1.z);
		const vec3_t<T> uv(cross(quatVector, vec));
		const vec3_t<T> uuv(cross(quatVector, uv));

		return vec + ((uv * q1.w) + uuv) * T{2};
	}

	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE vec3_t<T> operator*(vec3_t<T> vec, quat_t q2) noexcept {
		return q2.inverse_copy() * vec;
	}

	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE vec4_t<T> operator*(quat_t q1, vec4_t<T> vec) noexcept {
		return vec4_t<T>{q1 * xyz(vec), vec.w};
	}

	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE vec4_t<T> operator*(vec4_t<T> vec, quat_t q2) noexcept {
		return q2.inverse_copy() * vec;
	}

	// Quat, Scalar

	template <typename K>
	constexpr LIBV_FORCE_INLINE quat_t& operator*=(K scalar) noexcept {
		w *= scalar;
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	template <typename K>
	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE quat_t operator*(quat_t q, K scalar) noexcept {
		return quat_t{q.w * scalar, q.x * scalar, q.y * scalar, q.z * scalar
		};
	}

	template <typename K>
	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE quat_t operator*(K scalar, quat_t q) noexcept {
		return quat_t{scalar * q.w, scalar * q.x, scalar * q.y, scalar * q.z
		};
	}

	// operator/ -----------------------------------------------------------------------------------

	// Quat, Quat

//	constexpr inline quat_t& operator/=(quat_t rhs) noexcept {
//		...
//		return *this;
//	}

//	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE quat_t operator/(quat_t q1, quat_t q2) noexcept {
//		return quat_t(q1) *= q2;
//	}

	// Quat, Scalar

	template <typename K>
	constexpr LIBV_FORCE_INLINE quat_t& operator/=(K scalar) noexcept {
		w /= scalar;
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	template <typename K>
	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE quat_t operator/(quat_t q, K scalar) noexcept {
		return quat_t{q.w / scalar, q.x / scalar, q.y / scalar, q.z / scalar};
	}

	template <typename K>
	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE quat_t operator/(K scalar, quat_t q) noexcept {
		return quat_t{scalar / q.w, scalar / q.x, scalar / q.y, scalar / q.z};
	}

	// conjugate -----------------------------------------------------------------------------------

	constexpr LIBV_FORCE_INLINE quat_t& conjugate() noexcept {
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE quat_t conjugate_copy() const noexcept {
		return quat_t{w, -x, -y, -z};
	}

	// inverse -------------------------------------------------------------------------------------

	constexpr LIBV_FORCE_INLINE quat_t& inverse() noexcept {
		const auto d = length_sq();
		this->conjugate();
		*this /= d;
		return *this;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE quat_t inverse_copy() const noexcept {
		return conjugate_copy() /= length_sq();
	}

	// normalize -----------------------------------------------------------------------------------

	constexpr inline quat_t& normalize() noexcept {
		const T len = length();
		if (len <= static_cast<T>(0)) {
			w = 1;
			x = 0;
			y = 0;
			z = 0;
		}

		const T oneOverLen = static_cast<T>(1) / len;
		w *= oneOverLen;
		x *= oneOverLen;
		y *= oneOverLen;
		z *= oneOverLen;

		return *this;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE quat_t normalize_copy() const noexcept {
		return quat_t{*this}.normalize();
	}

	// rotate --------------------------------------------------------------------------------------

	constexpr LIBV_FORCE_INLINE quat_t& rotate(T angle, vec3_t<T> axis) noexcept {
		*this *= create_rotate(angle, axis);
		return *this;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE quat_t rotate_copy(T angle, vec3_t<T> axis) const noexcept {
		return *this * create_rotate(angle, axis);
	}

	// query ---------------------------------------------------------------------------------------

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool has_inf() const noexcept {
		return std::isinf(w) || std::isinf(x) || std::isinf(y) || std::isinf(z);
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool has_nan() const noexcept {
		return std::isnan(w) || std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE vec3_t<T> forward() const noexcept {
		return *this * vec3_t<T>{1, 0, 0};
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE vec3_t<T> right() const noexcept {
		return *this * vec3_t<T>{0, -1, 0};
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE vec3_t<T> up() const noexcept {
		return *this * vec3_t<T>{0, 0, 1};
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T length_sq() const noexcept {
		return w * w + x * x + y * y + z * z;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T length() const noexcept {
		return std::sqrt(length_sq());
	}

	/// The quaternion must be normalised before calling to_mat
	[[nodiscard]] constexpr inline mat3_t<T> to_mat3() const noexcept {
		const T qxx(x * x);
		const T qyy(y * y);
		const T qzz(z * z);
		const T qxz(x * z);
		const T qxy(x * y);
		const T qyz(y * z);
		const T qwx(w * x);
		const T qwy(w * y);
		const T qwz(w * z);

		return mat3_t<T>{
				T(1) - T(2) * (qyy + qzz),
				T(2) * (qxy + qwz),
				T(2) * (qxz - qwy),

				T(2) * (qxy - qwz),
				T(1) - T(2) * (qxx + qzz),
				T(2) * (qyz + qwx),

				T(2) * (qxz + qwy),
				T(2) * (qyz - qwx),
				T(1) - T(2) * (qxx + qyy)
		};
	}

	/// The quaternion must be normalised before calling to_mat
	[[nodiscard]] constexpr LIBV_FORCE_INLINE mat4_t<T> to_mat4() const noexcept {
		return mat4_t<T>{to_mat3()};
	}

	/// roll (x-axis rotation)
	[[nodiscard]] constexpr inline T roll() const noexcept {
		T sinr_cosp = T{2} * (w * x + y * z);
		T cosr_cosp = T{1} - T{2} * (x * x + y * y);

		if (sinr_cosp == T{0} && cosr_cosp == T{0})
			return T{0};

		return std::atan2(sinr_cosp, cosr_cosp);
	}

	/// pitch (y-axis rotation)
	[[nodiscard]] constexpr inline T pitch() const noexcept {
		T sinp = T{2} * (w * y - z * x);
		if (std::fabs(sinp) >= T{1})
			return std::copysign(libv::pi_v<T> / T{2}, sinp); // use 90 degrees if out of range
		else
			return std::asin(sinp);
	}

	/// yaw (z-axis rotation)
	[[nodiscard]] constexpr inline T yaw() const noexcept {
		T siny_cosp = T{2} * (w * z + x * y);
		T cosy_cosp = T{1} - T{2} * (y * y + z * z);

		if (siny_cosp == T{0} && cosy_cosp == T{0})
			return T{0};

		return std::atan2(siny_cosp, cosy_cosp);
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE vec3_t<T> euler_angles() const noexcept {
		return vec3_t<T>{roll(), pitch(), yaw()};
	}

	[[nodiscard]] constexpr inline T angle() const noexcept {
		if (std::abs(w) > std::cos(T{0.5})) {
			const T a = std::asin(std::sqrt(x * x + y * y + z * z)) * T{2};
			if (w < T{0})
				return libv::pi_v<T> * T{2} - a;
			return a;
		}

		return std::acos(w) * T{2};
	}

	[[nodiscard]] constexpr inline vec3_t<T> axis() const noexcept {
		const T tmp1 = T{1} - w * w;

		if (tmp1 <= T{0})
			return vec3_t<T>{0, 0, 1};

		const T tmp2 = T{1} / std::sqrt(tmp1);
		return vec3_t<T>{x * tmp2, y * tmp2, z * tmp2};
	}

	// operator== ----------------------------------------------------------------------------------

	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator==(const quat_t& other) const noexcept = default;
	[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator!=(const quat_t& other) const noexcept = default;

	// operator<<(ostream, vec) --------------------------------------------------------------------

	template <typename OStream>
			requires requires (OStream& o, const T& m) { o << m; }
	friend inline OStream& operator<<(OStream& os, const quat_t& q) {
		return os << q.w << ' ' << q.x << ' ' << q.y << ' ' << q.z;
	}
};

// format ------------------------------------------------------------------------------------------
} // namespace libv

template <typename T>
struct fmt::formatter<libv::quat_t<T>, char, void> : public fmt::formatter<T, char, void> {
	const char* sep_begin = " ";
	const char* sep_end = sep_begin + 1;

	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		const auto specified = ctx.begin() != ctx.end();
		const auto begin = ctx.begin();
		auto end = begin;
		for (; end != ctx.end() && *end != '}'; ++end);

		if (!specified)
			return formatter<T, char, void>::parse(ctx);

		auto it_colon = begin;
		for (; it_colon != end && *it_colon != ':'; ++it_colon);
		if (it_colon == end) {
			sep_begin = &*begin;
			sep_end = &*end;
			ctx.advance_to(end);
			return formatter<T, char, void>::parse(ctx);
		}

		sep_begin = &*begin;
		sep_end = &*it_colon;
		ctx.advance_to(it_colon + 1);
		return formatter<T, char, void>::parse(ctx);
	}

	template <typename FormatContext>
	auto format(const libv::quat_t<T>& quat, FormatContext& ctx) const {
		auto out = ctx.out();

		out = formatter<T, char, void>::format(quat.w, ctx);
		for (auto s = sep_begin; s != sep_end; ++s)
			*out++ = *s;
		ctx.advance_to(out);

		out = formatter<T, char, void>::format(quat.x, ctx);
		for (auto s = sep_begin; s != sep_end; ++s)
			*out++ = *s;
		ctx.advance_to(out);

		out = formatter<T, char, void>::format(quat.y, ctx);
		for (auto s = sep_begin; s != sep_end; ++s)
			*out++ = *s;
		ctx.advance_to(out);

		out = formatter<T, char, void>::format(quat.z, ctx);
		ctx.advance_to(out);

		return out;
	}
};

namespace libv {
// =================================================================================================

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE T length(const quat_t<T>& q) noexcept {
	return q.length();
}

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE quat_t<T> inverse(const quat_t<T>& q) noexcept {
	return q.inverse_copy();
}

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE quat_t<T> conjugate(const quat_t<T>& q) noexcept {
	return q.conjugate_copy();
}

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE quat_t<T> normalize(const quat_t<T>& q) noexcept {
	return q.normalize_copy();
}

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE mat3_t<T> to_mat3(quat_t<T> q) noexcept {
	return q.to_mat3();
}

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE mat4_t<T> to_mat4(quat_t<T> q) noexcept {
	return q.to_mat4();
}

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE vec3_t<T> euler_angles(quat_t<T> q) noexcept {
	return q.euler_angles();
}

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE T dot(quat_t<T> q1, quat_t<T> q2) noexcept {
	return q1.w * q2.w +
			q1.x * q2.x +
			q1.y * q2.y +
			q1.z * q2.z;
}

template <typename T>
[[nodiscard]] constexpr inline quat_t<T> cross(quat_t<T> q1, quat_t<T> q2) noexcept {
	return quat_t<T>{
			q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
			q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
			q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
			q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x
	};
}

/// Use slerp (spherical linear interpolation) instead to get proper quaternion interpolation (unless you really want pure linear interpolation)
/// Lerp is only defined in [0, 1] range
///	\param q1 - Value at \c a = 0
///	\param q2 - Value at \c a = 1
///	\param a - Interpolation value. Has to be in the [0, 1] range
template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE quat_t<T> lerp(quat_t<T> q1, quat_t<T> q2, T a) noexcept {
	// assert(a >= T{0});
	// assert(a <= T{1});
	return q1 * (T{1} - a) + (q2 * a);
}

/// Spherical linear interpolation
/// The interpolation always take the short path and the rotation is performed at constant speed.
///	\param q1 - Value at \c a = 0
///	\param q2 - Value at \c a = 1
///	\param a - Interpolation value
template <typename T>
[[nodiscard]] constexpr inline quat_t<T> slerp(quat_t<T> q1, quat_t<T> q2, T a) noexcept {
	T cosTheta = dot(q1, q2);

	// If cosTheta < 0, the interpolation will take the long way around the sphere.
	// To fix this, one quat must be negated.
	if (cosTheta < T{0}) {
		q2 = -q2;
		cosTheta = -cosTheta;
	}

	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
	if (cosTheta > T{1} - static_cast<T>(0.0001)) {
		// Linear interpolation
		return quat_t<T>{
				libv::mix(q1.w, q2.w, a),
				libv::mix(q1.x, q2.x, a),
				libv::mix(q1.y, q2.y, a),
				libv::mix(q1.z, q2.z, a)
		};
	} else {
		// Essential Mathematics, page 467
		const T angle = std::acos(cosTheta);
		return (std::sin((T{1} - a) * angle) * q1 + std::sin(a * angle) * q2) / std::sin(angle);
	}
}

/// Spherical linear interpolation
/// The interpolation always take the short path and the rotation is performed at constant speed.
///	\param q1 - Value at \c a = 0
///	\param q2 - Value at \c a = 1
///	\param a - Interpolation value
///	\param spin_count - Spin count
template <typename T>
[[nodiscard]] constexpr inline quat_t<T> slerp(quat_t<T> q1, quat_t<T> q2, T a, int spin_count) noexcept {
	T cosTheta = dot(q1, q2);

	// If cosTheta < 0, the interpolation will take the long way around the sphere.
	// To fix this, one quat must be negated.
	if (cosTheta < T{0}) {
		q2 = -q2;
		cosTheta = -cosTheta;
	}

	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
	if (cosTheta > T{1} - static_cast<T>(0.0001)) {
		// Linear interpolation
		return quat_t<T>{
				libv::mix(q1.w, q2.w, a),
				libv::mix(q1.x, q2.x, a),
				libv::mix(q1.y, q2.y, a),
				libv::mix(q1.z, q2.z, a)
		};
	} else {
		// Graphics Gems III, page 96
		const T angle = std::acos(cosTheta);
		const T phi = angle + static_cast<T>(spin_count) * libv::pi_v<T>;
		return (std::sin(angle - a * phi) * q1 + std::sin(a * phi) * q2) / std::sin(angle);
	}
}

/// Spherical linear interpolation
/// The interpolation is oriented and the rotation is performed at constant speed.
/// For short path spherical linear interpolation, use the slerp function.
///	\param q1 - Value at \c a = 0
///	\param q2 - Value at \c a = 1
///	\param a - Interpolation value
///	\param spin_count - Spin count
template <typename T>
[[nodiscard]] constexpr inline quat_t<T> mix(quat_t<T> q1, quat_t<T> q2, T a) noexcept {
	T cosTheta = dot(q1, q2);

	// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
	if (cosTheta > T{1} - static_cast<T>(0.0001)) {
		// Linear interpolation
		return quat_t<T>{
				libv::mix(q1.w, q2.w, a),
				libv::mix(q1.x, q2.x, a),
				libv::mix(q1.y, q2.y, a),
				libv::mix(q1.z, q2.z, a)
		};
	} else {
		// Essential Mathematics, page 467
		const T angle = std::acos(cosTheta);
		return (std::sin((T{1} - a) * angle) * q1 + std::sin(a * angle) * q2) / std::sin(angle);
	}
}

template <typename T>
[[nodiscard]] constexpr inline T angle_between(quat_t<T> q1, quat_t<T> q2) noexcept {
	float cosTheta = std::min(std::abs(dot(q1, q2)), 1.0f);
	return cosTheta > static_cast<T>(0.99999) ? 0.0f : std::acos(cosTheta) * 2.0f;
}

/// Like \c slerp, but forbids rotation greater than maxAngle (in radians)
///	\param q1 - Value at if \c maxAngle <= 0
///	\param q2 - Value at if \c maxAngle >= angleBetween_Q1_Q2
///	\param maxAngle - (Radians)
template <typename T>
[[nodiscard]] constexpr inline quat_t<T> rotate_towards(quat_t<T> q1, quat_t<T> q2, T maxAngle) noexcept {

	if (maxAngle < static_cast<T>(0.0001))
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;

	float angle = angle_between(q1, q2);
	if (angle < maxAngle)
		return q2;

	return slerp(q1, q2, std::min(1.0f, maxAngle / angle));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
