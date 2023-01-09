// Project: libv.ui, File: src/libv/ui/component_system/attach_state.hpp

#pragma once

// std
#include <string>
// pro
#include <libv/ui/component/canvas.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T, typename... Args>
[[nodiscard]] inline CanvasAdaptorT<T> createCanvas(std::string componentName, Args&&... args) {
	return CanvasAdaptorT<T>(std::move(componentName), std::forward<Args>(args)...);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
