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
#include <utility>
#include <array>
// pro
#include <libv/meta/n_times.hpp>
#include <libv/meta/type_traits.hpp>


// TODO P4: vec noexcept
//	example:
//		template <size_t N, typename T, typename K>
//		constexpr inline auto operator/(const vec_t<N, T>& lhs, const K& rhs) LIBV_RETURNS(
//			build_vec<N>([&](auto index) { return lhs.data[index] / rhs; }
//		)
//
// TODO P5: use warning disable macros with: _Pragma("argument")
// TODO P5: macro for debug asserts (mostly for div by 0)

// NOTE: Use concepts to enable every operation based on underlying types
// NOTE: Perfect forwarding is possible, seams like does not worth +600 line template
// NOTE: MSVC: Disable warnings for nameless struct on MSVC maybe:
//#pragma warning(push)
//#pragma warning(disable:4201) // warning C4201: nonstandard extension used : nameless struct/union
//#pragma warning(pop)

namespace libv {

// vec_t forward -----------------------------------------------------------------------------------

template <size_t N, typename T>
struct vec_t;

// build / make helper functions -------------------------------------------------------------------

template <size_t N, typename F>
constexpr inline decltype(auto) build_vec(F&& func) {
	return meta::call_with_n_index<N>([&](const auto... indices) {
		return vec_t<N, decltype(func(std::declval<size_t>()))>(func(indices)...);
	});
}

// vec bridges -------------------------------------------------------------------------------------

template <typename Vec, typename Target>
struct enable_implicit_vec_cast {};

#ifdef LIBV_USE_GLM_BRIDGE

template <typename T, glm::precision P>
struct enable_implicit_vec_cast<vec_t<1, T>, glm::tvec1<T, P>> { using type = void; };
template <typename T, glm::precision P>
struct enable_implicit_vec_cast<vec_t<2, T>, glm::tvec2<T, P>> { using type = void; };
template <typename T, glm::precision P>
struct enable_implicit_vec_cast<vec_t<3, T>, glm::tvec3<T, P>> { using type = void; };
template <typename T, glm::precision P>
struct enable_implicit_vec_cast<vec_t<4, T>, glm::tvec4<T, P>> { using type = void; };

#endif

// vec_base_t --------------------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
// nonstandard extension used : nameless struct / union

template <size_t N, typename T, typename = void>
struct vec_base_t {
	std::array<T, N> data;

	constexpr inline vec_base_t() : data{} { }
	constexpr inline vec_base_t(const vec_base_t& orig) : data{orig.data} { }
	constexpr inline vec_base_t(vec_base_t&& orig) : data{std::move(orig.data)} { }
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) { data = orig.data; return *this; }
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) { data = std::move(orig.data); return *this; }

	template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
	constexpr inline explicit vec_base_t(Args&&... values) : data{std::forward<Args>(values)...} { }
};

template <typename T>
struct vec_base_t<2, T, std::enable_if_t<std::is_trivially_destructible_v<T>>> {
	union {
		struct {
			union { T x, r, s, u; };
			union { T y, g, t, v; };
		};
		std::array<T, 2> data;
	};

	constexpr inline vec_base_t() : data{} { }
	constexpr inline vec_base_t(const vec_base_t& orig) : data{orig.data} { }
	constexpr inline vec_base_t(vec_base_t&& orig) : data{std::move(orig.data)} { }
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) { data = orig.data; return *this; }
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) { data = std::move(orig.data); return *this; }

	template <typename T0, typename T1>
	constexpr inline vec_base_t(T0 x, T1 y) : x(x), y(y) { }
	template <typename T0>
	constexpr inline explicit vec_base_t(const T0& xy) : x(xy.x), y(xy.y) { }
};

