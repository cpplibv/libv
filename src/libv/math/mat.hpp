// File: matrix.hpp Author: Vader Created on 2017. február 19., 19:55

#pragma once

// ext
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
// libv
#include <libv/meta/always.hpp>
#include <libv/meta/resolve.hpp>
// pro
#include <libv/math/angle.hpp>
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <size_t R, size_t C, typename T>
struct mat_t : public glm::mat<static_cast<int>(R), static_cast<int>(C), T, glm::precision::highp> {
	using value_type = T;
	static constexpr size_t Column = C;
	static constexpr size_t Row = R;
	static constexpr int Ci = static_cast<int>(C);
	static constexpr int Ri = static_cast<int>(R);

public:
	mat_t() : glm::mat<Ri, Ci, T, glm::precision::highp>() {}
	using glm::mat<Ri, Ci, T, glm::precision::highp>::mat;
	using glm::mat<Ri, Ci, T, glm::precision::highp>::operator=;

	inline constexpr mat_t(const mat_t&) = default;
	inline constexpr mat_t(mat_t&&) = default;
	inline ~mat_t() = default;

	inline mat_t& operator=(const mat_t& mat) & {
		mx() = mat.mx();
		return *this;
	}

	// ---------------------------------------------------------------------------------------------

private:
	using mt = glm::mat<Ri, Ci, T, glm::precision::highp>;
	[[nodiscard]] constexpr inline mt& mx() noexcept {
		return static_cast<mt&>(*this);
	}
	[[nodiscard]] constexpr inline const mt& mx() const noexcept {
		return static_cast<const mt&>(*this);
	}

public:
	[[nodiscard]] constexpr T* data() noexcept {
		return &mx()[0][0];
	}
	[[nodiscard]] constexpr const T* data() const noexcept {
		return &mx()[0][0];
	}

public:
	[[nodiscard]] constexpr inline value_type determinant() const noexcept {
		return glm::determinant(mx());
	}

	constexpr inline mat_t& inverse() noexcept {
		glm::inverse(mx());
		return *this;
	}
	[[nodiscard]] constexpr inline mat_t inverse_copy() const noexcept {
		mat_t result;
		result = glm::inverse(mx());
		return result;
	}

	constexpr inline mat_t& rotate(const Radian<T> angle, const libv::vec3_t<T>& axis) noexcept
			WISH_REQUIRES(Column == 4 && Row == 4) {
		mx() = glm::rotate(mx(), angle.value, to_glm(axis));
		return *this;
	}
	[[nodiscard]] constexpr inline mat_t rotate_copy(const Radian<T> angle, const libv::vec3_t<T>& axis) const noexcept
			WISH_REQUIRES(Column == 4 && Row == 4) {
		mat_t result;
		result = glm::rotate(mx(), angle.value, to_glm(axis));
		return result;
	}

	constexpr inline mat_t& rotate(const T angle, const libv::vec3_t<T>& axis) noexcept
			WISH_REQUIRES(Column == 4 && Row == 4) {
		mx() = glm::rotate(mx(), angle, to_glm(axis));
		return *this;
	}
	[[nodiscard]] constexpr inline mat_t rotate_copy(const T angle, const libv::vec3_t<T>& axis) const noexcept
			WISH_REQUIRES(Column == 4 && Row == 4) {
		mat_t result;
		result = glm::rotate(mx(), angle, to_glm(axis));
		return result;
	}

//	constexpr inline mat_t& rotate(const glm::quat& quat) noexcept {
//	[[nodiscard]] constexpr inline mat_t rotate_copy(const glm::quat& quat) const noexcept {

	constexpr inline mat_t& scale(const libv::vec3_t<T>& v) noexcept {
		mx() = glm::scale(mx(), to_glm(v));
		return *this;
	}
	[[nodiscard]] constexpr inline mat_t scale_copy(const libv::vec3_t<T>& v) const noexcept {
		mat_t result;
		result = glm::scale(mx(), to_glm(v));
		return result;
	}

	constexpr inline mat_t& translate(const libv::vec3_t<T>& v) noexcept {
		mx() = glm::translate(mx(), to_glm(v));
		return *this;
	}
	[[nodiscard]] constexpr inline mat_t translate_copy(const libv::vec3_t<T>& v) const noexcept {
		mat_t result;
		result = glm::translate(mx(), to_glm(v));
		return result;
	}

	// ---------------------------------------------------------------------------------------------

