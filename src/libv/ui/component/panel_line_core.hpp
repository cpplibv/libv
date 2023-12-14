// Project: libv.ui, File: src/libv/ui/component/panel_line_core.hpp

// hpp
#include <libv/ui/component/panel_line.hpp>
// pro
#include <libv/ui/component/base_panel_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CorePanelLine : CoreBasePanel {
	using base_type = CoreBasePanel;
	using base_type::base_type;

public:
	struct Properties {
		PropertyL2<AlignHorizontal> align_horizontal;
		PropertyL2<AlignVertical> align_vertical;
		PropertyL1L2<Orientation> orientation;
		PropertyL1L2<Spacing> spacing;
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);
	template <typename Access> void access_layout(Access&& access);

public:
	virtual void doStyle(StyleAccess& access) override;
	virtual void doStyleChild(StyleAccess& access, ChildID childID) override;
	virtual libv::vec2f doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CorePanelLine::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.align_horizontal; },
			AlignHorizontal::left,
			pgr::layout, pnm::align_horizontal,
			"Horizontal align of the inner content of the component"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.align_vertical; },
			AlignVertical::top,
			pgr::layout, pnm::align_vertical,
			"Vertical align of the inner content of the component"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.orientation; },
			Orientation::right,
			pgr::layout, pnm::orientation,
			"Orientation of subsequent components"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.property.spacing; },
			Spacing{0},
			pgr::layout, pnm::spacing,
			"Spacing between the components along the orientation"
	);
}

template <typename T>
void CorePanelLine::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
