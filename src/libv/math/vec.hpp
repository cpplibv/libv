// Project: libv.math, File: src/libv/math/vec.hpp

#pragma once

// fwd
#include <libv/math/vec_fwd.hpp>
// libv
#include <libv/meta/for_constexpr.hpp>
#include <libv/meta/force_inline.hpp>
// std
#include <cmath>
#include <ostream> // Remove include after https://github.com/fmtlib/fmt/issues/2449 is resolved
// pro
#include <libv/math/vec_concept.hpp>


// TODO P1: Modify (bool operator<) to (vec<bool, N> operator<)
// TODO P2: add swizzle custom getters back into member after measuring the impact, and consider the concept based swizzle too, so either both or only non-member
//			Interesing presentation on vec: https://www.youtube.com/watch?v=8FoAxasNssA

// NOTE: Use concepts to enable every operation based on underlying types | compile time hit not worth it
// NOTE: Perfect forwarding is possible, but it seams like does not worth additional 600 line template


// -------------------------------------------------------------------------------------------------

namespace libv {

// vec_base_t --------------------------------------------------------------------------------------

template <std::size_t N, typename T>
struct vec_base_t {
	T data_[N]{};

	constexpr LIBV_FORCE_INLINE vec_base_t() noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t(const vec_base_t& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t(vec_base_t&& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t& operator=(const vec_base_t& orig) & noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t& operator=(vec_base_t&& orig) & noexcept = default;

	explicit constexpr LIBV_FORCE_INLINE vec_base_t(T value) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			data_[i] = value;
		});
	}

	template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
	constexpr LIBV_FORCE_INLINE explicit vec_base_t(Args&&... values) noexcept : data_{values...} { }

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* data() noexcept {
		return data_;
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const T* data() const noexcept {
		return data_;
	}
};

template <typename T>
struct vec_base_t<2, T> {
	T x{};
	T y{};

