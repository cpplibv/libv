// Project: libv.math, File: src/libv/math/vec.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/math/vec_fwd.hpp>
// libv
#include <libv/meta/for_constexpr.hpp>
// std
#include <cmath>
#include <ostream> // Remove include after https://github.com/fmtlib/fmt/issues/2449 is resolved
// pro
#include <libv/concept/vec.hpp>


// TODO P1: Modify (bool operator<) to (vec<bool, N> operator<)
// TODO P2: add swizzle custom getters back into member after measuring the impact, and consider the concept based swizzle too, so either both or only non-member
//			Interesing presentation on vec: https://www.youtube.com/watch?v=8FoAxasNssA

// NOTE: Use concepts to enable every operation based on underlying types | compile time hit not worth it
// NOTE: Perfect forwarding is possible, but it seams like does not worth additional 600 line template


namespace libv {

// build / make helper functions -------------------------------------------------------------------

template <size_t N, typename F>
[[nodiscard]] constexpr inline auto build_vec(F&& func) noexcept {
	return libv::meta::call_with_n_index<N>([&](const auto... indices) {
		return vec_t<N, decltype(func(std::declval<size_t>()))>(func(indices)...);
	});
}

// vec_base_t --------------------------------------------------------------------------------------

template <size_t N, typename T>
struct vec_base_t {
	T data_[N]{};

	constexpr inline vec_base_t() noexcept = default;
	constexpr inline vec_base_t(const vec_base_t& orig) noexcept = default;
	constexpr inline vec_base_t(vec_base_t&& orig) noexcept = default;
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) & noexcept = default;
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) & noexcept = default;

	template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
	constexpr inline explicit vec_base_t(Args&&... values) noexcept : data_{values...} { }

	[[nodiscard]] constexpr inline T* data() noexcept {
		return data_;
	}
	[[nodiscard]] constexpr inline const T* data() const noexcept {
		return data_;
	}
};

template <typename T>
struct vec_base_t<2, T> {
	T x{};
	T y{};

	constexpr inline vec_base_t() noexcept = default;
	constexpr inline vec_base_t(const vec_base_t& orig) noexcept = default;
	constexpr inline vec_base_t(vec_base_t&& orig) noexcept = default;
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) & noexcept = default;
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) & noexcept = default;

	constexpr inline vec_base_t(T x, T y) noexcept : x(x), y(y) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline explicit vec_base_t(const V0& xy) noexcept : x(xy.x), y(xy.y) { }

	[[nodiscard]] constexpr inline T* data() noexcept {
		return &x;
	}
	[[nodiscard]] constexpr inline const T* data() const noexcept {
		return &x;
	}
};

template <typename T>
struct vec_base_t<3, T> {
	T x{};
	T y{};
	T z{};

	constexpr inline vec_base_t() noexcept = default;
	constexpr inline vec_base_t(const vec_base_t& orig) noexcept = default;
	constexpr inline vec_base_t(vec_base_t&& orig) noexcept = default;
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) & noexcept = default;
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) & noexcept = default;

	constexpr inline vec_base_t(T x, T y, T z) noexcept : x(x), y(y), z(z) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(const V0& xy, T z) noexcept : x(xy.x), y(xy.y), z(z) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(T x, const V0& yz) noexcept : x(x), y(yz.x), z(yz.y) { }
	template <typename V0>
			WISH_REQUIRES(Vec3<V0>)
	constexpr inline explicit vec_base_t(const V0& xyz) noexcept : x(xyz.x), y(xyz.y), z(xyz.z) { }

	[[nodiscard]] constexpr inline T* data() noexcept {
		return &x;
	}
	[[nodiscard]] constexpr inline const T* data() const noexcept {
		return &x;
	}
};

template <typename T>
struct vec_base_t<4, T> {
	T x{};
	T y{};
	T z{};
	T w{};

	constexpr inline vec_base_t() noexcept = default;
	constexpr inline vec_base_t(const vec_base_t& orig) noexcept = default;
	constexpr inline vec_base_t(vec_base_t&& orig) noexcept = default;
	constexpr inline vec_base_t& operator=(const vec_base_t& orig) & noexcept = default;
	constexpr inline vec_base_t& operator=(vec_base_t&& orig) & noexcept = default;

