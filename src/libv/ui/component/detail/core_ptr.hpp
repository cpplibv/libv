// Project: libv.ui, File: src/libv/ui/component/detail/core_ptr.hpp

#pragma once

// libv
#include <libv/utility/nexus_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Component;
class CoreComponent;

using core_ptr = CoreComponent*;
using core_cptr = const CoreComponent*;

[[nodiscard]] constexpr inline core_ptr get_core(Component& component) noexcept {
	// NOTE: Cast to first member (static_cast could be used, but reinterpret_cast allows incompleteness)
	return *reinterpret_cast<core_ptr*>(&component);
}

[[nodiscard]] constexpr inline core_cptr get_core(const Component& component) noexcept {
	// NOTE: Cast to first member (static_cast could be used, but reinterpret_cast allows incompleteness)
	return *reinterpret_cast<const core_cptr*>(&component);
}

// -------------------------------------------------------------------------------------------------

template <typename T>
	requires (!std::derived_from<T, Component>)
[[nodiscard]] constexpr inline auto get_core(const T& component_like) noexcept {
	return component_like.ptr();
}

[[nodiscard]] libv::Nexus& get_nexus(const Component& component) noexcept;
void mark_as_signal(Component& component) noexcept;
void mark_as_slot(Component& component) noexcept;
[[nodiscard]] bool is_marked_as_signal(const Component& component) noexcept;
[[nodiscard]] bool is_marked_as_slot(const Component& component) noexcept;

[[nodiscard]] libv::Nexus& get_nexus(...) noexcept;
inline void mark_as_signal(...) noexcept {}
inline void mark_as_slot(...) noexcept {}
[[nodiscard]] inline bool is_marked_as_signal(...) noexcept { return true; }
[[nodiscard]] inline bool is_marked_as_slot(...) noexcept { return true; }

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