template <typename T>
struct vec_base_t<3, T, std::enable_if_t<std::is_trivially_destructible_v<T>>> {
	union {
		struct {
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };
		};
		std::array<T, 3> data;
	};

	constexpr inline vec_base_t() : data{} { }
	constexpr inline vec_base_t(const vec_base_t& orig) : data{orig.data} { }
	constexpr inline vec_base_t(vec_base_t&& orig) : data{std::move(orig.data)} { }
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) { data = orig.data; return *this; }
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) { data = std::move(orig.data); return *this; }

	template <typename T0, typename T1, typename T2>
	constexpr inline vec_base_t(T0 x, T1 y, T2 z) : x(x), y(y), z(z) { }
	template <typename T0, typename T1>
	constexpr inline vec_base_t(T0 x, const vec_base_t<2, T1>& yz) : x(x), y(yz.x), z(yz.y) { }
	template <typename T0, typename T1>
	constexpr inline vec_base_t(const vec_base_t<2, T0>& xy, T1 z) : x(xy.x), y(xy.y), z(z) { }
	template <typename T0>
	constexpr inline explicit vec_base_t(const T0& xyz) : x(xyz.x), y(xyz.y), z(xyz.z) { }
};

template <typename T>
struct vec_base_t<4, T, std::enable_if_t<std::is_trivially_destructible_v<T>>> {
	union {
		struct {
			union { T x, r, s; };
			union { T y, g, t; };
			union { T z, b, p; };
			union { T w, a, q; };
		};
		std::array<T, 4> data;
	};

	constexpr inline vec_base_t() : data{} { }
	constexpr inline vec_base_t(const vec_base_t& orig) : data{orig.data} { }
	constexpr inline vec_base_t(vec_base_t&& orig) : data{std::move(orig.data)} { }
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) { data = orig.data; return *this; }
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) { data = std::move(orig.data); return *this; }

	template <typename T0, typename T1, typename T2, typename T3>
	constexpr inline vec_base_t(T0 x, T1 y, T2 z, T3 w) : x(x), y(y), z(z), w(w) { }
	template <typename T0, typename T1, typename T2>
	constexpr inline vec_base_t(const vec_base_t<2, T0>& xy, T1 z, T2 w) : x(xy.x), y(xy.y), z(z), w(w) { }
	template <typename T0, typename T1, typename T2>
	constexpr inline vec_base_t(T0 x, const vec_base_t<2, T1>& yz, T2 w) : x(x), y(yz.x), z(yz.y), w(w) { }
	template <typename T0, typename T1, typename T2>
	constexpr inline vec_base_t(T0 x, T1 y, const vec_base_t<2, T2>& zw) : x(x), y(y), z(zw.x), w(zw.y) { }
	template <typename T0, typename T1>
	constexpr inline vec_base_t(const vec_base_t<2, T0>& xy, const vec_base_t<2, T1>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	template <typename T0, typename T1>
	constexpr inline vec_base_t(const vec_base_t<3, T0>& xyz, T1 w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	template <typename T0, typename T1>
	constexpr inline vec_base_t(T0 x, const vec_base_t<3, T1>& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }
	template <typename T0>
	constexpr inline explicit vec_base_t(const T0& xyzw) : x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w) { }
};

#pragma GCC diagnostic pop

// vec_t -------------------------------------------------------------------------------------------

template <size_t N, typename T>
struct vec_t : vec_base_t<N, T> {

	static constexpr inline size_t dim = N;
	using value_type = T;

	// constructors --------------------------------------------------------------------------------
	constexpr inline vec_t() = default;
	constexpr inline vec_t(const vec_t&) = default;
	constexpr inline vec_t(vec_t&&) = default;
	constexpr inline vec_t& operator=(const vec_t&) = default;
	constexpr inline vec_t& operator=(vec_t&&) = default;

	using vec_base_t<N, T>::vec_base_t;

	template <typename K, typename = decltype(T(std::declval<const K&>()))>
	constexpr explicit inline vec_t(const vec_t<N, K>& vec) {
		meta::n_times<N>([&](auto index) { this->data[index] = vec.data[index]; });
	}