	constexpr inline vec_base_t(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(const V0& xy, T z, T w) noexcept : x(xy.x), y(xy.y), z(z), w(w) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(T x, const V0& yz, T w) noexcept : x(x), y(yz.x), z(yz.y), w(w) { }
	template <typename V0>
			WISH_REQUIRES(Vec2<V0>)
	constexpr inline vec_base_t(T x, T y, const V0& zw) noexcept : x(x), y(y), z(zw.x), w(zw.y) { }
	template <typename V0, typename V1>
			WISH_REQUIRES(Vec2<V0> && Vec2<V1>)
	constexpr inline vec_base_t(const V0& xy, const V1& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	template <typename V0>
			WISH_REQUIRES(Vec3<V0>)
	constexpr inline vec_base_t(const V0& xyz, T w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	template <typename V0>
			WISH_REQUIRES(Vec3<V0>)
	constexpr inline vec_base_t(T x, const V0& yzw) noexcept : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }
	template <typename V0>
			WISH_REQUIRES(Vec4<V0>)
	constexpr inline explicit vec_base_t(const V0& xyzw) noexcept : x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w) { }

	[[nodiscard]] constexpr inline T* data() noexcept {
		return &x;
	}
	[[nodiscard]] constexpr inline const T* data() const noexcept {
		return &x;
	}
};

// vec_t -------------------------------------------------------------------------------------------

template <size_t N, typename T>
struct vec_t : vec_base_t<N, T> {

	static constexpr inline size_t dim = N;
	using value_type = T;

	// constructors --------------------------------------------------------------------------------
	constexpr inline vec_t() noexcept = default;
	constexpr inline vec_t(const vec_t&) noexcept = default;
	constexpr inline vec_t(vec_t&&) noexcept = default;
	constexpr inline vec_t& operator=(const vec_t&) & noexcept = default;
	constexpr inline vec_t& operator=(vec_t&&) & noexcept = default;

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
	[[nodiscard]] constexpr inline T& operator[](size_t i) & noexcept {
		return this->data()[i];
	}
	[[nodiscard]] constexpr inline const T& operator[](size_t i) const & noexcept {
		return this->data()[i];
	}

	// operator= -----------------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t& operator=(const vec_t<N, K>& rhs) & noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] = rhs.data()[index]; });
		return *this;
	}

	// Special value constructors ------------------------------------------------------------------

	[[nodiscard]] static constexpr inline vec_t zero() noexcept {
		return vec_t{};
	}

	template <bool always_false = false>
	[[nodiscard]] static constexpr inline vec_t inf() noexcept {
		if constexpr (std::numeric_limits<T>::has_infinity)
			return build_vec<N>([&](const auto) { return std::numeric_limits<T>::infinity(); });
		else
			static_assert(always_false, "Underlying type has no infinity");
	}

	template <bool always_false = false>
	[[nodiscard]] static constexpr inline vec_t nan() noexcept {
		if constexpr (std::numeric_limits<T>::has_quiet_NaN)
			return build_vec<N>([&](const auto) { return std::numeric_limits<T>::quiet_NaN(); });
		else
			static_assert(always_false, "Underlying type has no NaN");
	}

	[[nodiscard]] static constexpr inline vec_t max() noexcept {
		return build_vec<N>([&](const auto) { return std::numeric_limits<T>::max(); });
	}

	[[nodiscard]] static constexpr inline vec_t min() noexcept {
		return build_vec<N>([&](const auto) { return std::numeric_limits<T>::min(); });
	}

	[[nodiscard]] static constexpr inline vec_t lowest() noexcept {
		return build_vec<N>([&](const auto) { return std::numeric_limits<T>::lowest(); });
	}

