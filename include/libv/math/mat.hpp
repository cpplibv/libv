// File:   matrix.hpp Author: Vader Created on 2017. február 19., 19:55

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
//#ifdef LIBV_USE_GLM_BRIDGE
//	operator glm::tmat3x3<T>() const {
//		return glm::tmat3x3<T>(m00, m01, m02, m10, m11, m12, m20, m21, m22);
//	}
//	operator glm::tmat3x3<T>& () {
//		return *reinterpret_cast<glm::tmat3x3<T>*> (this);
//	}
//	operator const glm::tmat3x3<T>& () const {
//		return *reinterpret_cast<const glm::tmat3x3<T>*> (this);
//	}
//#endif
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
//#ifdef LIBV_USE_GLM_BRIDGE
//	operator glm::tmat4x4<T>() const {
//		return glm::tmat4x4<T>(
//				m00, m01, m02, m03,
//				m10, m11, m12, m13,
//				m20, m21, m22, m23,
//				m30, m31, m32, m33);
//	}
//	operator glm::tmat4x4<T>& () {
//		return *reinterpret_cast<glm::tmat4x4<T>*> (this);
//	}
//	operator const glm::tmat4x4<T>& () const {
//		return *reinterpret_cast<const glm::tmat4x4<T>*> (this);
//	}
//#endif
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
	inline mat_base_t(const T& v) : mx(v) { }
	inline mat_base_t(
			const T& m00, const T& m01,
			const T& m10, const T& m11) :
		mx(
			m00, m01,
			m10, m11) { }
};

template <typename T>
struct mat_base_t<3, T> {
	glm::tmat3x3<T> mx;

	inline mat_base_t() = default;
	inline mat_base_t(const T& v) : mx(v) { }
	inline mat_base_t(
			const T& m00, const T& m01, const T& m02,
			const T& m10, const T& m11, const T& m12,
			const T& m20, const T& m21, const T& m22) :
		mx(
			m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22) { }
};

template <typename T>
struct mat_base_t<4, T> {
	glm::tmat4x4<T> mx;

	inline mat_base_t() = default;
	inline mat_base_t(const T& v) : mx(v) { }
	inline mat_base_t(
			const T& m00, const T& m01, const T& m02, const T& m03,
			const T& m10, const T& m11, const T& m12, const T& m13,
			const T& m20, const T& m21, const T& m22, const T& m23,
			const T& m30, const T& m31, const T& m32, const T& m33) :
		mx(
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33) { }
};

template <size_t N, typename T>
struct mat_t;

#define LIBV_MAT_GLM_CONVERTER(N)                                                                  \
template <typename T> inline decltype(auto) to_glm(libv::mat_t<N, T>& m) {                         \
	return reinterpret_cast<glm::tmat##N##x##N<T>&>(m);                                            \
}                                                                                                  \
template <typename T> inline decltype(auto) to_glm(const libv::mat_t<N, T>& m) {                   \
	return reinterpret_cast<const glm::tmat##N##x##N<T>&>(m);                                      \
}                                                                                                  \
template <typename T> inline decltype(auto) from_glm(glm::tmat##N##x##N<T>& m) {                   \
	return reinterpret_cast<mat_t<N, T>&>(m);                                                      \
}                                                                                                  \
template <typename T> inline decltype(auto) from_glm(const glm::tmat##N##x##N<T>& m) {             \
	return reinterpret_cast<const mat_t<N, T>&>(m);                                                \
}

LIBV_MAT_GLM_CONVERTER(2)
LIBV_MAT_GLM_CONVERTER(3)
LIBV_MAT_GLM_CONVERTER(4)

#undef LIBV_MAT_GLM_CONVERTER


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
		this->mx = glm::rotate(this->mx, angle, to_glm(axis));
		return *this;
	}
	inline mat_t<N, T> rotate_copy(T angle, const libv::vec3_t<T>& axis) const {
		return from_glm(glm::rotate(this->mx, angle, to_glm(axis)));
	}
	//rotate(const glm::quat& quat);
	inline mat_t<N, T>& scale(const libv::vec3_t<T>& v) {
		this->mx = glm::scale(this->mx, to_glm(v));
		return *this;
	}
	inline mat_t<N, T> scale_copy(T angle, const libv::vec3_t<T>& v) const {
		return from_glm(glm::scale(this->mx, to_glm(v)));
	}
	inline mat_t<N, T>& translate(const libv::vec3_t<T>& v) {
		this->mx = glm::translate(this->mx, to_glm(v));
		return *this;
	}
	inline mat_t<N, T> translate_copy(const libv::vec3_t<T>& v) const {
		return from_glm(glm::translate(this->mx, to_glm(v)));
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
	inline vec_t<N, T>& operator[](size_t col) {
		return from_glm(this->mx[col]);
	}
	inline const vec_t<N, T>& operator[](size_t col) const {
		return from_glm(this->mx[col]);
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
inline mat_t<4, T> ortho(const T& left, const T& right, const T& bottom, const T& top) {
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

//using mat2 = mat2f;
//using mat3 = mat3f;
//using mat4 = mat4f;

// -------------------------------------------------------------------------------------------------

} // namespace libv
