// File: matrix.hpp Author: Vader Created on 2017. február 19., 19:55

#pragma once

// ext
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
// pro
#include <libv/math/vec.hpp>


namespace libv {

//// mat_base_t --------------------------------------------------------------------------------------
//
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpedantic"
//// nonstandard extension used : nameless struct / union
//
//template <size_t N, typename T, typename = void>
//struct mat_base_t {
//	T value[N * N];
//
//	vec_base_t() = default;
//	template <typename... Args, typename = typename std::enable_if<sizeof...(Args) == N * N>::type>
//	explicit mat_base_t(Args&&... values) noexcept : ptr{std::forward<Args>(values)...} { }
//};
//
//template <typename T>
//struct mat_base_t<3, T, enable_if<std::is_trivially_destructible<T>>> {
//	union {
//		struct {
//			T
//				m00, m01, m02,
//				m10, m11, m12,
//				m20, m21, m22;
//		};
//		struct {
//			vec_t<3, T> col0;
//			vec_t<3, T> col1;
//			vec_t<3, T> col2;
//		};
//		T value[9];
//	};
//
//};
//
//template <typename T>
//struct mat_base_t<4, T, enable_if<std::is_trivially_destructible<T>>> {
//	union {
//		struct {
//			T
//				m00, m01, m02, m03,
//				m10, m11, m12, m13,
//				m20, m21, m22, m23,
//				m30, m31, m32, m33;
//		};
//		struct {
//			vec_t<4, T> col0;
//			vec_t<4, T> col1;
//			vec_t<4, T> col2;
//			vec_t<4, T> col3;
//		};
//		T value[16];
//	};
//
//};
//
//#pragma GCC diagnostic pop
//
//// -------------------------------------------------------------------------------------------------
//
//template <size_t N, typename T>
//struct mat_helper_t : mat_base_t<N, T> {
//
//};

// -------------------------------------------------------------------------------------------------


template <size_t N, typename T>
struct mat_base_t {
};

template <typename T>
struct mat_base_t<2, T> {
	glm::tmat2x2<T> mx;

	inline mat_base_t() = default;
	inline mat_base_t(T v) : mx(v) { }
	inline mat_base_t(glm::tmat2x2<T> mx) : mx(mx) { }
	inline mat_base_t(
			T m00, T m01,
			T m10, T m11) :
		mx(
			m00, m01,
			m10, m11) { }
};

template <typename T>
struct mat_base_t<3, T> {
	glm::tmat3x3<T> mx;

	inline mat_base_t() = default;
	inline mat_base_t(T v) : mx(v) { }
	inline mat_base_t(glm::tmat3x3<T> mx) : mx(mx) { }
	inline mat_base_t(
			T m00, T m01, T m02,
			T m10, T m11, T m12,
			T m20, T m21, T m22) :
		mx(
			m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22) { }
};

template <typename T>
struct mat_base_t<4, T> {
	glm::tmat4x4<T> mx;

	inline mat_base_t() = default;
	inline mat_base_t(T v) : mx(v) { }
	inline mat_base_t(glm::tmat4x4<T> mx) : mx(mx) { }
	inline mat_base_t(
			T m00, T m01, T m02, T m03,
			T m10, T m11, T m12, T m13,
			T m20, T m21, T m22, T m23,
			T m30, T m31, T m32, T m33) :
		mx(
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33) { }
};

template <size_t N, typename T>
struct mat_t;

template <typename T> inline auto to_glm(const mat_t<2, T>& m) {
	return m.mx;
}
template <typename T> inline auto to_glm(const mat_t<3, T>& m) {
	return m.mx;
}
template <typename T> inline auto to_glm(const mat_t<4, T>& m) {
	return m.mx;
}
template <typename T> inline auto from_glm(const glm::tmat2x2<T>& m) {
	return mat_t<2, T>{m};
}
template <typename T> inline auto from_glm(const glm::tmat3x3<T>& m) {
	return mat_t<3, T>{m};
}
template <typename T> inline auto from_glm(const glm::tmat4x4<T>& m) {
	return mat_t<4, T>{m};
}

template <size_t N, typename T>
struct mat_t : mat_base_t<N, T> {
	using value_type = T;