	// operator*=(scalar) --------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t& operator+=(const K& v) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] += v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t& operator-=(const K& v) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] -= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t& operator*=(const K& v) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] *= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t& operator/=(const K& v) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] /= v; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t& operator%=(const K& v) noexcept {
		if constexpr (std::is_floating_point_v<T>)
			libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] = std::fmod(this->data()[index], v); });
		else
			libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] %= v; });
		return *this;
	}

	// operator*=(vec) -----------------------------------------------------------------------------
	template <typename K>
	constexpr inline vec_t& operator+=(const vec_t<N, K>& rhs) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] += rhs.data()[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t& operator-=(const vec_t<N, K>& rhs) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] -= rhs.data()[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t& operator*=(const vec_t<N, K>& rhs) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] *= rhs.data()[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t& operator/=(const vec_t<N, K>& rhs) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] /= rhs.data()[index]; });
		return *this;
	}
	template <typename K>
	constexpr inline vec_t& operator%=(const vec_t<N, K>& rhs) noexcept {
		if constexpr (std::is_floating_point_v<T>)
			libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] = std::fmod(this->data()[index], rhs.data()[index]); });
		else
			libv::meta::for_constexpr<0, N>([&](const auto index) { this->data()[index] %= rhs.data()[index]; });
		return *this;
	}

	// operator+ -----------------------------------------------------------------------------------
	[[nodiscard]] constexpr inline vec_t operator+() const noexcept {
		return build_vec<N>([&](const auto index) { return +this->data()[index]; });
	}

	[[nodiscard]] constexpr inline vec_t operator-() const noexcept {
		return build_vec<N>([&](const auto index) { return -this->data()[index]; });
	}

	// ---------------------------------------------------------------------------------------------

	/** Return the square length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	[[nodiscard]] constexpr inline T length_sq() const noexcept {
		T result{};
		libv::meta::for_constexpr<0, N>([&](const auto index) { result += this->data()[index] * this->data()[index]; });
		return result;
	}

	/** Return the length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	[[nodiscard]] constexpr inline T length() const noexcept {
		return std::sqrt(length_sq());
	}

	[[nodiscard]] constexpr inline auto length_and_dir() const noexcept {
		struct Result {
			value_type length;
			vec_t dir;
		};

		const auto len = length();
		const auto dir =  *this / len;

		return Result{len, dir};
	}

	/** Normalize the vector (by divide each component by the length)
	 * @return The vector */
	constexpr inline vec_t& normalize() noexcept {
		return operator/=(length());
	}

	/** Return the normalized vector (by divide each component by the length).
	 * @note Does not change the original vector
	 * @return The normalized vector */
	[[nodiscard]] constexpr inline vec_t normalize_copy() const noexcept {
		return *this / length();
	}

	/// \return The static_cast-ed vector to the requested K type
	template <typename K>
	[[nodiscard]] constexpr inline vec_t<N, K> cast() const noexcept {
		return build_vec<N>([&](const auto index) { return static_cast<K>(this->data()[index]); });
	}

	// observers -----------------------------------------------------------------------------------

	[[nodiscard]] constexpr inline T* ptr() noexcept {
		return &this->data()[0];
	}

	[[nodiscard]] constexpr inline T* ptr() const noexcept {
		return &this->data()[0];
	}

	template <typename F>
	constexpr inline void sequential_foreach(F&& func) noexcept {
		libv::meta::for_constexpr<0, N>([&](const auto index) { func(this->data()[index]); });
	}

	// operator==(vec, vec) ------------------------------------------------------------------------
	template <typename K>
	[[nodiscard]] friend constexpr inline bool operator==(const vec_t& lhs, const vec_t<N, K>& rhs) noexcept {
		bool result = true;
		libv::meta::for_constexpr<0, N>([&](const auto index) { result = result && lhs.data()[index] == rhs.data()[index]; });
		return result;
	}
	template <typename K>
	[[nodiscard]] friend constexpr inline bool operator!=(const vec_t& lhs, const vec_t<N, K>& rhs) noexcept {
		bool result = false;
		libv::meta::for_constexpr<0, N>([&](const auto index) { result = result || lhs.data()[index] != rhs.data()[index]; });
		return result;
	}

	// operator<<(ostream, vec) --------------------------------------------------------------------

	// --- WORKAROUND ---
	// Workaround until https://github.com/fmtlib/fmt/issues/2449 is not resolved
	// Also remove the ostream include
	// --- BEGIN ---
	friend inline std::ostream& operator<<(std::ostream& os, const vec_t& vec)
			requires requires (std::ostream& o, const T& m) { o << m; } {
	// --- CORRECT ---
	//	template <typename OStream>
	//		requires requires (OStream& o, const T& m) { o << m; }
	//	friend inline OStream& operator<<(OStream& os, const vec_t& vec) {
	// --- END ---

		libv::meta::for_constexpr<0, N>([&](const auto index) {
			if constexpr (index != 0)
				os << ' ';
			os << vec.data()[index];
		});
		return os;
	}
};

