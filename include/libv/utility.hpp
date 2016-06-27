// File:   utility.hpp

#pragma once

#include <utility>
#include <thread>

// -------------------------------------------------------------------------------------------------

#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// -------------------------------------------------------------------------------------------------

namespace libv {

// -------------------------------------------------------------------------------------------------

template<typename T, typename... Args>
inline T* new_f(Args&&... args) {
	return new T(std::forward<Args>(args)...);
}

struct new_t {
	template<typename T, typename... Args>
	inline T* operator()(Args&&... args) {
		return new T(std::forward<Args>(args)...);
	}
};

// -------------------------------------------------------------------------------------------------

inline uint64_t get_this_thread_id() {
	static_assert(sizeof (std::thread::id) == sizeof (uint64_t), "thead::id size is not 64bit");
	auto id = std::this_thread::get_id();
	uint64_t* ptr = reinterpret_cast<uint64_t*>(&id);
	return (*ptr);
}

// -------------------------------------------------------------------------------------------------

} //namespace libv

// Strong Typedef ----------------------------------------------------------------------------------
// Based on boost strong typedef <boost/serialization/strong_typedef.hpp>

// TODO P4: constexpr

#define LIBV_STRONG_TYPEDEF(T, D)                                                                    \
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