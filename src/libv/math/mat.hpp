// Project: libv.math, File: src/libv/math/mat.hpp

#pragma once

// fwd
#include <libv/math/mat_fwd.hpp>
// ext
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
// libv
#include <libv/meta/for_constexpr.hpp>
// pro
#include <libv/math/angle.hpp>
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------
// TODO P4: Replace to_glm function with conversion operators, with that glm includes will become obsolete

template <typename T>
[[nodiscard]] constexpr inline auto to_glm(const vec_t<2, T>& v) noexcept {
	return glm::tvec2<T>(v.x, v.y);
}
template <typename T>
[[nodiscard]] constexpr inline auto to_glm(const vec_t<3, T>& v) noexcept {
	return glm::tvec3<T>(v.x, v.y, v.z);
}
template <typename T>
[[nodiscard]] constexpr inline auto to_glm(const vec_t<4, T>& v) noexcept {
	return glm::tvec4<T>(v.x, v.y, v.z, v.w);
}

// -------------------------------------------------------------------------------------------------

template <std::size_t R, std::size_t C, typename T>
struct mat_t : public glm::mat<static_cast<int>(C), static_cast<int>(R), T, glm::precision::highp> {
	using base = glm::mat<static_cast<int>(C), static_cast<int>(R), T, glm::precision::highp>;
	using value_type = T;
	static constexpr std::size_t Column = C;
	static constexpr std::size_t Row = R;
	static constexpr int Ci = static_cast<int>(C);
	static constexpr int Ri = static_cast<int>(R);

public:
	constexpr inline mat_t() : base() {}
	using base::mat;
	using base::operator=;

	constexpr inline mat_t(const mat_t&) = default;
	constexpr inline mat_t(mat_t&&) = default;
	inline ~mat_t() = default;

	inline mat_t& operator=(const mat_t& mat) & {
		mx() = mat.mx();
		return *this;
	}

	static constexpr inline mat_t from_rows(libv::vec_t<C, T> row0, libv::vec_t<C, T> row1)
			requires (Row == 2) {
		mat_t result;
		libv::meta::call_with_n_index<C>([&](auto... c) {
			((result[c][0] = row0[c]), ...);
			((result[c][1] = row1[c]), ...);
		});
		return result;
	}

	static constexpr inline mat_t from_rows(libv::vec_t<C, T> row0, libv::vec_t<C, T> row1, libv::vec_t<C, T> row2)
			requires (Row == 3) {
		mat_t result;
		libv::meta::call_with_n_index<C>([&](auto... c) {
			((result[c][0] = row0[c]), ...);
			((result[c][1] = row1[c]), ...);
			((result[c][2] = row2[c]), ...);
		});
		return result;
	}

	static constexpr inline mat_t from_rows(libv::vec_t<C, T> row0, libv::vec_t<C, T> row1, libv::vec_t<C, T> row2, libv::vec_t<C, T> row3)
			requires (Row == 4) {
		mat_t result;
		libv::meta::call_with_n_index<C>([&](auto... c) {
			((result[c][0] = row0[c]), ...);
			((result[c][1] = row1[c]), ...);
			((result[c][2] = row2[c]), ...);
			((result[c][3] = row3[c]), ...);
		});
		return result;
	}

	static constexpr inline mat_t from_columns(libv::vec_t<R, T> row0, libv::vec_t<R, T> row1)
			requires (Column == 2) {
		mat_t result;
		libv::meta::call_with_n_index<R>([&](auto... r) {
			((result[0][r] = row0[r]), ...);
			((result[1][r] = row1[r]), ...);
		});
		return result;
	}

	static constexpr inline mat_t from_columns(libv::vec_t<R, T> row0, libv::vec_t<R, T> row1, libv::vec_t<R, T> row2)
			requires (Column == 3) {
		mat_t result;
		libv::meta::call_with_n_index<R>([&](auto... r) {
			((result[0][r] = row0[r]), ...);
			((result[1][r] = row1[r]), ...);
			((result[2][r] = row2[r]), ...);
		});
		return result;
	}

	static constexpr inline mat_t from_columns(libv::vec_t<R, T> row0, libv::vec_t<R, T> row1, libv::vec_t<R, T> row2, libv::vec_t<R, T> row3)
			requires (Column == 4) {
		mat_t result;
		libv::meta::call_with_n_index<R>([&](auto... r) {
			((result[0][r] = row0[r]), ...);
			((result[1][r] = row1[r]), ...);
			((result[2][r] = row2[r]), ...);
			((result[3][r] = row3[r]), ...);
		});
		return result;
	}