	constexpr LIBV_FORCE_INLINE vec_base_t() noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t(const vec_base_t& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t(vec_base_t&& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t& operator=(const vec_base_t& orig) & noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t& operator=(vec_base_t&& orig) & noexcept = default;

	explicit constexpr LIBV_FORCE_INLINE vec_base_t(T value) noexcept : x(value), y(value) { }

	constexpr LIBV_FORCE_INLINE vec_base_t(T x, T y) noexcept : x(x), y(y) { }
	template <typename V0>
			requires Vec2<V0>
	constexpr LIBV_FORCE_INLINE explicit vec_base_t(const V0& xy) noexcept : x(xy.x), y(xy.y) { }

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* data() noexcept {
		return &x;
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const T* data() const noexcept {
		return &x;
	}
};

template <typename T>
struct vec_base_t<3, T> {
	T x{};
	T y{};
	T z{};

	constexpr LIBV_FORCE_INLINE vec_base_t() noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t(const vec_base_t& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t(vec_base_t&& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t& operator=(const vec_base_t& orig) & noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t& operator=(vec_base_t&& orig) & noexcept = default;

	explicit constexpr LIBV_FORCE_INLINE vec_base_t(T value) noexcept : x(value), y(value), z(value) { }

	constexpr LIBV_FORCE_INLINE vec_base_t(T x, T y, T z) noexcept : x(x), y(y), z(z) { }
	template <typename V0>
			requires Vec2<V0>
	constexpr LIBV_FORCE_INLINE vec_base_t(const V0& xy, T z) noexcept : x(xy.x), y(xy.y), z(z) { }
	template <typename V0>
			requires Vec2<V0>
	constexpr LIBV_FORCE_INLINE vec_base_t(T x, const V0& yz) noexcept : x(x), y(yz.x), z(yz.y) { }
	template <typename V0>
			requires Vec3<V0>
	constexpr LIBV_FORCE_INLINE explicit vec_base_t(const V0& xyz) noexcept : x(xyz.x), y(xyz.y), z(xyz.z) { }

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* data() noexcept {
		return &x;
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const T* data() const noexcept {
		return &x;
	}
};

template <typename T>
struct vec_base_t<4, T> {
	T x{};
	T y{};
	T z{};
	T w{};

	constexpr LIBV_FORCE_INLINE vec_base_t() noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t(const vec_base_t& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t(vec_base_t&& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t& operator=(const vec_base_t& orig) & noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_base_t& operator=(vec_base_t&& orig) & noexcept = default;

	explicit constexpr LIBV_FORCE_INLINE vec_base_t(T value) noexcept : x(value), y(value), z(value), w(value) { }

	constexpr LIBV_FORCE_INLINE vec_base_t(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) { }
	template <typename V0>
			requires Vec2<V0>
	constexpr LIBV_FORCE_INLINE vec_base_t(const V0& xy, T z, T w) noexcept : x(xy.x), y(xy.y), z(z), w(w) { }
	template <typename V0>
			requires Vec2<V0>
	constexpr LIBV_FORCE_INLINE vec_base_t(T x, const V0& yz, T w) noexcept : x(x), y(yz.x), z(yz.y), w(w) { }
	template <typename V0>
			requires Vec2<V0>
	constexpr LIBV_FORCE_INLINE vec_base_t(T x, T y, const V0& zw) noexcept : x(x), y(y), z(zw.x), w(zw.y) { }
	template <typename V0, typename V1>
			requires (Vec2<V0> && Vec2<V1>)
	constexpr LIBV_FORCE_INLINE vec_base_t(const V0& xy, const V1& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	template <typename V0>
			requires Vec3<V0>
	constexpr LIBV_FORCE_INLINE vec_base_t(const V0& xyz, T w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }
	template <typename V0>
			requires Vec3<V0>
	constexpr LIBV_FORCE_INLINE vec_base_t(T x, const V0& yzw) noexcept : x(x), y(yzw.x), z(yzw.y), w(yzw.z) { }
	template <typename V0>
			requires Vec4<V0>
	constexpr LIBV_FORCE_INLINE explicit vec_base_t(const V0& xyzw) noexcept : x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w) { }

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* data() noexcept {
		return &x;
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const T* data() const noexcept {
		return &x;
	}
};

// vec_t -------------------------------------------------------------------------------------------

template <std::size_t N, typename T>
struct vec_t : vec_base_t<N, T> {

	static constexpr inline std::size_t dim = N;
	using value_type = T;

	// constructors --------------------------------------------------------------------------------
	constexpr LIBV_FORCE_INLINE vec_t() noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_t(const vec_t&) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_t(vec_t&&) noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_t& operator=(const vec_t&) & noexcept = default;
	constexpr LIBV_FORCE_INLINE vec_t& operator=(vec_t&&) & noexcept = default;

	using vec_base_t<N, T>::vec_base_t;

	// IDEA: With a index_into_vec_pack helper and 2 funny concept over packs all of the vec_base_t ctors could be generalized
	//	template <typename... Components,
	//			CONCEPT_REQUIRES_(VecOrElement<Components, T>()...),
	//			CONCEPT_REQUIRES_(VecSumDimension<Components...>() == N)>
	//	constexpr LIBV_FORCE_INLINE vec_base_t(Components&&... args) :
	//		x(detail::index_into_vec_pack<0>(args...)),
	//		y(detail::index_into_vec_pack<1>(args...)),
	//		z(detail::index_into_vec_pack<2>(args...)),
	//		w(detail::index_into_vec_pack<3>(args...)) { }

	// operator[] ----------------------------------------------------------------------------------
	[[nodiscard]] constexpr LIBV_FORCE_INLINE T& operator[](std::size_t i) & noexcept {
		return this->data()[i];
	}
	[[nodiscard]] constexpr LIBV_FORCE_INLINE const T& operator[](std::size_t i) const & noexcept {
		return this->data()[i];
	}

	// operator= -----------------------------------------------------------------------------------
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator=(const vec_t<N, K>& rhs) & noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] = rhs.data()[i];
		});
		return *this;
	}

	// Special value constructors ------------------------------------------------------------------

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t one(T value) noexcept {
		vec_t result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = value;
		});
		return result;
	}

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t one() noexcept {
		return one(T{1});
	}

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t zero() noexcept {
		return vec_t{};
	}

	template <bool always_false = false>
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t inf() noexcept {
		if constexpr (std::numeric_limits<T>::has_infinity)
			return one(std::numeric_limits<T>::infinity());
		else
			static_assert(always_false, "Underlying type has no infinity");
	}

	template <bool always_false = false>
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t neg_inf() noexcept {
		if constexpr (std::numeric_limits<T>::has_infinity)
			return one(-std::numeric_limits<T>::infinity());
		else
			static_assert(always_false, "Underlying type has no infinity");
	}

	template <bool always_false = false>
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t nan() noexcept {
		if constexpr (std::numeric_limits<T>::has_quiet_NaN)
			return one(std::numeric_limits<T>::quiet_NaN());
		else
			static_assert(always_false, "Underlying type has no NaN");
	}

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t max() noexcept {
		return one(std::numeric_limits<T>::max());
	}

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t min() noexcept {
		return one(std::numeric_limits<T>::min());
	}

	[[nodiscard]] static constexpr LIBV_FORCE_INLINE vec_t lowest() noexcept {
		return one(std::numeric_limits<T>::lowest());
	}

	// operator*=(scalar) --------------------------------------------------------------------------
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator+=(const K& v) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] += v;
		});
		return *this;
	}
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator-=(const K& v) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] -= v;
		});
		return *this;
	}
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator*=(const K& v) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] *= v;
		});
		return *this;
	}
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator/=(const K& v) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] /= v;
		});
		return *this;
	}
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator%=(const K& v) noexcept {
		if constexpr (std::is_floating_point_v<T>)
			libv::meta::for_constexpr<0, N>([&](int i) {
				this->data()[i] = std::fmod(this->data()[i], v);
			});
		else
			libv::meta::for_constexpr<0, N>([&](int i) {
				this->data()[i] %= v;
			});
		return *this;
	}

	// operator*=(vec) -----------------------------------------------------------------------------
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator+=(const vec_t<N, K>& rhs) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] += rhs.data()[i];
		});
		return *this;
	}
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator-=(const vec_t<N, K>& rhs) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] -= rhs.data()[i];
		});
		return *this;
	}
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator*=(const vec_t<N, K>& rhs) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] *= rhs.data()[i];
		});
		return *this;
	}
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator/=(const vec_t<N, K>& rhs) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			this->data()[i] /= rhs.data()[i];
		});
		return *this;
	}
	template <typename K>
	constexpr LIBV_FORCE_INLINE vec_t& operator%=(const vec_t<N, K>& rhs) noexcept {
		if constexpr (std::is_floating_point_v<T>)
			libv::meta::for_constexpr<0, N>([&](int i) {
				this->data()[i] = std::fmod(this->data()[i], rhs.data()[i]);
			});
		else
			libv::meta::for_constexpr<0, N>([&](int i) {
				this->data()[i] %= rhs.data()[i];
			});
		return *this;
	}

	// operator+ -----------------------------------------------------------------------------------
	[[nodiscard]] constexpr LIBV_FORCE_INLINE vec_t operator+() const noexcept {
		vec_t result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = +this->data()[i];
		});
		return result;
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE vec_t operator-() const noexcept {
		vec_t result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = -this->data()[i];
		});
		return result;
	}

	// ---------------------------------------------------------------------------------------------

	/** Return the square length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	[[nodiscard]] constexpr LIBV_FORCE_INLINE T length_sq() const noexcept {
		T result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result += this->data()[i] * this->data()[i];
		});
		return result;
	}

	/** Return the length of the vector
	 * @note Does not change the original vector
	 * @return The vector */
	[[nodiscard]] constexpr LIBV_FORCE_INLINE T length() const noexcept {
		return std::sqrt(length_sq());
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE auto length_and_dir() const noexcept {
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
	constexpr LIBV_FORCE_INLINE vec_t& normalize() noexcept {
		return operator/=(length());
	}

	/** Return the normalized vector (by divide each component by the length).
	 * @note Does not change the original vector
	 * @return The normalized vector */
	[[nodiscard]] constexpr LIBV_FORCE_INLINE vec_t normalize_copy() const noexcept {
		return *this / length();
	}

	/// \return The static_cast-ed vector to the requested K type
	template <typename K>
	[[nodiscard]] constexpr LIBV_FORCE_INLINE vec_t<N, K> cast() const noexcept {
		vec_t<N, K> result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = static_cast<K>(this->data()[i]);
		});
		return result;
	}

	// observers -----------------------------------------------------------------------------------

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* ptr() noexcept {
		return &this->data()[0];
	}

	[[nodiscard]] constexpr LIBV_FORCE_INLINE T* ptr() const noexcept {
		return &this->data()[0];
	}

	template <typename F>
	constexpr LIBV_FORCE_INLINE void sequential_foreach(F&& func) noexcept {
		libv::meta::for_constexpr<0, N>([&](int i) {
			func(this->data()[i]);
		});
	}

	// operator==(vec, vec) ------------------------------------------------------------------------
	template <typename K>
	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE bool operator==(const vec_t& lhs, const vec_t<N, K>& rhs) noexcept {
		bool result = true;
		libv::meta::for_constexpr<0, N>([&](int i) {
			result &= lhs.data()[i] == rhs.data()[i];
		});
		return result;
	}
	template <typename K>
	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE bool operator!=(const vec_t& lhs, const vec_t<N, K>& rhs) noexcept {
		bool result = false;
		libv::meta::for_constexpr<0, N>([&](int i) {
			result |= lhs.data()[i] != rhs.data()[i];
		});
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

		for (std::size_t i = 0; i < N; ++i) {
			if (i != 0)
				os << ' ';
			os << vec.data()[i];
		}
		return os;
	}
};