	// converters ----------------------------------------------------------------------------------
	template <typename K, typename = typename enable_implicit_vec_cast<vec_t<N, T>, K>::type>
	constexpr inline operator K() const {
		return meta::call_with_n_index([&](const auto... indices) {
			return K{this->data[indices]...};
		});
	}
	template <typename K, typename = typename enable_implicit_vec_cast<vec_t<N, T>, K>::type>
	constexpr inline operator K&() & {
		return reinterpret_cast<K&>(*this);
	}
	template <typename K, typename = typename enable_implicit_vec_cast<vec_t<N, T>, K>::type>
	constexpr inline operator const K&() const & {
		return reinterpret_cast<const K&>(*this);
	}
	template <typename K, typename = typename enable_implicit_vec_cast<vec_t<N, T>, K>::type>
	constexpr inline operator K&&() && {
		return reinterpret_cast<K&&>(*this);
	}

	// operator[] ----------------------------------------------------------------------------------
	constexpr inline T& operator[](size_t i) & {
		return this->data[i];
	}
	constexpr inline const T& operator[](size_t i) const & {
		return this->data[i];
	}
	constexpr inline T&& operator[](size_t i) && {
		return std::move(this->data[i]);
	}

	// operator= -----------------------------------------------------------------------------------

	template <typename K>
	constexpr inline vec_t<N, T>& operator=(const vec_t<N, K>& rhs) {
		meta::n_times<N>([&](auto index) { this->data[index] = rhs.data[index]; });
		return *this;
	}

