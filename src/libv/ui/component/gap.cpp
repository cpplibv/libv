// Project: libv.ui, File: src/libv/ui/component/gap.cpp

// hpp
#include <libv/ui/component/gap.hpp>
// pro
#include <libv/ui/component/component_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreGap : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;
};

// =================================================================================================

core_ptr Gap::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool Gap::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