// operator*(vec, vec) -----------------------------------------------------------------------------

template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator+(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() + std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs.data()[i] + rhs.data()[i];
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator-(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() - std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs.data()[i] - rhs.data()[i];
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator*(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() * std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs.data()[i] * rhs.data()[i];
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator/(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() / std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs.data()[i] / rhs.data()[i];
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator%(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	if constexpr (std::is_floating_point_v<T>) {
		vec_t<N, decltype(std::fmod(std::declval<T>(), std::declval<K>()))> result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = std::fmod(lhs.data()[i], rhs.data()[i]);
		});
		return result;
	} else {
		vec_t<N, decltype(std::declval<T>() % std::declval<K>())> result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = lhs.data()[i] % rhs.data()[i];
		});
		return result;
	}
}

// operator*(vec, scalar) --------------------------------------------------------------------------

template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator+(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() + std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs.data()[i] + rhs;
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator-(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() - std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs.data()[i] - rhs;
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator*(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() * std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs.data()[i] * rhs;
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator/(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() / std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs.data()[i] / rhs;
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator%(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	if constexpr (std::is_floating_point_v<T>) {
		vec_t<N, decltype(std::fmod(std::declval<T>(), std::declval<K>()))> result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = std::fmod(lhs.data()[i], rhs);
		});
		return result;
	} else {
		vec_t<N, decltype(std::declval<T>() % std::declval<K>())> result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = lhs.data()[i] % rhs;
		});
		return result;
	}
}

