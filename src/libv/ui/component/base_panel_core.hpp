// Project: libv.ui, File: src/libv/ui/component/base_panel.hpp

#pragma once

// hpp
#include <libv/ui/component/base_panel.hpp>
// std
#include <vector>
// pro
#include <libv/ui/component/detail/component.hpp>
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreBasePanel : public CoreComponent {
public:
	friend BasePanel;
	[[nodiscard]] inline auto handler() { return BasePanel{this}; }

private:
	template <typename T> static void access_properties(T& ctx);

	struct Properties {
		PropertyR<Background> background;
	} property;

protected:
	std::vector<Component> children;

public:
	using CoreComponent::CoreComponent;

public:
	void add(Component component);
	void add_front(Component component);
	void remove(Component& component);
	void remove(std::string_view component_name);
	void clear();

protected:
	virtual void doStyle(ContextStyle& ctx) override;
	virtual void doRender(Renderer& r) override;
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual libv::observer_ptr<CoreComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current) override;
	virtual void doForeachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doForeachChildren(libv::function_ref<void(Component&)> callback) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreBasePanel::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.background; },
			Background::none(),
			pgr::appearance, pnm::background,
			"Background"
	);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
