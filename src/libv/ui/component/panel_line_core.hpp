// Project: libv.ui, File: src/libv/ui/component/panel_line_core.hpp

// hpp
#include <libv/ui/component/panel_line.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/base_panel_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelLine : public CoreBasePanel {
public:
	friend PanelLine;
	[[nodiscard]] inline auto handler() { return PanelLine{this}; }

private:
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

public:
	using CoreBasePanel::CoreBasePanel;

protected:
	virtual void doStyle(ContextStyle& context) override;
	virtual void doStyle(ContextStyle& context, ChildID childID) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CorePanelLine::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.align_horizontal; },
			AlignHorizontal::left,
			pgr::layout, pnm::align_horizontal,
			"Horizontal align of the inner content of the component"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.align_vertical; },
			AlignVertical::top,
			pgr::layout, pnm::align_vertical,
			"Vertical align of the inner content of the component"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.orientation; },
			Orientation::right,
			pgr::layout, pnm::orientation,
			"Orientation of subsequent components"
	);
	ctx.property(
			[](auto& c) -> auto& { return c.spacing; },
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