// operator*(scalar, vec) --------------------------------------------------------------------------

template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator+(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() + std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs + rhs.data()[i];
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator-(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() - std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs - rhs.data()[i];
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator*(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() * std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs * rhs.data()[i];
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator/(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	vec_t<N, decltype(std::declval<T>() / std::declval<K>())> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = lhs / rhs.data()[i];
	});
	return result;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto operator%(const T& lhs, const vec_t<N, K>& rhs) noexcept {
	if constexpr (std::is_floating_point_v<T>) {
		vec_t<N, decltype(std::fmod(std::declval<T>(), std::declval<K>()))> result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = std::fmod(lhs, rhs.data()[i]);
		});
		return result;
	} else {
		vec_t<N, decltype(std::declval<T>() % std::declval<K>())> result{};
		libv::meta::for_constexpr<0, N>([&](int i) {
			result.data()[i] = lhs % rhs.data()[i];
		});
		return result;
	}
}

// operator<(vec, vec) -----------------------------------------------------------------------------
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator<(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.length_sq() < rhs.length_sq();
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator<=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.length_sq() <= rhs.length_sq();
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator>(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.length_sq() > rhs.length_sq();
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator>=(const vec_t<N, T>& lhs, const vec_t<N, K>& rhs) noexcept {
	return lhs.length_sq() >= rhs.length_sq();
}

// operator<(vec, scalar) --------------------------------------------------------------------------
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator<(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length_sq() < rhs * rhs;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator<=(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length_sq() <= rhs * rhs;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator>(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length_sq() > rhs * rhs;
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator>=(const vec_t<N, T>& lhs, const K& rhs) noexcept {
	return lhs.length_sq() >= rhs * rhs;
}

// operator<(scalar, vec) --------------------------------------------------------------------------
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator<(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs * lhs < rhs.length_sq();
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator<=(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs * lhs <= rhs.length_sq();
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator>(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs * lhs > rhs.length_sq();
}
template <std::size_t N, typename T, typename K>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool operator>=(const K& lhs, const vec_t<N, T>& rhs) noexcept {
	return lhs * lhs >= rhs.length_sq();
}

inline namespace vec { // vec utility namespace ----------------------------------------------------

#define __libv_vec_get2(p1,p2)                                                                     \
	template <typename V0>                                                                         \
	[[nodiscard]] constexpr LIBV_FORCE_INLINE auto p1##p2(const V0 vec) noexcept {                 \
		return ::libv::vec_t<2, typename V0::value_type>(vec.p1, vec.p2);                          \
	}
#define __libv_vec_get3(p1,p2,p3)                                                                  \
	template <typename V0>                                                                         \
	[[nodiscard]] constexpr LIBV_FORCE_INLINE auto p1##p2##p3(const V0 vec) noexcept {             \
		return ::libv::vec_t<3, typename V0::value_type>(vec.p1, vec.p2, vec.p3);                  \
	}
#define __libv_vec_get4(p1,p2,p3,p4)                                                               \
	template <typename V0>                                                                         \
	[[nodiscard]] constexpr LIBV_FORCE_INLINE auto p1##p2##p3##p4(const V0 vec) noexcept {         \
		return ::libv::vec_t<4, typename V0::value_type>(vec.p1, vec.p2, vec.p3, vec.p4);          \
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

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE T& x(vec_t<N, T>& vec) noexcept {
	static_assert(N > 0, "vec_t with less then 1 dimension does not have 'x' coordinate");
	return vec.data()[0];
}
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE T& y(vec_t<N, T>& vec) noexcept {
	static_assert(N > 1, "vec_t with less then 2 dimension does not have 'y' coordinate");
	return vec.data()[1];
}
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE T& z(vec_t<N, T>& vec) noexcept {
	static_assert(N > 2, "vec_t with less then 3 dimension does not have 'z' coordinate");
	return vec.data()[2];
}
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE T& w(vec_t<N, T>& vec) noexcept {
	static_assert(N > 3, "vec_t with less then 4 dimension does not have 'w' coordinate");
	return vec.data()[3];
}
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE const T& x(const vec_t<N, T>& vec) noexcept {
	static_assert(N > 0, "vec_t with less then 1 dimension does not have 'x' coordinate");
	return vec.data()[0];
}
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE const T& y(const vec_t<N, T>& vec) noexcept {
	static_assert(N > 1, "vec_t with less then 2 dimension does not have 'y' coordinate");
	return vec.data()[1];
}
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE const T& z(const vec_t<N, T>& vec) noexcept {
	static_assert(N > 2, "vec_t with less then 3 dimension does not have 'z' coordinate");
	return vec.data()[2];
}
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE const T& w(const vec_t<N, T>& vec) noexcept {
	static_assert(N > 3, "vec_t with less then 4 dimension does not have 'w' coordinate");
	return vec.data()[3];
}

// -------------------------------------------------------------------------------------------------

/// \return The dot product of the two vector
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto dot(const vec_t<N, T>& lhs, const vec_t<N, T>& rhs) noexcept {
	T result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result += lhs.data()[i] * rhs.data()[i];
	});
	return result;
}

/// \return The cross product of the two 2D vector
template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto cross(const vec_t<2, T>& lhs, const vec_t<2, T>& rhs) noexcept {
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

/// \return The cross product of the two 3D vector
template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto cross(const vec_t<3, T>& lhs, const vec_t<3, T>& rhs) noexcept {
	return vec_t<3, T>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
}

/// \return The maximum vector with the greater value on each dimension from the two vector
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto max(const vec_t<N, T>& lhs, const vec_t<N, T>& rhs) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::max(lhs.data()[i], rhs.data()[i]);
	});
	return result;
}
/// \return The maximum vector with the greater value on each dimension
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto max(const vec_t<N, T>& lhs, const T& rhs) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::max(lhs.data()[i], rhs);
	});
	return result;
}
/// \return The maximum vector with the greater value on each dimension
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto max(const T& lhs, const vec_t<N, T>& rhs) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::max(lhs, rhs.data()[i]);
	});
	return result;
}

