// File: Vec.hpp, Created on 2014. január 7. 7:08, Author: Vader

#pragma once

#include <cassert>
#include <cmath>
#include <ostream>
#include <utility>

//TODO P1: glm implicit cast for vec
//TODO P1: vec implicit cast for glm
//TODO PMSVC: Disable warnings for nameless struct on MSVC
//#pragma warning(push)
//#pragma warning(disable:4201) // warning C4201: nonstandard extension used : nameless struct/union
//#pragma warning(pop)

//TODO P3: remove static_casts from the vecs
//TODO P5: perfect forwarding for constructors
//TODO P5: conditional noexcept
//TODO P5: macro for debug asserts
//TODO P5: make sure every function is sfiane frendly

namespace vl {

// <editor-fold defaultstate="collapsed" desc="Generated Internal Macros: Custom vec to vec getters ...">
#define __v2get(p1,p2)       /** Composite a new vector based on members @return The new vector */\
                   inline vec_base_t<2, T> p1##p2() const noexcept {        \
                        return vec_base_t<2, T>(p1,p2);}
#define __v3get(p1,p2,p3)    /** Composite a new vector based on members @return The new vector */\
                   inline vec_base_t<3, T> p1##p2##p3() const noexcept {    \
                       return vec_base_t<3, T>(p1,p2,p3);}
#define __v4get(p1,p2,p3,p4) /** Composite a new vector based on members @return The new vector */\
				   inline vec_base_t<4, T> p1##p2##p3##p4() const noexcept {\
                       return vec_base_t<4, T>(p1,p2,p3,p4);}

#define __v22p1(p1)      __v2get(p1,x)       __v2get(p1,y)
#define __v22            __v22p1(x)          __v22p1(y)
#define __v23p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)
#define __v23p1(p1)      __v23p2(p1,x)       __v23p2(p1,y)
#define __v23            __v23p1(x)          __v23p1(y)
#define __v24p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y)
#define __v24p2(p1,p2)   __v24p3(p1,p2,x)    __v24p3(p1,p2,y)
#define __v24p1(p1)	     __v24p2(p1,x)       __v24p2(p1,y)
#define __v24            __v24p1(x)          __v24p1(y)

#define __v32p1(p1)      __v2get(p1,x)       __v2get(p1,y)       __v2get(p1,z)
#define __v32            __v32p1(x)          __v32p1(y)          __v32p1(z)
#define __v33p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)    __v3get(p1,p2,z)
#define __v33p1(p1)      __v33p2(p1,x)       __v33p2(p1,y)       __v33p2(p1,z)
#define __v33            __v33p1(x)          __v33p1(y)          __v33p1(z)
#define __v34p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y) __v4get(p1,p2,p3,z)
#define __v34p2(p1,p2)   __v34p3(p1,p2,x)    __v34p3(p1,p2,y)    __v34p3(p1,p2,z)
#define __v34p1(p1)	     __v34p2(p1,x)       __v34p2(p1,y)       __v34p2(p1,z)
#define __v34            __v34p1(x)          __v34p1(y)          __v34p1(z)

#define __v42p1(p1)      __v2get(p1,x)       __v2get(p1,y)       __v2get(p1,z)       __v2get(p1,w)
#define __v42            __v42p1(x)          __v42p1(y)          __v42p1(z)          __v42p1(w)
#define __v43p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)    __v3get(p1,p2,z)    __v3get(p1,p2,w)
#define __v43p1(p1)      __v43p2(p1,x)       __v43p2(p1,y)       __v43p2(p1,z)       __v43p2(p1,w)
#define __v43            __v43p1(x)          __v43p1(y)          __v43p1(z)          __v43p1(w)
#define __v44p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y) __v4get(p1,p2,p3,z) __v4get(p1,p2,p3,w)
#define __v44p2(p1,p2)   __v44p3(p1,p2,x)    __v44p3(p1,p2,y)    __v44p3(p1,p2,z)    __v44p3(p1,p2,w)
#define __v44p1(p1)	     __v44p2(p1,x)       __v44p2(p1,y)       __v44p2(p1,z)       __v44p2(p1,w)
#define __v44            __v44p1(x)          __v44p1(y)          __v44p1(z)          __v44p1(w)
// </editor-fold>

