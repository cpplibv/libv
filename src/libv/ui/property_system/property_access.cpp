// Project: libv.ui, File: src/libv/ui/property_system/property_access.cpp

// hpp
#include <libv/ui/property_system/property_access.hpp>
// libv
#include <libv/utility/concat.hpp>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/component_system/core_ptr.hpp>
#include <libv/ui/context/context_style.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void assign_nested_style(CoreComponent& component, std::string_view nesting_name) {
	if (component.style() != nullptr || component.parent()->style() == nullptr)
		return;

	// I don't really like this lookup/assignment solution, but it will do for now
	component.style(
		component.ui().style(
			libv::concat(component.parent()->style()->style_name, ">", nesting_name)
		)
	);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
