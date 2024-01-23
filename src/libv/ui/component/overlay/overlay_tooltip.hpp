// Project: libv.ui, File: src/libv/ui/component/overlay/overlay_tooltip.hpp

#pragma once

//pro
#include <libv/ui/property/margin.hpp>
#include <libv/ui/component/component.hpp>
#include <libv/ui/component_system/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

enum class AnchorType {
	mouse,
	mouse_no_follow,
	component,
	screen,
	callback
};

enum class Edge {
	clipping_space, //(scroll pane)
	window
};

enum class EdgeBehaviour {
	overhang,
	clipped,
	push_in_axis,
	flip_to_opposite_side
};

enum class Clipping {
	use_clipping, // (scroll pane)
	render_above_everything
};

// -------------------------------------------------------------------------------------------------

class OverlayTooltip : public ComponentAPI<Component, OverlayTooltip, class CoreOverlayTooltip, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "overlay-tooltip";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void show_tooltip(Component owner, Component tooltip);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
