// Project: libv.ui, File: src/libv/ui/component/panel_anchor_3d.hpp

#pragma once

// pro
#include <libv/ui/component/base_panel.hpp>
#include <libv/ui/component_system/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelAnchor3D : public ComponentAPI<BasePanel, PanelAnchor3D, class CorePanelAnchor3D, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "anchor3D";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
//	void add(Component component);
//	void add(libv::vec3f position, Component component);
//	void add(Component component, std::function<libv::vec3f()> position_scraper);
//	PositionSetterProxy add(Component component);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