// operator*(vec, vec) -----------------------------------------------------------------------------

template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator+(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs.data()[index] + rhs.data()[index]; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator-(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs.data()[index] - rhs.data()[index]; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator*(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs.data()[index] * rhs.data()[index]; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator/(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs.data()[index] / rhs.data()[index]; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator%(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	if constexpr (std::is_floating_point_v<T>)
		return build_vec<N>([&](const auto index) { return std::fmod(lhs.data()[index], rhs.data()[index]); });
	else
		return build_vec<N>([&](const auto index) { return lhs.data()[index] % rhs.data()[index]; });
}

// operator*(vec, scalar) --------------------------------------------------------------------------

template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator+(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs.data()[index] + rhs; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator-(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs.data()[index] - rhs; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator*(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs.data()[index] * rhs; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator/(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs.data()[index] / rhs; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator%(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	if constexpr (std::is_floating_point_v<T>)
		return build_vec<N>([&](const auto index) { return std::fmod(lhs.data()[index], rhs); });
	else
		return build_vec<N>([&](const auto index) { return lhs.data()[index] % rhs; });
}

// operator*(scalar, vec) --------------------------------------------------------------------------

template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator+(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs + rhs.data()[index]; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator-(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs - rhs.data()[index]; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator*(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs * rhs.data()[index]; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator/(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return lhs / rhs.data()[index]; });
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto operator%(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	if constexpr (std::is_floating_point_v<T>)
		return build_vec<N>([&](const auto index) { return std::fmod(lhs, rhs.data()[index]); });
	else
		return build_vec<N>([&](const auto index) { return lhs % rhs.data()[index]; });
}

// operator<(vec, vec) -----------------------------------------------------------------------------
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator<(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.length_sq() < rhs.length_sq();
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator<=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.length_sq() <= rhs.length_sq();
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator>(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.length_sq() > rhs.length_sq();
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator>=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.length_sq() >= rhs.length_sq();
}

// operator<(vec, scalar) --------------------------------------------------------------------------
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator<(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length_sq() < rhs * rhs;
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator<=(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length_sq() <= rhs * rhs;
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator>(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length_sq() > rhs * rhs;
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator>=(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length_sq() >= rhs * rhs;
}

// operator<(scalar, vec) --------------------------------------------------------------------------
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator<(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs * lhs < rhs.length_sq();
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator<=(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs * lhs <= rhs.length_sq();
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator>(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs * lhs > rhs.length_sq();
}
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline bool operator>=(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs * lhs >= rhs.length_sq();
}

inline namespace vec { // vec utility namespace ----------------------------------------------------

#define __libv_vec_get2(p1,p2)                                                                     \
	template <typename V0>                                                                         \
	[[nodiscard]] constexpr inline auto p1##p2(const V0& vec) noexcept {                           \
		return ::libv::vec_t<2, decltype(vec.p1)>(vec.p1, vec.p2);                                 \
	}
#define __libv_vec_get3(p1,p2,p3)                                                                  \
	template <typename V0>                                                                         \
	[[nodiscard]] constexpr inline auto p1##p2##p3(const V0& vec) noexcept {                       \
		return ::libv::vec_t<3, decltype(vec.p1)>(vec.p1, vec.p2, vec.p3);                         \
	}
#define __libv_vec_get4(p1,p2,p3,p4)                                                               \
	template <typename V0>                                                                         \
	[[nodiscard]] constexpr inline auto p1##p2##p3##p4(const V0& vec) noexcept {                   \
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

template <size_t N, typename T>
[[nodiscard]] constexpr inline T& x(vec_t<N, T>& vec) noexcept {
	static_assert(N > 0, "vec_t with less then 1 dimension does not have 'x' coordinate");
	return vec.data()[0];
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline T& y(vec_t<N, T>& vec) noexcept {
	static_assert(N > 1, "vec_t with less then 2 dimension does not have 'y' coordinate");
	return vec.data()[1];
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline T& z(vec_t<N, T>& vec) noexcept {
	static_assert(N > 2, "vec_t with less then 3 dimension does not have 'z' coordinate");
	return vec.data()[2];
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline T& w(vec_t<N, T>& vec) noexcept {
	static_assert(N > 3, "vec_t with less then 4 dimension does not have 'w' coordinate");
	return vec.data()[3];
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline const T& x(const vec_t<N, T>& vec) noexcept {
	static_assert(N > 0, "vec_t with less then 1 dimension does not have 'x' coordinate");
	return vec.data()[0];
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline const T& y(const vec_t<N, T>& vec) noexcept {
	static_assert(N > 1, "vec_t with less then 2 dimension does not have 'y' coordinate");
	return vec.data()[1];
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline const T& z(const vec_t<N, T>& vec) noexcept {
	static_assert(N > 2, "vec_t with less then 3 dimension does not have 'z' coordinate");
	return vec.data()[2];
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline const T& w(const vec_t<N, T>& vec) noexcept {
	static_assert(N > 3, "vec_t with less then 4 dimension does not have 'w' coordinate");
	return vec.data()[3];
}

// -------------------------------------------------------------------------------------------------

/// \return The dot product of the two vector
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline std::common_type_t<T, K> dot(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	std::common_type_t<T, K> result{};
	libv::meta::for_constexpr<0, N>([&](const auto index) { result += lhs.data()[index] * rhs.data()[index]; });
	return result;
}

/// \return The cross product of the two 2D vector
template <typename V0, typename V1>
		WISH_REQUIRES(Vec2<V0> && Vec2<V1>)
[[nodiscard]] constexpr inline auto cross(const V0& lhs, const V1& rhs) noexcept {
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

/// \return The cross product of the two 3D vector
template <typename V0, typename V1>
		WISH_REQUIRES(Vec3<V0> && Vec3<V1>)
[[nodiscard]] constexpr inline auto cross(const V0& lhs, const V1& rhs) noexcept {
	return vec_t<3, decltype(lhs.y * rhs.z - lhs.z * rhs.y)>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
}

/// \return The maximum vector with the greater value on each dimension from the two vector
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto max(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return std::max(lhs.data()[index], rhs.data()[index]); });
}
/// \return The maximum vector with the greater value on each dimension
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto max(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return std::max(lhs.data()[index], rhs); });
}
/// \return The maximum vector with the greater value on each dimension
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto max(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return std::max(lhs, rhs.data()[index]); });
}

/// \return The minimum vector with the smaller value on each dimension from the two vector
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto min(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return std::min(lhs.data()[index], rhs.data()[index]); });
}
/// \return The minimum vector with the smaller value on each dimension
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto min(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return std::min(lhs.data()[index], rhs); });
}
/// \return The minimum vector with the smaller value on each dimension
template <size_t N, typename T, typename K>
[[nodiscard]] constexpr inline auto min(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	return build_vec<N>([&](const auto index) { return std::min(lhs, rhs.data()[index]); });
}

/// \return Clamps the vector's each dimension within the range of [\c high, \c low]
template <size_t N, typename T>
[[nodiscard]] constexpr inline auto clamp(const vec_t<N, T>& vec, const T& low, const T& high) noexcept {
	return build_vec<N>([&](const auto index) {
		// NOTE: vec.hpp is a sensitive header: <algorithm> would be too heavy include for std::clamp only
		auto value = vec.data()[index];
		return (value < low) ? low : (high < value) ? high : value;
	});
}

/// \return Clamps the vector's each dimension within the range of [\c high, \c low] matching component
template <size_t N, typename T>
[[nodiscard]] constexpr inline auto clamp(const vec_t<N, T>& vec, const vec_t<N, T>&  low, const vec_t<N, T>& high) noexcept {
	return build_vec<N>([&](const auto index) {
		// NOTE: vec.hpp is a sensitive header: <algorithm> would be too heavy include for std::clamp only
		auto value = vec.data()[index];
		auto low = low.data()[index];
		auto high = high.data()[index];
		return (value < low) ? low : (high < value) ? high : value;
	});
}

/// \return Abs the vector's each dimension
template <size_t N, typename T>
[[nodiscard]] constexpr inline auto abs(const vec_t<N, T>& vec) noexcept {
	return build_vec<N>([&](const auto index) { return std::abs(vec.data()[index]); });
}

// -------------------------------------------------------------------------------------------------

template <size_t N, typename T>
[[nodiscard]] constexpr inline auto length(const vec_t<N, T>& vec) noexcept {
	return vec.length();
}

template <size_t N, typename T>
[[nodiscard]] constexpr inline auto length_sq(const vec_t<N, T>& vec) noexcept {
	return vec.length_sq();
}

template <size_t N, typename T>
[[nodiscard]] constexpr inline auto length_and_dir(const vec_t<N, T>& vec) noexcept {
	return vec.length_and_dir();
}

template <size_t N, typename T>
[[nodiscard]] constexpr inline auto normalize(const vec_t<N, T>& vec) noexcept {
	return vec.normalize_copy();
}

/// \return The static_cast-ed vector to the requested K type
template <typename K, size_t N, typename T>
[[nodiscard]] constexpr inline vec_t<N, K> cast(const vec_t<N, T>& vec) noexcept {
	return vec.template cast<K>();
}

// -------------------------------------------------------------------------------------------------

template <size_t N, typename T>
[[nodiscard]] constexpr inline auto floor(const vec_t<N, T>& vec) noexcept {
	return build_vec<N>([&](const auto index) { return std::floor(vec.data()[index]); });
}

template <size_t N, typename T>
[[nodiscard]] constexpr inline auto ceil(const vec_t<N, T>& vec) noexcept {
	return build_vec<N>([&](const auto index) { return std::ceil(vec.data()[index]); });
}

template <size_t N, typename T>
[[nodiscard]] constexpr inline auto round(const vec_t<N, T>& vec) noexcept {
	return build_vec<N>([&](const auto index) { return std::round(vec.data()[index]); });
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline auto lround(const vec_t<N, T>& vec) noexcept {
	return build_vec<N>([&](const auto index) { return std::lround(vec.data()[index]); });
}
template <size_t N, typename T>
[[nodiscard]] constexpr inline auto llround(const vec_t<N, T>& vec) noexcept {
	return build_vec<N>([&](const auto index) { return std::llround(vec.data()[index]); });
}

template <size_t N, typename T, typename K, typename L = std::common_type_t<T, K>>
[[nodiscard]] constexpr inline bool approx_eq(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs, L&& epsilon = L{0.00001}) noexcept {
	bool result = true;
	libv::meta::for_constexpr<0, N>([&](const auto index) {
		result = result && std::abs(lhs.data()[index] - rhs.data()[index]) < epsilon;
	});
	return result;
}

template <size_t N, typename T, typename K, typename L>
[[nodiscard]] constexpr inline bool within(const vec_t<N, T>& value, const vec_t<N, K>& min, const vec_t<N, L>& max) noexcept {
	bool result = true;
	libv::meta::for_constexpr<0, N>([&](const auto index) {
		result = result &&
				value.data()[index] >= min.data()[index] &&
				value.data()[index] <= max.data()[index];
	});
	return result;
}

template <typename T>
[[nodiscard]] constexpr inline auto from_rgba(const T& v) noexcept {
	return vec_t<4, decltype(v.r)>{v.r, v.g, v.b, v.a};
}
template <typename T>
[[nodiscard]] constexpr inline auto from_rgb(const T& v) noexcept {
	return vec_t<3, decltype(v.r)>{v.r, v.g, v.b};
}
template <typename T>
[[nodiscard]] constexpr inline auto from_rg(const T& v) noexcept {
	return vec_t<2, decltype(v.r)>{v.r, v.g};
}

} // namespace vec ---------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------

} // namespace libv
