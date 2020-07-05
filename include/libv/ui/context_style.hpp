// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/utility/observer_ptr.hpp>
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