#define implement_vec2_to_vec2_gets __v22
#define implement_vec2_to_vec3_gets __v23
#define implement_vec2_to_vec4_gets __v24
#define implement_vec3_to_vec2_gets __v32
#define implement_vec3_to_vec3_gets __v33
#define implement_vec3_to_vec4_gets __v34
#define implement_vec4_to_vec2_gets __v42
#define implement_vec4_to_vec3_gets __v43
#define implement_vec4_to_vec4_gets __v44

// -------------------------------------------------------------------------------------------------

// vec_base_t --------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
// nonstandard extension used : nameless struct/union

template <size_t N, typename T> struct vec_base_t {
	T ptr[N];

	vec_base_t() = default;

	template <typename... Args, typename = typename std::enable_if<sizeof...(Args) == N>::type>
	explicit vec_base_t(Args&&... values) noexcept : ptr{std::forward<Args>(values)...} {}
};

template <typename T> struct vec_base_t<2, T> {
	union {
		struct {
			union { T x, r, s, u; };
			union { T y, g, t, v; };
		};
		T ptr[2];
	};

	explicit vec_base_t() noexcept : x(), y() { }
	explicit vec_base_t(T x, T y) noexcept : x(x), y(y) { }

	implement_vec2_to_vec2_gets
	implement_vec2_to_vec3_gets
	implement_vec2_to_vec4_gets
};

template <typename T> struct vec_base_t<3, T> {
	union {
		struct {
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };
		};
		T ptr[3];
	};

	explicit vec_base_t() noexcept : x(), y(), z() { }
	explicit vec_base_t(T x, T y, T z) noexcept : x(x), y(y), z(z) { }
	explicit vec_base_t(T x, const vec_base_t<2, T>& yz) noexcept : x(x), y(yz.x), z(yz.y) { }
	explicit vec_base_t(const vec_base_t<2, T>& xy, T z) noexcept : x(xy.x), y(xy.y), z(z) { }

	implement_vec3_to_vec2_gets
	implement_vec3_to_vec3_gets
	implement_vec3_to_vec4_gets
};

template <typename T> struct vec_base_t<4, T> {
	union {
		struct {
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };
			union { T w, a, q; };
		};
		T ptr[4];
	};

	explicit vec_base_t() noexcept : x(), y(), z(), w() { }
	explicit vec_base_t(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) { }
	explicit vec_base_t(const vec_base_t<2, T>& xy, T z, T w) noexcept : x(xy.x), y(xy.y), z(z), w(w) { }
	explicit vec_base_t(T x, const vec_base_t<2, T>& yz, T w) noexcept : x(x), y(yz.x), z(yz.y), w(w) { }
	explicit vec_base_t(T x, T y, const vec_base_t<2, T>& zw) noexcept : x(x), y(y), z(zw.x), w(zw.y) { }
	explicit vec_base_t(const vec_base_t<2, T>& xy, const vec_base_t<2, T>& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	explicit vec_base_t(const vec_base_t<3, T>& xyz, T w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	explicit vec_base_t(T x, const vec_base_t<3, T>& yzw) noexcept : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }

	implement_vec4_to_vec2_gets
	implement_vec4_to_vec3_gets
	implement_vec4_to_vec4_gets
};

#pragma GCC diagnostic pop
// vec_helper_t forward ----------------------------------------------------------------------------

template <size_t N, typename T, typename = std::make_index_sequence<N>>
struct vec_helper_t;

// vec_iteration_helper ----------------------------------------------------------------------------

