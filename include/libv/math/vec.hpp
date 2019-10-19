// File: Vec.hpp, Created on 2014. január 7. 7:08, Author: Vader

#pragma once

// ext
// TODO P4: Remove glm includes after
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
// libv
#include <libv/meta/for_constexpr.hpp>
#include <libv/meta/type_traits.hpp>
// std
#include <array>
#include <cassert>
#include <cmath>
#include <ostream>
#include <utility>
// pro
#include <libv/concept/vec.hpp>


// TODO P1: Modify (bool operator<) to (vec<bool, N> operator<)
// TODO P2: add swizzle custom getters back into member after measuring the impact, and consider the concept based swizzle too, so either both or only non-member
//			Interesing presentation on vec: https://www.youtube.com/watch?v=8FoAxasNssA
// TODO P2: use 'concepts' for N dim vector for: from_xyzw, xyz, xy, rgba, ...
// TODO P2: I think the whole glm bridge could be resolved by a pretty rough template conversion function
//			Concept allow conversion to and from that type
// TODO P3: conditionally explicit ctors: 41:00 https://www.youtube.com/watch?v=ybaE9qlhHvw or wait out explicit(bool)
// TODO P4: vec noexcept example with the only problem that a lambda cannot be part of an unevaluated context:
//		template <size_t N, typename T, typename K, CONCEPT_REQUIRES_(not Vec<K, N>)>
//		constexpr inline auto operator/(const vec_t<N, T>& lhs, const K& rhs) LIBV_RETURNS(
//			build_vec<N>([&](const auto index) { return lhs.data[index] / rhs; }
//		)
//		template <size_t N, typename T, typename K,
//			CONCEPT_REQUIRES_(Vec<K, N>)>
//		constexpr inline auto operator/(const vec_t<N, T>& lhs, const K& rhs) LIBV_RETURNS(
//			build_vec<N>([&](const auto index) { return lhs.data[index] / rhs.data[index]; }
//		)
// TODO P5: use warning disable macros with: _Pragma("argument")
// TODO P5: implement operator%

// NOTE: Use concepts to enable every operation based on underlying types
// NOTE: Perfect forwarding is possible, but it seams like does not worth +600 line template
// NOTE: MSVC: Disable warnings for nameless struct on MSVC maybe, but then again, I don't care about MSVC:
//		#pragma warning(push)
//		#pragma warning(disable:4201) // warning C4201: nonstandard extension used : nameless struct/union
//		#pragma warning(pop)

