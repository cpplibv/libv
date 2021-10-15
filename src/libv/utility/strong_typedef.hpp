// Project: libv.utility, File: src/libv/utility/strong_typedef.hpp

#pragma once


// Strong Typedef ----------------------------------------------------------------------------------
// Based on boost strong typedef <boost/serialization/strong_typedef.hpp>
// But take a look at this article http://foonathan.net/blog/2016/10/19/strong-typedefs.html

#define LIBV_STRONG_TYPEDEF(T, D)                                                                  \
struct D {                                                                                         \
	T t;                                                                                           \
	explicit D(const T t_) : t(t_) {};                                                             \
	D(): t() {};                                                                                   \
	D(const D & t_) : t(t_.t){}                                                                    \
	D & operator=(const D & rhs) & { t = rhs.t; return *this;}                                     \
	D & operator=(const T & rhs) & { t = rhs; return *this;}                                       \
	operator const T & () const {return t; }                                                       \
	operator T & () { return t; }                                                                  \
	bool operator==(const D & rhs) const { return t == rhs.t; }                                    \
	bool operator<(const D & rhs) const { return t < rhs.t; }                                      \
};
