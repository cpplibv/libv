// File: component.hpp Author: Vader Created on 2017. szeptember 10., 18:27

#pragma once

// libv
#include <libv/utility/observer_ref.hpp>
// std
#include <functional>
#include <string_view>
// pro
#include <libv/ui/base_component.hpp>
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

// TODO P4: verify cardinalities (in attach time) of every module (0 layout, 0 child; 1 layout, 1+ child; etc...)
//			log_ui.warn("More than one layout found for {}", path());
// TODO P4: use ModuleAttach, ModuleRender, ModuleLayout concepts during static_access

template <typename CRTP>
class ComponentStatic : public BaseComponent {
public:
	ComponentStatic(std::string name) :
		BaseComponent(std::move(name)) { }

	ComponentStatic(GenerateName, const std::string_view type = "static") :
		BaseComponent(GenerateName{}, type) { }

private:
	constexpr inline auto& self() noexcept {
		return static_cast<CRTP&>(*this);
	}
	constexpr inline const auto& self() const noexcept {
		return static_cast<const CRTP&>(*this);
	}

private:
	virtual void doAttach(context) override final {
		ComponentStaticAccess::access(self(), [&context](auto& member) {
			if constexpr (std::is_base_of_v<BaseComponent, std::remove_cvref_t<decltype(member)>>)
				member.attach(context);
		});
	};

//	virtual void doCreate(ContextRender& context) override final {
//		ComponentStaticAccess::access(self(), [&context](auto& member) {
//			if constexpr (std::is_base_of_v<BaseComponent, std::remove_cvref_t<decltype(member)>>)
//				member.create(context);
//		});
//	};
//
//	virtual void doDestroy(ContextRender& context) override final {
//		ComponentStaticAccess::access(self(), [&context](auto& member) {
//			if constexpr (std::is_base_of_v<BaseComponent, std::remove_cvref_t<decltype(member)>>)
//				member.destroy(context);
//		});
//	};

	virtual void doRender(ContextRender& context) override final {
		ComponentStaticAccess::access(self(), [&context](auto& member) {
			if constexpr (std::is_base_of_v<BaseComponent, std::remove_cvref_t<decltype(member)>>)
				member.render(context);
		});
	};

	virtual void doLayout1(const ContextLayout1& environment) override final {
		bool layouted = false;

		ComponentStaticAccess::access(self(), [&environment, this, &layouted](auto& member) {
			if constexpr (std::is_base_of_v<ModuleLayout, std::remove_cvref_t<decltype(member)>>) {
				member.layout1(environment, *this);
				layouted = true;
			}
		});

		if (not layouted) {
			LayoutDefault defaultLayout;
			defaultLayout.layout1(environment, *this);
		}
	}

	virtual void doLayout2(const ContextLayout2& environment) override final {
		bool layouted = false;

		ComponentStaticAccess::access(self(), [&environment, this, &layouted](auto& member) {
			if constexpr (std::is_base_of_v<ModuleLayout, std::remove_cvref_t<decltype(member)>>) {
				member.layout2(environment, *this);
				layouted = true;
			}
		});

		if (not layouted) {
			LayoutDefault defaultLayout;
			defaultLayout.layout2(environment, *this);
		}
	}

	virtual void doForeachChildren(const std::function<void(BaseComponent&)>& callback) override final {
		ComponentStaticAccess::access(self(), [&callback](auto& member) {
			if constexpr (std::is_base_of_v<BaseComponent, std::remove_cvref_t<decltype(member)>>)
				callback(member);
		});
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
