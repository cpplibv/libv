// Project: libv.ui, File: src/libv/ui/component/panel_full.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/component/panel_full.hpp>
// libv
#include <libv/utility/approx.hpp>
#include <libv/utility/min_max.hpp>
#include <libv/utility/to_underlying.hpp>
// pro
#include <libv/ui/component/base_panel.lpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/layout/view_layouted.lpp>
#include <libv/ui/log.hpp>
#include <libv/ui/property_access_context.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CorePanelFull : public CoreBasePanel {
	friend class PanelFull;
	[[nodiscard]] inline auto handler() { return PanelFull{this}; }

private:
	struct Properties {
	} property;

	struct ChildProperties {
	};

	template <typename T> static void access_properties(T& ctx);
	template <typename T> static void access_child_properties(T& ctx);

//	static ComponentPropertyDescription description;
//	static ComponentPropertyDescription child_description;

public:
	using CoreBasePanel::CoreBasePanel;

private:
	virtual void doStyle(ContextStyle& context) override;
	virtual void doStyle(ContextStyle& context, ChildID childID) override;
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

void CorePanelFull::doStyle(ContextStyle& ctx) {
	// TODO P2: Having property mentioned here is incorrect, but the class based access context will solve he problem
	PropertyAccessContext<Properties> setter{property, ctx.component, ctx.style, ctx.component.context()};
	access_properties(setter);
	CoreComponent::access_properties(setter);
}

void CorePanelFull::doStyle(ContextStyle& ctx, ChildID childID) {
	(void) ctx;
	(void) childID;
}

// -------------------------------------------------------------------------------------------------

libv::vec3f CorePanelFull::doLayout1(const ContextLayout1& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	const auto resolvePercent = [](const float fix, const float percent, auto& component) {
		if (fix == libv::Approx(0.f)) {
			return fix;
		} else if (percent == libv::Approx(100.f)) {
			log_ui.warn("Invalid sum of size percent {} with fixed width of {} during layout of {}", percent, fix, component.path());
			return fix * 2.f;
		} else {
			return fix / (1.f - percent * 0.01f);
		}
	};

	auto result = libv::vec3f{};

	for (auto& child : children | view_layouted()) {
		const auto child_dynamic = child.size().has_dynamic() ?
				AccessLayout::layout1(child.core(), ContextLayout1{env_size}) :
				libv::vec3f{};

		libv::meta::for_constexpr<0, 3>([&](auto i) {
			result[i] = libv::max(
					result[i],
					resolvePercent(
							child.size()[i].pixel + (child.size()[i].dynamic ? child_dynamic[i] : 0.f),
							child.size()[i].percent, child.core())
			);
		});
	}

	return result + padding_size3();
}

void CorePanelFull::doLayout2(const ContextLayout2& layout_env) {
	const auto env_size = layout_env.size - padding_size3();

	const auto position = padding_LB3();
	const auto roundedPosition = libv::vec::round(position);
	const auto roundedSize = libv::vec::round(position + env_size) - roundedPosition;

	for (auto& child : children | view_layouted()) {
		AccessLayout::layout2(
				child.core(),
				layout_env.enter(roundedPosition, roundedSize)
		);
	}
}

// =================================================================================================

PanelFull::PanelFull(std::string name) :
	ComponentHandler<CorePanelFull, EventHostGeneral<PanelFull>>(std::move(name)) { }

PanelFull::PanelFull(GenerateName_t gen, const std::string_view type) :
	ComponentHandler<CorePanelFull, EventHostGeneral<PanelFull>>(gen, type) { }

PanelFull::PanelFull(core_ptr core) noexcept :
	ComponentHandler<CorePanelFull, EventHostGeneral<PanelFull>>(core) { }

// -------------------------------------------------------------------------------------------------

void PanelFull::add(Component component) {
	self().add(std::move(component));
}

void PanelFull::remove(Component& component) {
	self().remove(component);
}

void PanelFull::clear() {
	self().clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
