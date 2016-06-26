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

// TODO P1: add static_vec_cast
// TODO P1: vec implicit cast for glm // marked in code with "vec-glm" uses
// TODO P1: refactor to force unrolled loops to remove some of the template magic ?
// TODO P1: constexpr vec // this will be fun...
// TODO PMSVC: Disable warnings for nameless struct on MSVC
//#pragma warning(push)
//#pragma warning(disable:4201) // warning C4201: nonstandard extension used : nameless struct/union
//#pragma warning(pop)
// TODO P4: remove template recursion // Just a little left with iterators
// TODO P5: perfect forwarding for constructors
// TODO P5: conditional noexcept // this will be fun...
// TODO P5: fast compare operator with vec and skalar by comparing skalar² and veclenght²
// TODO P5: reuse rvalues where possible ?
// TODO P5: macro for debug asserts
// TODO P5: make sure every function is sfiane frendly based on vec's T and it is producing a nice
//		error message not 7 page of template monster

namespace libv {

// <editor-fold defaultstate="collapsed" desc="Generated Internal Macros: Custom vec to vec getters ...">
#define __v2get(p1,p2)       /** Composite a new vector based on members @return The new vector */\
                   inline vec_t<2, T> p1##p2() const noexcept {        \
                        return vec_t<2, T>(this->p1,this->p2);}
#define __v3get(p1,p2,p3)    /** Composite a new vector based on members @return The new vector */\
                   inline vec_t<3, T> p1##p2##p3() const noexcept {    \
                       return vec_t<3, T>(this->p1,this->p2,this->p3);}
#define __v4get(p1,p2,p3,p4) /** Composite a new vector based on members @return The new vector */\
				   inline vec_t<4, T> p1##p2##p3##p4() const noexcept {\
                       return vec_t<4, T>(this->p1,this->p2,this->p3,this->p4);}

#define __v22p1(p1)      __v2get(p1,x)       __v2get(p1,y)
#define __v22            __v22p1(x)          __v22p1(y)
#define __v23p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)
#define __v23p1(p1)      __v23p2(p1,x)       __v23p2(p1,y)
#define __v23            __v23p1(x)          __v23p1(y)
#define __v24p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y)
#define __v24p2(p1,p2)   __v24p3(p1,p2,x)    __v24p3(p1,p2,y)
#define __v24p1(p1)      __v24p2(p1,x)       __v24p2(p1,y)
#define __v24            __v24p1(x)          __v24p1(y)

#define __v32p1(p1)      __v2get(p1,x)       __v2get(p1,y)       __v2get(p1,z)
#define __v32            __v32p1(x)          __v32p1(y)          __v32p1(z)
#define __v33p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)    __v3get(p1,p2,z)
#define __v33p1(p1)      __v33p2(p1,x)       __v33p2(p1,y)       __v33p2(p1,z)
#define __v33            __v33p1(x)          __v33p1(y)          __v33p1(z)
#define __v34p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y) __v4get(p1,p2,p3,z)
#define __v34p2(p1,p2)   __v34p3(p1,p2,x)    __v34p3(p1,p2,y)    __v34p3(p1,p2,z)
#define __v34p1(p1)      __v34p2(p1,x)       __v34p2(p1,y)       __v34p2(p1,z)
#define __v34            __v34p1(x)          __v34p1(y)          __v34p1(z)

#define __v42p1(p1)      __v2get(p1,x)       __v2get(p1,y)       __v2get(p1,z)       __v2get(p1,w)
#define __v42            __v42p1(x)          __v42p1(y)          __v42p1(z)          __v42p1(w)
#define __v43p2(p1,p2)   __v3get(p1,p2,x)    __v3get(p1,p2,y)    __v3get(p1,p2,z)    __v3get(p1,p2,w)
#define __v43p1(p1)      __v43p2(p1,x)       __v43p2(p1,y)       __v43p2(p1,z)       __v43p2(p1,w)
#define __v43            __v43p1(x)          __v43p1(y)          __v43p1(z)          __v43p1(w)
#define __v44p3(p1,p2,p3)__v4get(p1,p2,p3,x) __v4get(p1,p2,p3,y) __v4get(p1,p2,p3,z) __v4get(p1,p2,p3,w)
#define __v44p2(p1,p2)   __v44p3(p1,p2,x)    __v44p3(p1,p2,y)    __v44p3(p1,p2,z)    __v44p3(p1,p2,w)
#define __v44p1(p1)      __v44p2(p1,x)       __v44p2(p1,y)       __v44p2(p1,z)       __v44p2(p1,w)
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

