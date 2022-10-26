// Project: libv.ui, File: src/libv/ui/component/panel_board_core.hpp

// hpp
#include <libv/ui/component/panel_board.hpp>
// pro
#include <libv/ui/component/base_panel_core.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CorePanelBoard : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;

public:
	struct Properties {
		PropertyR<Background> background;
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

public:
	struct Child {
		Component component;
		libv::vec2f position;
		libv::vec2f size;
	};
	std::vector<Child> children;

public:
	virtual void doStyle(StyleAccess& access) override;
	virtual void doStyleChild(StyleAccess& access, ChildID childID) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& le) override;
	virtual void doLayout2(const ContextLayout2& le) override;
	virtual void doRender(Renderer& r) override;
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual libv::observer_ptr<CoreComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current) override;
	virtual void doForeachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doForeachChildren(libv::function_ref<void(Component&)> callback) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CorePanelBoard::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.background; },
			Background::none(),
			pgr::appearance, pnm::background,
			"Background"
	);
}

template <typename T>
void CorePanelBoard::access_child_properties(T& ctx) {
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
