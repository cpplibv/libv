// File: Vec.hpp, Created on 2014. január 7. 7:08, Author: Vader

#pragma once

// ext
#ifdef LIBV_USE_GLM_BRIDGE
#    include <glm/vec2.hpp>
#    include <glm/vec3.hpp>
#    include <glm/vec4.hpp>
#endif
// std
#include <cassert>
#include <cmath>
#include <ostream>
#include <utility>
// pro
#include <libv/type_traits.hpp>
#include <libv/utility.hpp>

// TODO P1: GCC 7.0+ C++17 fold expressions
// TODO P5: perfect forwarding for constructors
// TODO P5: reuse rvalues where possible ? 4 function
// TODO P5: macro for debug asserts
// TODO P5: make sure every function is sfiane friendly based on vec's T and it is producing a nice
//		error message not 7 page of template monster
// TODO PMSVC: Disable warnings for nameless struct on MSVC maybe:
//#pragma warning(push)
//#pragma warning(disable:4201) // warning C4201: nonstandard extension used : nameless struct/union
//#pragma warning(pop)

namespace libv {

// vec_t forward -----------------------------------------------------------------------------------

template <size_t N, typename T>
struct vec_t;

// build / make helper functions -------------------------------------------------------------------

template <size_t N, typename F, size_t... Indices>
constexpr inline auto build_vec_helper(F&& func, std::index_sequence<Indices...>) {
	return vec_t<N, decltype(std::declval<F>()(std::declval<size_t>()))>(func(Indices)...);
}

template <size_t N, typename F>
constexpr inline auto build_vec(F&& func) {
	return build_vec_helper<N>(std::forward<F>(func), std::make_index_sequence<N>{});
}

// vec_base_t --------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
// nonstandard extension used : nameless struct / union

template <size_t N, typename T, typename = void>
struct vec_base_t {
	T ptr[N];

	vec_base_t() = default;
	template <typename... Args, typename = typename std::enable_if<sizeof...(Args) == N>::type>
	constexpr explicit vec_base_t(Args&&... values) : ptr{std::forward<Args>(values)...} { }
};

template <typename T>
struct vec_base_t<2, T, enable_if<std::is_trivially_destructible<T>>> {
	union {
		struct {
			union { T x, r, s, u; };
			union { T y, g, t, v; };
		};
		T ptr[2];
	};

	constexpr vec_base_t() : x(), y() { }
	template <typename T0, typename T1>
	constexpr vec_base_t(T0 x, T1 y) : x(x), y(y) { }

#ifdef LIBV_USE_GLM_BRIDGE
	constexpr operator glm::tvec2<T>() const {
		return glm::tvec2<T>(x, y);
	}
	constexpr operator glm::tvec2<T>& () {
		return *reinterpret_cast<glm::tvec2<T>*>(this);
	}
	constexpr operator const glm::tvec2<T>& () const {
		return *reinterpret_cast<const glm::tvec2<T>*>(this);
	}
#endif
};

template <typename T>
struct vec_base_t<3, T, enable_if<std::is_trivially_destructible<T>>> {
	union {
		struct {
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };
		};
		T ptr[3];
	};

	constexpr vec_base_t() : x(), y(), z() { }
	template <typename T0, typename T1, typename T2>
	constexpr vec_base_t(T0 x, T1 y, T2 z) : x(x), y(y), z(z) { }
	template <typename T0, typename T1>
	constexpr vec_base_t(T0 x, const vec_base_t<2, T1>& yz) : x(x), y(yz.x), z(yz.y) { }
	template <typename T0, typename T1>
	constexpr vec_base_t(const vec_base_t<2, T0>& xy, T1 z) : x(xy.x), y(xy.y), z(z) { }

#ifdef LIBV_USE_GLM_BRIDGE
	constexpr operator glm::tvec3<T>() const {
		return glm::tvec3<T>(x, y, z);
	}
	constexpr operator glm::tvec3<T>& () {
		return *reinterpret_cast<glm::tvec3<T>*>(this);
	}
	constexpr operator const glm::tvec3<T>& () const {
		return *reinterpret_cast<const glm::tvec3<T>*>(this);
	}
#endif
};