// vec_helper_t forward -----------------------------------------------------------------------------

template <size_t N, typename T>
struct vec_t;

template <size_t N, typename T, typename = std::make_index_sequence<N>>
struct vec_helper_t;

// type_trait --------------------------------------------------------------------------------------

template <typename T>
struct is_vec : std::false_type {
};

template <size_t N, typename T>
struct is_vec<vec_t<N, T>> : std::true_type {
};

template <typename T>
using is_vec_t = typename is_vec<T>::type;

template <size_t N, typename V>
struct is_vec_n_dim : std::false_type {
};

template <size_t N, typename T>
struct is_vec_n_dim<N, vec_t<N, T>> : std::true_type {
};

template <size_t N, typename T>
using is_vec_n_dim_t = typename is_vec_n_dim<N, T>::type;

// vec_base_t --------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
// nonstandard extension used : nameless struct/union

template <size_t N, typename T, typename = void>
struct vec_base_t {
	T ptr[N];

	vec_base_t() = default;
	template <typename... Args, typename = typename std::enable_if<sizeof...(Args) == N>::type>
	vec_base_t(Args&&... values) noexcept : ptr{std::forward<Args>(values)...} { }
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

	vec_base_t() noexcept : x(), y() { }
	template <typename T0, typename T1>
	vec_base_t(T0 x, T1 y) noexcept : x(x), y(y) { }

#ifdef LIBV_USE_GLM_BRIDGE
	explicit operator glm::tvec2<T>(){
		return glm::tvec2<T>(x, y);
	}
#endif

	implement_vec2_to_vec2_gets
	implement_vec2_to_vec3_gets
	implement_vec2_to_vec4_gets
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

	vec_base_t() noexcept : x(), y(), z() { }
	template <typename T0, typename T1, typename T2>
	vec_base_t(T0 x, T1 y, T2 z) noexcept : x(x), y(y), z(z) { }
	template <typename T0, typename T1>
	vec_base_t(T0 x, const vec_base_t<2, T1>& yz) noexcept : x(x), y(yz.x), z(yz.y) { }
	template <typename T0, typename T1>
	vec_base_t(const vec_base_t<2, T0>& xy, T1 z) noexcept : x(xy.x), y(xy.y), z(z) { }

#ifdef LIBV_USE_GLM_BRIDGE
	explicit operator glm::tvec3<T>(){
		return glm::tvec3<T>(x, y, z);
	}
#endif

	implement_vec3_to_vec2_gets
	implement_vec3_to_vec3_gets
	implement_vec3_to_vec4_gets
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

	vec_base_t() noexcept : x(), y(), z(), w() { }
	template <typename T0, typename T1, typename T2, typename T3>
	vec_base_t(T0 x, T1 y, T2 z, T3 w) noexcept : x(x), y(y), z(z), w(w) { }
	template <typename T0, typename T1, typename T2>
	vec_base_t(const vec_base_t<2, T0>& xy, T1 z, T2 w) noexcept : x(xy.x), y(xy.y), z(z), w(w) { }
	template <typename T0, typename T1, typename T2>
	vec_base_t(T0 x, const vec_base_t<2, T1>& yz, T2 w) noexcept : x(x), y(yz.x), z(yz.y), w(w) { }
	template <typename T0, typename T1, typename T2>
	vec_base_t(T0 x, T1 y, const vec_base_t<2, T2>& zw) noexcept : x(x), y(y), z(zw.x), w(zw.y) { }
	template <typename T0, typename T1>
	vec_base_t(const vec_base_t<2, T0>& xy, const vec_base_t<2, T1>& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	template <typename T0, typename T1>
	vec_base_t(const vec_base_t<3, T0>& xyz, T1 w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	template <typename T0, typename T1>
	vec_base_t(T0 x, const vec_base_t<3, T1>& yzw) noexcept : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }

#ifdef LIBV_USE_GLM_BRIDGE
	explicit operator glm::tvec4<T>(){
		return glm::tvec4<T>(x, y, z, w);
	}
#endif

	implement_vec4_to_vec2_gets
	implement_vec4_to_vec3_gets
	implement_vec4_to_vec4_gets
};

#pragma GCC diagnostic pop

// vec_iteration_helper ----------------------------------------------------------------------------

template<size_t N, size_t I = 0 >
struct vec_iteration_helper {
	template<typename T, typename K>
	static inline bool eq(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
		return lhs.ptr[I] == rhs.ptr[I] && vec_iteration_helper<N, I + 1 > ::eq(lhs, rhs);
	}
	template<typename T, typename K>
	static inline bool ne(const vec_helper_t<N, T>& lhs, const vec_helper_t<N, K>& rhs) noexcept {
		return lhs.ptr[I] != rhs.ptr[I] || vec_iteration_helper<N, I + 1 > ::ne(lhs, rhs);
	}
	template<typename T>
	static inline decltype(auto) lengthSquare(const vec_helper_t<N, T>& v) noexcept {
		return v.ptr[I] * v.ptr[I] + vec_iteration_helper<N, I + 1 >::lengthSquare(v);
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
		return false;
	}
	template<typename T>
	static inline T lengthSquare(const vec_helper_t<N, T>&) noexcept {
		return T();
	}
};

// vec_helper_t -------------------------------------------------------------------------------------

template <size_t N, typename T, size_t... Indices>
struct vec_helper_t<N, T, std::index_sequence<Indices...>> : vec_base_t<N, T> {

