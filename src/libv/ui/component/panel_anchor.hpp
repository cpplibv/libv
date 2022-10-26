// Project: libv.ui, File: src/libv/ui/component/panel_anchor.hpp

#pragma once

// pro
#include <libv/ui/component/base_panel.hpp>
#include <libv/ui/component_system/component_api.hpp>
#include <libv/ui/property/snap_to_edge.hpp>
#include <libv/ui/property/squish.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelAnchor : public ComponentAPI<BasePanel, PanelAnchor, class CorePanelAnchor, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "anchor";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void snap_to_edge(SnapToEdge value);
	[[nodiscard]] SnapToEdge snap_to_edge() const noexcept;

	void squish(Squish value);
	[[nodiscard]] Squish squish() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
