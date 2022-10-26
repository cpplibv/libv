// Project: libv.ui, File: src/libv/ui/component/panel_full.hpp

#pragma once

// pro
#include <libv/ui/component/base_panel.hpp>
#include <libv/ui/component_system/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelFull : public ComponentAPI<BasePanel, PanelFull, class CorePanelFull, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "full";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
