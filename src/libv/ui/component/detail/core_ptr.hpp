// Project: libv.ui, File: src/libv/ui/component/detail/core_ptr.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>


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

} // namespace ui
} // namespace libv
