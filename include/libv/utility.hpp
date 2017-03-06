// File:   utility.hpp

#pragma once

#include <memory>
#include <thread>
#include <utility>

// -------------------------------------------------------------------------------------------------

namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename M>
constexpr inline size_t member_offset(M T::* member) {
	return sizeof (char[reinterpret_cast<size_t>(&(static_cast<T*>(nullptr)->*member))]);
}

// -------------------------------------------------------------------------------------------------

template<size_t N, typename F, size_t... Indices>
constexpr inline void n_times_helper(F&& func, std::index_sequence<Indices...>) {
	(void) std::initializer_list<int> {
		((void) (func(Indices)), 0)...
	};
}

template<size_t N, typename F>
constexpr inline void n_times(F&& func) {
	n_times_helper<N>(std::forward<F>(func), std::make_index_sequence<N>{});
}

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
	static_assert(sizeof (std::thread::id) == sizeof (uint64_t), "thread::id size is not 64bit");
	auto id = std::this_thread::get_id();
	uint64_t* ptr = reinterpret_cast<uint64_t*>(&id);
	return (*ptr);
}

// -------------------------------------------------------------------------------------------------

template<typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
constexpr auto to_value(E e) -> typename std::underlying_type<E>::type {
   return static_cast<typename std::underlying_type<E>::type>(std::forward<E>(e));
}

// -------------------------------------------------------------------------------------------------

template <typename T, typename U>
inline bool equals(const std::weak_ptr<T>& t, const std::weak_ptr<U>& u) {
	return !t.owner_before(u) && !u.owner_before(t);
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

template<typename T, size_t N>
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
	template<> struct hash<type> {                                                                 \
		std::size_t operator()(const type &t) const {                                              \
			std::size_t ret = 0;                                                                   \
			hash_combine(ret, __VA_ARGS__);                                                        \
			return ret;                                                                            \
}};}

// -------------------------------------------------------------------------------------------------