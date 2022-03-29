// Project: libv.ui, File: src/libv/ui/context/context_style.hpp

#pragma once

// libv
#include <libv/utility/memory/observer_ptr.hpp>
// pro
#include <libv/ui/style_fwd.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreComponent;
class ContextUI;

struct ContextStyle {
	libv::observer_ptr<Style> style;
	CoreComponent& component;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