	[[nodiscard]] constexpr inline vec_t<R, T> operator*(const vec_t<R, T>& vec) const noexcept {
		return vec_t<R, T>{mx() * to_glm(vec)};
	}
	[[nodiscard]] constexpr inline mat_t operator*(const mat_t& mat) const noexcept {
		mat_t result;
		result = mx() * mat.mx();
		return result;
	}
	[[nodiscard]] constexpr inline decltype(auto) operator[](int col) noexcept {
		return mx()[col];
	}
	[[nodiscard]] constexpr inline decltype(auto) operator[](int col) const noexcept {
		return mx()[col];
	}
	[[nodiscard]] constexpr inline const vec_t<R, T> at_v(int col) const noexcept {
		return vec_t<R, T>{mx()[col]};
	}

	// ---------------------------------------------------------------------------------------------

	[[nodiscard]] static constexpr inline mat_t perspective(const T fovy, const T aspect, const T near, const T far) noexcept {
		mat_t result;
		result = glm::perspective<T>(fovy, aspect, near, far);
		return result;
	}

	[[nodiscard]] static constexpr inline mat_t lookAt(const libv::vec3_t<T> eye, const libv::vec3_t<T> target, const libv::vec3_t<T> up) noexcept {
		mat_t result;
		result = glm::lookAt<T>(to_glm(eye), to_glm(target), to_glm(up));
		return result;
	}

	[[nodiscard]] static constexpr inline mat_t ortho(const T left, const T right, const T bottom, const T top) noexcept {
		mat_t result;
		result = glm::ortho<T>(left, right, bottom, top);
		return result;
	}

	[[nodiscard]] static constexpr inline mat_t ortho(const libv::vec2_t<T> position, const libv::vec2_t<T> size) noexcept {
		return ortho(position.x, position.x + size.x, position.y, position.y + size.y);
	}

	[[nodiscard]] static constexpr inline mat_t ortho(const T left, const T right, const T bottom, const T top, const T near, const T far) noexcept {
		mat_t result;
		result = glm::ortho<T>(left, right, bottom, top, near, far);
		return result;
	}

	[[nodiscard]] static constexpr inline mat_t ortho(const libv::vec3_t<T> position, const libv::vec3_t<T> size) noexcept {
		return ortho(position.x, position.x + size.x, position.y, position.y + size.y, position.z, position.z + size.z);
	}

	[[nodiscard]] static constexpr inline mat_t identity() noexcept {
		return mat_t(1);
	}

	[[nodiscard]] static constexpr inline mat_t texture() noexcept
			WISH_REQUIRES(Column == 4 && Row == 4) {
		return mat_t(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0);
	}
};

// aliases -----------------------------------------------------------------------------------------

template <typename T> using mat2x2_t = mat_t<2, 2, T>;
template <typename T> using mat2x3_t = mat_t<2, 3, T>;
template <typename T> using mat2x4_t = mat_t<2, 4, T>;
template <typename T> using mat3x2_t = mat_t<3, 2, T>;
template <typename T> using mat3x3_t = mat_t<3, 3, T>;
template <typename T> using mat3x4_t = mat_t<3, 4, T>;
template <typename T> using mat4x2_t = mat_t<4, 2, T>;
template <typename T> using mat4x3_t = mat_t<4, 3, T>;
template <typename T> using mat4x4_t = mat_t<4, 4, T>;

template <typename T> using mat2_t = mat2x2_t<T>;
template <typename T> using mat3_t = mat3x3_t<T>;
template <typename T> using mat4_t = mat4x4_t<T>;

using mat2x2f = mat2x2_t<float>;
using mat2x3f = mat2x3_t<float>;
using mat2x4f = mat2x4_t<float>;
using mat2x2d = mat2x2_t<double>;
using mat2x3d = mat2x3_t<double>;
using mat2x4d = mat2x4_t<double>;
using mat3x2f = mat3x2_t<float>;
using mat3x3f = mat3x3_t<float>;
using mat3x4f = mat3x4_t<float>;
using mat3x2d = mat3x2_t<double>;
using mat3x3d = mat3x3_t<double>;
using mat3x4d = mat3x4_t<double>;
using mat4x2f = mat4x2_t<float>;
using mat4x3f = mat4x3_t<float>;
using mat4x4f = mat4x4_t<float>;
using mat4x2d = mat4x2_t<double>;
using mat4x3d = mat4x3_t<double>;
using mat4x4d = mat4x4_t<double>;

using mat2f = mat2x2_t<float>;
using mat3f = mat3x3_t<float>;
using mat4f = mat4x4_t<float>;
using mat2d = mat2x2_t<double>;
using mat3d = mat3x3_t<double>;
using mat4d = mat4x4_t<double>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
