// // Project: libv.ui, File: src/libv/ui/component/panel_anchor_3d.cpp
//
// // hpp
// #include <libv/ui/component/panel_anchor_3d.hpp>
// // libv
// #include <libv/meta/for_constexpr.hpp>
// #include <libv/utility/min_max.hpp>
// #include <libv/utility/to_underlying.hpp>
// // pro
// #include <libv/ui/component/base_panel_core.hpp>
// #include <libv/ui/component/layout/layout_utility.hpp>
// #include <libv/ui/component/layout/view_layouted.hxx>
// #include <libv/ui/context/context_layout.hpp>
// #include <libv/ui/log.hpp>
// #include <libv/ui/property_system/property_access.hpp>
//
//
// namespace libv {
// namespace ui {
//
// // -------------------------------------------------------------------------------------------------
//
// struct CorePanelAnchor3D : CoreBasePanel {
// 	using base_type = CoreBasePanel;
// 	using base_type::base_type;
//
// public:
// 	struct Properties {
// 	} property;
//
// 	struct ChildProperties {
// 	};
//
// 	template <typename T> static void access_properties(T& ctx);
// 	template <typename T> static void access_child_properties(T& ctx);
//
// //	static ComponentPropertyDescription description;
// //	static ComponentPropertyDescription child_description;
//
// public:
// 	virtual void doStyle(StyleAccess& access) override;
// 	virtual void doStyleChild(StyleAccess& access, ChildID childID) override;
// 	virtual libv::vec2f doLayout1(const ContextLayout1& le) override;
// 	virtual void doLayout2(const ContextLayout2& le) override;
// };
//
// // -------------------------------------------------------------------------------------------------
//
// template <typename T>
// void CorePanelAnchor3D::access_properties(T& ctx) {
// 	(void) ctx;
// }
//
// template <typename T>
// void CorePanelAnchor3D::access_child_properties(T& ctx) {
// 	(void) ctx;
// }
//
// // -------------------------------------------------------------------------------------------------
//
// void CorePanelAnchor3D::doStyle(StyleAccess& access) {
// 	access.self(*this);
// }
//
// void CorePanelAnchor3D::doStyleChild(StyleAccess& access, ChildID childID) {
// 	(void) access;
// 	(void) childID;
// }
//
// // -------------------------------------------------------------------------------------------------
//
// libv::vec2f CorePanelAnchor3D::doLayout1(const ContextLayout1& layout_env) {
// 	const auto env_size = layout_env.size - padding_size();
//
// 	auto result = libv::vec2f{};
//
// 	for (auto& child : children | view_layouted()) {
// 		const auto child_dynamic = child.size().has_dynamic() ?
// 				AccessLayout::layout1(child.core(), ContextLayout1{env_size}) :
// 				libv::vec2f{};
//
// 		libv::meta::for_constexpr<0, 2>([&](auto i) {
// 			result[i] = libv::max(
// 					result[i],
// 					resolvePercent(
// 							child.size()[i].pixel + (child.size()[i].dynamic ? child_dynamic[i] : 0.f),
// 							child.size()[i].percent, child.core())
// 						+ child.core().margin_size()[i]
// 			);
// 		});
// 	}
//
// 	return result + padding_size();
// }
//
// void CorePanelAnchor3D::doLayout2(const ContextLayout2& layout_env) {
// 	for (auto& child : children | view_layouted()) {
// 		const auto env_size = layout_env.size - padding_size() - child.core().margin_size();
// 		const auto position = padding_LB() + child.core().margin_LB();
//
// 		const auto roundedPosition = libv::vec::round(position);
// 		const auto roundedSize = libv::vec::round(position + env_size) - roundedPosition;
//
// 		AccessLayout::layout2(
// 				child.core(),
// 				layout_env.enter(roundedPosition, roundedSize)
// 		);
// 	}
// }
//
// // =================================================================================================
//
// core_ptr PanelAnchor3D::create_core(std::string name) {
// 	return create_core_ptr<CoreType>(std::move(name));
// }
//
// bool PanelAnchor3D::castable(libv::ui::core_ptr core) noexcept {
// 	return dynamic_cast<CoreType*>(core) != nullptr;
// }
//
// // -------------------------------------------------------------------------------------------------
//
// } // namespace ui
// } // namespace libv