	// operator*=(scalar) --------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t<N, T>& operator+=(const K& v) {
		meta::n_times<N>([&](auto index) { this->data[index] += v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator-=(const K& v) {
		meta::n_times<N>([&](auto index) { this->data[index] -= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator*=(const K& v) {
		meta::n_times<N>([&](auto index) { this->data[index] *= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator/=(const K& v) {
		meta::n_times<N>([&](auto index) { this->data[index] /= v; });
		return *this;
	}

	// operator*=(vec) -----------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t<N, T>& operator+=(const vec_t<N, K>& rhs) {
		meta::n_times<N>([&](auto index) { this->data[index] += rhs.data[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator-=(const vec_t<N, K>& rhs) {
		meta::n_times<N>([&](auto index) { this->data[index] -= rhs.data[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator*=(const vec_t<N, K>& rhs) {
		meta::n_times<N>([&](auto index) { this->data[index] *= rhs.data[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator/=(const vec_t<N, K>& rhs) {
		meta::n_times<N>([&](auto index) { this->data[index] /= rhs.data[index]; });
		return *this;
	}

	// operator+ -----------------------------------------------------------------------------------
	constexpr inline vec_t<N, T> operator+() const {
		return build_vec<N>([&](auto index) { return +this->data[index]; });
	}

	constexpr inline vec_t<N, T> operator-() const {
		return build_vec<N>([&](auto index) { return -this->data[index]; });
	}

	// ---------------------------------------------------------------------------------------------

	constexpr inline decltype(auto) ptr() {
		return &this->data[0];
	}

	constexpr inline decltype(auto) ptr() const {
		return &this->data[0];
	}

	/** Return the square length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	constexpr inline decltype(auto) lengthSQ() const {
		T result{};
		meta::n_times<N>([&](auto index) { result += this->data[index] * this->data[index]; });
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
		return build_vec<N>([&](auto index) { return this->data[index] / l; });
	}

	// observers -----------------------------------------------------------------------------------
	template <typename F>
	constexpr inline void sequential_foreach(F&& func) {
		meta::n_times<N>([&](auto index) { func(this->data[index]); });
	}
};

// operator*(vec, vec) -----------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs.data[index] + rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs.data[index] - rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs.data[index] * rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs.data[index] / rhs.data[index]; });
}

// operator*(vec, scalar) --------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](auto index) { return lhs.data[index] + rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](auto index) { return lhs.data[index] - rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](auto index) { return lhs.data[index] * rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](auto index) { return lhs.data[index] / rhs; });
}

// operator*(scalar, vec) --------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs + rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs - rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs * rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return lhs / rhs.data[index]; });
}

// operator==(vec, vec) ----------------------------------------------------------------------------
template <size_t N, typename T, typename K>
constexpr inline bool operator==(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	bool result = true;
	meta::n_times<N>([&](auto index) { result = result && lhs.data[index] == rhs.data[index]; });
	return result;
}
template <size_t N, typename T, typename K>
constexpr inline bool operator!=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	bool result = false;
	meta::n_times<N>([&](auto index) { result = result || lhs.data[index] != rhs.data[index]; });
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
template <typename OStream, size_t N, typename T>
constexpr inline OStream& operator<<(OStream& os, const vec_t<N, T>& vec) {
	meta::n_times<N>([&](auto index) {
		if constexpr (decltype(index)::value != 0)
			os << ' ';
		os << vec.data[index];
	});
	return os;
}

inline namespace vec { // vec utility namespace ----------------------------------------------------

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

// <editor-fold defaultstate="collapsed" desc="Generated Internal Macros: Custom vec to vec getters ...">

#define __libv_vec_2p1(p1)      __libv_vec_get2(p1,x)      __libv_vec_get2(p1,y)      __libv_vec_get2(p1,z)      __libv_vec_get2(p1,w)
#define LIBV_IMPLEMENT_TO_VEC2_GETS() __libv_vec_2p1(x)    __libv_vec_2p1(y)          __libv_vec_2p1(z)          __libv_vec_2p1(w)
#define __libv_vec_3p2(p1,p2)   __libv_vec_get3(p1,p2,x)   __libv_vec_get3(p1,p2,y)   __libv_vec_get3(p1,p2,z)   __libv_vec_get3(p1,p2,w)
#define __libv_vec_3p1(p1)      __libv_vec_3p2(p1,x)       __libv_vec_3p2(p1,y)       __libv_vec_3p2(p1,z)       __libv_vec_3p2(p1,w)
#define LIBV_IMPLEMENT_TO_VEC3_GETS() __libv_vec_3p1(x)    __libv_vec_3p1(y)          __libv_vec_3p1(z)          __libv_vec_3p1(w)
#define __libv_vec_4p3(p1,p2,p3)__libv_vec_get4(p1,p2,p3,x)__libv_vec_get4(p1,p2,p3,y)__libv_vec_get4(p1,p2,p3,z)__libv_vec_get4(p1,p2,p3,w)
#define __libv_vec_4p2(p1,p2)   __libv_vec_4p3(p1,p2,x)    __libv_vec_4p3(p1,p2,y)    __libv_vec_4p3(p1,p2,z)    __libv_vec_4p3(p1,p2,w)
#define __libv_vec_4p1(p1)      __libv_vec_4p2(p1,x)       __libv_vec_4p2(p1,y)       __libv_vec_4p2(p1,z)       __libv_vec_4p2(p1,w)
#define LIBV_IMPLEMENT_TO_VEC4_GETS() __libv_vec_4p1(x)    __libv_vec_4p1(y)          __libv_vec_4p1(z)          __libv_vec_4p1(w)
// </editor-fold>

LIBV_IMPLEMENT_TO_VEC2_GETS()
LIBV_IMPLEMENT_TO_VEC3_GETS()
LIBV_IMPLEMENT_TO_VEC4_GETS()

#undef LIBV_IMPLEMENT_TO_VEC2_GETS
#undef LIBV_IMPLEMENT_TO_VEC3_GETS
#undef LIBV_IMPLEMENT_TO_VEC4_GETS

template <size_t N, typename T> constexpr inline T& x(vec_t<N, T>& vec) {
	static_assert(N > 0, "vec_t with less then 1 dimension does not have 'x' coordinate");
	return vec.data[0];
}
template <size_t N, typename T> constexpr inline T& y(vec_t<N, T>& vec) {
	static_assert(N > 1, "vec_t with less then 2 dimension does not have 'y' coordinate");
	return vec.data[1];
}
template <size_t N, typename T> constexpr inline T& z(vec_t<N, T>& vec) {
	static_assert(N > 2, "vec_t with less then 3 dimension does not have 'z' coordinate");
	return vec.data[2];
}
template <size_t N, typename T> constexpr inline T& w(vec_t<N, T>& vec) {
	static_assert(N > 3, "vec_t with less then 4 dimension does not have 'w' coordinate");
	return vec.data[3];
}
template <size_t N, typename T> constexpr inline const T& x(const vec_t<N, T>& vec) {
	static_assert(N > 0, "vec_t with less then 1 dimension does not have 'x' coordinate");
	return vec.data[0];
}
template <size_t N, typename T> constexpr inline const T& y(const vec_t<N, T>& vec) {
	static_assert(N > 1, "vec_t with less then 2 dimension does not have 'y' coordinate");
	return vec.data[1];
}
template <size_t N, typename T> constexpr inline const T& z(const vec_t<N, T>& vec) {
	static_assert(N > 2, "vec_t with less then 3 dimension does not have 'z' coordinate");
	return vec.data[2];
}
template <size_t N, typename T> constexpr inline const T& w(const vec_t<N, T>& vec) {
	static_assert(N > 3, "vec_t with less then 4 dimension does not have 'w' coordinate");
	return vec.data[3];
}

/// \return The dot product of the two vector
template <size_t N, typename T, typename K>
constexpr inline auto dot(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	std::common_type_t<T, K> result{};
	meta::n_times<N>([&](auto index) { result += lhs.data[index] * rhs.data[index]; });
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
	return build_vec<N>([&](auto index) { return std::max(lhs.data[index], rhs.data[index]); });
}

/// \return The maximum vector with the bigger value on each dimension from the two vector
template <size_t N, typename T, typename K>
constexpr inline auto min(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](auto index) { return std::min(lhs.data[index], rhs.data[index]); });
}

/// \return The static casted vector to the requested K type
template <typename K, size_t N, typename T>
constexpr inline auto cast(const vec_t<N, T>& vec) {
	return build_vec<N>([&](auto index) { return static_cast<K>(vec.data[index]); });
}

template <size_t N, typename T>
constexpr inline auto round(const vec_t<N, T>& vec) {
	return build_vec<N>([&](auto index) { return std::round(vec.data[index]); });
}
template <size_t N, typename T>
constexpr inline auto lround(const vec_t<N, T>& vec) {
	return build_vec<N>([&](auto index) { return std::lround(vec.data[index]); });
}
template <size_t N, typename T>
constexpr inline auto llround(const vec_t<N, T>& vec) {
	return build_vec<N>([&](auto index) { return std::llround(vec.data[index]); });
}

template <size_t N, typename T, typename K, typename L = std::common_type_t<T, K>>
constexpr inline bool approx(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs, L&& epsilon = 0.00001) {
	bool result = true;
	meta::n_times<N>([&](auto index) {
		result = result && std::abs(lhs.data[index] - rhs.data[index]) < epsilon;
	});
	return result;
}

template <typename T>
constexpr inline decltype(auto) from_rgba(const T& v) {
	return vec_t<4, decltype(v.r)>{v.r, v.g, v.b, v.a};
}
template <typename T>
constexpr inline decltype(auto) from_rgb(const T& v) {
	return vec_t<3, decltype(v.r)>{v.r, v.g, v.b};
}
template <typename T>
constexpr inline decltype(auto) from_rg(const T& v) {
	return vec_t<2, decltype(v.r)>{v.r, v.g};
}
template <typename T>
constexpr inline decltype(auto) from_xyzw(const T& v) {
	return vec_t<4, decltype(v.x)>{v.x, v.y, v.z, v.w};
}
template <typename T>
constexpr inline decltype(auto) from_xyz(const T& v) {
	return vec_t<3, decltype(v.x)>{v.x, v.y, v.z};
}
template <typename T>
constexpr inline decltype(auto) from_xy(const T& v) {
	return vec_t<2, decltype(v.x)>{v.x, v.y};
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

} // namespace libv