template <typename T>
struct vec_base_t<4, T, enable_if<std::is_trivially_destructible<T>>> {
	union {
		struct {
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };
			union { T w, a, q; };
		};
		T ptr[4];
	};

	constexpr vec_base_t() : x(), y(), z(), w() { }
	template <typename T0, typename T1, typename T2, typename T3>
	constexpr vec_base_t(T0 x, T1 y, T2 z, T3 w) : x(x), y(y), z(z), w(w) { }
	template <typename T0, typename T1, typename T2>
	constexpr vec_base_t(const vec_base_t<2, T0>& xy, T1 z, T2 w) : x(xy.x), y(xy.y), z(z), w(w) { }
	template <typename T0, typename T1, typename T2>
	constexpr vec_base_t(T0 x, const vec_base_t<2, T1>& yz, T2 w) : x(x), y(yz.x), z(yz.y), w(w) { }
	template <typename T0, typename T1, typename T2>
	constexpr vec_base_t(T0 x, T1 y, const vec_base_t<2, T2>& zw) : x(x), y(y), z(zw.x), w(zw.y) { }
	template <typename T0, typename T1>
	constexpr vec_base_t(const vec_base_t<2, T0>& xy, const vec_base_t<2, T1>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	template <typename T0, typename T1>
	constexpr vec_base_t(const vec_base_t<3, T0>& xyz, T1 w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	template <typename T0, typename T1>
	constexpr vec_base_t(T0 x, const vec_base_t<3, T1>& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }

#ifdef LIBV_USE_GLM_BRIDGE
	constexpr operator glm::tvec4<T>() const {
		return glm::tvec4<T>(x, y, z, w);
	}
	constexpr operator glm::tvec4<T>& () {
		return *reinterpret_cast<glm::tvec4<T>*>(this);
	}
	constexpr operator const glm::tvec4<T>& () const {
		return *reinterpret_cast<const glm::tvec4<T>*>(this);
	}
#endif
};

#pragma GCC diagnostic pop

// vec_t -------------------------------------------------------------------------------------

template <size_t N, typename T>
struct vec_t : vec_base_t<N, T> {

	static constexpr size_t dim = N;
	using value_type = T;

	// constructors --------------------------------------------------------------------------------

	using vec_base_t<N, T>::vec_base_t;
	constexpr inline vec_t() : vec_base_t<N, T>() { }

	template <typename K, typename = decltype(T(std::declval<const K&>()))>
	constexpr explicit inline vec_t(const vec_t<N, K>& vec) {
		libv::n_times<N>([&](auto index) { this->ptr[index] = vec.ptr[index]; });
	}

	// operator[] ----------------------------------------------------------------------------------
	constexpr inline T& operator[](size_t i) {
		return this->ptr[i];
	}
	constexpr inline const T& operator[](size_t i) const {
		return this->ptr[i];
	}

	// operator= -----------------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t<N, T>& operator=(const vec_t<N, K>& rhs) {
		libv::n_times<N>([&](auto index) { this->ptr[index] = rhs.ptr[index]; });
		return *this;
	}

	// operator*=(scalar) --------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t<N, T>& operator+=(const K& v) {
		libv::n_times<N>([&](auto index) { this->ptr[index] += v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator-=(const K& v) {
		libv::n_times<N>([&](auto index) { this->ptr[index] -= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator*=(const K& v) {
		libv::n_times<N>([&](auto index) { this->ptr[index] *= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator/=(const K& v) {
		libv::n_times<N>([&](auto index) { this->ptr[index] /= v; });
		return *this;
	}

	// operator*=(vec) -----------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t<N, T>& operator+=(const vec_t<N, K>& rhs) {
		libv::n_times<N>([&](auto index) { this->ptr[index] += rhs.ptr[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator-=(const vec_t<N, K>& rhs) {
		libv::n_times<N>([&](auto index) { this->ptr[index] -= rhs.ptr[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator*=(const vec_t<N, K>& rhs) {
		libv::n_times<N>([&](auto index) { this->ptr[index] *= rhs.ptr[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator/=(const vec_t<N, K>& rhs) {
		libv::n_times<N>([&](auto index) { this->ptr[index] /= rhs.ptr[index]; });
		return *this;
	}

	// operator+ -----------------------------------------------------------------------------------
	constexpr inline vec_t<N, T> operator+() const {
		return build_vec<N>([&](auto index) { return +this->ptr[index]; });
	}

	constexpr inline vec_t<N, T> operator-() const {
		return build_vec<N>([&](auto index) { return -this->ptr[index]; });
	}

	// ---------------------------------------------------------------------------------------------

	/** Return the square length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	constexpr inline decltype(auto) lengthSQ() const {
		T result{};
		n_times<N>([&](auto index) { result += this->ptr[index] * this->ptr[index]; });
		return result;
	}

	/** Return the length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	constexpr inline decltype(auto) length() const {
		return std::sqrt(lengthSQ());
	}

	/** Normalize the vector (by divide each component by the length)
	 * @return The vector */
	constexpr inline vec_t<N, T>& normalize() {
		assert(lengthSQ() != 0);
		return operator/=(length());
	}

	/** Return the normalized vector (by divide each component by the length).
	 * @note Does not change the original vector
	 * @template K The minimum precision type Default: T
	 * @return The normalized vector */
	template <typename K = T>
	constexpr inline auto normalize_copy() const {
		assert(lengthSQ() != 0);
		auto l = length();
		return build_vec<N>([&](auto index) { return this->ptr[index] / l; });
	}

	// observers -----------------------------------------------------------------------------------
	template <typename F>
	constexpr inline void sequential_foreach(F&& func) {
		n_times<N>([&](auto index) { func(this->ptr[index]); });
	}
};

// operator*(vec, vec) -----------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs.ptr[index] + rhs.ptr[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs.ptr[index] - rhs.ptr[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs.ptr[index] * rhs.ptr[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs.ptr[index] / rhs.ptr[index]; });
}

// operator*(vec, scalar) --------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](auto index) { return lhs.ptr[index] + rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](auto index) { return lhs.ptr[index] - rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](auto index) { return lhs.ptr[index] * rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](auto index) { return lhs.ptr[index] / rhs; });
}

// operator*(scalar, vec) --------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs + rhs.ptr[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs - rhs.ptr[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs * rhs.ptr[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs / rhs.ptr[index]; });
}

// operator==(vec, vec) ----------------------------------------------------------------------------
template <size_t N, typename T, typename K>
constexpr inline bool operator==(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	bool result = true;
	n_times<N>([&](auto index) { result = result && lhs.ptr[index] == rhs.ptr[index]; });
	return result;
}
template <size_t N, typename T, typename K>
constexpr inline bool operator!=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	bool result = false;
	n_times<N>([&](auto index) { result = result || lhs.ptr[index] != rhs.ptr[index]; });
	return result;
}

// operator<(vec, vec) -----------------------------------------------------------------------------
template <size_t N, typename T, typename K>
constexpr inline bool operator<(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return lhs.lengthSQ() < rhs.lengthSQ();
}
template <size_t N, typename T, typename K>
constexpr inline bool operator<=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return lhs.lengthSQ() <= rhs.lengthSQ();
}
template <size_t N, typename T, typename K>
constexpr inline bool operator>(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return lhs.lengthSQ() > rhs.lengthSQ();
}
template <size_t N, typename T, typename K>
constexpr inline bool operator>=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return lhs.lengthSQ() >= rhs.lengthSQ();
}

// operator<(vec, scalar) --------------------------------------------------------------------------
template <size_t N, typename T, typename K>
constexpr inline bool operator<(const vec_t<N, T>& lhs, const K& rhs) {
	return lhs.lengthSQ() < rhs * rhs;
}
template <size_t N, typename T, typename K>
constexpr inline bool operator<=(const vec_t<N, T>& lhs, const K& rhs) {
	return lhs.lengthSQ() <= rhs * rhs;
}
template <size_t N, typename T, typename K>
constexpr inline bool operator>(const vec_t<N, T>& lhs, const K& rhs) {
	return lhs.lengthSQ() > rhs * rhs;
}
template <size_t N, typename T, typename K>
constexpr inline bool operator>=(const vec_t<N, T>& lhs, const K& rhs) {
	return lhs.lengthSQ() >= rhs * rhs;
}

// operator<(scalar, vec) --------------------------------------------------------------------------
template <size_t N, typename T, typename K>
constexpr inline bool operator<(const K& lhs, const vec_t<N, T>& rhs) {
	return lhs * lhs < rhs.lengthSQ();
}
template <size_t N, typename T, typename K>
constexpr inline bool operator<=(const K& lhs, const vec_t<N, T>& rhs) {
	return lhs * lhs <= rhs.lengthSQ();
}
template <size_t N, typename T, typename K>
constexpr inline bool operator>(const K& lhs, const vec_t<N, T>& rhs) {
	return lhs * lhs > rhs.lengthSQ();
}
template <size_t N, typename T, typename K>
constexpr inline bool operator>=(const K& lhs, const vec_t<N, T>& rhs) {
	return lhs * lhs >= rhs.lengthSQ();
}

// operator<<(ostream, vec) ------------------------------------------------------------------------
template <size_t N, typename T>
constexpr inline std::ostream& operator<<(std::ostream& os, const vec_t<N, T>& vec) {
	n_times<N>([&](auto index) { os << vec.ptr[index] << ' '; });
	return os;
}

namespace vec { // vec utility namespace -----------------------------------------------------------

// <editor-fold defaultstate="collapsed" desc="Generated Internal Macros: Custom vec to vec getters ...">
#define __libv_vec_get2(p1,p2)                                                                     \
	template <size_t N, typename T>                                                                \
	constexpr inline ::libv::vec_t<2, T> p1##p2(const ::libv::vec_t<N, T>& vec) {                  \
		return ::libv::vec_t<2, T>(vec.p1, vec.p2);                                                \
	}
#define __libv_vec_get3(p1,p2,p3)                                                                  \
	template <size_t N, typename T>                                                                \
	constexpr inline ::libv::vec_t<3, T> p1##p2##p3(const ::libv::vec_t<N, T>& vec) {              \
		return ::libv::vec_t<3, T>(vec.p1, vec.p2, vec.p3);                                        \
	}
#define __libv_vec_get4(p1,p2,p3,p4)                                                               \
	template <size_t N, typename T>                                                                \
	constexpr inline ::libv::vec_t<4, T> p1##p2##p3##p4(const ::libv::vec_t<N, T>& vec) {          \
		return ::libv::vec_t<4, T>(vec.p1, vec.p2, vec.p3, vec.p4);                                \
	}

#define __libv_vec_2p1(p1)      __libv_vec_get2(p1,x)      __libv_vec_get2(p1,y)      __libv_vec_get2(p1,z)      __libv_vec_get2(p1,w)
#define __libv_vec_2            __libv_vec_2p1(x)          __libv_vec_2p1(y)          __libv_vec_2p1(z)          __libv_vec_2p1(w)
#define __libv_vec_3p2(p1,p2)   __libv_vec_get3(p1,p2,x)   __libv_vec_get3(p1,p2,y)   __libv_vec_get3(p1,p2,z)   __libv_vec_get3(p1,p2,w)
#define __libv_vec_3p1(p1)      __libv_vec_3p2(p1,x)       __libv_vec_3p2(p1,y)       __libv_vec_3p2(p1,z)       __libv_vec_3p2(p1,w)
#define __libv_vec_3            __libv_vec_3p1(x)          __libv_vec_3p1(y)          __libv_vec_3p1(z)          __libv_vec_3p1(w)
#define __libv_vec_4p3(p1,p2,p3)__libv_vec_get4(p1,p2,p3,x)__libv_vec_get4(p1,p2,p3,y)__libv_vec_get4(p1,p2,p3,z)__libv_vec_get4(p1,p2,p3,w)
#define __libv_vec_4p2(p1,p2)   __libv_vec_4p3(p1,p2,x)    __libv_vec_4p3(p1,p2,y)    __libv_vec_4p3(p1,p2,z)    __libv_vec_4p3(p1,p2,w)
#define __libv_vec_4p1(p1)      __libv_vec_4p2(p1,x)       __libv_vec_4p2(p1,y)       __libv_vec_4p2(p1,z)       __libv_vec_4p2(p1,w)
#define __libv_vec_4            __libv_vec_4p1(x)          __libv_vec_4p1(y)          __libv_vec_4p1(z)          __libv_vec_4p1(w)

#define implement_to_vec2_gets() __libv_vec_2
#define implement_to_vec3_gets() __libv_vec_3
#define implement_to_vec4_gets() __libv_vec_4
// </editor-fold>

implement_to_vec2_gets()
implement_to_vec3_gets()
implement_to_vec4_gets()

/// \return The dot product of the two vector
template <size_t N, typename T, typename K>
constexpr inline auto dot(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	std::common_type_t<T, K> result{};
	n_times<N>([&](auto index) { result += lhs.ptr[index] * rhs.ptr[index]; });
	return result;
}

/// \return The cross product of the two vector
template <typename T, typename K>
constexpr inline auto cross(const vec_t<3, T>& lhs, const vec_t<3, K>& rhs) {
	return vec_t<3, T>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
}

/// \return The minimum vector with the smaller value on each dimension from the two vector
template <size_t N, typename T, typename K>
constexpr inline auto max(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return std::max(lhs.ptr[index], rhs.ptr[index]); });
}

/// \return The maximum vector with the bigger value on each dimension from the two vector
template <size_t N, typename T, typename K>
constexpr inline auto min(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return std::min(lhs.ptr[index], rhs.ptr[index]); });
}

/// \return The static casted vector to the requested K type
template <typename K, size_t N, typename T>
constexpr inline auto cast(const vec_t<N, T>& vec) {
	return build_vec<N>([&](auto index) { return static_cast<K>(vec.ptr[index]); });
}

template <size_t N, typename T>
constexpr inline auto round(const vec_t<N, T>& vec) {
	return build_vec<N>([&](auto index) { return std::round(vec.ptr[index]); });
}
template <size_t N, typename T>
constexpr inline auto lround(const vec_t<N, T>& vec) {
	return build_vec<N>([&](auto index) { return std::lround(vec.ptr[index]); });
}
template <size_t N, typename T>
constexpr inline auto llround(const vec_t<N, T>& vec) {
	return build_vec<N>([&](auto index) { return std::llround(vec.ptr[index]); });
}

} // namespace vec ---------------------------------------------------------------------------------

// =================================================================================================

#ifdef LIBV_USE_GLM_BRIDGE

#define LIBV_VEC_GLM_CONVERTER(N)                                                                  \
template <typename T> constexpr inline decltype(auto) from_glm(glm::tvec##N<T>& v) {               \
	return reinterpret_cast<vec_t<N, T>&>(v);                                                      \
}                                                                                                  \
template <typename T> constexpr inline decltype(auto) from_glm(const glm::tvec##N<T>& v) {         \
	return reinterpret_cast<const vec_t<N, T>&>(v);                                                \
}                                                                                                  \
template <typename T> constexpr inline decltype(auto) to_glm(vec_t<N, T>& v) {                     \
	return reinterpret_cast<glm::tvec##N<T>&>(v);                                                  \
}                                                                                                  \
template <typename T> constexpr inline decltype(auto) to_glm(const vec_t<N, T>& v) {               \
	return reinterpret_cast<const glm::tvec##N<T>&>(v);                                            \
}

LIBV_VEC_GLM_CONVERTER(2)
LIBV_VEC_GLM_CONVERTER(3)
LIBV_VEC_GLM_CONVERTER(4)

#undef LIBV_VEC_GLM_CONVERTER

#endif

// aliases -----------------------------------------------------------------------------------------

template <typename T> using vec2_t = vec_t<2, T>;
template <typename T> using vec3_t = vec_t<3, T>;
template <typename T> using vec4_t = vec_t<4, T>;

using vec2f = vec2_t<float>;
using vec3f = vec3_t<float>;
using vec4f = vec4_t<float>;
using vec2d = vec2_t<double>;
using vec3d = vec3_t<double>;
using vec4d = vec4_t<double>;
using vec2i = vec2_t<int32_t>;
using vec3i = vec3_t<int32_t>;
using vec4i = vec4_t<int32_t>;
using vec2u = vec2_t<uint32_t>;
using vec3u = vec3_t<uint32_t>;
using vec4u = vec4_t<uint32_t>;
using vec2ui = vec2_t<uint32_t>;
using vec3ui = vec3_t<uint32_t>;
using vec4ui = vec4_t<uint32_t>;
using vec2l = vec2_t<int64_t>;
using vec3l = vec3_t<int64_t>;
using vec4l = vec4_t<int64_t>;
using vec2ul = vec2_t<uint64_t>;
using vec3ul = vec3_t<uint64_t>;
using vec4ul = vec4_t<uint64_t>;

//using vec2 = vec2d;
//using vec3 = vec3d;
//using vec4 = vec4d;

} // namespace libv