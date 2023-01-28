// Project: libv.ui, File: src/libv/ui/component/base_panel_core.hpp

#pragma once

// hpp
#include <libv/ui/component/base_panel.hpp>
// std
#include <vector>
// pro
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreBasePanel : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;

public:
	PropertyR<Background> background;

	template <typename T> static void access_properties(T& ctx);

public:
	std::vector<Component> children;

public:
	void add(Component component);
	void add(Component component, std::size_t index);
	void add_front(Component component);
	void remove(Component& component);
	void remove(std::string_view component_name);
	void clear();

	[[nodiscard]] size_t children_size() const;

public:
	virtual void doStyle(StyleAccess& access) override;
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
			[](auto& c) -> auto& { return c.background; },
			Background::none(),
			pgr::appearance, pnm::background,
			"Background"
	);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
