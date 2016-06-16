// File:   efl.hpp

#pragma once

#include <libv/type_traits.hpp>

namespace libv {
namespace efl { // ---------------------------------------------------------------------------------

void load();
void unload();

} //namespace efl // -------------------------------------------------------------------------------

// --- is_efl_loadable -----------------------------------------------------------------------------
template <typename T, typename = void> struct is_efl_loadable : std::false_type {
};
template <typename T>
struct is_efl_loadable<T, void_t<decltype(
		::libv::efl::load(std::declval<T&>())
		)>> : std::true_type { };

// --- is_efl_unloadable ---
template <typename T, typename = void> struct is_efl_unloadable : std::false_type {
};
template <typename T>
struct is_efl_unloadable<T, void_t<decltype(
		::libv::efl::unload(std::declval<T&>())
		)>> : std::true_type { };

namespace efl { // ---------------------------------------------------------------------------------
namespace lookup { // ------------------------------------------------------------------------------

template <typename T,
typename = libv::disable_if<libv::is_efl_loadable<T>>,
typename = libv::disable_if<libv::is_member_loadable<T>>>
inline void load(T&) {
	static_assert(libv::always_false<T>::value,
			"\n\t\tT type has no way of loading."
			"\n\t\tNeither ::libv::efl::load(T&) nor member T::load() function was not found."
			"\n\t\tFor more information see libv::efl - External Function Location");
}
template <typename T,
typename = libv::disable_if<libv::is_efl_loadable<T>>,
typename = libv::enable_if<libv::is_member_loadable<T>>>
inline auto load(T& p) -> decltype(p.load()) {
	return p.load();
}
template <typename T,
typename = libv::enable_if<libv::is_efl_loadable<T>>>
inline auto load(T& p) -> decltype(::libv::efl::load(p)) {
	return ::libv::efl::load(p);
}

// -------------------------------------------------------------------------------------------------

template <typename T,
typename = libv::disable_if<libv::is_efl_unloadable<T>>,
typename = libv::disable_if<libv::is_member_unloadable<T>>>
inline void unload(T&) {
	static_assert(libv::always_false<T>::value,
			"\n\t\tT type has no way of unloading."
			"\n\t\tNeither ::libv::efl::unload(T&) nor member T::unload() function was not found."
			"\n\t\tFor more information see libv::efl - External Function Location");
}
template <typename T,
typename = libv::disable_if<libv::is_efl_unloadable<T>>,
typename = libv::enable_if<libv::is_member_unloadable<T>>>
inline auto unload(T& p) -> decltype(p.unload()) {
	return p.unload();
}
template <typename T,
typename = libv::enable_if<libv::is_efl_unloadable<T>>>
inline auto unload(T& p) -> decltype(::libv::efl::unload(*p)) {
	return ::libv::efl::unload(*p);
}

// -------------------------------------------------------------------------------------------------

} //namespace lookup
} //namespace efl
} //namespace libv