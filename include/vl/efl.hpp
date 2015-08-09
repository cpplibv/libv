// File:   efl.hpp

#pragma once

#include <vl/type_traits.hpp>

namespace vl {
namespace efl { // ---------------------------------------------------------------------------------

void load();
void unload();

} //namespace efl // -------------------------------------------------------------------------------

// --- is_efl_loadable -----------------------------------------------------------------------------
template <typename T, typename = void> struct is_efl_loadable : std::false_type {
};
template <typename T>
struct is_efl_loadable<T, void_t<decltype(
		::vl::efl::load(std::declval<T&>())
		)>> : std::true_type { };
		
// --- is_efl_unloadable ---
template <typename T, typename = void> struct is_efl_unloadable : std::false_type {
};
template <typename T>
struct is_efl_unloadable<T, void_t<decltype(
		::vl::efl::unload(std::declval<T&>())
		)>> : std::true_type { };
		
namespace efl { // ---------------------------------------------------------------------------------
namespace lookup { // ------------------------------------------------------------------------------

template <typename T,
typename = vl::disable_if<vl::is_efl_loadable<T>>,
typename = vl::disable_if<vl::is_member_loadable<T>>>
inline void load(T&) {
	static_assert(vl::always_false<T>::value,
			"\n\t\tT type has no way of loading."
			"\n\t\tNeither ::vl::efl::load(T&) nor member T::load() function was not found."
			"\n\t\tFor more information see vl::efl - External Function Location");
}
template <typename T,
typename = vl::disable_if<vl::is_efl_loadable<T>>,
typename = vl::enable_if<vl::is_member_loadable<T>>>
inline auto load(T& p) -> decltype(p.load()) {
	return p.load();
}
template <typename T,
typename = vl::enable_if<vl::is_efl_loadable<T>>>
inline auto load(T& p) -> decltype(::vl::efl::load(p)) {
	return ::vl::efl::load(p);
}

// -------------------------------------------------------------------------------------------------

template <typename T,
typename = vl::disable_if<vl::is_efl_unloadable<T>>,
typename = vl::disable_if<vl::is_member_unloadable<T>>>
inline void unload(T&) {
	static_assert(vl::always_false<T>::value,
			"\n\t\tT type has no way of unloading."
			"\n\t\tNeither ::vl::efl::unload(T&) nor member T::unload() function was not found."
			"\n\t\tFor more information see vl::efl - External Function Location");
}
template <typename T,
typename = vl::disable_if<vl::is_efl_unloadable<T>>,
typename = vl::enable_if<vl::is_member_unloadable<T>>>
inline auto unload(T& p) -> decltype(p.unload()) {
	return p.unload();
}
template <typename T,
typename = vl::enable_if<vl::is_efl_unloadable<T>>>
inline auto unload(T& p) -> decltype(::vl::efl::unload(*p)) {
	return ::vl::efl::unload(*p);
}

// -------------------------------------------------------------------------------------------------

} //namespace lookup
} //namespace efl
} //namespace vl