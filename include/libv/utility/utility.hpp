// File: utility.hpp

#pragma once

#include <memory>
#include <thread>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename M>
constexpr inline size_t member_offset(M T::* member) {
	return sizeof (char[reinterpret_cast<size_t>(&(static_cast<T*>(nullptr)->*member))]);
}

// -------------------------------------------------------------------------------------------------

template <typename T, typename... Args>
inline T* new_f(Args&&... args) {
	return new T(std::forward<Args>(args)...);
}

struct new_t {
	template <typename T, typename... Args>
	inline T* operator()(Args&&... args) {
		return new T(std::forward<Args>(args)...);
	}
};

// -------------------------------------------------------------------------------------------------

// TODO P5: move to thread/number(?).hpp
inline size_t current_thread_number() {
	static_assert(sizeof (std::thread::id) == sizeof (size_t), "thread::id size is not matching size_t");
	auto id = std::this_thread::get_id();
	return reinterpret_cast<size_t&>(id);
}

// -------------------------------------------------------------------------------------------------

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
constexpr auto to_value(E e) -> typename std::underlying_type<E>::type {
   return static_cast<typename std::underlying_type<E>::type>(std::forward<E>(e));
}

// -------------------------------------------------------------------------------------------------

inline void hash_combine(std::size_t&) { }

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    hash_combine(seed, rest...);
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

#define LIBV_MAKE_HASHABLE(type, ...)                                                              \
namespace std {                                                                                    \
	template <> struct hash<type> {                                                                \
		std::size_t operator()(const type &t) const {                                              \
			std::size_t ret = 0;                                                                   \
			hash_combine(ret, __VA_ARGS__);                                                        \
			return ret;                                                                            \
}};}

// -------------------------------------------------------------------------------------------------