	// constructors --------------------------------------------------------------------------------

	using vec_base_t<N, T>::vec_base_t;
	inline vec_helper_t() : vec_base_t<N, T>() { }
	template<typename K, typename = decltype(T(std::declval<const K&>()))>
	explicit inline vec_helper_t(const vec_helper_t<N, K>& v) :
			vec_base_t<N, T>(static_cast<T>(v.ptr[Indices])...) { }

	// cast ----------------------------------------------------------------------------------------

	//	template<typename K>
	//	explicit inline operator vec_helper_t<N, K>() { // explicit ?
	//		return vec_t<N, K>(static_cast<K>(this->ptr[Indices])...);
	//	}

	// operator[] ----------------------------------------------------------------------------------
	inline T& operator[](size_t i) noexcept {
		return this->ptr[i];
	}
	inline const T& operator[](size_t i) const noexcept {
		return this->ptr[i];
	}

	// operator= -----------------------------------------------------------------------------------
	template<typename K>
	inline vec_t<N, T>& operator=(const vec_helper_t<N, K, std::index_sequence<Indices...>>& rhs) noexcept {
		(void) std::initializer_list<int> {
			((void) (this->ptr[Indices] = rhs.ptr[Indices]), 0)...
		};
		return static_cast<vec_t<N, T>&>(*this);
	}

	// operator*=(skalar) --------------------------------------------------------------------------
	template<typename K, typename = disable_if<is_vec_n_dim_t<N, K>>>
	inline vec_t<N, T>& operator*=(const K& v) noexcept {
		(void) std::initializer_list<int> {
			((void) (this->ptr[Indices] *= v), 0)...
		};
		return static_cast<vec_t<N, T>&>(*this);
	}

	template<typename K, typename = disable_if<is_vec_n_dim_t<N, K>>>
	inline vec_t<N, T>& operator/=(const K& v) noexcept {
		(void) std::initializer_list<int> {
			((void) (this->ptr[Indices] /= v), 0)...
		};
		return static_cast<vec_t<N, T>&>(*this);
	}

	// operator*=(vec) -----------------------------------------------------------------------------
	template<typename K>
	inline vec_t<N, T>& operator+=(const vec_t<N, K>& rhs) noexcept {
		(void) std::initializer_list<int> {
			((void) (this->ptr[Indices] += rhs.ptr[Indices]), 0)...
		};
		return static_cast<vec_t<N, T>&>(*this);
	}

	template<typename K>
	inline vec_t<N, T>& operator-=(const vec_t<N, K>& rhs) noexcept {
		(void) std::initializer_list<int> {
			((void) (this->ptr[Indices] -= rhs.ptr[Indices]), 0)...
		};
		return static_cast<vec_t<N, T>&>(*this);
	}

	template<typename K>
	inline vec_t<N, T>& operator*=(const vec_t<N, K>& rhs) noexcept {
		(void) std::initializer_list<int> {
			((void) (this->ptr[Indices] *= rhs.ptr[Indices]), 0)...
		};
		return static_cast<vec_t<N, T>&>(*this);
	}

	template<typename K>
	inline vec_t<N, T>& operator/=(const vec_t<N, K>& rhs) noexcept {
		(void) std::initializer_list<int> {
			((void) (this->ptr[Indices] /= rhs.ptr[Indices]), 0)...
		};
		return static_cast<vec_t<N, T>&>(*this);
	}

