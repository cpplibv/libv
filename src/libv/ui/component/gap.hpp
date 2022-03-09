// Project: libv.ui, File: src/libv/ui/component/label.hpp

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Gap : public ComponentAPI<Component, Gap, class CoreGap, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "gap";
	static core_ptr create_core(std::string name);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
