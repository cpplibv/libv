// Project: libv.ui, File: src/libv/ui/zzz_pastebin/component_static.hpp

#pragma once

// libv
#include <libv/utility/memory/observer_ref.hpp>
// std
#include <functional>
#include <string_view>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/layout_default.hpp>
#include <libv/ui/module_layout.hpp>
//#include <libv/ui/module_render.hpp>


namespace libv {
namespace ui {

// =================================================================================================

// Hive {
//     0..* Render
//     0..* Hive
//     0..1 Focus
//     0..1 Layout
//     0..1 Event_Mouse
//     0..1 Event_Keyboard
// }

//struct ModuleFocus : Module {};
//struct ModuleProperties : Module {};
//struct Render_StretchPlane : ModuleRender {};
//struct Render_ImagePlane : ModuleRender {};
//struct Event_Keyboard : Module {};
//struct Event_Mouse : Module {};
//struct Event_MousePosition : Module {};
//struct Focus : ModuleFocus {};
//struct Layout_Fix : ModuleLayout {};
//struct Layout_Float : ModuleLayout {};
//struct Layout_Flow : ModuleLayout {};
//struct Layout_Layer : ModuleLayout {};
//struct Layout_ToolTip : ModuleLayout {};

// -------------------------------------------------------------------------------------------------

//struct Label : public Hive<Label> {
//    // String2D, Something that requires complex binding of size and layouting
//};
//
//struct ToolTip : public Hive<ToolTip> {
//	Render_StretchPlane background;
//	Label text;
//	Layout_Flow layout;
//};
//
//struct ToolTip_Host : public Hive<ToolTip_Host> {
//	// This tooltip should follow the mouse, there should be ones that does not
//	ToolTip toolTip;
//	Layout_ToolTip layout;
//	Event_Mouse eventMouse;
//};
//
//struct CivIcon : public Hive<CivIcon> {
//	Render_ImagePlane plane;
//	Layout_Layer layout;
//
//	ToolTip_Host toolTip;
//	Event_Mouse eventMouse;
//};
//
//struct LoadingBar : public Hive<LoadingBar> {
//
//};
//
//struct DeclarationOfWarIcon : public Hive<DeclarationOfWarIcon> {
//	CivIcon left;
//	CivIcon right;
//	Render_ImagePlane roundBackground;
//
//	Layout_Layer layout;
//
//	ToolTip toolTip;
//	Event_Mouse eventMouse;
//};
//
//struct RightSidePanel : public Hive<RightSidePanel> {
//	std::vector<DeclarationOfWarIcon> notifications;
//
//    struct Layout_DroppingNewEntriesFromSky {};
//	Layout_DroppingNewEntriesFromSky layout;
//
//	template <typename T>
//    void ui_access(T& access) {
//        access(layout);
//
//		for (auto& notification : notifications)
//			access(notification);
//    }
//};
//
//struct PlanetHud : public Hive<PlanetHud> {
//	Label name;
//};
//
//struct Button : public Hive<Button> {
//	Label text;
//	Render_StretchPlane plane;
//
//	Layout_Layer layout;
//
//	Focus focus;
//	Event_Keyboard eventKeyboard;
//	Event_Mouse eventMouse;
//
//    template <typename T>
//    void ui_access(T& access) {
//        access(text);
//        access(plane);
//        access(layout);
//        access(focus);
//        access(eventKeyboard);
//        access(eventMouse);
//    }
//};
//
//struct InputField : public Hive<InputField> {
//    Label text;
//    Render_StretchPlane plane;
//
//    Layout_Layer layout;
//
//    Focus focus;
//    Event_Keyboard eventKeyboard;
//    Event_Mouse eventMouse;
//
//    template <typename T>
//    void ui_access(T& access) {
//        access(text);
//        access(plane);
//        access(layout);
//        access(focus);
//        access(eventKeyboard);
//        access(eventMouse);
//    }
//};
//
//struct LuaComponent : public Hive<LuaComponent> {
//	std::vector<Hive<LuaComponent>> children;
//	std::vector<std::unique_ptr<Render>> renders;
//
//	std::unique_ptr<Layout> layout;
//	std::unique_ptr<Focus> focus;
//	std::unique_ptr<Event_Keyboard> eventKeyboard;
//	std::unique_ptr<Event_Mouse> eventMouse;
//
//    template <typename T>
//    void ui_access(T& access) {
//        access(layout);
//
//		for (auto& child : children)
//			access(child);
//		for (auto& render : renders)
//			access(render);
//
//		if(layout) access(*layout);
//		if(focus) access(*focus);
//		if(eventKeyboard) access(*eventKeyboard);
//		if(eventMouse) access(*eventMouse);
//    }
//};

// =================================================================================================

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
class ComponentStatic : public CoreComponent {
public:
	ComponentStatic(std::string name) :
		CoreComponent(std::move(name)) { }

	ComponentStatic(GenerateName, const std::string_view type = "static") :
		CoreComponent(GenerateName{}, type) { }

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
			if constexpr (std::is_base_of_v<CoreComponent, std::remove_cvref_t<decltype(member)>>)
				member.attach(context);
		});
	};

//	virtual void doCreate(ContextRender& context) override final {
//		ComponentStaticAccess::access(self(), [&context](auto& member) {
//			if constexpr (std::is_base_of_v<CoreComponent, std::remove_cvref_t<decltype(member)>>)
//				member.create(context);
//		});
//	};
//
//	virtual void doDestroy(ContextRender& context) override final {
//		ComponentStaticAccess::access(self(), [&context](auto& member) {
//			if constexpr (std::is_base_of_v<CoreComponent, std::remove_cvref_t<decltype(member)>>)
//				member.destroy(context);
//		});
//	};

	virtual void doRender(Renderer& r) override final {
		ComponentStaticAccess::access(self(), [&context](auto& member) {
			if constexpr (std::is_base_of_v<CoreComponent, std::remove_cvref_t<decltype(member)>>)
				member.render(context);
		});
	};

	virtual libv::vec2f doLayout1(const ContextLayout1& environment) override final {
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

	virtual void doForeachChildren(const std::function<void(CoreComponent&)>& callback) override final {
		ComponentStaticAccess::access(self(), [&callback](auto& member) {
			if constexpr (std::is_base_of_v<CoreComponent, std::remove_cvref_t<decltype(member)>>)
				callback(member);
		});
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
