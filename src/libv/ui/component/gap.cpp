// Project: libv.ui, File: src/libv/ui/component/gap.cpp

// hpp
#include <libv/ui/component/gap.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreGap : CoreComponent {
public:
	friend Gap;
	[[nodiscard]] inline auto handler() { return Gap{this}; }

public:
	using CoreComponent::CoreComponent;
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