	// operator+ -----------------------------------------------------------------------------------
	inline vec_t<N, T> operator+() const noexcept {
		return vec_t<N, T>(+this->ptr[Indices]...);
	}

	inline vec_t<N, T> operator-() const noexcept {
		return vec_t<N, T>(-this->ptr[Indices]...);
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
	inline vec_t<N, T>& normalize() noexcept {
		assert(lengthSquare() != 0);
		return operator/=(length());
	}

	/** Return the normalized vector (by divide each component by the length).
	 * @note Does not change the original vector
	 * @template K The minimum precision type Default: T
	 * @return The normalized vector */
	template<typename K = T>
	inline auto normalized() const noexcept
	-> vec_t<N, decltype(std::declval<T>() / std::declval<K>())> {
		assert(lengthSquare() != 0);
		return *this / length();
	}

	// observers -----------------------------------------------------------------------------------
	template<typename F>
	void sequential_foreach(F&& func) {
		(void) std::initializer_list<int> {
			((void) (func(this->ptr[Indices])), 0)...
		};
	}
};

// vec_t -------------------------------------------------------------------------------------------

template <size_t N, typename T> struct vec_t : vec_helper_t<N, T> {
	using vec_helper_t<N, T>::vec_helper_t;
	using vec_helper_t<N, T>::operator=;
};

// operator*(vec, vec) -----------------------------------------------------------------------------
template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator-(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>&lhs,
		const vec_helper_t<N, K, std::index_sequence<Indices...>>&rhs) noexcept
-> vec_t<N, decltype(std::declval<T>() - std::declval<K>())> {
	return vec_t<N, decltype(std::declval<T>() - std::declval<K>())>(
			(lhs.ptr[Indices] - rhs.ptr[Indices])...
			);
}
template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator+(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>&lhs,
		const vec_helper_t<N, K, std::index_sequence<Indices...>>&rhs) noexcept
-> vec_t<N, decltype(std::declval<T>() + std::declval<K>())> {
	return vec_t<N, decltype(std::declval<T>() + std::declval<K>())>(
			(lhs.ptr[Indices] + rhs.ptr[Indices])...
			);
}
template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator*(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>&lhs,
		const vec_helper_t<N, K, std::index_sequence<Indices...>>&rhs) noexcept
-> vec_t<N, decltype(std::declval<T>() * std::declval<K>())> {
	return vec_t<N, decltype(std::declval<T>() * std::declval<K>())>(
			(lhs.ptr[Indices] * rhs.ptr[Indices])...
			);
}
template<size_t N, typename T, typename K, size_t... Indices>
inline auto operator/(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>&lhs,
		const vec_helper_t<N, K, std::index_sequence<Indices...>>&rhs) noexcept
-> vec_t<N, decltype(std::declval<T>() / std::declval<K>())> {
	return vec_t<N, decltype(std::declval<T>() / std::declval<K>())>(
			(lhs.ptr[Indices] / rhs.ptr[Indices])...
			);
}

// operator*(vec, skalar) --------------------------------------------------------------------------
template<size_t N, typename T, typename K, size_t... Indices,
typename = disable_if<is_vec_n_dim_t<N, K>>>
inline auto operator*(const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs, const K& rhs)
noexcept -> vec_t<N, decltype(std::declval<T>() * std::declval<K>())> {
	return vec_t<N, decltype(std::declval<T>() * std::declval<K>())>((lhs.ptr[Indices] * rhs)...);
}

template<size_t N, typename T, typename K, size_t... Indices,
typename = disable_if<is_vec_n_dim_t<N, K>>>
inline auto operator*(const K& lhs, const vec_helper_t<N, T, std::index_sequence<Indices...>>& rhs)
noexcept -> vec_t<N, decltype(std::declval<K>() * std::declval<T>())> {
	return vec_t<N, decltype(std::declval<K>() * std::declval<T>())>((lhs * rhs.ptr[Indices])...);
}

// operator/(vec, skalar) --------------------------------------------------------------------------
template<size_t N, typename T, typename K, size_t... Indices,
typename = disable_if<is_vec_n_dim_t<N, K>>>
inline auto operator/(const vec_helper_t<N, T, std::index_sequence<Indices...>>& lhs, const K& rhs)
noexcept -> vec_t<N, decltype(std::declval<T>() / std::declval<K>())> {
	return vec_t<N, decltype(std::declval<T>() / std::declval<K>())>((lhs.ptr[Indices] / rhs)...);
}

template<size_t N, typename T, typename K, size_t... Indices,
typename = disable_if<is_vec_n_dim_t<N, K>>>
inline auto operator/(const K& lhs, const vec_helper_t<N, T, std::index_sequence<Indices...>>& rhs)
noexcept -> vec_t<N, decltype(std::declval<K>() / std::declval<T>())> {
	return vec_t<N, decltype(std::declval<K>() / std::declval<T>())>((lhs / rhs.ptr[Indices])...);
}

// operator==(vec, vec) ----------------------------------------------------------------------------
template<size_t N, typename T, typename K>
inline bool operator==(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return vec_iteration_helper<N>::eq(lhs, rhs);
}
template<size_t N, typename T, typename K>
inline bool operator!=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return vec_iteration_helper<N>::ne(lhs, rhs);
}

// operator<(vec, vec) -----------------------------------------------------------------------------
template<size_t N, typename T, typename K>
inline bool operator<(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.lengthSquare() < rhs.lengthSquare();
}
template<size_t N, typename T, typename K>
inline bool operator<=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.lengthSquare() <= rhs.lengthSquare();
}
template<size_t N, typename T, typename K>
inline bool operator>(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.lengthSquare() > rhs.lengthSquare();
}
template<size_t N, typename T, typename K>
inline bool operator>=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.lengthSquare() >= rhs.lengthSquare();
}