	// ---------------------------------------------------------------------------------------------

	using mat_base_t<N, T>::mat_base_t;

	// ---------------------------------------------------------------------------------------------

	inline mat_t<N, T>& inverse() {
		this->mx = glm::inverse(this->mx);
		return *this;
	}
	inline mat_t<N, T> inverse_copy() const {
		return from_glm(glm::inverse(this->mx));
	}
	inline mat_t<N, T>& rotate(T angle, const libv::vec3_t<T>& axis) {
		this->mx *= glm::rotate(glm::mat4{1.0f}, angle, to_glm(axis));
		return *this;
	}
	inline mat_t<N, T> rotate_copy(T angle, const libv::vec3_t<T>& axis) const {
		return from_glm(this->mx * glm::rotate(glm::mat4{1.0f}, angle, to_glm(axis)));
	}
	//rotate(const glm::quat& quat);
	inline mat_t<N, T>& scale(const libv::vec3_t<T>& v) {
		this->mx *= glm::scale(glm::mat4{1.0f}, to_glm(v));
		return *this;
	}
	inline mat_t<N, T> scale_copy(T angle, const libv::vec3_t<T>& v) const {
		return from_glm(this->mx * glm::scale(glm::mat4{1.0f}, to_glm(v)));
	}
	inline mat_t<N, T>& translate(const libv::vec3_t<T>& v) {
		this->mx *= glm::translate(glm::mat4{1.0f}, to_glm(v));
		return *this;
	}
	inline mat_t<N, T> translate_copy(const libv::vec3_t<T>& v) const {
		return from_glm(this->mx * glm::translate(glm::mat4{1.0f}, to_glm(v)));
	}

	// ---------------------------------------------------------------------------------------------

	inline mat_t<N, T>& operator=(const mat_t<N, T>& mat) {
		this->mx = mat.mx;
		return *this;
	}
	inline vec_t<N, T> operator*(const vec_t<N, T>& vec) const {
		return from_glm(this->mx * to_glm(vec));
	}
	inline mat_t<N, T> operator*(const mat_t<N, T>& mat) const {
		return from_glm(this->mx * mat.mx);
	}
	inline decltype(auto) operator[](size_t col) {
		return this->mx[col];
	}
	inline decltype(auto) operator[](size_t col) const {
		return this->mx[col];
	}

	// ---------------------------------------------------------------------------------------------
};

// global matrix creators --------------------------------------------------------------------------

template <typename T>
inline mat_t<4, T> perspective(T fovy, T aspect, T near, T far) {
	return from_glm(glm::perspective<T>(fovy, aspect, near, far));
}

template <typename T>
inline mat_t<4, T> lookAt(const libv::vec3_t<T>& eye, const libv::vec3_t<T>& target, const libv::vec3_t<T>& up) {
	return from_glm(glm::lookAt<T>(to_glm(eye), to_glm(target), to_glm(up)));
}

template <typename T>
inline mat_t<4, T> ortho(T left, T right, T bottom, T top) {
	return from_glm(glm::ortho<T>(left, right, bottom, top));
}

template <size_t N, typename T>
inline mat_t<N, T> identity() {
	return mat_t<N, T>(static_cast<T>(1.0));
}

// -------------------------------------------------------------------------------------------------

template <size_t N, typename T>
inline T* value_ptr(mat_t<N, T>& mat) {
	return &mat[0][0];
}
template <size_t N, typename T>
inline const T* value_ptr(const mat_t<N, T>& mat) {
	return &mat[0][0];
}

// aliases -----------------------------------------------------------------------------------------

template <typename T> using mat2_t = mat_t<2, T>;
template <typename T> using mat3_t = mat_t<3, T>;
template <typename T> using mat4_t = mat_t<4, T>;

using mat2f = mat2_t<float>;
using mat3f = mat3_t<float>;
using mat4f = mat4_t<float>;
using mat2d = mat2_t<double>;
using mat3d = mat3_t<double>;
using mat4d = mat4_t<double>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