/// \return The minimum vector with the smaller value on each dimension from the two vector
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto min(const vec_t<N, T>& lhs, const vec_t<N, T>& rhs) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::min(lhs.data()[i], rhs.data()[i]);
	});
	return result;
}
/// \return The minimum vector with the smaller value on each dimension
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto min(const vec_t<N, T>& lhs, const T& rhs) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::min(lhs.data()[i], rhs);
	});
	return result;
}
/// \return The minimum vector with the smaller value on each dimension
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto min(const T& lhs, const vec_t<N, T>& rhs) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::min(lhs, rhs.data()[i]);
	});
	return result;
}

/// \return Clamps the vector's each dimension within the range of [\c high, \c low]
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto clamp(const vec_t<N, T>& vec, const T& low, const T& high) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		// NOTE: vec.hpp is a sensitive header: <algorithm> would be too heavy include for only std::clamp
		const auto& value = vec.data()[i];
		result.data()[i] = (value < low) ? low : (high < value) ? high : value;
	});
	return result;
}

/// \return Clamps the vector's each dimension within the range of [\c high, \c low] matching component
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto clamp(const vec_t<N, T>& vec, const vec_t<N, T>& low, const vec_t<N, T>& high) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		// NOTE: vec.hpp is a sensitive header: <algorithm> would be too heavy include for only std::clamp
		const auto& value = vec.data()[i];
		const auto& low_ = low.data()[i];
		const auto& high_ = high.data()[i];
		result.data()[i] = (value < low_) ? low_ : (high_ < value) ? high_ : value;
	});
	return result;
}

