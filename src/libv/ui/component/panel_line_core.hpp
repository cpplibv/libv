// Project: libv.ui, File: src/libv/ui/component/panel_line_core.hpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/panel_line.hpp>
// ext
//#include <boost/container/small_vector.hpp>
//#include <range/v3/view/zip.hpp>
// libv
//#include <libv/utility/approx.hpp>
//#include <libv/utility/min_max.hpp>
//#include <libv/utility/to_underlying.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/base_panel.lpp>
//#include <libv/ui/context/context_layout.hpp>
//#include <libv/ui/context/context_style.hpp>
//#include <libv/ui/layout/view_layouted.lpp>
//#include <libv/ui/log.hpp>
//#include <libv/ui/property.hpp>
//#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelLine : public CoreBasePanel {
	friend class PanelLine;
	[[nodiscard]] inline auto handler() { return PanelLine{this}; }

private:
	struct Properties {
		PropertyL<AlignHorizontal> align_horizontal;
		PropertyL<AlignVertical> align_vertical;
		PropertyL<Orientation> orientation;
		PropertyL<Spacing> spacing;
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
			Orientation::LEFT_TO_RIGHT,
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