template<size_t N, size_t I = 0>
struct vec_iteration_helper {
	template<typename T, typename K>
	static inline bool eq(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
		return lhs.ptr[I] == rhs.ptr[I] && vec_iteration_helper<N, I + 1>::eq(lhs, rhs);
	}
	template<typename T, typename K>
	static inline bool ne(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
		return lhs.ptr[I] != rhs.ptr[I] && vec_iteration_helper<N, I + 1>::ne(lhs, rhs);
	}
	template<typename T>
	static inline decltype(auto) lengthSquare(const vec_helper_t<N, T>& v) noexcept {
		return v.ptr[I] * v.ptr[I] + vec_iteration_helper<N, I + 1>::lengthSquare(v);
	}
};
template<size_t N>
struct vec_iteration_helper<N, N> {
	template<typename T, typename K>
	static inline bool eq(const vec_helper_t<N, T>&, const vec_helper_t<N, K>&) noexcept {
		return true;
	}
	template<typename T, typename K>
	static inline bool ne(const vec_helper_t<N, T>&, const vec_helper_t<N, K>&) noexcept {
		return true;
	}
	template<typename T>
	static inline T lengthSquare(const vec_helper_t<N, T>&) noexcept {
		return 0;
	}
};

// vec_helper_t ------------------------------------------------------------------------------------

template <size_t N, typename T, size_t... Indices>
struct vec_helper_t<N, T, std::index_sequence<Indices...>> : vec_base_t<N, T> {

	// constructors --------------------------------------------------------------------------------

	using vec_base_t<N, T>::vec_base_t;

	inline vec_helper_t() : vec_base_t<N, T>() { }

	// cast ----------------------------------------------------------------------------------------

	template<typename K>
	inline operator vec_helper_t<N, K>() { // explicit ?
		return vec_helper_t<N, K>(static_cast<K>(this->ptr[Indices])...);
	}

	// operator= -----------------------------------------------------------------------------------

	inline vec_helper_t<N, T>& operator=(const vec_helper_t<N, T>& rhs) noexcept {
		(void) std::initializer_list<int>{
			((void) (this->ptr[Indices] = rhs.ptr[Indices]), 0)...
		};
		return *this;
	}

	// operator*=(vec) -----------------------------------------------------------------------------

	template<typename K>
	inline vec_helper_t<N, T>& operator+=(const vec_helper_t<N, K>& rhs) noexcept {
		(void) std::initializer_list<int>{
			((void) (this->ptr[Indices] += rhs.ptr[Indices]), 0)...
		};
		return *this;
	}

	template<typename K>
	inline vec_helper_t<N, T>& operator-=(const vec_helper_t<N, K>& rhs) noexcept {
		(void) std::initializer_list<int>{
			((void) (this->ptr[Indices] -= rhs.ptr[Indices]), 0)...
		};
		return *this;
	}

	template<typename K>
	inline vec_helper_t<N, T>& operator*=(const vec_helper_t<N, K>& rhs) noexcept {
		(void) std::initializer_list<int>{
			((void) (this->ptr[Indices] *= rhs.ptr[Indices]), 0)...
		};
		return *this;
	}

	template<typename K>
	inline vec_helper_t<N, T>& operator/=(const vec_helper_t<N, K>& rhs) noexcept {
		(void) std::initializer_list<int>{
			((void) (this->ptr[Indices] /= rhs.ptr[Indices]), 0)...
		};
		return *this;
	}

	// operator*=(skalar) --------------------------------------------------------------------------

	template<typename K>
	inline vec_helper_t<N, T>& operator*=(const K& v) noexcept {
		(void) std::initializer_list<int>{
			((void) (this->ptr[Indices] *= v), 0)...
		};
		return *this;
	}

	template<typename K>
	inline vec_helper_t<N, T>& operator/=(const K& v) noexcept {
		(void) std::initializer_list<int>{
			((void) (this->ptr[Indices] /= v), 0)...
		};
		return *this;
	}

	// ---------------------------------------------------------------------------------------------

	/** Return the square length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	inline decltype(auto) lengthSquare() const noexcept {
		return vec_iteration_helper<N>::lengthSquare(*this);
	}

	/** Return the length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	inline decltype(auto) length() const noexcept {
		return std::sqrt(lengthSquare());
	}

	/** Normalize the vector (by divide each component by the length)
	 * @return The vector */
	inline vec_helper_t<N, T>& normalize() noexcept {
		assert(lengthSquare() != 0);
		return operator/=(length());
	}

