// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

#pragma once

// libv
#include <libv/utility/observer_ref.hpp>
// std
#include <functional>
#include <string_view>
// pro
#include <libv/ui/component_base.hpp>
#include <libv/ui/layout_default.hpp>
#include <libv/ui/module_layout.hpp>
//#include <libv/ui/module_render.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ContextUI;
template <typename CRTP> struct ComponentStatic;

class ComponentStaticAccess {
	template <typename CRTP> friend class ComponentStatic;

	template <typename T, typename F>
	static void access(T& component, F&& func) {
		component.ui_access(std::forward<F>(func));
	}
};

// -------------------------------------------------------------------------------------------------

template <typename CRTP>
class ComponentStatic : public ComponentBase {
public:
	ComponentStatic(std::string name) :
		ComponentBase(std::move(name)) { }

	ComponentStatic(UnnamedTag, const std::string_view type = "static") :
		ComponentBase(UnnamedTag{}, type) { }

private:
	template <typename Base, typename Func>
	inline void access_every(Func&& func) {
		auto& self = static_cast<CRTP&>(*this);
		ComponentStaticAccess::access(self, [&func](auto&& member) {
			if constexpr (std::is_base_of_v<Base, std::decay_t<decltype(member)>>) {
				func(std::forward<decltype(member)>(member));
			}
		});
	}

	virtual void doAttach(ContextUI& context) override final {
		// TODO P4: use ModuleAttach (??)
		access_every<ComponentBase>([&context](ComponentBase& member) {
			member.attach(context);
		});
	};
	virtual void doCreate(ContextRender& context) override final {
		// TODO P4: use ModuleRender (?)
		access_every<ComponentBase>([&context](ComponentBase& member) {
			member.create(context);
		});
	};
	virtual void doDestroy(ContextRender& context) override final {
		access_every<ComponentBase>([&context](ComponentBase& member) {
			member.destroy(context);
		});
	};
	virtual void doRender(ContextRender& context) override final {
		access_every<ComponentBase>([&context](ComponentBase& member) {
			member.render(context);
		});
	};

	virtual void doLayoutPass1(const ContextLayoutPass1& environment) override final {
		// TODO P5: Find a more static place for the default layout (?)
		LayoutDefault defaultLayout;
		libv::observer_ref<ModuleLayout> layout = libv::make_observer_ref(defaultLayout);

		access_every<ModuleLayout>([&layout](auto&& member) {
			// TODO P1: manual devirtualize layout (every modules) calls, 'member' has the correct static type
			layout = libv::make_observer_ref(member);
		});

		layout->layoutPass1(environment, *this);
	}

	virtual void doLayoutPass2(const ContextLayoutPass2& environment) override final {
		LayoutDefault defaultLayout;
		libv::observer_ref<ModuleLayout> layout = libv::make_observer_ref(defaultLayout);

		access_every<ModuleLayout>([&layout](auto&& member) {
			// TODO P1: manual devirtualize layout (every modules) calls, 'member' has the correct static type
			layout = libv::make_observer_ref(member);
		});

		layout->layoutPass2(environment, *this);
	}

	virtual void doForeachChildren(const std::function<void(ComponentBase&)>& callback) override final {
		access_every<ComponentBase>(callback);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
