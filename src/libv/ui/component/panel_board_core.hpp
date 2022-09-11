// Project: libv.ui, File: src/libv/ui/component/panel_board_core.hpp

// hpp
#include <libv/ui/component/panel_board.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/base_panel_core.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelBoard : public CoreComponent {
public:
	friend PanelBoard;
	[[nodiscard]] inline auto handler() { return PanelBoard{this}; }

private:
	struct Properties {
		PropertyR<Background> background;
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

private:
	struct Child {
		Component component;
		libv::vec2f position;
		libv::vec2f size;
	};
	std::vector<Child> children;

public:
	using CoreComponent::CoreComponent;

private:
	virtual void doStyle(ContextStyle& context) override;
	virtual void doStyle(ContextStyle& context, ChildID childID) override;
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
