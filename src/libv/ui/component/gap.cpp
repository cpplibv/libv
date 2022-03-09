// Project: libv.ui, File: src/libv/ui/component/gap.cpp

// hpp
#include <libv/ui/component/gap.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreGap : CoreComponent {
	friend Gap;
	[[nodiscard]] inline auto handler() { return Gap{this}; }

public:
	using CoreComponent::CoreComponent;
};

// =================================================================================================

core_ptr Gap::create_core(std::string name) {
	return create_core_ptr<CoreGap>(std::move(name));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
