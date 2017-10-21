// File: utility.hpp

#pragma once

#include <memory>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
constexpr auto to_value(E e) -> typename std::underlying_type<E>::type {
   return static_cast<typename std::underlying_type<E>::type>(std::forward<E>(e));
}

// -------------------------------------------------------------------------------------------------

template <typename T, size_t N>
constexpr inline size_t count_of(const T (&)[N]) {
	return N;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

// Strong Typedef ----------------------------------------------------------------------------------
// Based on boost strong typedef <boost/serialization/strong_typedef.hpp>

// TODO P4: constexpr
// TODO P5: move to utility/strong_typedef(?).hpp
#define LIBV_STRONG_TYPEDEF(T, D)                                                                  \
struct D {                                                                                         \
	T t;                                                                                           \
	explicit D(const T t_) : t(t_) {};                                                             \
	D(): t() {};                                                                                   \
	D(const D & t_) : t(t_.t){}                                                                    \
	D & operator=(const D & rhs) { t = rhs.t; return *this;}                                       \
	D & operator=(const T & rhs) { t = rhs; return *this;}                                         \
	operator const T & () const {return t; }                                                       \
	operator T & () { return t; }                                                                  \
	bool operator==(const D & rhs) const { return t == rhs.t; }                                    \
	bool operator<(const D & rhs) const { return t < rhs.t; }                                      \
};

// -------------------------------------------------------------------------------------------------