	/** Return the normalized vector (by divide each component by the length).
	 * @note Does not change the original vector
	 * @template K The minimum precision type Default: T
	 * @return The normalized vector */
	template<typename K = T>
	inline auto normalized() const noexcept
	-> vec_helper_t<N, decltype(std::declval<T>() / std::declval<K>())> {
		assert(lengthSquare() != 0);
		return *this / length();
	}

	// observers -----------------------------------------------------------------------------------

	template<typename F>
	void sequential_foreach(F&& func) {
		(void) std::initializer_list<int>{
			((void) func(this->ptr[Indices]), 0)...
		};
	}
};

// -------------------------------------------------------------------------------------------------

// operator*(vec, vec) -----------------------------------------------------------------------------

template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator-(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs,
		const vec_helper_t<N, K, std::index_sequence<Indices...>>& rhs) noexcept ->
vec_helper_t<N, decltype(std::declval<T>() - std::declval<K>())> {
	return vec_helper_t<N, decltype(std::declval<T>() - std::declval<K>())>(
				(lhs.ptr[Indices] - rhs.ptr[Indices])...
			);
}
template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator+(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs,
		const vec_helper_t<N, K, std::index_sequence<Indices...>>& rhs) noexcept ->
vec_helper_t<N, decltype(std::declval<T>() + std::declval<K>())> {
	return vec_helper_t<N, decltype(std::declval<T>() + std::declval<K>())>(
				(lhs.ptr[Indices] + rhs.ptr[Indices])...
			);
}
template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator*(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs,
		const vec_helper_t<N, K, std::index_sequence<Indices...>>& rhs) noexcept ->
vec_helper_t<N, decltype(std::declval<T>() * std::declval<K>())> {
	return vec_helper_t<N, decltype(std::declval<T>() * std::declval<K>())>(
				(lhs.ptr[Indices] * rhs.ptr[Indices])...
			);
}
template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator/(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs,
		const vec_helper_t<N, K, std::index_sequence<Indices...>>& rhs) noexcept ->
vec_helper_t<N, decltype(std::declval<T>() / std::declval<K>())> {
	return vec_helper_t<N, decltype(std::declval<T>() / std::declval<K>())>(
				(lhs.ptr[Indices] / rhs.ptr[Indices])...
			);
}

// operator*(vec, skalar) --------------------------------------------------------------------------

template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator*(const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs, const K& rhs)
noexcept -> vec_helper_t<N, decltype(std::declval<T>() * std::declval<K>())> {
	return vec_helper_t<N, decltype(std::declval<T>() * std::declval<K>())>(
				(lhs.ptr[Indices] * rhs)...
			);
}

template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator*(const K& lhs, const vec_helper_t<N, T, std::index_sequence<Indices...>>& rhs)
noexcept -> vec_helper_t<N, decltype(std::declval<K>() * std::declval<T>())> {
	return vec_helper_t<N, decltype(std::declval<K>() * std::declval<T>())>(
				(lhs * rhs.ptr[Indices])...
			);
}

// operator/(vec, skalar) --------------------------------------------------------------------------

template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator/(const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs, const K& rhs)
noexcept -> vec_helper_t<N, decltype(std::declval<T>() / std::declval<K>())> {
	return vec_helper_t<N, decltype(std::declval<T>() / std::declval<K>())>(
				(lhs.ptr[Indices] / rhs)...
			);
}

template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator/(const K& lhs, const vec_helper_t<N, T, std::index_sequence<Indices...>>& rhs)
noexcept -> vec_helper_t<N, decltype(std::declval<K>() / std::declval<T>())> {
	return vec_helper_t<N, decltype(std::declval<K>() / std::declval<T>())>(
				(lhs / rhs.ptr[Indices])...
			);
}

// operator==(vec, vec) ----------------------------------------------------------------------------
template<size_t N, typename T, typename K>
inline bool operator==(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
	return vec_iteration_helper<N>::eq(lhs, rhs);
}

