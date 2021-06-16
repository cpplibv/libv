// Project: libv.ui, File: src/libv/ui/component/panel_full.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component/detail/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFull : public ComponentAPI<Component, PanelFull, class CorePanelFull, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "full";
	static core_ptr create_core(std::string name);

public:
	void add(Component component);
	void remove(Component& component);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
