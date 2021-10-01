// Project: libv.ui, File: src/libv/ui/component/base_panel.lpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <vector>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/component/detail/component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreBasePanel : public CoreComponent {
protected:
	std::vector<Component> children;

public:
	using CoreComponent::CoreComponent;

public:
	void add(Component component);
	void add_front(Component component);
	void remove(Component& component);
	void clear();

protected:
	virtual void doRender(Renderer& r) override;
	virtual void doDetachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual libv::observer_ptr<CoreComponent> doFocusTraverse(const ContextFocusTraverse& context, ChildID current) override;
	virtual void doForeachChildren(libv::function_ref<bool(Component&)> callback) override;
	virtual void doForeachChildren(libv::function_ref<void(Component&)> callback) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