	// ---------------------------------------------------------------------------------------------

private:
	using mt = base;
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
		mx() = glm::inverse(mx());
		return *this;
	}
	[[nodiscard]] constexpr inline mat_t inverse_copy() const noexcept {
		mat_t result;
		result = glm::inverse(mx());
		return result;
	}

	[[nodiscard]] static constexpr inline mat_t create_rotate(const radian<T> angle) noexcept
			requires (Column == 2 && Row == 2) {
		mat_t result;
		const auto s = std::sin(angle.value);
		const auto c = std::cos(angle.value);
		result[0][0] = c;
		result[0][1] = s;
		result[1][0] = -s;
		result[1][1] = c;
		return result;
	}
	constexpr inline mat_t& rotate(const radian<T> angle, const libv::vec3_t<T>& axis) noexcept
			requires (Column == 4 && Row == 4 || Column == 3 && Row == 3) {
		mx() = glm::rotate(mx(), angle.value, to_glm(axis));
		return *this;
	}
	[[nodiscard]] constexpr inline mat_t rotate_copy(const radian<T> angle, const libv::vec3_t<T>& axis) const noexcept
			requires (Column == 4 && Row == 4 || Column == 3 && Row == 3) {
		mat_t result;
		result = glm::rotate(mx(), angle.value, to_glm(axis));
		return result;
	}

	constexpr inline mat_t& rotate(const T angle, const libv::vec3_t<T>& axis) noexcept
			requires (Column == 4 && Row == 4 || Column == 3 && Row == 3) {
		mx() = glm::rotate(mx(), angle, to_glm(axis));
		return *this;
	}
	[[nodiscard]] constexpr inline mat_t rotate_copy(const T angle, const libv::vec3_t<T>& axis) const noexcept
			requires (Column == 4 && Row == 4 || Column == 3 && Row == 3) {
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

	constexpr inline mat_t& translate(const T x, const T y, const T z) noexcept {
		return translate(libv::vec3_t<T>(x, y, z));
	}
	[[nodiscard]] constexpr inline mat_t translate_copy(const T x, const T y, const T z) const noexcept {
		return translate_copy(libv::vec3_t<T>(x, y, z));
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
	[[nodiscard]] constexpr inline vec_t<C, T> at_v(int col) const noexcept {
		return vec_t<C, T>{mx()[col]};
	}

	[[nodiscard]] constexpr inline vec_t<R, T> column(int c) const noexcept {
		assert(c < Ci);
		return at_v(c);
	}
	[[nodiscard]] constexpr inline vec_t<C, T> row(int r) const noexcept {
		assert(r < Ri);
		vec_t<C, T> result;
		for (int i = 0; i < Ci; ++i)
			result[i] = mx()[i][r];
		return result;
	}

	// ---------------------------------------------------------------------------------------------

	[[nodiscard]] static constexpr inline mat_t perspective(const T fovy, const T aspect, const T near, const T far) noexcept {
		mat_t result;
		result = glm::perspective<T>(fovy, aspect, near, far);
		return result;
	}
	[[nodiscard]] static constexpr inline mat_t perspectiveRevereZ(const T fovy, const T aspect, const T near, const T far) noexcept {
		mat_t result;
		result = glm::perspectiveRH_ZO<T>(fovy, aspect, far, near);
		return result;
	}
	[[nodiscard]] static constexpr inline mat_t perspectiveRevereZInf(const T fovy, const T aspect, const T near) noexcept {
		float f = 1.0f / std::tan(fovy / 2.0f);
		mat_t result{
				f / aspect, 0.0f,  0.0f,  0.0f,
				0.0f,    f,  0.0f,  0.0f,
				0.0f, 0.0f,  0.0f, -1.0f,
				0.0f, 0.0f, near,  0.0f};
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

	[[nodiscard]] static constexpr inline mat_t ortho(const libv::vec2_t<T> position, const libv::vec2_t<T> size, const T near, const T far) noexcept {
		return ortho(position.x, position.x + size.x, position.y, position.y + size.y, near, far);
	}

	[[nodiscard]] static constexpr inline mat_t ortho(const T left, const T right, const T bottom, const T top, const T near, const T far) noexcept {
		mat_t result;
		result = glm::ortho<T>(left, right, bottom, top, near, far);
		return result;
	}

	[[nodiscard]] static constexpr inline mat_t ortho(const libv::vec3_t<T> position, const libv::vec3_t<T> size) noexcept {
		return ortho(position.x, position.x + size.x, position.y, position.y + size.y, position.z, position.z + size.z);
	}

//	[[nodiscard]] static constexpr inline mat_t make_translate(const libv::vec3_t<T>& v) noexcept {
//		mat_t result;
//		mx() = glm::translate(mx(), to_glm(v));
//		return result;
//	}
//
//	[[nodiscard]] static constexpr inline mat_t make_translate(const T x, const T y, const T z) noexcept {
//		return translate(libv::vec3_t<T>(x, y, z));
//	}

	[[nodiscard]] static constexpr inline mat_t identity() noexcept {
		return mat_t(1);
	}

	[[nodiscard]] static constexpr inline mat_t texture() noexcept
			requires (Column == 4 && Row == 4) {
		return mat_t(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