// operator<(vec, skalar) --------------------------------------------------------------------------
template<size_t N, typename T, typename K>
inline bool operator<(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length() < rhs;
}
template<size_t N, typename T, typename K>
inline bool operator<=(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length() <= rhs;
}
template<size_t N, typename T, typename K>
inline bool operator>(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length() > rhs;
}
template<size_t N, typename T, typename K>
inline bool operator>=(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length() >= rhs;
}

// operator<(skalar, vec) --------------------------------------------------------------------------
template<size_t N, typename T, typename K>
inline bool operator<(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs < rhs.length();
}
template<size_t N, typename T, typename K>
inline bool operator<=(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs <= rhs.length();
}
template<size_t N, typename T, typename K>
inline bool operator>(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs > rhs.length();
}
template<size_t N, typename T, typename K>
inline bool operator>=(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs >= rhs.length();
}

// operator<<(ostream, vec) ------------------------------------------------------------------------
template<size_t N, typename T, size_t... Indices>
inline std::ostream& operator<<(
		std::ostream& os, const vec_helper_t<N, T, std::index_sequence<Indices...>>&v) {
	(void) std::initializer_list<int> {
		((void) (os << v.ptr[Indices] << ' '), 0)...
	};
	return os;
}

// maxByDimensions ---------------------------------------------------------------------------------
template<size_t N, typename T, size_t... Indices>
inline vec_t<N, T> maxByDimensions(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>&lhs,
		const vec_helper_t<N, T, std::index_sequence<Indices...>>&rhs) noexcept {
	return vec_t<N, T>(
			std::max(lhs.ptr[Indices], rhs.ptr[Indices])...
			);
}

// minByDimensions ---------------------------------------------------------------------------------
template<size_t N, typename T, size_t... Indices>
inline vec_t<N, T> minByDimensions(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>&lhs,
		const vec_helper_t<N, T, std::index_sequence<Indices...>>&rhs) noexcept {
	return vec_t<N, T>(
			std::min(lhs.ptr[Indices], rhs.ptr[Indices])...
			);
}

// vec_static_cast ---------------------------------------------------------------------------------

template<typename K, size_t N, typename T, size_t... Indices>
inline vec_t<N, K> vec_static_cast(
		const vec_helper_t<N, T, std::index_sequence<Indices...>>& vec) noexcept {
	return vec_t<N, K>(static_cast<K>(vec.ptr[Indices])...);
}

// =================================================================================================

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
using uivec2 = vec2_t<uint32_t>;
using uivec3 = vec3_t<uint32_t>;
using uivec4 = vec4_t<uint32_t>;
using lvec2 = vec2_t<int64_t>;
using lvec3 = vec3_t<int64_t>;
using lvec4 = vec4_t<int64_t>;
using ulvec2 = vec2_t<uint64_t>;
using ulvec3 = vec3_t<uint64_t>;
using ulvec4 = vec4_t<uint64_t>;

using vec2 = vec2_t<float>;
using vec3 = vec3_t<float>;
using vec4 = vec4_t<float>;

} //namespace libv