// Project: libv.ui, File: src/libv/ui/component/panel_full_core.hpp

// hpp
#include <libv/ui/component/panel_full.hpp>
// pro
#include <libv/ui/component/base_panel_core.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CorePanelFull : CoreBasePanel {
	using base_type = CoreBasePanel;
	using base_type::base_type;

public:
	struct Properties {
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

//	static ComponentPropertyDescription description;
//	static ComponentPropertyDescription child_description;

public:
	virtual void doStyle(StyleAccess& access) override;
	virtual void doStyleChild(StyleAccess& access, ChildID childID) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CorePanelFull::access_properties(T& ctx) {
	(void) ctx;
}

template <typename T>
void CorePanelFull::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
