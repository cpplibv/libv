// Project: libv.ui, File: src/libv/ui/component/panel_anchor_core.hpp

#pragma once

// hpp
#include <libv/ui/component/panel_anchor.hpp>
// pro
#include <libv/ui/component/base_panel_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelAnchor : public CoreBasePanel {
public:
	friend PanelAnchor;
	[[nodiscard]] inline auto handler() { return PanelAnchor{this}; }

private:
	struct Properties {
		// TODO P5: libv.ui: Implement Snap to edge in float layout
		PropertyL1L2<SnapToEdge> snap_to_edge;
		// TODO P5: libv.ui: Implement Squish in float layout
		PropertyL1L2<Squish> squish;
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

//	static ComponentPropertyDescription description;
//	static ComponentPropertyDescription child_description;

public:
	using CoreBasePanel::CoreBasePanel;

private:
	virtual void doStyle(ContextStyle& context) override;
	virtual void doStyle(ContextStyle& context, ChildID childID) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CorePanelAnchor::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.snap_to_edge; },
			SnapToEdge{false},
			pgr::layout, pnm::snap_to_edge,
			"Snap to edge any child that otherwise would hang out"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.squish; },
			Squish{false},
			pgr::layout, pnm::squish,
			"Squish any child that otherwise would hang out"
	);
}

template <typename T>
void CorePanelAnchor::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