/// \return Abs the vector's each dimension
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto abs(const vec_t<N, T>& vec) noexcept {
	vec_t<N, T> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::abs(vec.data()[i]);
	});
	return result;
}

// -------------------------------------------------------------------------------------------------

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto length(const vec_t<N, T>& vec) noexcept {
	return vec.length();
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto length_sq(const vec_t<N, T>& vec) noexcept {
	return vec.length_sq();
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto length_and_dir(const vec_t<N, T>& vec) noexcept {
	return vec.length_and_dir();
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto normalize(const vec_t<N, T>& vec) noexcept {
	return vec.normalize_copy();
}

/// \return The static_cast-ed vector to the requested K type
template <typename K, std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE vec_t<N, K> cast(const vec_t<N, T>& vec) noexcept {
	return vec.template cast<K>();
}

// -------------------------------------------------------------------------------------------------

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto floor(const vec_t<N, T>& vec) noexcept {
	vec_t<N, decltype(std::floor(std::declval<T>()))> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::floor(vec.data()[i]);
	});
	return result;
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto ceil(const vec_t<N, T>& vec) noexcept {
	vec_t<N, decltype(std::ceil(std::declval<T>()))> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::ceil(vec.data()[i]);
	});
	return result;
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto round(const vec_t<N, T>& vec) noexcept {
	vec_t<N, decltype(std::round(std::declval<T>()))> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::round(vec.data()[i]);
	});
	return result;
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto lround(const vec_t<N, T>& vec) noexcept {
	vec_t<N, decltype(std::lround(std::declval<T>()))> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::lround(vec.data()[i]);
	});
	return result;
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto llround(const vec_t<N, T>& vec) noexcept {
	vec_t<N, decltype(std::llround(std::declval<T>()))> result{};
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::llround(vec.data()[i]);
	});
	return result;
}