template<size_t N, typename T, typename K>
inline bool operator!=(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
	return vec_iteration_helper<N>::ne(lhs, rhs);
}

// operator<(vec, vec) -----------------------------------------------------------------------------
template<size_t N, typename T, typename K>
inline bool operator<(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
	return lhs.lengthSquare() < rhs.lengthSquare();
}
template<size_t N, typename T, typename K>
inline bool operator<=(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
	return lhs.lengthSquare() <= rhs.lengthSquare();
}
template<size_t N, typename T, typename K>
inline bool operator>(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
	return lhs.lengthSquare() > rhs.lengthSquare();
}
template<size_t N, typename T, typename K>
inline bool operator>=(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
	return lhs.lengthSquare() >= rhs.lengthSquare();
}

// operator<(vec, skalar) --------------------------------------------------------------------------
template<size_t N, typename T, typename K>
inline bool operator<(const vec_helper_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length() < rhs;
}
template<size_t N, typename T, typename K>
inline bool operator<=(const vec_helper_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length() <= rhs;
}
template<size_t N, typename T, typename K>
inline bool operator>(const vec_helper_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length() > rhs;
}
template<size_t N, typename T, typename K>
inline bool operator>=(const vec_helper_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length() >= rhs;
}

// operator<(skalar, vec) --------------------------------------------------------------------------
template<size_t N, typename T, typename K>
inline bool operator<(const K& lhs, const vec_helper_t<N, T>& rhs) noexcept {
	return lhs < rhs.length();
}
template<size_t N, typename T, typename K>
inline bool operator<=(const K& lhs, const vec_helper_t<N, T>& rhs) noexcept {
	return lhs <= rhs.length();
}
template<size_t N, typename T, typename K>
inline bool operator>(const K& lhs, const vec_helper_t<N, T>& rhs) noexcept {
	return lhs > rhs.length();
}
template<size_t N, typename T, typename K>
inline bool operator>=(const K& lhs, const vec_helper_t<N, T>& rhs) noexcept {
	return lhs >= rhs.length();
}

// operator<<(ostream, vec) ------------------------------------------------------------------------

template<size_t N, typename T, size_t... Indices>
inline std::ostream& operator<<(
	std::ostream& os, const vec_helper_t<N, T, std::index_sequence<Indices...>>& v) {
	(void) std::initializer_list<int>{
		((void) (os << v.ptr[Indices] << ' '), 0)...
	};
	return os;
}

// maxByComponents ---------------------------------------------------------------------------------

template<size_t N, typename T, size_t... Indices>
inline vec_helper_t<N, T> maxByComponents(
	const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs,
	const vec_helper_t<N, T, std::index_sequence<Indices...>>& rhs) noexcept {
	return vec_helper_t<N, T>(
				std::max(lhs.ptr[Indices], rhs.ptr[Indices])...
			);
}

// =================================================================================================

// vec_t -------------------------------------------------------------------------------------------

template <size_t N, typename T> using vec_t = vec_helper_t<N, T>;

// aliases -----------------------------------------------------------------------------------------

template <typename T> using vec2_t = vec_t<2, T>;
template <typename T> using vec3_t = vec_t<3, T>;
template <typename T> using vec4_t = vec_t<4, T>;

using fvec2 = vec2_t<float>;
using fvec3 = vec3_t<float>;
using fvec4 = vec4_t<float>;
using dvec2 = vec2_t<double>;
using dvec3 = vec3_t<double>;
using dvec4 = vec4_t<double>;
using ivec2 = vec2_t<int32_t>;
using ivec3 = vec3_t<int32_t>;
using ivec4 = vec4_t<int32_t>;
using lvec2 = vec2_t<int64_t>;
using lvec3 = vec3_t<int64_t>;
using lvec4 = vec4_t<int64_t>;

using vec2 = vec2_t<float>;
using vec3 = vec3_t<float>;
using vec4 = vec4_t<float>;

} //namespace vl