namespace libv {

// vec_t forward -----------------------------------------------------------------------------------

template <size_t N, typename T>
struct vec_t;

// build / make helper functions -------------------------------------------------------------------

template <size_t N, typename F>
constexpr inline decltype(auto) build_vec(F&& func) {
	return libv::meta::call_with_n_index<N>([&](const auto... indices) {
		return vec_t<N, decltype(func(std::declval<size_t>()))>(func(indices)...);
	});
}

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
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) & { data = orig.data; return *this; }
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) & { data = std::move(orig.data); return *this; }

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
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) & { data = orig.data; return *this; }
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) & { data = std::move(orig.data); return *this; }

	constexpr inline vec_base_t(T x, T y) : x(x), y(y) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline explicit vec_base_t(const V0& xy) : x(xy.x), y(xy.y) { }
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
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) & { data = orig.data; return *this; }
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) & { data = std::move(orig.data); return *this; }

	constexpr inline vec_base_t(T x, T y, T z) : x(x), y(y), z(z) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(const V0& xy, T z) : x(xy.x), y(xy.y), z(z) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(T x, const V0& yz) : x(x), y(yz.x), z(yz.y) { }
	template <typename V0>
			WISH_REQUIRES(Vec3<V0>)
	constexpr inline explicit vec_base_t(const V0& xyz) : x(xyz.x), y(xyz.y), z(xyz.z) { }
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
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) & { data = orig.data; return *this; }
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) & { data = std::move(orig.data); return *this; }

	constexpr inline vec_base_t(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(const V0& xy, T z, T w) : x(xy.x), y(xy.y), z(z), w(w) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(T x, const V0& yz, T w) : x(x), y(yz.x), z(yz.y), w(w) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(T x, T y, const V0& zw) : x(x), y(y), z(zw.x), w(zw.y) { }
	template <typename V0, typename V1>
			WISH_REQUIRES(Vec2<V0> && Vec2<V1>)
	constexpr inline vec_base_t(const V0& xy, const V1& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	template <typename V0>
			WISH_REQUIRES(Vec3<V0>)
	constexpr inline vec_base_t(const V0& xyz, T w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	template <typename V0>
			WISH_REQUIRES(Vec3<V0>)
	constexpr inline vec_base_t(T x, const V0& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }
	template <typename V0>
			WISH_REQUIRES(Vec4<V0>)
	constexpr inline explicit vec_base_t(const V0& xyzw) : x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w) { }
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
	constexpr inline vec_t& operator=(const vec_t&) & = default;
	constexpr inline vec_t& operator=(vec_t&&) & = default;

	using vec_base_t<N, T>::vec_base_t;

	// IDEA: With a index_into_vec_pack helper and 2 funny concept over packs all of the vec_base_t ctors could be generalized
	//	template <typename... Components,
	//			CONCEPT_REQUIRES_(VecOrElement<Components, T>()...),
	//			CONCEPT_REQUIRES_(VecSumDimension<Components...>() == N)>
	//	constexpr inline vec_base_t(Components&&... args) :
	//		x(detail::index_into_vec_pack<0>(args...)),
	//		y(detail::index_into_vec_pack<1>(args...)),
	//		z(detail::index_into_vec_pack<2>(args...)),
	//		w(detail::index_into_vec_pack<3>(args...)) { }

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
	constexpr inline vec_t<N, T>& operator=(const vec_t<N, K>& rhs) & {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] = rhs.data[index]; });
		return *this;
	}

	// operator*=(scalar) --------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t<N, T>& operator+=(const K& v) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] += v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator-=(const K& v) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] -= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator*=(const K& v) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] *= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator/=(const K& v) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] /= v; });
		return *this;
	}

	// operator*=(vec) -----------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t<N, T>& operator+=(const vec_t<N, K>& rhs) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] += rhs.data[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator-=(const vec_t<N, K>& rhs) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] -= rhs.data[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator*=(const vec_t<N, K>& rhs) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] *= rhs.data[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t<N, T>& operator/=(const vec_t<N, K>& rhs) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data[index] /= rhs.data[index]; });
		return *this;
	}

	// operator+ -----------------------------------------------------------------------------------
	constexpr inline vec_t<N, T> operator+() const {
		return build_vec<N>([&](const auto index) { return +this->data[index]; });
	}

	constexpr inline vec_t<N, T> operator-() const {
		return build_vec<N>([&](const auto index) { return -this->data[index]; });
	}

	// ---------------------------------------------------------------------------------------------

	/** Return the square length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	constexpr inline decltype(auto) lengthSQ() const {
		T result{};
		libv::meta::for_constexpr<0, N>([&](const auto index) { result += this->data[index] * this->data[index]; });
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
		return build_vec<N>([&](const auto index) { return this->data[index] / l; });
	}

	/// \return The static_cast-ed vector to the requested K type
	template <typename K>
	constexpr inline auto cast() const {
		return build_vec<N>([&](const auto index) { return static_cast<K>(this->data[index]); });
	}

	// observers -----------------------------------------------------------------------------------

	constexpr inline decltype(auto) ptr() {
		return &this->data[0];
	}

	constexpr inline decltype(auto) ptr() const {
		return &this->data[0];
	}

	template <typename F>
	constexpr inline void sequential_foreach(F&& func) {
		libv::meta::for_constexpr<0, N>([&](const auto index) { func(this->data[index]); });
	}
};

// operator*(vec, vec) -----------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return lhs.data[index] + rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return lhs.data[index] - rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return lhs.data[index] * rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return lhs.data[index] / rhs.data[index]; });
}
//template <size_t N, typename T, typename K>
//constexpr inline auto operator%(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
//	return build_vec<N>([&](const auto index) { return lhs.data[index] % rhs.data[index]; });
//}

// operator*(vec, scalar) --------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](const auto index) { return lhs.data[index] + rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](const auto index) { return lhs.data[index] - rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](const auto index) { return lhs.data[index] * rhs; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](const auto index) { return lhs.data[index] / rhs; });
}
//template <size_t N, typename T, typename K>
//constexpr inline auto operator%(const vec_t<N, T>& lhs, const K& rhs) {
//	return build_vec<N>([&](const auto index) { return lhs.data[index] % rhs; });
//}

// operator*(scalar, vec) --------------------------------------------------------------------------

template <size_t N, typename T, typename K>
constexpr inline auto operator+(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return lhs + rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator-(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return lhs - rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator*(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return lhs * rhs.data[index]; });
}
template <size_t N, typename T, typename K>
constexpr inline auto operator/(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return lhs / rhs.data[index]; });
}
//template <size_t N, typename T, typename K>
//constexpr inline auto operator%(const T& lhs, const vec_t<N, K>& rhs) {
//	return build_vec<N>([&](const auto index) { return lhs % rhs.data[index]; });
//}

// operator==(vec, vec) ----------------------------------------------------------------------------
template <size_t N, typename T, typename K>
constexpr inline bool operator==(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	bool result = true;
	libv::meta::for_constexpr<0, N>([&](const auto index) { result = result && lhs.data[index] == rhs.data[index]; });
	return result;
}
template <size_t N, typename T, typename K>
constexpr inline bool operator!=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	bool result = false;
	libv::meta::for_constexpr<0, N>([&](const auto index) { result = result || lhs.data[index] != rhs.data[index]; });
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
template <size_t N, typename T,
		typename = std::enable_if_t<libv::meta::is_ostreamable<std::ostream, T>::value>>
constexpr inline std::ostream& operator<<(std::ostream& os, const vec_t<N, T>& vec) {
	libv::meta::for_constexpr<0, N>([&](const auto index) {
		if constexpr (index != 0)
			os << ' ';
		os << vec.data[index];
	});
	return os;
}

inline namespace vec { // vec utility namespace ----------------------------------------------------

#define __libv_vec_get2(p1,p2)                                                                     \
	template <typename V0>                                                                         \
	constexpr inline auto p1##p2(const V0& vec) {                                                  \
		return ::libv::vec_t<2, decltype(vec.p1)>(vec.p1, vec.p2);                                 \
	}
#define __libv_vec_get3(p1,p2,p3)                                                                  \
	template <typename V0>                                                                         \
	constexpr inline auto p1##p2##p3(const V0& vec) {                                              \
		return ::libv::vec_t<3, decltype(vec.p1)>(vec.p1, vec.p2, vec.p3);                         \
	}
#define __libv_vec_get4(p1,p2,p3,p4)                                                               \
	template <typename V0>                                                                         \
	constexpr inline auto p1##p2##p3##p4(const V0& vec) {                                          \
		return ::libv::vec_t<4, decltype(vec.p1)>(vec.p1, vec.p2, vec.p3, vec.p4);                 \
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
	libv::meta::for_constexpr<0, N>([&](const auto index) { result += lhs.data[index] * rhs.data[index]; });
	return result;
}

/// \return The cross product of the two vector
template <typename V0, typename V1>
		WISH_REQUIRES(Vec3<V0> && Vec3<V1>)
constexpr inline auto cross(const V0& lhs, const V1& rhs) {
	return vec_t<3, decltype(lhs.y * rhs.z - lhs.z * rhs.y)>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
}

/// \return The maximum vector with the greater value on each dimension from the two vector
template <size_t N, typename T, typename K>
constexpr inline auto max(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return std::max(lhs.data[index], rhs.data[index]); });
}
/// \return The maximum vector with the greater value on each dimension
template <size_t N, typename T, typename K>
constexpr inline auto max(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](const auto index) { return std::max(lhs.data[index], rhs); });
}
/// \return The maximum vector with the greater value on each dimension
template <size_t N, typename T, typename K>
constexpr inline auto max(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return std::max(lhs, rhs.data[index]); });
}

/// \return The minimum vector with the smaller value on each dimension from the two vector
template <size_t N, typename T, typename K>
constexpr inline auto min(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return std::min(lhs.data[index], rhs.data[index]); });
}
/// \return The minimum vector with the smaller value on each dimension
template <size_t N, typename T, typename K>
constexpr inline auto min(const vec_t<N, T>& lhs, const K& rhs) {
	return build_vec<N>([&](const auto index) { return std::min(lhs.data[index], rhs); });
}
/// \return The minimum vector with the smaller value on each dimension
template <size_t N, typename T, typename K>
constexpr inline auto min(const T& lhs, const vec_t<N, K>& rhs) {
	return build_vec<N>([&](const auto index) { return std::min(lhs, rhs.data[index]); });
}

/// \return Clamps the vector's each dimension within the range of [\c high, \c low]
template <size_t N, typename T>
constexpr inline auto clamp(const vec_t<N, T>& vec, const T& low, const T& high) {
	return build_vec<N>([&](const auto index) { return std::clamp(vec.data[index], low, high); });
}

/// \return Abs the vector's each dimension
template <size_t N, typename T>
constexpr inline auto abs(const vec_t<N, T>& vec) {
	return build_vec<N>([&](const auto index) { return std::abs(vec.data[index]); });
}

/// \return The static_cast-ed vector to the requested K type
template <typename K, size_t N, typename T>
constexpr inline auto cast(const vec_t<N, T>& vec) {
	return build_vec<N>([&](const auto index) { return static_cast<K>(vec.data[index]); });
}

template <size_t N, typename T>
constexpr inline auto round(const vec_t<N, T>& vec) {
	return build_vec<N>([&](const auto index) { return std::round(vec.data[index]); });
}
template <size_t N, typename T>
constexpr inline auto lround(const vec_t<N, T>& vec) {
	return build_vec<N>([&](const auto index) { return std::lround(vec.data[index]); });
}
template <size_t N, typename T>
constexpr inline auto llround(const vec_t<N, T>& vec) {
	return build_vec<N>([&](const auto index) { return std::llround(vec.data[index]); });
}

template <size_t N, typename T, typename K, typename L = std::common_type_t<T, K>>
constexpr inline bool approx(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs, L&& epsilon = 0.00001) {
	bool result = true;
	libv::meta::for_constexpr<0, N>([&](const auto index) {
		result = result && std::abs(lhs.data[index] - rhs.data[index]) < epsilon;
	});
	return result;
}

template <size_t N, typename T, typename K, typename L>
constexpr inline bool within(const vec_t<N, T>& value, const vec_t<N, K>& min, const vec_t<N, L>& max) {
	bool result = true;
	libv::meta::for_constexpr<0, N>([&](const auto index) {
		result = result &&
				value.data[index] >= min.data[index] &&
				value.data[index] <= max.data[index];
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

} // namespace vec ---------------------------------------------------------------------------------

// =================================================================================================

// TODO P4: Replace to_glm function with conversion operators, with that glm includes will become obsolete

template <typename T> constexpr inline auto to_glm(const vec_t<2, T>& v) {
	return glm::tvec2<T>(v.x, v.y);
}
template <typename T> constexpr inline auto to_glm(const vec_t<3, T>& v) {
	return glm::tvec3<T>(v.x, v.y, v.z);
}
template <typename T> constexpr inline auto to_glm(const vec_t<4, T>& v) {
	return glm::tvec4<T>(v.x, v.y, v.z, v.w);
}

// aliases -----------------------------------------------------------------------------------------

template <typename T> using vec2_t = vec_t<2, T>;
template <typename T> using vec3_t = vec_t<3, T>;
template <typename T> using vec4_t = vec_t<4, T>;

using vec2b = vec2_t<bool>;
using vec3b = vec3_t<bool>;
using vec4b = vec4_t<bool>;
using vec2f = vec2_t<float>;
using vec3f = vec3_t<float>;
using vec4f = vec4_t<float>;
using vec2d = vec2_t<double>;
using vec3d = vec3_t<double>;
using vec4d = vec4_t<double>;
using vec2c = vec2_t<int8_t>;
using vec3c = vec3_t<int8_t>;
using vec4c = vec4_t<int8_t>;
using vec2uc = vec2_t<uint8_t>;
using vec3uc = vec3_t<uint8_t>;
using vec4uc = vec4_t<uint8_t>;
using vec2s = vec2_t<int16_t>;
using vec3s = vec3_t<int16_t>;
using vec4s = vec4_t<int16_t>;
using vec2us = vec2_t<uint16_t>;
using vec3us = vec3_t<uint16_t>;
using vec4us = vec4_t<uint16_t>;
using vec2i = vec2_t<int32_t>;
using vec3i = vec3_t<int32_t>;
using vec4i = vec4_t<int32_t>;
using vec2ui = vec2_t<uint32_t>;
using vec3ui = vec3_t<uint32_t>;
using vec4ui = vec4_t<uint32_t>;
using vec2l = vec2_t<int64_t>;
using vec3l = vec3_t<int64_t>;
using vec4l = vec4_t<int64_t>;
using vec2ul = vec2_t<uint64_t>;
using vec3ul = vec3_t<uint64_t>;
using vec4ul = vec4_t<uint64_t>;
using vec2z = vec2_t<size_t>;
using vec3z = vec3_t<size_t>;
using vec4z = vec4_t<size_t>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