/// Matches glsl fract: x - floor(x)
template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE vec_t<N, T> fract(const vec_t<N, T>& vec) noexcept {
	vec_t<N, T> result;
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = vec.data()[i] - std::floor(vec.data()[i]);
	});
	return result;
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE vec_t<N, T> sqrt(const vec_t<N, T>& vec) noexcept {
	vec_t<N, T> result;
	libv::meta::for_constexpr<0, N>([&](int i) {
		result.data()[i] = std::sqrt(vec.data()[i]);
	});
	return result;
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE vec_t<N, T> inversesqrt(const vec_t<N, T>& vec) noexcept {
	return 1.f / sqrt(vec);
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool approx_eq(const vec_t<N, T>& lhs, const vec_t<N, T>& rhs, T epsilon = T{0.00001}) noexcept {
	bool result = true;
	libv::meta::for_constexpr<0, N>([&](int i) {
		result &= std::abs(lhs.data()[i] - rhs.data()[i]) < epsilon;
	});
	return result;
}

template <std::size_t N, typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool within(const vec_t<N, T>& value, const vec_t<N, T>& min, const vec_t<N, T>& max) noexcept {
	bool result = true;
	libv::meta::for_constexpr<0, N>([&](int i) {
		result &=
				value.data()[i] >= min.data()[i] &&
				value.data()[i] <= max.data()[i];
	});
	return result;
}

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto from_rgba(const T& v) noexcept {
	return vec_t<4, decltype(v.r)>{v.r, v.g, v.b, v.a};
}
template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto from_rgb(const T& v) noexcept {
	return vec_t<3, decltype(v.r)>{v.r, v.g, v.b};
}
template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto from_rg(const T& v) noexcept {
	return vec_t<2, decltype(v.r)>{v.r, v.g};
}

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto from_xyzw(const T& v) noexcept {
	return vec_t<4, decltype(v.x)>{v.x, v.y, v.z, v.w};
}
template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto from_xyz(const T& v) noexcept {
	return vec_t<3, decltype(v.x)>{v.x, v.y, v.z};
}
template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE auto from_xy(const T& v) noexcept {
	return vec_t<2, decltype(v.x)>{v.x, v.y};
}

} // namespace vec ---------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------

} // namespace libv
